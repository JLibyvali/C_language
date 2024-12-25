#include "debug.h"

#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>

int main()
{
    printf(FMT("################## Test open a message queue ##########3\n", YELLOW));
    struct mq_attr mq_attr_set;

    mqd_t          mq_fd = mq_open("/mq_first", O_RDONLY);
    if (mq_fd == -1)
    {
        printf("Error: %d \n", errno);
        error("mq_open()");
    }

    if (mq_getattr(mq_fd, &mq_attr_set) == -1)
        error("mq_getattr()");

    printf("##Maximum of message size in queue: %ld \n", mq_attr_set.mq_msgsize);
    printf("##Maximum of queue size: %ld \n", mq_attr_set.mq_maxmsg);
    printf("##Current message: %ld \n", mq_attr_set.mq_curmsgs);

    mq_unlink("/mq_first");

    return 0;
}