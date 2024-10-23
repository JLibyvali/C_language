#include "debug.h"
#include "macro.h"

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF 256

int main(int argc, char **argv)
{

    int                serv_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t          clnt_addr_len = sizeof(clnt_addr_len);
    char               msg[BUF]      = {};

    if (argc != 2)
    {
        printf("Usage %s <port>.\n", argv[0]);
        return 1;
    }

    serv_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serv_sock == -1)
        Error("socket()");

    // Init socket address.
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port        = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        Error("bind()");

    printf(ANSI_FG_BLUE "Waiting for connection...............\n" ANSI_NONE);

    while (1)
    {
        int len = -1;

        len     = recvfrom(serv_sock, msg, BUF, 0, (struct sockaddr *)&clnt_addr, &clnt_addr_len);
        if (len == -1)
            Error("recvfrom()");

        len = sendto(serv_sock, msg, len, 0, (struct sockaddr *)&clnt_addr, clnt_addr_len);
        if (len == -1)
            Error("sendto()");

        printf(ANSI_FG_BLUE "Shot one\n" ANSI_NONE);
    }

    close(serv_sock);
    return 0;
}