#include "debug.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


int main(int argc, char **argv)
{

    char               buf[256] = {};
    int                sock, i = 0;
    struct sockaddr_in addr;

    if (argc != 3)
    {
        printf("Usage %s <IP> <port>.\n", argv[0]);
        return 1;
    }

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1)
        Error("socket()");

    memset(&addr, 0, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port        = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1)
        Error("connect()");

    // TCP is stream oriented protocol ,
    int read_len, times = 0;
    while (read_len = read(sock, &buf[i++], 1))
    {
        if (read_len == -1)
            Error("read()");

        times++;
    }
    printf("Message:%s.\n", buf);
    printf("Times:%d.\n", times);

    close(sock);

    return 0;
}