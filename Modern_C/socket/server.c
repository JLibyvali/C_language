#include "debug.h"

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define ARRLEN(arr) sizeof((arr)) / sizeof(arr[0])

int main(int argc, char **argv)
{
    int                serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t          clnt_sock_len;

    char               msg[] = "Hello World.\n          \
    22222222222222222222222222222222222222222222222222222\n     \
    33333333333333333333333333333333333333333333333333333\n     \
    4444444444444444444444444444444444444444444444444444444.\n";

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
        Error("bing()");

    if (listen(serv_sock, 5) == -1)
        Error("listen()");

    clnt_sock_len = sizeof(clnt_addr);
    clnt_sock     = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_sock_len);

    if (clnt_sock == -1)
        Error("accept()");

    write(clnt_sock, msg, sizeof(msg));

    close(clnt_sock);
    close(serv_sock);

    return 0;
}