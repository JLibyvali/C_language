#ifndef FIFO_MSG__H
#define FIFO_MSG__H

#include <string.h>
#include <time.h>

#define SERVER_FIFO     "/tmp/server_fifo"
#define CLINET_FIFO     "/tmp/client_fifo.%ld"
#define CLIENT_FIFO_LEN (50 + 20)

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