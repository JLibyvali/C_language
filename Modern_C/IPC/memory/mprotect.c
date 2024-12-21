#include "debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

#define LENGTH 1024 * 1024

int main()
{

    char *addr = mmap(NULL, LENGTH, PROT_NONE, MAP_SHARED| MAP_ANONYMOUS, -1, 0);
    if (addr == MAP_FAILED)
        error("mmap()");

    char cmd[256];
    printf("Before mprotect().\n");
    snprintf(cmd, 256, "cat /proc/%ld/maps | grep zero", (long)getpid());
    system(cmd);

    if (mprotect(addr, LENGTH, PROT_READ | PROT_WRITE) == -1)
        error("mprotect()");

    printf("After mprotect().\n");
    system(cmd);

    return 0;
}