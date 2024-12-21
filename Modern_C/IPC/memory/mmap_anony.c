#include "debug.h"

#include <stdio.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main()
{

    pid_t child;

    int  *addr = mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (addr == MAP_FAILED)
        error("mmap()");

    child = fork();

    *addr = 11;
    if (child)
    {
        wait(NULL);
        printf("Value in parent process: %d\n", *addr);
        munmap(addr, 1024);
    }
    else
    {
        (*addr) += 22;
        munmap(addr, 1024);
        _exit(0);
    }

    return 0;
}