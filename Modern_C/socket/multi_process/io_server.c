#define _XOPEN_SOURCE 700

#include "debug.h"
#include "macro.h"

#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUF_LEN 256
void child_proc(int sig);

int  main(int argc, char **argv)
{

    if (argc != 2)
    {
        printf("Usage %s <PORT> \n", argv[0]);
        return 1;
    }

    int                serv_sock, clnt_sock, ret;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t          clnt_addr_len = sizeof(clnt_addr);
    struct sigaction   sig;
    pid_t              pid;
    char               msg[BUF_LEN] = {};

    // socket init
    serv_sock                       = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    CHECK_RET(serv_sock, "socket()");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port        = htons(atoi(argv[1]));

    ret                       = bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    CHECK_RET(ret, "bind()");

    ret = listen(serv_sock, 5);
    CHECK_RET(ret, "listen()");

    printf(ANSI_FMT("Listening...............\n", ANSI_FG_BLACK));

    // Signal
    sig.sa_handler = child_proc;
    sigemptyset(&sig.sa_mask);
    sig.sa_flags = SA_RESTART;
    ret          = sigaction(SIGCHLD, &sig, NULL);
    CHECK_RET(ret, "sigaction()");

    // child process
    pid = fork();

    while (1)
    {
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_len);
        CHECK_RET(clnt_sock, "accept()");

        // In child process
        if (pid == 0)
        {
            printf(ANSI_FMT("Child Online..................\n", ANSI_FG_BLUE));
            close(serv_sock);

            int   len = read(clnt_sock, msg, BUF_LEN - 1);
            char *tmp = strtok(msg, " ");

            // If socket is connecting, and no input 'q`
            while (len > 0 && strcmp(tmp, "q\n"))
            {
                write(clnt_sock, msg, len);
            }

            close(clnt_sock);
            printf(ANSI_FMT("Child offline..............\n", ANSI_FG_BLUE));
            return 0;
        }
        else
        {
            printf(ANSI_FMT("Connected..............\n", ANSI_FG_BLACK));
            close(clnt_sock);
        }
    }

    return 0;
}

void child_proc(int sig)
{
    pid_t id;
    int   status;
    id = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status))
    {
        printf(ANSI_FG_BLACK "Removed process %d\n" ANSI_NONE, id);
    }
}