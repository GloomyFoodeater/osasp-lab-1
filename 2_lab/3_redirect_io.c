#include <stdio.h>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Error: Not enough arguments\n");
        return 1;
    }

    FILE* f = fopen(argv[1], "w");

    if (!f)
    {
        fprintf(stderr, "Error: Fail to open the file\n");
        return 1;
    }

    char c;
    while ((c = getc(stdin)) != 6)
        putc(c, f);
    putc('\n', f);

    if(fclose(f) == EOF)
    {
        fprintf(stderr, "Error: Fail to close the file\n");
        return 1;
    }

    return 0;
}