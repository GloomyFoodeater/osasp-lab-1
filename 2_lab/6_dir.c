#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <linux/limits.h>

int printDir(char *name)
{
    // Open directory
    DIR *dir = opendir(name);
    if (!dir)
    {
        fprintf(stderr, "Error: Fail to open '%s'\n", name);
        return 1;
    }

    // Read directory
    printf("Directory '%s' files:\n", name);
    struct dirent *d;
    while (d = readdir(dir))
        printf("\t%s\n", d->d_name);

    // Close directory
    if (closedir(dir))
    {
        fprintf(stderr, "Error: Fail to close '%s'\n", name);
        return 1;
    }

    return 0;
}

int main()
{
    int res;
    char buf[PATH_MAX];
    getcwd(buf, PATH_MAX);
    res = printDir(buf);
    printf("------------------------------------------------\n");
    res |= printDir("/");

    return res;
}