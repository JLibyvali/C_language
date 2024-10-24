#include "debug.h"
#include "macro.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <threads.h>
#include <unistd.h>

#define BUF_LEN 256

int main(int argc, char **argv)
{

    if (argc != 3)
    {
        printf("Usage: %s <IP> <PROT> \n", argv[0]);
        return 1;
    }

    int                sock;
    struct sockaddr_in serv_addr;
    char               buf[BUF_LEN] = {};

    sock                            = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    CHECK_RET(sock, "socket()");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port        = htons(atoi(argv[2]));

    int ret                   = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    CHECK_RET(ret, "connect()");

    char recv[BUF_LEN] = {};
    while (1)
    {
        printf("Input message(q quit): ");
        fgets(buf, BUF_LEN - 1, stdin);
        char *tmp = strtok(buf, " ");

        if (!strcmp(tmp, "q\n"))
            break;

        write(sock, buf, BUF_LEN);

        if (read(sock, recv, BUF_LEN - 1) <= 0)
            break;

        printf("Received message: %s \n", recv);
    }

    close(sock);
    printf(ANSI_FG_RED "Exit..............\n" ANSI_NONE);

    return 0;
}
