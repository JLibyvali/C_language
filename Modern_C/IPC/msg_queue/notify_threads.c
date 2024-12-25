#include "debug.h"

#include <bits/types/sigevent_t.h>
#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void notify_init(mqd_t *_fd_ptr);

static int  func(sigval_t *_sv)
{
    int            read_bytes;
    mqd_t         *mq_fdp;
    struct mq_attr attrs;

    // parse event information
    mq_fdp = _sv->sival_ptr;

    if (mq_getattr(*mq_fdp, &attrs) == -1)
        error("mq_getattr()");

    void *buffer = malloc(attrs.mq_msgsize + 10);

    // register notification event again
    notify_init(mq_fdp);

    while ((read_bytes = mq_receive(*mq_fdp, buffer, attrs.mq_msgsize, NULL)) >= 0)
        printf("READ %d BYTES .\n", read_bytes);

    if (errno == EAGAIN)
        error("mq_receive()");

    free(buffer);
    pthread_exit(NULL);
}

static void notify_init(mqd_t *_fd_ptr)
{

    printf("Register threads agian!! \n");
    // set signal event and  register thread function
    sigevent_t sevent;

    // When event occurred, will create a thread with given function pointer.
    sevent.sigev_notify            = SIGEV_THREAD;
    sevent.sigev_notify_function   = func;
    sevent.sigev_notify_attributes = NULL;  // or thread  attributions information
    sevent.sigev_value.sival_ptr   = _fd_ptr;

    if (mq_notify(*_fd_ptr, &sevent) == -1)
        return;
}

int main()
{

    mqd_t mq_fd;

    if (mq_open("/mymq", O_RDONLY | O_NONBLOCK) == -1)
        error("mq_open()");

    notify_init(&mq_fd);

    // main threads always keep blocking,
    // action complete in another threads.
    pause();
    return 0;
}