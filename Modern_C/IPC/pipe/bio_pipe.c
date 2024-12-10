#include "debug.h"

#include <fcntl.h>
#include <stdio.h>
#include <sys/pidfd.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/**
 * @brief pipe only allowed single-way communication.
 * Using 2 pipes to complete double-way communication.
 *
 * @return int
 */

int main()
{

    // Create 2 pipe;
    int   pipe_1[2], pipe_2[2], status;
    FILE *p1_wptr, *p1_rptr, *p2_wptr, *p2_rptr;
    pid_t parent, child;
    char  buf[4096];

    if (pipe(pipe_1) || pipe(pipe_2))
        error("pipe Created Failed");

    // fork child process
    child = fork();

    if (child == -1)
        error("fork() error.");

    if (child)
    {

        // close unused fd
        close(pipe_1[0]);
        close(pipe_2[1]);
        p1_wptr = fdopen(pipe_1[1], "w");
        p2_rptr = fdopen(pipe_2[0], "r");

        if (!p1_wptr || !p2_rptr)
            error("fdopen()  error.");

        parent = getpid();
        printf("Begin Test bio pipe: parent: %d == child: %d \n", parent, child);

        snprintf(
            buf, 4096,
            "Hello World!\nThis is a bio-way pipe test, with byte-stream feature test and pipe band width "
            "measure.\nWis you cna extract all 414-3134-1243134 information."
        );

        // Just Write Once.
        if (fputs(buf, p1_wptr) == EOF)
            error("fputs() error.");
        fclose(p1_wptr);

        char recv_buf[4096];
        while (fgets(recv_buf, 4096, p2_rptr))
        {
            printf("Get word: %s \n", recv_buf);
        }
        fclose(p2_rptr);

        printf("No blocked case close one-end pipe.  Wait child process end.\n");
        // https://man7.org/linux/man-pages/man3/wait.3p.html
        waitpid(child, &status, 0);

        if (WIFEXITED(status))
        {
            printf(YELLOW "Exited code: %d.\n" NONE, WEXITSTATUS(status));
        }
    }
    else
    {
        close(pipe_1[1]);
        close(pipe_2[0]);

        p1_rptr = fdopen(pipe_1[0], "r");
        p2_wptr = fdopen(pipe_2[1], "w");

        char recv_buf[6];
        int  times = 0;
        while (1)
        {
            if (fgets(recv_buf, 5, p1_rptr) == NULL)
                break;

            recv_buf[5] = '\t';
            printf("Read times: %d word: %s \n", times++, recv_buf);
            fputs(recv_buf, p2_wptr);
        }
        fclose(p1_rptr);
        fclose(p2_wptr);
        _exit(0);
    }

    return 0;
}