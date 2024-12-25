#include "debug.h"

#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int    flags     = O_CREAT | O_EXCL;
    int    value     = 2;
    mode_t sem_perms = S_IRUSR | S_IWUSR;

    // semaphore create and  initialize is atomic
    // It's named semaphore
    sem_t *sem       = sem_open("/sem1", flags, sem_perms, value);
    if (sem == SEM_FAILED)
        error("sem_open()");

    sem_close(sem);

    return 0;
}