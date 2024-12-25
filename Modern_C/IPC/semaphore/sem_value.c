#include "debug.h"

#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf(FMT("######### Test Semaphore Value ######\n", YELLOW));
    char   sem_name[] = "/sem1";
    sem_t *sem        = sem_open(sem_name, O_RDWR);
    int value;

    sem_getvalue(sem, &value);
    printf("Current semaphore value: %d \n", value);

    sem_wait(sem);
    sem_getvalue(sem, &value);
    printf("Current semaphore value: %d \n", value);
    sem_wait(sem);
    sem_getvalue(sem, &value);
    printf("Current semaphore value: %d \n", value);

    sem_post(sem);
    sem_getvalue(sem, &value);
    printf("Current semaphore value: %d \n", value);

    sem_close(sem);

    return 0;
}