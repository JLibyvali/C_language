#include "debug.h"

#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

int main()
{

    printf(FMT("############## Test Create Shared Memory ##########\n", YELLOW));
    int    flags  = O_CREAT | O_EXCL | O_RDWR;
    mode_t perms  = S_IRUSR | S_IWUSR;

    int    shm_fd = shm_open("/shm_first", flags, perms);
    if (shm_fd == -1)
        error("shm_open()");

    if (ftruncate(shm_fd, 4096) == -1)
        error("ftruncate()");

    void *addr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (addr == MAP_FAILED)
        error("mmap()");

    close(shm_fd);
    munmap(addr, 4096);

    return 0;
}