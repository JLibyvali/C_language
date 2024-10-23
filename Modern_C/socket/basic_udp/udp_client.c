#include "debug.h"
#include "macro.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF 256

int main(int argc, char **argv)
{

    int                sock;
    char               msg[BUF]    = {};
    char               revbuf[BUF] = {};

    struct sockaddr_in serv_addr, from_addr;
    socklen_t          from_addr_len = sizeof(from_addr);

    if (argc != 3)
    {
        printf("Usage %s <IP> <port>.\n", argv[0]);
        return 1;
    }

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == -1)
        Error("socket()");

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port        = htons(atoi(argv[2]));

    while (1)
    {
        printf("Input message.\n");
        scanf("%s", msg);

        char *tmp = strtok(msg, " ");

        if (!strcmp(tmp, "Q") || !strcmp(tmp, "q"))
            break;

        // Echo
        sendto(sock, msg, BUF, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

        int len = recvfrom(sock, revbuf, sizeof(revbuf), 0, (struct sockaddr *)&from_addr, &from_addr_len);
        if (len == -1)
            Error("recvfrom()");

        printf(ANSI_FG_RED "Server message: %s.\n" ANSI_NONE, revbuf);
    }

    close(sock);
    return 0;
}