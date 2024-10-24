#include "debug.h"
#include "macro.h"

#include <complex.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUF_LEN 512

int sock_opt[] = {SO_TYPE, SO_SNDBUF, SO_RCVBUF, SO_REUSEADDR, TCP_NODELAY};

struct sock_info
{
    int ret;
    int options[ARRLEN(sock_opt)];
};

typedef struct sock_info sock_info;

void                     print_sockinfo(int *sock)
{
    if (!sock)
        Error("sock is error");

    sock_info info;
    socklen_t optlen = sizeof(info.options[0]);
    info.ret         = -1;

    int len = ARRLEN(sock_opt), i = 0;
    for (; i < len; i++)
    {

        if (i == len - 1)
        {
            info.ret = getsockopt(*sock, IPPROTO_TCP, sock_opt[i], (void *)&info.options[i], &optlen);
            break;
        }

        info.ret = getsockopt(*sock, SOL_SOCKET, sock_opt[i], (void *)&info.options[i], &optlen);

        if (info.ret == -1)
            Error("getsocketopt()");
    }

    char *pool     = (char *)malloc(ARRLEN(sock_opt) * BUF_LEN);
    if(!pool)
        Error("Malloc()");

    char *msg[len] = {};
    for (i=0; i<len; i++) {
        msg[i] = pool + (i*BUF_LEN);
    }


    // Build the output infomation.
    if (info.options[0] == SOCK_STREAM)
        snprintf(msg[0], BUF_LEN, "the SOCK_TYPE: %d is SOCK_STREAM \n", info.options[0]);

    if (info.options[0] == SOCK_DGRAM)
        snprintf(msg[0], BUF_LEN, "the SOCK_TYPE: %d is SOCK_DGRAM \n", info.options[0]);

    snprintf(msg[1], BUF_LEN, "the Send buffer size: %d Bytes \n", info.options[1]);
    snprintf(msg[2], BUF_LEN, "the Receive buffer size: %d Bytes \n", info.options[2]);

    if (info.options[3])
        snprintf(msg[3], BUF_LEN, "Socket reusedaddr: %d , CAN use socket in TIME_WAIT. \n", info.options[3]);
    else
        snprintf(msg[3], BUF_LEN, "Socket reusedaddr: %d , CAN NOT use socket in TIME_WAIT. \n", info.options[3]);

    if (info.options[4])
        snprintf(msg[4], BUF_LEN, "TCP forbid Nagle: %d \n", info.options[4]);
    else
        snprintf(msg[4], BUF_LEN, "TCP used Nagle: %d \n", info.options[4]);

    for (i = 0; i < len; i++)
        printf(ANSI_FG_BLUE"%s"ANSI_NONE, msg[i]);
}

int main(int argc, char **argv)

{
    int sock;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1)
        Error("socket()");

    print_sockinfo(&sock);

    return 0;
}