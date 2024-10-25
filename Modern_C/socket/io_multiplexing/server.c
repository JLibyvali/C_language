#define _XOPEN_SOURCE 700

#include "debug.h"
#include "macro.h"

#include <bits/types/struct_timeval.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_LEN 256

int main(int argc, char **argv)
{

    if (argc != 2)
    {
        printf("Usage %s <IP> \n");
        return 1;
    }

    int                serv_sock, clnt_sock = -1, i;
    struct sockaddr_in ser_addr, clnt_addr;
    socklen_t          clnt_addr_len = sizeof(clnt_addr);

    // Using Standard IO function to dividing io, more buffer, may more high performance
    FILE              *read_fp = NULL, *write_fp = NULL;

    // Using select() to reduce the multiprocess coding
    fd_set             reads, cpy_reads;
    int                fd_max = -1;
    struct timeval     timeout;

    char               buf[BUF_LEN] = {};

    // socket init.
    serv_sock                       = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    CHECK_RET(serv_sock, "socket()");

    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family      = AF_INET;
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    ser_addr.sin_port        = htons(atoi(argv[1]));

    int ret                  = bind(serv_sock, (struct sockaddr *)&ser_addr, sizeof(ser_addr));
    CHECK_RET(ret, "bind()");

    ret = listen(serv_sock, 5);
    CHECK_RET(ret, "listen()");

    printf(ANSI_FMT("Listening.............\n", ANSI_FG_BLACK));

    // init fd_set for `select()`
    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    fd_max          = serv_sock;
    timeout.tv_sec  = 5;
    timeout.tv_usec = 0;

    while (1)
    {
        // select() to  watch many file descriptors
        cpy_reads  = reads;
        int fd_num = select(fd_max + 1, &cpy_reads, NULL, NULL, &timeout);
        CHECK_RET(fd_num, "select()");

        if (!fd_num)
            continue;

        for (i = 0; i < fd_max + 1; i++)
        {
            // FD must be  watched by select()
            if (FD_ISSET(i, &cpy_reads))
            {
                // The serv_sock has change, trigger the select()
                if (i == serv_sock)
                {
                    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_len);
                    CHECK_RET(clnt_sock, "accept()");

                    FD_SET(clnt_sock, &reads);

                    // Update fd_max for next select()
                    if (clnt_sock > fd_max)
                        fd_max = clnt_sock;

                    printf(ANSI_FMT("Connected..........\n", ANSI_FG_BLACK));
                }

                // Handling other event. from clnt_sock
                // USing FILE* to read and write.
                read_fp  = fdopen(clnt_sock, "r");

                // #########################
                //  copied fd to half_closed
                //  read_fp --> clnt_sock       --|
                //                               (socket)
                //  write_fp --> dup(clnt_sock) --|
                // #################################33
                write_fp = fdopen(dup(clnt_sock), "w");

                if (!read_fp || !write_fp)
                    Error("fdopen()");

                fgets(buf, BUF_LEN, read_fp);
                if (feof(read_fp))
                {
                    // Clear resources Half_closed
                    shutdown(fileno(write_fp), SHUT_WR);
                    fclose(write_fp);

                    fgets(buf, BUF_LEN, read_fp);
                    fclose(read_fp);
                    FD_CLR(clnt_sock, &reads);
                    close(i);

                    printf(ANSI_FG_BLUE "END message: %s \n" ANSI_NONE, buf);
                    printf("Client %d exit \n", i);
                    close(serv_sock);
                    return 0;
                }

                while (!feof(read_fp))
                {
                    fputs(buf, write_fp);
                    fflush(write_fp);  // Ensure IO function buffer is clear.

                    fgets(buf, BUF_LEN, read_fp);
                }
            }
        }
    }

    close(serv_sock);

    return 0;
}
