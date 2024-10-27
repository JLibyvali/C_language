#include "macro.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

pthread_mutex_t  mutex;
static long long sum = 0;

void            *thread_inc(void *params)
{

    int i;

    for (i = 0; i < 1000000; i++)
    {

        pthread_mutex_lock(&mutex);
        sum += 1;
        pthread_mutex_unlock(&mutex);
    }

    char *msg = (char *)malloc(128 * sizeof(char));
    strcpy(msg, ANSI_FMT("Thread1 return ", ANSI_FG_RED));
    return (void *)msg;
}

void *thread_dec(void *params)
{

    int i;

    for (i = 0; i < 1000000; i++)
    {
        pthread_mutex_lock(&mutex);
        sum -= 1;
        pthread_mutex_unlock(&mutex);
    }

    char *msg = (char *)malloc(128 * sizeof(char));
    strcpy(msg, ANSI_FMT("Thread2  return ", ANSI_FG_RED));
    return (void *)msg;
}

int main()
{
    pthread_t th_ids[10] = {};
    int       loop_cnt   = 5, i;
    void     *th_state;

    // init mutex
    pthread_mutex_init(&mutex, NULL);

    for (i = 0; i < 10; i++)
    {
        if (i & 1)
            pthread_create(&th_ids[i], NULL, thread_dec, NULL);
        else
            pthread_create(&th_ids[i], NULL, thread_inc, NULL);
    }

    for (i = 0; i < 10; i++)
        pthread_join(th_ids[i], NULL);

    // Main task have to  leave enough time for sub  threads.
    // printf("Threads return value: %s \n", (char *)th_state);

    printf("Res: %lld \n", sum);
    printf("End main tasks.\n");

    // free(th_state);
    pthread_mutex_destroy(&mutex);
    return 0;
}