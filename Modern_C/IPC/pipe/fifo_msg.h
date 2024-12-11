#ifndef FIFO_MSG__H
#define FIFO_MSG__H

#include <time.h>
#include <string.h>

#define SERVER_FIFO     "/tmp/server_fifo"
#define CLINET_FIFO     "/tmp/client_fifo.%ld"
#define CLIENT_FIFO_LEN (strlen(CLINET_FIFO) + 20)

struct request
{
    pid_t m_pid;
    int   m_seqlen;
};
typedef struct request request;

struct response
{
    int m_seqlen;
};
typedef struct response response;

#endif