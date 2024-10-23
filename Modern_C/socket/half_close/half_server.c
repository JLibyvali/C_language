#include "debug.h"
#include "macro.h"

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MSG 256
static const char *file = "buffer.txt";

int                main(int argc, char **argv)
{

    if (argc != 2)
    {
        printf("Usage %s <PROT>.\n", argv[0]);
        return 1;
    }
    int                serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t          clnt_addr_len = sizeof(clnt_addr);
    FILE              *fp            = NULL;
    char               msg[MSG]      = {};

    // socket init.
    serv_sock                        = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serv_sock == -1)
    {
        Error("socket()");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port        = htons(atoi(argv[1]));

    int ret                   = bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (ret == -1)
    {
        Error("bind()");
    }

    // init buffer files.
    fp = fopen(file, "w+");
    if (fp == NULL)
        Error("fopen()");

    ret          = -1;
    char hello[] = "Test First message.\n";
    ret          = fwrite(hello, 1, (sizeof(hello) / sizeof(hello[0])), fp);
    if (ret != (sizeof(hello) / sizeof(hello[0])))
    {
        Error("fwrite()");
    }

    ret = -1;
    ret = listen(serv_sock, 5);
    if (ret == -1)
    {
        Error("listen()");
    }

    printf(ANSI_FG_BLUE "Listening connection.....................\n" ANSI_NONE);

    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_len);
    ret       = -1;
    while (1)
    {
        // First read data from client.
        ret = read(clnt_sock, msg, MSG - 1);
        if (ret == -1)
        {
            Error("read()");
        }
        else if (ret == 0)  // Connect closed.
        {
            break;
        }
        else
        {
            msg[ret] = '\0';  // Safe for `strtok()` with null-terminated string.
            printf(ANSI_FG_BLUE "Received %d bytes: %s \n" ANSI_NONE, ret, msg);
        }

        // Write data  into buffer file.
        fwrite(msg, 1, ret, fp);
        fflush(fp);

        // Read all data in file and write it into socket.
        fseek(fp, 0, SEEK_END);
        size_t filesize = ftell(fp);  // Get current stream position, which is also the file size.
        rewind(fp);

        char *tmp = (char *)malloc(filesize + 1);
        if (!tmp)
        {
            close(serv_sock);
            close(clnt_sock);
            fclose(fp);
            Error("malloc()");
        }

        ret = fread(tmp, 1, filesize, fp);
        if (ret != filesize)
        {
            fclose(fp);
            Error("fread()");
        }

        tmp[filesize] = '\0';  // Treat as string.

        write(clnt_sock, tmp, (filesize + 1));
        free(tmp);

        // Quit handler.
        char *token = strtok(msg, " ");
        if (!token)
            Error("strtok()");

        if (!strcmp(token, "q\n") || !strcmp(token, "FIN"))
            break;
    }
    write(clnt_sock, "EOF", 3);

    shutdown(serv_sock, SHUT_WR);
    printf(ANSI_FG_BLUE "Half closed.\n" ANSI_NONE);

    // After half-closed received data.
    read(clnt_sock, msg, MSG);
    printf(ANSI_FG_BLUE "End message: %s.\n" ANSI_NONE, msg);

    fclose(fp);
    close(serv_sock);
    close(clnt_sock);

    return 0;
}