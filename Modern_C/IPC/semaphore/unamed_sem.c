#include "debug.h"

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static sem_t sem;
static int   global = 0;

static void *func(void *arg)
{
    int loops = *((int *)arg);
    int local;

    for (int i = 0; i < loops; i++)
    {

        if (sem_wait(&sem) == -1)
            return NULL;

        local = global;
        global++;
        global = local;

        if (sem_post(&sem) == -1)
            return NULL;
    }

    return NULL;
}

/**
 * @brief  POSIX semaphore compared to sysV semaphore:
 * POSIX semaphore is more easy to use, better performance when `sem+wait()` don't need blocking,
 *
 */
int main()
{
    printf(FMT("######### Test Unamed semaphore ############# \n", YELLOW));
    pthread_t p1, p2;
    int       loops = 10000;

    if (sem_init(&sem, 0, 1) == -1)
        error("sem_init()");

    int ret = pthread_create(&p1, NULL, func, &loops);
    if (ret == -1)
        error("pthread_create()");

    ret = pthread_create(&p2, NULL, func, &loops);
    if (ret == -1)
        error("pthread_create()");

    ret = pthread_join(p1, NULL);
    ret = pthread_join(p2, NULL);

    printf("Global Value: %d \n", global);

    sem_destroy(&sem);  // Destroy should ahead of lower memory resource released.

    return 0;
}