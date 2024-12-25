#include "debug.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <unistd.h>

int main()
{

    // create a file
    int fd = open("./lock.data", O_CREAT | O_RDWR);
    if (fd == -1)
        error("open()");

    // file lock
    int flags = LOCK_EX | LOCK_NB;
    if (flock(fd, flags) == -1)
    {
        if (errno == EWOULDBLOCK)
            error("flock(), Already Has process catch the file ");
    }
    printf("Process: %d get file lock.\n", getpid());

    // Initialize file
    if (ftruncate(fd, 8192) == -1)
        error("ftruncate()");

    char *addr = mmap(NULL, 8192, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
        error("mmap()");

    char buffer[] = "Write Information Continued: id-%d \n";
    int  len      = sizeof(buffer);

    for (int i = 0; i < 10; i++)
    {
        sleep(2);
        snprintf(addr + (i * len) + 1, len + 1, buffer, i);
    }
    msync(addr, 8192, MS_SYNC);
    munmap(addr, 8192);

    return 0;
}