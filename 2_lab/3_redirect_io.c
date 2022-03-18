#include <stdio.h>
#include <unistd.h>
#include <termios.h>

int getch()
{
    int ch;
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getc(stdin);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

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
    while ((c = getch()) != 6)
        putc(c, f);
    putc('\n', stdout);
    putc('\n', f);

    if(fclose(f) == EOF)
    {
        fprintf(stderr, "Error: Fail to close the file\n");
        return 1;
    }

    return 0;
}