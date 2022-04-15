#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>

// Print local time, pid of current and parent processes
void print_time_and_pid()
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

        printf("time: %02d:%02d:%02d:%03d | pid: %d | parent pid: %d\n", hrs, min, sec, millisec, getpid(), getppid());
    }
    else
    {
        perror("Fail to get time");
    }
}

// Wait process and output error
// If pid is lesser or equal than zero does nothing
void waitpid_with_err(int pid)
{
    if (pid > 0 && waitpid(pid, NULL, 0) == -1)
    {
        perror("Fail to wait process");
    }
}

// Handle child process and exit from it or print error in parent process
// If pid is zero does nothing
void handle_child(pid_t pid)
{
    if (pid == -1)
    {
        perror("Fail to create child");
    }
    else if (pid == 0)
    {
        // Child process entry point
        print_time_and_pid();
        exit(0);
    }
}

void main()
{
    pid_t child_1 = fork();
    handle_child(child_1);

    pid_t child_2 = fork();
    handle_child(child_2);

    system("ps -x");
    waitpid_with_err(child_1);
    waitpid_with_err(child_2);
}