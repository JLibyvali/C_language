#define _XOPEN_SOURCE 700

#include "debug.h"
#include "macro.h"

#include <arpa/inet.h>
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
void write_routine(int sock, char *buf);
void read_routine(int sock, char *buf);

int  main(int argc, char **argv)
{
    int                sock, ret;
    struct sockaddr_in serv_addr;
    char               msg[BUF_LEN] = {};
    pid_t              pid;

    if (argc != 3)
    {
        printf("Usage %s <IP> <PORT> \n", argv[0]);
        return 1;
    }

    // socket init.
    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    CHECK_RET(sock, "socket()");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port        = htons(atoi(argv[2]));

    ret                       = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    CHECK_RET(ret, "connect()");

    // child process is responsible for I/O dividing.
    pid = fork();

    if (pid == 0)
    {
        write_routine(sock, msg);
    }
    else
    {
        read_routine(sock, msg);
    }

    close(sock);

    return 0;
}

void write_routine(int sock, char *buf)
{
    while (1)
    {
        printf("Input message (q quit). \n");
        fgets(buf, BUF_LEN, stdin);

        if (!strcmp(buf, "q\n"))
            break;

        write(sock, buf, BUF_LEN);
    }

    shutdown(sock, SHUT_WR);
    return;
}

void read_routine(int sock, char *buf)
{
    while (1)
    {
        int len = read(sock, buf, BUF_LEN - 1);

        if (len <= 0)
            break;

        buf[len] = '\0';
        printf("Received: %s \n", buf);
    }

    return;
}