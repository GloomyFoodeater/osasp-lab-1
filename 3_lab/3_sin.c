#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>

// Calculate sin(x) as Taylor series with given number of members
double my_sin(double x, int n)
{
    // sin(x) = x - x^3/3! + x^5/5! - x^7/7! + ...
    double member = x, res = member;
    for (int i = 1; i < n; i++)
    {
        member *= (-1) * x * x / (2 * i + 1) / (2 * i);
        res += member;
    }
    return res;
}

// Wait for temination of all children processes
void wait_all()
{
    pid_t child;
    do
    {
        child = wait(NULL);
    } while (child > 0); // -1 only when no unawaited child processes
}

// Validate user input and convert it to suitable format
int convert_args(int argc, char *argv[], int *N, int *n)
{
    // Validate argc & print help
    if (argc < 3)
    {
        fprintf(stderr, "Not enough arguments (%d/2)\
                        \n1st: N - divisor of sinus argument\
                        \n2nd: n - number of members in Taylor series\n",
                argc - 1);
        return -1;
    }

    char *endptr;

    // Validate argv[1] - number of divisors
    *N = strtol(argv[1], &endptr, 10);
    if (errno == ERANGE || *endptr != '\0' || *N < 0 || strcmp(argv[1], "") == 0)
    {
        fprintf(stderr, "1st argument must be non-negative integer, was '%s'\n", argv[1]);
        return -1;
    }

    // Validate argv[2] - number of members in series
    *n = strtol(argv[2], &endptr, 10);
    if (errno == ERANGE || *endptr != '\0' || *n < 0 || strcmp(argv[2], "") == 0)
    {
        fprintf(stderr, "2nd argument must be non-negative integer, was '%s'\n", argv[2]);
        return -1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    // Validate & get arguments
    int N, n;
    if (convert_args(argc, argv, &N, &n) == -1)
    {
        return -1;
    }

    // Start processes for each numbers in range [0; N]
    for (int i = 0; i < N; i++)
    {
        pid_t child = fork();
        if (child == -1)
        {
            // Parent process error
            fprintf(stderr, "Fail to create process for i=%d\n", i);
        }
        else if (child == 0)
        {
            // Child process entry point
            double x = 2 * M_PI * i / N;
            double f = my_sin(x, n);
            printf("pid=%5d | sin(%f)=%+f | i=%d\n", getpid(), x, f, i);
            return 0;
        }
    }
    wait_all();
    return 0;
}
