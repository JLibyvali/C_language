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

    int                serv_sock, clnt_sock;
    int                ret = -1;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t          clnt_addr_len = sizeof(clnt_addr);
    char               buf[BUF_LEN]  = {};

    // Signal
    struct sigaction   sig;
    sig.sa_handler = child_proc;
    sigemptyset(&sig.sa_mask);
    sig.sa_flags = SA_RESTART;  // For accept(), Will not return `EINTR`, continue running.
    ret          = sigaction(SIGCHLD, &sig, NULL);
    CHECK_RET(ret, "sigaction()");

    // For multi process server
    pid_t child_pid;

    if (argc != 2)
    {
        printf("Usage: %s <PORT> \n", argv[0]);
        return 1;
    }

    // Socket setting.
    serv_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    CHECK_RET(serv_sock, "socket()");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port        = htons(atoi(argv[1]));

    ret                       = bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    CHECK_RET(ret, "bind()");

    ret = listen(serv_sock, 5);
    CHECK_RET(ret, "listen()");

    printf(ANSI_FMT("Listening for connecting....................\n", ANSI_FG_BLACK));

    while (1)
    {
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_len);
        CHECK_RET(clnt_sock, "accept()");

        printf(ANSI_FMT("New connected!!!\n", ANSI_FG_BLUE));

        child_pid = fork();
        if (child_pid == -1)
        {
            printf(ANSI_FMT("fork() creating process failed.\n", ANSI_FG_RED));
            close(clnt_sock);
            continue;
        }

        // #########################################################
        //  In child process, child-process is responsible for data exchange.
        // Parent process is responsible for creating child process.
        // ##########################################################
        /**
         * @brief The `fork()` will copy All parent process resources, including the file descriptor.
         * If more than 1 fd point to single system  opened socket resource, the socket will NOT be free.
         * So After `fork()` remember that closing ANY excess fd.
         *
         */
        if (child_pid == 0)
        {
            printf(ANSI_FMT("Server is online.......\n", ANSI_FG_BLUE));

            close(serv_sock);
            int len;
            while ((len = read(clnt_sock, buf, BUF_LEN - 1)) != 0)
            {
                write(clnt_sock, buf, len);
            }

            close(clnt_sock);
            printf("Client disconnected...............\n");
            return 0;
        }
        else
        {
            printf(ANSI_FG_BLUE "In parent process, child pid: %d \n" ANSI_NONE, child_pid);
            close(clnt_sock);
        }
    }

    close(serv_sock);
    return 0;
}

void child_proc(int sig)
{

    pid_t id;
    int   status;

    id = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status))
    {
        printf("Removed process id: %d \n", id);
    }
}