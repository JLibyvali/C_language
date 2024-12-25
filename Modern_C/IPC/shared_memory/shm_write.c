#include "debug.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    printf(FMT("############ Test Write Shared Memory ########\n", YELLOW));
    if (argc < 2)
    {
        printf("Usage: %s <string> \n", argv[0]);
        return -1;
    }
    int fd;
    fd = shm_open("/shm_first", O_RDWR, 0);
    if (fd == -1)
        error("shm_open()");

    int len = strlen(argv[1]);

    if (ftruncate(fd, len) == -1)
        error("ftruncate()");

    char *addr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
        error("mmap()");

    close(fd);
    strcpy(addr, argv[1]);
    munmap(addr, len);

    return 0;
}