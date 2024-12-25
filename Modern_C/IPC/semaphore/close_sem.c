#include "debug.h"

#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{

    printf(FMT("#### Test Close and delete a semaphore ##########\n", YELLOW));

    sem_t *sem = sem_open("/sem1", O_RDONLY);
    if (sem == SEM_FAILED)
        error("sem_open()");

    sem_close(sem);

    if (sem_unlink("sem1") == -1)
        error("sem_unlink()");

    return 0;
}