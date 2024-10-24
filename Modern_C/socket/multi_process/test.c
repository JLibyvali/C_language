#include "macro.h"

#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

void child_proc(int sig)
{

    int   status;
    pid_t id = waitpid(-1, &status, WNOHANG);

    if (WIFEXITED(status))
    {

        printf("Removed process: %d \n", id);
        printf("Child process exit with: %d \n", WEXITSTATUS(status));
    }
}

int main()
{
    signal(SIGCHLD, child_proc);
    pid_t pid;
    pid = fork();

    // Child proc()
    if (!pid)
    {
        printf(ANSI_FG_BLUE "This is Child process 1.\n" ANSI_NONE);
        sleep(5);
        return (11);
    }

    // Parent proc()
    if (pid)
    {
        printf(ANSI_FG_RED "This is Parent process.\n" ANSI_NONE);
        printf(ANSI_FG_RED "Child process 1 pid: %d \n" ANSI_NONE, pid);

        // another child process.
        pid = fork();

        if (!pid)
        {
            printf(ANSI_FMT("This is  Child process 2 \n", ANSI_FG_GREEN));
            sleep(10);
            return (22);
        }

        if (pid)
        {
            printf(ANSI_FG_RED "Child process 2 pid: %d \n" ANSI_NONE, pid);

            int i;
            for (i = 0; i < 5; i++)
            {
                printf(ANSI_FG_RED "Waiting................\n" ANSI_NONE);
                sleep(5);
            }
        }
    }

    return 0;
}
