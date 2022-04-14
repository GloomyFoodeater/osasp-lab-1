#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    // Argc validation
    if (argc < 3)
    {
        fprintf(stderr, "Error: Not enough arguments\n1st - name of source file to copy\n2nd - name of destination file\n");
        return 1;
    }

    // Open source file
    FILE *src = fopen(argv[1], "r");
    if (!src)
    {
        fprintf(stderr, "Error: Fail to open source file\n");
        return 1;
    }

    // Open destination file
    FILE *dst = fopen(argv[2], "w");
    if (!dst)
    {
        fprintf(stderr, "Error: Fail to open destination file\n");
        if (fclose(src) == EOF)
            fprintf(stderr, "Error: Fail to close source file\n");
        return 1;
    }

    // Copying file byte-by-byte
    int bytes = 0;
    char c;
    while ((c = getc(src)) != EOF)
    {
        putc(c, dst);
        bytes++;
    }

    // Permissions' transfer
    struct stat srcStat;
    if (stat(argv[1], &srcStat) || chmod(argv[2], srcStat.st_mode))
        fprintf(stderr, "Error: Fail to copy file permissons\n");

    // Console output
    printf("%d bytes copied\n", bytes);

    // Bitwise or to assure second fclose call
    if (fclose(src) | fclose(dst))
    {
        fprintf(stderr, "Error: Fail to close files");
        return 1;
    }

    return 0;
}
