#include "debug.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

const char *clnt_log = "log.txt";
#define MSG 256

int main(int argc, char **argv)
{

    if (argc != 3)
    {
        printf("Usage %s <IP> <PORT>.\n", argv[0]);
        return 1;
    }

    int                sock;
    struct sockaddr_in serv_addr;
    FILE              *fp       = NULL;
    char               msg[MSG] = {};

    // socket init
    sock                        = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1)
        Error("socket()");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port        = htons(atoi(argv[2]));

    fp                        = fopen(clnt_log, "w+");
    if (!fp)
        Error("fopen()");

    int ret = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (ret == -1)
        Error("connect()");
    printf("Connected!!!\n");

    // Send data to server  and read all server data back.
    while (1)
    {
        printf("Input message: ");
        scanf("%s", msg);
        strncat(msg, "\n", sizeof(msg) - strlen(msg) - 1);
        write(sock, msg, strlen(msg));

        char *tmp = strtok(msg, " ");
        if (!strcmp(tmp, "q\n"))
            break;
    }

    // Read the whole file from server
    while (1)
    {

        ret = read(sock, msg, MSG - 1);
        if (ret <= 0)
            break;

        msg[ret] = '\0';
        printf("Writing %d : %s\n", ret, msg);

        if (!strcmp(msg, "EOF"))
            break;

        fwrite(msg, 1, ret, fp);
    }
    fflush(fp);

    printf("Received all data.\n");
    write(sock, "FIN", 3);

    fclose(fp);
    close(sock);
    return 0;
}