#include "debug.h"

#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    printf(FMT("############# Test Send a Message ########### \n", YELLOW));
    unsigned int msg_privo = 1;
    int          msg_flags = O_WRONLY;  // default is blocking

    // open a message queue
    mqd_t        mq_fd     = mq_open("/mymq", msg_flags);
    if (mq_fd == -1)
        error("mq_open()");

    if (mq_send(mq_fd, argv[1], strlen(argv[1]) + 1, msg_privo) == -1)
        error("mq_end()");

    return 0;
}