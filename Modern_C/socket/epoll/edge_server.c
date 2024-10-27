
#include "debug.h"
#include "macro.h"

#include <arpa/inet.h>
#include <bits/types/struct_iovec.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <time.h>
#include <unistd.h>

#define BUF_LEN    256
#define EPOLL_SIZE 50
#define READ_LEN   4

int main(int argc, char **argv)
{

    if (argc != 2)
    {
        printf("Usage %s <PORT> \n", argv[0]);
        return 1;
    }

    // iovec for just io once
    char         in_buf[BUF_LEN]  = {};
    struct iovec vecs[1]          = {};
    vecs[0].iov_base              = in_buf;
    vecs[0].iov_len               = BUF_LEN;

    int                 serv_sock, clnt_sock;
    struct sockaddr_in  serv_addr, clnt_addr;
    socklen_t           clnt_addr_len = sizeof(clnt_addr);

    // epoll
    int                 epoll_fd, event_cnt;
    struct epoll_event *ep_buf, cfg_event;

    int                 i, invoked_cnt = 0;

    // socket init.
    serv_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    CHECK_RET(serv_sock, "socket()");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port        = htons(atoi(argv[1]));

    int ret                   = bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    CHECK_RET(ret, "bind()");

    // epoll init
    ep_buf   = malloc(EPOLL_SIZE * sizeof(struct epoll_event));

    epoll_fd = epoll_create(EPOLL_SIZE);
    CHECK_RET(epoll_fd, "epoll_create()");

    // socket transfer to  unblocking io
    int fd_info = fcntl(serv_sock, F_GETFL);
    fcntl(serv_sock, F_SETFL, fd_info | O_NONBLOCK);

    cfg_event.events  = EPOLLIN;  // the monitored epoll event type
    cfg_event.data.fd = serv_sock;
    ret               = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, serv_sock, &cfg_event);
    CHECK_RET(ret, "epoll_ctl()");

    ret = listen(serv_sock, 5);
    CHECK_RET(ret, "listen()");

    printf(ANSI_FMT("Listen...........\n", ANSI_FG_BLUE));
    while (1)
    {
        printf("Waiting.........................\n");

        event_cnt = epoll_wait(epoll_fd, ep_buf, EPOLL_SIZE, -1);
        CHECK_RET(event_cnt, "epoll_wait()");

        printf(
            ANSI_FMT("epoll_wait() invoked times: %d  return Changed fd: %d .\n", ANSI_FG_RED), ++invoked_cnt, event_cnt
        );

        for (i = 0; i < event_cnt; i++)
        {
            if (ep_buf[i].data.fd == serv_sock)
            {
                clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_len);

                // Change socket fd to non-block fd
                fd_info   = -1;
                fd_info   = fcntl(clnt_sock, F_GETFL);
                fcntl(clnt_sock, F_SETFL, fd_info | O_NONBLOCK);

                cfg_event.events  = EPOLLIN | EPOLLET;  // The client socket using Edge trigger handling.

                cfg_event.data.fd = clnt_sock;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clnt_sock, &cfg_event);
                printf(ANSI_FMT("Connected..........\n", ANSI_FG_BLUE));
            }
            else
            {

                // When in Edge trigger , need to read all data in epoll buffer.
                // But for test, leave some data in epoll buffer
                // See what's  client  received.

                // #######################################################3
                //  Result: If leaved data in epoll buffer,
                //  Client  received buffer will have garbage data.
                // ############################################

                // int str_len = read(ep_buf[i].data.fd, buf, READ_LEN);
                int str_len = readv(ep_buf[i].data.fd, vecs, 1);

                if (str_len == 0)  // close connection
                {
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, ep_buf[i].data.fd, NULL);
                    close(ep_buf[i].data.fd);
                    printf(ANSI_FMT("Close client %d .............\n", ANSI_FG_BLUE), ep_buf[i].data.fd);
                    break;
                }
                else if (str_len < 0 && str_len == EAGAIN)
                {
                    break;
                }
                else
                {
                    // write(ep_buf[i].data.fd, buf, READ_LEN);
                    writev(ep_buf[i].data.fd, vecs, 1);
                }
            }
        }
    }

    printf("Exit.............\n");
    close(epoll_fd);
    close(serv_sock);

    return 0;
}