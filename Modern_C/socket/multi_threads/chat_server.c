#define _XOPEN_SOURCE 700

#include "debug.h"
#include "macro.h"

#include <arpa/inet.h>
#include <bits/pthreadtypes.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/cdefs.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define BUF_LEN    256
#define MAX_CLIENT 100

void      *clnt_thread(void *sock_fd);
void       send_msg(char *msg, int str_len);
void      *fp_manage(void *sock_fd);
void       ___FLAG_false(bool *_f) __attribute__((nonnull));
void       ___FLAG_true(bool *_f) __attribute__((nonnull));

// Global resources
static int client_num = 0;
static int clients[MAX_CLIENT];

struct fp
{
    int   sock;
    FILE *read_fp;
    FILE *write_fp;
};
typedef struct fp sock_fps;
static sock_fps   clients_fps[MAX_CLIENT] = {};

// Using mutex to synchronous
pthread_mutex_t   client_mutex;  // Protect clients[], client_num
pthread_mutex_t   fp_mutex;      // Protect clients_fps[]

pthread_cond_t    fp_cond;       // cond_variable itself doesn't hold true or false, it just a signal
bool              flag_exit = false;
bool              flag_fp   = false;

int               main(int argc, char **argv)
{

    if (argc != 2)
    {
        printf("Usage: %s <PORT> \n", argv[0]);
        return 1;
    }

    // sock
    int                serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t          clnt_addr_len          = sizeof(clnt_addr);

    // mutex
    pthread_t          thread_ids[MAX_CLIENT] = {};
    pthread_mutex_init(&client_mutex, NULL);
    pthread_cond_init(&fp_cond, NULL);
    memset(&clients, -1, MAX_CLIENT);
    memset(&clients_fps, 0, MAX_CLIENT);

    // sock init;
    serv_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    CHECK_RET(serv_sock, "socket()");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port        = htons(atoi(argv[1]));

    int ret                   = bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    CHECK_RET(ret, "bind()");

    ret = listen(serv_sock, 5);
    CHECK_RET(ret, "listen()");
    printf(ANSI_FMT("Listen..........\n", ANSI_BG_BLUE));

    while (1)
    {
        printf("Waiting...................\n");
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_len);
        CHECK_RET(clnt_sock, "accept()");

        pthread_mutex_lock(&client_mutex);
        clients[client_num++] = clnt_sock;
        pthread_mutex_unlock(&client_mutex);

        // Create task
        // Every clients has two threads,
        pthread_create(&thread_ids[(client_num - 1)], NULL, clnt_thread, (void *)&clnt_sock);
        pthread_detach(thread_ids[(client_num - 1)]);

        printf(ANSI_FMT("Connected IP: %s......\n", ANSI_BG_BLUE), inet_ntoa(clnt_addr.sin_addr));
    }

    pthread_mutex_destroy(&client_mutex);
    pthread_cond_destroy(&fp_cond);
    close(serv_sock);
    return 0;
}

// One thread to  handle connection
void *clnt_thread(void *_sock_fd)
{
    int       sock = *(int *)_sock_fd, i;
    pthread_t _id;
    char      msg[BUF_LEN] = {}, *str_ret = NULL;
    FILE     *read_fp = NULL;

    // Create  resource thread.
    pthread_create(&_id, NULL, fp_manage, (void *)&sock);
    pthread_detach(_id);

    // Get read FILE ptr
    pthread_mutex_lock(&fp_mutex);
    // Waiting fp initialized.
    while (!flag_fp)
        pthread_cond_wait(&fp_cond, &fp_mutex);

    for (i = 0; i < client_num; i++)
    {
        if (sock == clients_fps[i].sock)
            read_fp = clients_fps[i].read_fp;
    }
    pthread_mutex_unlock(&fp_mutex);

    if (read_fp == NULL)
        Error("Get read_fp");
    ___FLAG_false(&flag_fp);

    while (1)
    {

        str_ret = fgets(msg, BUF_LEN, read_fp);

        if (str_ret == NULL)  // EOF
            break;
        if (str_ret < 0)
            Error("fread()");

        size_t str_len = strlen(msg);
        send_msg(msg, str_len);
    }

    // Disconnect...............
    pthread_mutex_lock(&client_mutex);
    flag_exit = true;
    pthread_cond_signal(&fp_cond);  // Signal to half close existed client.
    pthread_mutex_unlock(&client_mutex);

    pthread_mutex_lock(&fp_mutex);
    while (!flag_fp)
        pthread_cond_wait(&fp_cond, &fp_mutex);

    // After half closed, handle last read message
    pthread_mutex_lock(&client_mutex);
    for (i = 0; i < client_num; i++)
    {
        if (clients[i] == sock)
            memmove(&clients[i], &clients[i + 1], (client_num - i - 1) * sizeof(clients[0]));
    }

    client_num--;
    pthread_mutex_unlock(&client_mutex);

    fgets(msg, BUF_LEN, read_fp);
    printf("Last message: %s \n", msg);
    fclose(read_fp);

    ___FLAG_false(&flag_fp);
    pthread_mutex_unlock(&fp_mutex);

    close(sock);

    return NULL;
}

// Manage all sock fd
void *fp_manage(void *sock_fd)
{
    int clnt_sock = *(int *)sock_fd;
    int i         = 0;

    // ##############################################33
    //  fp_cond and fp_mutex lock
    // #############################################3
    //  Init write/read fp.
    pthread_mutex_lock(&fp_mutex);
    for (i; i < client_num; i++)
    {
        if (clnt_sock == clients[i])
        {
            clients_fps[i].sock     = clnt_sock;
            clients_fps[i].read_fp  = fdopen(clients_fps[i].sock, "r");
            clients_fps[i].write_fp = fdopen(dup(clients_fps[i].sock), "w");
        }
    }

    ___FLAG_true(&flag_fp);
    pthread_cond_signal(&fp_cond);
    pthread_mutex_unlock(&fp_mutex);

    // ##################################################
    //  fp_cond and client_mutex lock
    // ##################################################
    pthread_mutex_lock(&client_mutex);
    while (!flag_exit)  // If client exit;
        pthread_cond_wait(&fp_cond, &client_mutex);

    // ###################################################
    //  fp_cond and fp_mutex
    // ###################################################
    pthread_mutex_lock(&fp_mutex);
    for (i = 0; i < client_num; i++)
    {
        if (clients_fps[i].sock == clnt_sock)
        {
            // Half close
            shutdown(fileno(clients_fps[i].write_fp), SHUT_WR);
            fclose(clients_fps[i].write_fp);

            printf(ANSI_FMT("Client exit..................\n", ANSI_FG_RED));

            memmove(&clients_fps[i], &clients_fps[i + 1], (client_num - 1 - i) * sizeof(clients_fps[0]));
        }
    }

    ___FLAG_true(&flag_fp);
    pthread_cond_signal(&fp_cond);
    pthread_mutex_unlock(&fp_mutex);

    ___FLAG_false(&flag_exit);
    pthread_mutex_unlock(&client_mutex);

    return NULL;
}

// Send message to all client
void send_msg(char *msg, int str_len)
{
    int   i        = 0;
    FILE *write_fp = NULL;

    pthread_mutex_lock(&fp_mutex);
    for (i; i < client_num; i++)
    {
        write_fp = clients_fps[i].write_fp;
        fwrite(msg, sizeof(char), str_len, write_fp);
        fflush(write_fp);
    }
    pthread_mutex_unlock(&fp_mutex);

    return;
}

void ___FLAG_false(bool *_f) { *_f = false; }

void ___FLAG_true(bool *_f) { *_f = true; }