#include "debug.h"

#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{

    printf(FMT("############# Test receive a message ################# \n", YELLOW));
    unsigned int msg_privo;
    int          flags = O_RDONLY;
    mqd_t        mq_fd = mq_open("/mq_first", flags);
    if (mq_fd == -1)
        error("mq_open");

    // get message queue attributions
    struct mq_attr mq_attrs;
    if (mq_getattr(mq_fd, &mq_attrs) == -1)
        error("mq_getattr()");

    char *buffer = malloc(mq_attrs.mq_msgsize + 2);

    int   nbytes = mq_receive(mq_fd, buffer, mq_attrs.mq_msgsize, &msg_privo);
    if (nbytes == -1)
        error("mq_receive()");

    buffer[mq_attrs.mq_msgsize] = '\n';
    fwrite(buffer, mq_attrs.mq_msgsize, 1, stdout);
    fflush(stdout);

    return 0;
}