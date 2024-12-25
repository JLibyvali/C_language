#include "debug.h"

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define MEM_SIZE 50
char file_path[] = "/home/jlibyvali/Developer/projetcs/VSCode/C_language/Modern_C/IPC/memory/data.shared";

/**
 * @brief It's also a kind of IPC named Memory IO
 *
 * @param argc
 * @param argv
 * @return int
 */
int  main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <shared data> .\n", argv[0]);
        return 0;
    }

    int ffd;
    ffd = open(file_path, O_RDWR);
    if (ffd == -1)
        error("open()");

    void *addr = mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, ffd, 0);
    if (addr == MAP_FAILED)
        error("mmap()");

    close(ffd);

    printf("Current string in shared data file: %.*s, Will do copy work .\n", MEM_SIZE, (char *)addr);
    memset(addr, 0, MEM_SIZE);

    strncpy(addr, argv[1], MEM_SIZE - 1);

    if (msync(addr, MEM_SIZE, MS_SYNC) == -1)
        error("msyn()");

    return 0;
}