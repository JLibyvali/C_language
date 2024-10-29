#include "macro.h"

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// Synchronous programming
// using mutex lock
pthread_mutex_t  mutex;
static long long sum = 0;

// using  semaphore to synchronous
static sem_t     sem_one;
static sem_t     sem_two;

void            *thread_inc(void *params)
{

    int i;

    sem_wait(&sem_one);
    for (i = 0; i < 0x3f3f3f3f; i++)
    {
        sum += 1;
    }
    sem_post(&sem_two);

    char *msg = (char *)malloc(128 * sizeof(char));
    strcpy(msg, ANSI_FMT("Thread1 return ", ANSI_FG_RED));
    return (void *)msg;
}

void *thread_dec(void *params)
{

    int i;

    sem_wait(&sem_two);
    for (i = 0; i < 0x3f3f3f3f; i++)
    {
        sum -= 1;
    }
    sem_post(&sem_one);

    char *msg = (char *)malloc(128 * sizeof(char));
    strcpy(msg, ANSI_FMT("Thread2  return ", ANSI_FG_BLUE));
    return (void *)msg;
}

int main()
{
    pthread_t th_ids[10]   = {};
    int       loop_cnt     = 5, i;
    void     *th_state[10] = {};

    // init mutex
    pthread_mutex_init(&mutex, NULL);

    // init semaphore
    sem_init(&sem_one, 0, 1);  // Semaphore init value is 1
    sem_init(&sem_two, 0, 0);

    for (i = 0; i < 10; i++)
    {
        if (i & 1)
            pthread_create(&th_ids[i], NULL, thread_dec, NULL);
        else
            pthread_create(&th_ids[i], NULL, thread_inc, NULL);
    }

    for (i = 0; i < 10; i++)
    {
        pthread_join(th_ids[i], &th_state[i]);

        printf("Threads return value: %s, address: %p \n", (char *)th_state[i], th_state[i]);
    }

    // Main task have to  leave enough time for sub  threads.
    // sleep(10);

    printf("After threads operations Res: %lld \n", sum);
    printf("End main tasks.\n");

    pthread_mutex_destroy(&mutex);
    sem_destroy(&sem_one);
    sem_destroy(&sem_two);
    return 0;
}