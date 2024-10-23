#include "debug.h"
#include "macro.h"

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
    int                sock;
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

    printf("Connected.....................\n");

    // TCP is stream oriented protocol
    int len = 0;
    while (1)
    {
        printf("Input msg (Q quit).\n");
        scanf("%s", buf);

        char *token = strtok(buf, " ");
        if (!strcmp(token, "q") || !strcmp(token, "Q"))
            break;

        write(sock, buf, sizeof(buf));

        char tmp[256] = {};
        len           = read(sock, tmp, sizeof(tmp));
        if (len)
            printf(ANSI_FG_RED "Received message: %s\n" ANSI_NONE, tmp);
    }

    printf("Client exit.\n");
    close(sock);

    return 0;
}