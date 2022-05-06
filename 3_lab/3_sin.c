#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>

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
void convert_args(int argc, char *argv[], int *N, int *n)
{
    // Validate argc & print help
    if (argc < 3)
    {
        fprintf(stderr, "Not enough arguments (%d/2)\
                        \n1st: N - number of function results\
                        \n2nd: n - number of members in Taylor series\n",
                argc - 1);
        exit(1);
    }

    // Validate argv[1] - number of function results
    char *endptr;
    *N = strtol(argv[1], &endptr, 10);
    if (errno == ERANGE || *endptr != '\0' || *N <= 0 || strcmp(argv[1], "") == 0)
    {
        fprintf(stderr, "1st argument must be natural number, was '%s'\n", argv[1]);
        exit(1);
    }

    // Validate argv[2] - number of members in series
    *n = strtol(argv[2], &endptr, 10);
    if (errno == ERANGE || *endptr != '\0' || *n <= 0 || strcmp(argv[2], "") == 0)
    {
        fprintf(stderr, "2nd argument must be natural number, was '%s'\n", argv[2]);
        exit(1);
    }
}

// Factorial of number
unsigned long long int fact(int n)
{
    unsigned long long int res = 1;
    for (int i = 1; i <= n; i++)
        res *= i;
    return res;
}

// Calculate member of Taylor series with 0-based index
double get_member(double x, int i)
{
    return pow(-1, i) * pow(x, 2 * i + 1) / fact(2 * i + 1);
}

// Closes file with perror call if needed
void perror_fclose(FILE *f)
{
    if (fclose(f) == -1)
        perror("Fail to close file");
}

// Create empty file or rewrite existening one
// Return 0 on success and -1 otherwise
int rewrite_file(char *filename)
{

    FILE *f = fopen(filename, "w");
    if (!f)
        perror("Fail to rewrite file");
    int res = (f == NULL) ? -1 : 0;
    perror_fclose(f);
    return res;
}

// Finding term of Teylor's series by summing double numbers in file
// Return 0 on success and -1 otherwise
int sum_doubles(char *filename, double *res)
{
    FILE *f = fopen(filename, "r");
    if (!f)
    {
        perror("Fail to open file to read");
        return -1;
    }

    char str[100];
    *res = 0;
    while (fgets(str, 100, f))
    {
        str[strlen(str) - 1] = '\0';

        // Split string by white space
        char *delim = " ";
        char *token = strtok(str, delim); // pid
        token = strtok(NULL, delim);      // member

        // Conversion string -> double
        char *endptr;
        double member = strtod(token, &endptr);
        if (errno == ERANGE || *endptr != '\0' || strcmp(token, "") == 0)
        {
            fprintf(stderr, "Fail to convert string to double\n");
            perror_fclose(f);
            return -1;
        }
        *res += member;
    }
    perror_fclose(f);
    return 0;
}

void main(int argc, char *argv[])
{
    // Collect input
    int N, n;
    convert_args(argc, argv, &N, &n);

    // Fill result array
    double *y = (double *)calloc(N, sizeof(double));
    if (!y)
    {
        perror("Not enough memory for y array");
        exit(1);
    }

    for (int i = 0; i < N; i++)
    {
        // Get sinus argument
        double x = 2 * M_PI * i / N;
        printf("i: %d\tx: %f\n----------------------------------------------------------------------\n", i, x);

        // Rewriting temp file
        if (rewrite_file("tmp.txt") == -1)
        {
            // y[i] is invalid since no members can be written in file
            fprintf(stderr, "ERROR: y[%d] is invalid\n", i);
            y[i] = 0;
            printf("----------------------------------------------------------------------\n");
            continue;
        }

        // Iterate over Taylor series
        for (int j = 0; j < n; j++)
        {
            pid_t child = fork();
            if (child == 0)
            {
                free(y);
                // Child entry point
                FILE *tmp_file = fopen("tmp.txt", "a");
                if (!tmp_file)
                {
                    perror("Fail to open temp file");
                    exit(1); // Error child exit
                }
                double member = get_member(x, j);

                // Output
                printf("pid: %d\tmember: %f\tj: %d\n", getpid(), member, j);
                fprintf(tmp_file, "%d %f\n", getpid(), member);

                perror_fclose(tmp_file);
                exit(0); // Normal child exit
            }
            else if (child == -1)
                perror("Fail to create child");
        }
        wait_all();

        // Array filling
        if (sum_doubles("tmp.txt", &y[i]) == -1)
        {
            // y[i] is invalid since there was error while reading file
            fprintf(stderr, "ERROR: y[%d] is invalid\n", i);
            y[i] = 0;
        }
        printf("----------------------------------------------------------------------\n");
    }

    // Output y array in file
    FILE *out_file = fopen("output.txt", "w");
    if (out_file)
    {
        for (int i = 0; i < N; i++)
            fprintf(out_file, "y[%d] = %f\n", i, y[i]);
        perror_fclose(out_file);
    }
    else
        perror("Fail to open output file");
    free(y);
}
