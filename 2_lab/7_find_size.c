#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <linux/limits.h>
#include <errno.h>
#include <string.h>

int printDir(char *name, FILE *f, int min, int max, int *foundCounter, int *peekedCounter)
{
    // Open directory
    DIR *dir = opendir(name);
    if (!dir)
    {
        fprintf(stderr, "Error: Fail to open dir '%s'\n", name);
        return 1;
    }

    // Read directory
    struct dirent *entry;
    while (entry = readdir(dir))
    {
        // Get full name of file
        char fullName[PATH_MAX] = {0};
        strcat(fullName, name);
        strcat(fullName, "/");
        strcat(fullName, entry->d_name);

        // Canonise path
        char resolvedPath[PATH_MAX];
        realpath(fullName, resolvedPath);

        // Get file status
        struct stat status;
        if (!stat(resolvedPath, &status))
        {
            (*peekedCounter)++;

            // Print file
            if (entry->d_type == DT_REG && min <= status.st_size && status.st_size <= max)
            {
                // Output
                fprintf(f, "%s\t%ldb\n", resolvedPath, status.st_size);
                (*foundCounter)++;
            }

            // Enter child directory
            else if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
            {
                printf("Opening '%s'\n", resolvedPath);
                printDir(resolvedPath, f, min, max, foundCounter, peekedCounter);
            }
        }
    }

    // Close directory
    if (closedir(dir))
    {
        fprintf(stderr, "Error: Fail to close dir '%s'\n", name);
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    // Argc validation
    if (argc < 5)
    {
        fprintf(stderr, "Error: Not enough arguments\n1st - name of dir to start search\n2nd - min size of files\
                        \n3rd - max size of files\n4th - name of file for output\n");
        return 1;
    }

    // Argv[2] validation (min size) & str -> int
    char *endptr;
    int min = strtol(argv[2], &endptr, 10);
    if (errno == ERANGE || strlen(argv[2]) == 0 || *endptr != '\0' || min < 0)
    {
        fprintf(stderr, "Error: 2nd argument must be natural number or 0\n");
        return 1;
    }

    // Argv[3] validation (max size) & str -> int
    int max = strtol(argv[3], &endptr, 10);
    if (errno == ERANGE || strlen(argv[3]) == 0 || *endptr != '\0' || max < 0)
    {
        fprintf(stderr, "Error: 3rd argument must be natural number or 0\n");
        return 1;
    }

    if (max < min)
    {
        fprintf(stderr, "Error: 3rd arguments was lesser than 2nd\n");
        return 1;
    }

    // Argv[4] validation & file opening
    FILE *f = fopen(argv[4], "w");
    if (!f)
    {
        fprintf(stderr, "Error: Fail to open file '%s'\n", argv[4]);
        return 1;
    }

    // Print files
    int foundCounter = 0, peekedCounter = 0, res;
    res = printDir(argv[1], f, min, max, &foundCounter, &peekedCounter);
    if (res == 0)
        printf("%d files found, %d files checked\n", foundCounter, peekedCounter);

    // Close file
    if (fclose(f))
    {
        fprintf(stderr, "Error: Fail to close file '%s'\n", argv[4]);
        return 1;
    }

    return res;
}
