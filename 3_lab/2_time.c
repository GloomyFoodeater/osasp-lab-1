#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

// Print local time, pid of current and parent processes
void print_time_and_pid(char *caller)
{
    struct timeval timeval_now;
    if (gettimeofday(&timeval_now, NULL) != -1)
    {
        int millisec = timeval_now.tv_usec / 1000; // microseconds -> milliseconds

        // Conversions & consideration of timezone for hours
        struct tm tm_now = *localtime(&timeval_now.tv_sec);
        int sec = tm_now.tm_sec;
        int min = tm_now.tm_min;
        int hrs = tm_now.tm_hour;

        printf("%-6s: time: %02d:%02d:%02d:%03d | pid: %d | parent pid: %d\n",
               caller, hrs, min, sec, millisec, getpid(), getppid());
    }
    else
        perror("Fail to get time");
}

// Handle child process and exit from it or print error in parent process
// If pid is zero does nothing
void handle_child(pid_t pid)
{
    if (pid == 0)
    {
        // Child process entry point
        print_time_and_pid("Child");
        exit(0);
    }
    else if (pid == -1)
        perror("Fail to create child");
}

// Wait all child processes to execute
void wait_all()
{
    pid_t child;
    do
    {
        child = wait(NULL);
    } while (child > 0); // -1 only when no unawaited child processes
}

void main()
{
    pid_t child_1 = fork();
    handle_child(child_1);

    pid_t child_2 = fork();
    handle_child(child_2);

    print_time_and_pid("Parent");
    system("ps -x");

    wait_all();
}