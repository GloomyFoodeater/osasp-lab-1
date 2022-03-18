#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <linux/limits.h>

int printDir(char* name, FILE* f, int min, int max, int* counter)
{
    // Open directory
    DIR* dir = opendir(name);
    if(!dir)
    {
        fprintf(stderr, "Error: Fail to open %s\n", name);
        return 1;
    }

    // Read directory    
    struct dirent* d;
    while (d = readdir(dir))
    {
        // Get full name of file
        char fullName[PATH_MAX] = {0};
        strcat(fullName, name);
        strcat(fullName, "/");
        strcat(fullName, d->d_name);

        // Get file status
        struct stat fileStat;
        if(!stat(fullName, &fileStat))
        {
            // Print file
            if (d->d_type == DT_REG && min < fileStat.st_size && fileStat.st_size < max)
            {
                // Canonise path
                char resolvedPath[PATH_MAX];
                realpath(fullName, resolvedPath);
                
                // Output
                fprintf(f, "%s\t%ldb\n", resolvedPath, fileStat.st_size);
                (*counter)++;
            }
            
            // Enter child directory
            else if(d->d_type == DT_DIR && strcmp(d->d_name, ".") && strcmp(d->d_name, ".."))
            {
                printf("Opening %s\n", fullName);
                printDir(fullName, f, min, max, counter);
            }
        }
    }

    // Close directory
    if(closedir(dir))
    {
        fprintf(stderr, "Error: Fail to close %s\n", name);
        return 1;
    }

    return 0;
}

int main(int argc, char* argv[])
{
    // Argc validation
    if (argc < 5)
    {
        fprintf(stderr, "Error: Not enough arguments\n");
        return 1;
    }

    // Argv[2] validation (min size) & str -> int
    char *endptr;
    int min = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0' || min < 0)
    {
        fprintf(stderr, "Error: 2nd argument must be natural number or 0\n");
        return 1;
    }

    // Argv[3] validation (max size) & str -> int
    int max = strtol(argv[3], &endptr, 10);
    if (*endptr != '\0' || max < 0)
    {
        fprintf(stderr, "Error: 3rd argument must be natural number or 0\n");
        return 1;
    }
    
    // Argv[4] validation & file opening
    FILE* f = fopen(argv[4], "w");
    if(!f)
    {
        fprintf(stderr, "Error: Fail to open %s\n", argv[4]);
        return 1;
    }

    // Erase redundant slash
    int n = strlen(argv[1]);
    if(argv[1][n - 1] == '/')
        argv[1][n - 1] = '\0';


    // Print files
    int counter = 0;
    printDir(argv[1], f, min, max, &counter);
    printf("%d files found\n", counter);

    // Close file
    if(fclose(f))
    {
        fprintf(stderr, "Error: Fail to close %s\n", argv[4]);
        return 1;
    }

    return 0;
}