#include "debug.h"

#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>

int main()
{
    mqd_t          mq_fd;
    mode_t         perms;
    struct mq_attr mq_attr_set, *mq_attr_ptr;
    int            mq_flags;

    // init message queue's attribution
    mq_attr_set.mq_msgsize = 1024;
    mq_attr_set.mq_maxmsg  = 10;
    mq_attr_ptr            = &mq_attr_set;

    mq_flags               = O_CREAT | O_EXCL;
    perms                  = S_IWUSR | S_IRUSR;

    mq_fd                  = mq_open("/mq_first", mq_flags, perms, &mq_attr_set);
    if (mq_fd == (mqd_t)-1)
    {
        printf("Error: %d \n", errno);
        error("mq_open()");
    }

    mq_close(mq_fd);

    return 0;
}