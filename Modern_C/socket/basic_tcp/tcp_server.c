#include "debug.h"
#include "macro.h"

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define ARRLEN(arr) sizeof((arr)) / sizeof(arr[0])
#define CLIENT_NUM  5

int main(int argc, char **argv)
{
    int                serv_sock, clnt_sock, i = 0;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t          clnt_sock_len;
    char               buf[256] = {};

    if (argc != 2)
    {
        printf("Ussage: %s <port>\n", argv[0]);
        return 1;
    }

    // Creating socket
    serv_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (serv_sock == -1)
        Error("socket()");

    // ### IMPORTANT struct must be set to all zero for align
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port        = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        Error("bind()");

    if (listen(serv_sock, 5) == -1)
    {
        Error("listen()");
    }
    else
    {

        printf("Waiting for connection.............\n");
    }

    clnt_sock_len = sizeof(clnt_addr);
    // accept() will create a new socket for data transmission.

    for (; i < CLIENT_NUM; i++)
    {

        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_sock_len);
        if (clnt_sock == -1)
            Error("accept()");

        printf(ANSI_FG_BLUE "Client %d connected.\n" ANSI_NONE, i);

        // Echo server
        int str_len = -1;
        while ((str_len = read(clnt_sock, buf, sizeof(buf)) != 0))  // If client call `close()` the will be false.
        {
            write(clnt_sock, buf, sizeof(buf));
        }

        memset(buf, '\0', sizeof(buf));

        printf(ANSI_FG_RED "Client %d quit connection.\n" ANSI_NONE, i);
        close(clnt_sock);
    }

    printf(ANSI_FG_BLUE "Server quit.\n" ANSI_NONE);
    close(serv_sock);
    return 0;
}