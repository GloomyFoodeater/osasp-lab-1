#include <stdio.h>
#include <unistd.h>
#include <dirent.h>

int printDir(char* name)
{
    printf("Directory %s files:\n", name);

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
        printf("\t%s\n", d->d_name);

    // Close directory
    if(closedir(dir))
    {
        fprintf(stderr, "Error: Fail to close %s\n", name);
        return 1;
    }

    return 0;
}

int main()
{
    int res;
    char buf[256];
    getcwd(buf, 256);
    res = printDir(buf);
    printf("------------------------------------------------\n");
    res |= printDir("/");
    
    return res;
}