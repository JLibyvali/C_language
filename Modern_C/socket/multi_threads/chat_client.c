#define _XOPEN_SOURCE 700

#include "debug.h"
#include "macro.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <time.h>
#include <unistd.h>

#define BUF_LEN   256
#define NAME_SIZE 10

static char *name = "[Default]";
void        *write_buf(void *_sock);
void        *read_buf(void *_sock);

int          main(int argc, char **argv)
{

    if (argc != 3)
    {
        printf("Usage %s <IP> <PORT> \n", argv[0]);
        return 1;
    }

    int                sock;
    struct sockaddr_in serv_addr;
    pthread_t          read_id, write_id;
    void              *thread_ret;

    // socket init
    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    CHECK_RET(sock, "socket()");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port        = htons(atoi(argv[2]));

    int ret                   = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    CHECK_RET(ret, "connect");

    pthread_create(&write_id, NULL, write_buf, (void *)&sock);
    pthread_create(&read_id, NULL, read_buf, (void *)&sock);
    pthread_join(write_id, &thread_ret);
    pthread_join(read_id, &thread_ret);

    return 0;
}

void *write_buf(void *_sock)
{
    int          sock                          = *(int *)_sock;
    char         buf[BUF_LEN]                  = {};
    char         send_msg[BUF_LEN + NAME_SIZE] = {};
    struct iovec vecs[0]                       = {};
    vecs[0].iov_len                            = BUF_LEN + NAME_SIZE;

    while (1)
    {
        printf("Input message: \n");
        fgets(buf, BUF_LEN, stdin);

        // Terminate the program
        if (!strcmp(buf, "q\n"))
        {
            close(sock);
            exit(0);
        }

        snprintf(send_msg, BUF_LEN + NAME_SIZE, "%s %s", name, buf);

        vecs[0].iov_base = send_msg;
        writev(sock, vecs, 1);

        memset(buf, '\0', sizeof(buf));
    }

    return NULL;
}

void *read_buf(void *_sock)
{

    int          sock                          = *(int *)_sock, str_len;
    char         read_msg[BUF_LEN + NAME_SIZE] = {};

    struct iovec vecs[0]                       = {};
    vecs[0].iov_len                            = BUF_LEN + NAME_SIZE;
    vecs[0].iov_base                           = read_msg;

    while (1)
    {
        str_len = readv(sock, vecs, 1);

        if (str_len <= 0)
            break;

        read_msg[str_len] = '\0';

        printf("Message from server: ");
        fputs(read_msg, stdout);

        memset(read_msg, '\0', sizeof(read_msg));
    }

    return NULL;
}