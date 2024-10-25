#include "debug.h"

#include <bits/types/struct_timeval.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>

#define BUF_LEN 256

int main()
{
    fd_set         reads, temps;
    int            fd_num       = (0 + 1);  // stdin 0
    char           buf[BUF_LEN] = {};

    struct timeval timeout;

    // Init for select()
    FD_ZERO(&reads);
    FD_SET(0, &reads);
    timeout.tv_sec  = 5;
    timeout.tv_usec = 0;

    while (1)
    {
        temps   = reads;
        int ret = select(fd_num, &temps, NULL, NULL, &timeout);
        CHECK_RET(ret, "select()");

        if (ret == 0)
        {
            printf("Time out of select() \n");
            break;
        }

        if (FD_ISSET(0, &temps))
        {
            int str_len  = read(0, buf, BUF_LEN);
            buf[str_len] = '\0';

            printf("Message from stdin : %s \n", buf);
        }
    }

    return 0;
}