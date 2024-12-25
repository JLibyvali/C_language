#include "debug.h"

#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{

    printf(FMT("########## Test Read shared memory ####### \n", YELLOW));
    int fd = shm_open("/shm_first", O_RDONLY, 0);
    if (fd == -1)
        error("shm_open()");

    struct stat state;

    if (fstat(fd, &state) == -1)
        error("fstat()");

    char *addr = mmap(NULL, state.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
        error("mmap()");

    close(fd);
    fwrite(addr, state.st_size, 1, stdout);
    munmap(addr, state.st_size);
    

    return 0;
}