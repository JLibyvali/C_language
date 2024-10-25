#define _XOPEN_SOURCE 700

#include "debug.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define BUF_LEN 256

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Usage %s <IP> <PORT> \n", argv[0]);
        return 1;
    }

    int                sock;
    struct sockaddr_in serv_addr;
    char               buf[BUF_LEN] = {};
    FILE              *read_fp = NULL, *write_fp = NULL;

    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    CHECK_RET(sock, "socket()");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port        = htons(atoi(argv[2]));

    int ret                   = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    CHECK_RET(ret, "connect()");

    printf("Connected.................\n");

    read_fp  = fdopen(sock, "r");
    write_fp = fdopen(sock, "w");

    while (1)
    {
        fputs("Input message: ", stdout);
        fgets(buf, BUF_LEN, stdin);

        if (!strcmp(buf, "q\n"))
        {
            break;
        }

        fputs(buf, write_fp);
        fflush(write_fp);

        fgets(buf, BUF_LEN, read_fp);
        printf("Message from Server: %s \n", buf);
    }

    fclose(read_fp);
    fclose(write_fp);

    return 0;
}