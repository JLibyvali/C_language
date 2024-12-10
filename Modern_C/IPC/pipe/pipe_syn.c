#include "debug.h"

#include <fcntl.h>
#include <stdio.h>
#include <sys/pidfd.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * @brief Using pipes complete process synchronous.
 *  using the pipe blocking feature.
 * @return int
 */

int main()
{
    int   pfd[2];
    pid_t parent, child_1, child_2;
    pipe(pfd);

    // fork child process
    child_1 = fork();

    if (child_1)
    {
        parent = getpid();
        printf("Parent process: %d --> child: %d .\n", parent, child_1);
        close(pfd[1]);
        FILE *r_ptr = fdopen(pfd[0], "r");

        char  rev_buf[4096];
        int   status;
        while (fgets(rev_buf, 4096, r_ptr) != NULL)
        {
            printf("Get Word: %s ", rev_buf);
        }

        wait(&status);
        if (WIFEXITED(status))
            printf("Exited code: %d \n", WEXITSTATUS(status));
    }
    else
    {
        sleep(1);
        child_2 = fork();

        if (child_2)
        {
            close(pfd[0]);
            FILE *w_ptr = fdopen(pfd[1], "w");

            int   status;
            printf("Child 1: %d --> Child2: %d .\n", getpid(), child_2);
            fputs(YELLOW "Child1 Work finished.\n" NONE, w_ptr);
            fclose(w_ptr);

            waitpid(child_2, &status, 0);

            if (WIFEXITED(status))
                _exit(WEXITSTATUS(status));

            _exit(-1);
        }
        else
        {
            close(pfd[0]);
            FILE *w_ptr = fdopen(pfd[1], "w");
            sleep(2);

            fputs(YELLOW "CHILD2 work finished.\n" NONE, w_ptr);
            fclose(w_ptr);

            _exit(10);
        }
    }

    return 0;
}