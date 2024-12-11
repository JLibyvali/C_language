#include "debug.h"
#include "fifo_msg.h"

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

static void rm_fifo_cb(void) { unlink(SERVER_FIFO); }

int         main()
{

    int      serv_fd, clnt_fd, dummy_fd;
    request  req;
    response resp;
    int      resp_order = 0;

    char     client_fifo[CLIENT_FIFO_LEN];

    atexit(rm_fifo_cb);

    // create server fifo
    umask(0);
    if (mkfifo(SERVER_FIFO, S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno == EEXIST)
        error("mkfifo() error, fifo file existed.");

    if ((serv_fd = open(SERVER_FIFO, O_RDONLY)) == -1)
        error("open server read process failed.");

    FILE *serv_rptr = fdopen(serv_fd, "r");

    // open extra write fd, so that never see `EOF` when all clients closed.
    if ((dummy_fd = open(SERVER_FIFO, O_WRONLY)) == -1)
        error("open() server write process failed.");

    // ignore `SIG_PIPE` signal, when write to an un-existed fifo. forbid that signal kill process.
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
        error("signal) set SIG_IGN error.");

    // running server.
    int times = 0;
    while (1)
    {
        if (fread(&req, sizeof(request), 1, serv_rptr) != 1)
        {
            fprintf(stderr, "fread() error when reading request.");
            continue;
        }

        printf("Get request from %d, times: %d.\n", req.m_pid, times++);

        // get client fifo path
        snprintf(client_fifo, CLIENT_FIFO_LEN, CLINET_FIFO, (long)req.m_pid);

        clnt_fd         = open(client_fifo, O_WRONLY);
        FILE *clnt_wptr = fdopen(clnt_fd, "w");

        resp.m_seqlen   = resp_order;

        if (fwrite(&resp, sizeof(response), 1, clnt_wptr) != 1)
            fprintf(stderr, "Write client fifo %s error.", client_fifo);
        if (fclose(clnt_wptr) == -1)
            error("close client write pointer error.");

        resp_order += req.m_seqlen;
    }

    exit(EXIT_SUCCESS);
}
