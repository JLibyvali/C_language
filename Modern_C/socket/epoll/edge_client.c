#include "debug.h"
#include "macro.h"

#include <arpa/inet.h>
#include <bits/types/struct_iovec.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <time.h>
#include <unistd.h>

#define BUF_LEN 256

int main(int argc, char **argv)
{

    if (argc != 3)
    {
        printf("Usage %s <IP> <PORT>", argv[0]);
        return 1;
    }

    // buff io vector
    char         out_buf[BUF_LEN] = {};
    char         in_buf[BUF_LEN]  = {};
    struct iovec vecs[1]          = {};
    vecs[0].iov_base              = out_buf;

    // socket init
    int                sock       = -1;
    struct sockaddr_in serv_addr;

    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    CHECK_RET(sock, "socket()");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port        = htons(atoi(argv[2]));

    int ret                   = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    CHECK_RET(ret, "connect()");

    printf("Connected.................\n");

    while (1)
    {
        printf("Input message: ");
        fgets(out_buf, BUF_LEN, stdin);

        if (!strcmp(out_buf, "q\n"))
        {
            break;
        }

        // Just using io once.
        int str_len     = strlen(out_buf);
        vecs[0].iov_len = str_len;

        // Client still read/write all data once
        ret             = writev(sock, vecs, 1);
        CHECK_RET(ret, "writev()");

        vecs[0].iov_base = in_buf;
        vecs[0].iov_len  = BUF_LEN;
        ret              = readv(sock, vecs, 1);
        if (ret <= 0)
            break;
        else
        {
            printf("Message from Server: %s \n", in_buf);
        }

        memset(out_buf, '\0', BUF_LEN);
        memset(in_buf, '\0', BUF_LEN);
    }

    close(sock);
    return 0;
}
