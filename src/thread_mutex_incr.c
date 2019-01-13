#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "common.h"

static int glob = 0;
static pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;

static void *thread_func(void *arg)
{
    int loops = *((int *) arg);
    int loc, i, s;

    for (i = 0; i < loops; i++) {
        s = pthread_mutex_lock(&mu);
        if (s != 0)
            handle_error_en(s, "pthread_mutex_lock");

        loc = glob;
        loc++;
        glob = loc;

        s = pthread_mutex_unlock(&mu);
        if (s != 0)
            handle_error_en(s, "pthread_mutex_unlock");
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t t1, t2;
    int loops, s;

    loops = (argc > 1) ? atoi(argv[1]) : 1000;

    s = pthread_create(&t1, NULL, thread_func, &loops);
    if (s != 0)
        handle_error_en(s, "pthread_create");
    s = pthread_create(&t2, NULL, thread_func, &loops);
    if (s != 0)
        handle_error_en(s, "pthread_create");

    s = pthread_join(t1, NULL);
    if (s != 0)
        handle_error_en(s, "pthread_join");
    s = pthread_join(t2, NULL);
    if (s != 0)
        handle_error_en(s, "pthread_join");

    printf("glob=%d\n", glob);
    pause();
    exit(EXIT_SUCCESS);
}
