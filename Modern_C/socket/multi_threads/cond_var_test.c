#include "macro.h"

#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

pthread_mutex_t mutex;
pthread_cond_t  cond;
int             ready = 0;

void           *wait_thread(void *arg)
{
    pthread_mutex_lock(&mutex);
    // while (!ready)
    {  // Loop to handle spurious wakeups
        printf(ANSI_FMT("Thread waiting for condition...\n", ANSI_FG_BLUE));
        pthread_cond_wait(&cond, &mutex);
    }

    printf(ANSI_FMT("Get mutex lock  AND Condition variable turn into TRUE \n", ANSI_FG_RED));
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void *signal_thread(void *arg)
{
    pthread_mutex_lock(&mutex);
    ready = 1;  // Change the condition
    printf(ANSI_FMT("\"ready\" Changed, after 2 secs \n", ANSI_FG_BLUE));
    sleep(2);

    pthread_cond_signal(&cond);  // Signal the waiting thread
    printf(ANSI_FMT("Signaled condition variable SEND AND free lock \n", ANSI_FG_BLACK));

    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main()
{
    pthread_t t1, t2;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&t1, NULL, wait_thread, NULL);
    pthread_create(&t2, NULL, signal_thread, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}
