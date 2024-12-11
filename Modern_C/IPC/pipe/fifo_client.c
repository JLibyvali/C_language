#include "debug.h"
#include "fifo_msg.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static char client_fifo[CLIENT_FIFO_LEN];

static void rm_fifo_cb(void) { unlink(client_fifo); }

int         main(int argc, char *argv[])
{

    if (argc <= 1 || (argc > 1 && strcmp(argv[1], "-h") == 0))
        error("Usage: client [seq-len..]\n");

    // fifo naturally keep synchronous.
    pid_t childs[10];
    for (int i = 0; i < 3; i++)
    {
        pid_t first = fork();
        if (!first)
            childs[i] = first;
    }

    int      serv_fd, clnt_fd;
    request  req;
    response resp;

    // create client fifo
    umask(0);
    snprintf(client_fifo, CLIENT_FIFO_LEN, CLINET_FIFO, (long)getpid());
    if (mkfifo(client_fifo, S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno == EEXIST)
        error("mkfifo() error when making: ");

    if (atexit(rm_fifo_cb) != 0)
        error("error when setting exit handler.");

    // begin write
    req.m_pid    = getpid();
    req.m_seqlen = atoi(argv[1]);

    serv_fd      = open(SERVER_FIFO, O_WRONLY);
    FILE *w_ptr  = fdopen(serv_fd, "w");
    if (fwrite(&req, sizeof(request), 1, w_ptr) != 1)
        error("Can't write message into fifo.");
    fclose(w_ptr);

    clnt_fd     = open(client_fifo, O_RDONLY);
    FILE *r_ptr = fdopen(clnt_fd, "r");
    if (fread(&resp, sizeof(response), 1, r_ptr) != 1)
        error("Can't read message from fifo.");

    printf(YELLOW "%d READ response len: %d .\n" NONE, getpid(), resp.m_seqlen);
    exit(EXIT_SUCCESS);
}