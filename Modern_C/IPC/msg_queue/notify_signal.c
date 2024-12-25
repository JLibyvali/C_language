#include "debug.h"

#include <bits/types/sigset_t.h>
#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Using signal to notify process that the `message queue` becoming Un-empty queue.
 *
 * @return int
 */

#define NOTIFY_SIG SIGUSR1

static void notify(int sig)  // Using for interrupt `sigsuspend()`
{

    printf("An event occurred! \n");
    return;
}

int main()
{
    // signal
    sigevent_t       sig_event;
    struct sigaction sig_sa;
    sigset_t         blockMask, emptyMask;

    if (sigemptyset(&blockMask) == -1)
        error("sigemptyset()");
    if (sigaddset(&blockMask, NOTIFY_SIG) == -1)
        error("sigaddset()");
    // stop SIGUSR_1 to this process first
    if (sigprocmask(SIG_BLOCK, &blockMask, NULL) == -1)
        error("sigprocmask()");

    // set sigaction_t
    if (sigemptyset(&sig_sa.sa_mask) == -1)
        error("sigemptyset()");
    sig_sa.sa_flags   = 0;
    sig_sa.sa_handler = notify;
    if (sigaction(NOTIFY_SIG, &sig_sa, NULL) == -1)
        error("sigaction()");

    sig_event.sigev_notify = SIGEV_SIGNAL;  // event action
    sig_event.sigev_signo  = NOTIFY_SIG;

    // message queue
    mqd_t          mq_fd;
    int            mq_flags = O_RDONLY | O_NONBLOCK, read_nbytes;
    struct mq_attr mq_attrs;

    mq_fd = mq_open("/mymq", mq_flags);
    if (mq_fd == -1)
        error("mq_open()");

    if (mq_getattr(mq_fd, &mq_attrs) == -1)
        error("mq_getattr()");

    void *buffer = malloc(mq_attrs.mq_msgsize + 10);
    // Register this process to be notified.
    // `mq_notify()` will send notification signal, and delete signal register information.
    if (mq_notify(mq_fd, &sig_event) == -1)
        error("mq_notify()");

    sigemptyset(&emptyMask);  // using for `sigsuspend()`
    while (1)
    {

        // Blocking until process original signal handler return;
        printf(FMT("#### Waiting For message queue be notified #####\n", YELLOW));
        sigsuspend(&emptyMask);

        // Register process again.
        if (mq_notify(mq_fd, &sig_event) == -1)
            error("mq_notify");

        /**
         * @brief The order is IMPORTANT, Must register process first, then read message
         * IF inverse, When After reading all message from queue, before invoke `mq_notifY()`,
         * if there arrive a message, and then register process. `mq_notify()` will never send notification cause
         * message queue is not empty. And `sigsuspend()` will always keep  blocking .
         *
         */

        // Try read most message from queue, then every new message will trigger a event notification.
        while ((read_nbytes = mq_receive(mq_fd, buffer, 1024, NULL)) >= 0)
            printf("READ %d BYTES.\n", read_nbytes);
        printf("RECEIVE Message: %s \n", (char*)buffer);

        // Open message queue with `O_NOBLOCKING`, if there are no more messages, `mq_receive()` will set errno to
        // EAGAIN.
        if (errno != EAGAIN)
            error("mq_receive");
    }

    return 0;
}