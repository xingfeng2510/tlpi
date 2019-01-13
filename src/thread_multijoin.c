#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common.h"

static pthread_cond_t thread_died = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t thread_mutex = PTHREAD_MUTEX_INITIALIZER;

static int total_threads = 0;
static int num_live = 0;

static int num_unjoined = 0;

enum tstate {
    TS_ALIVE,
    TS_TERMINATED,
    TS_JOINED
};

static struct {
    int idx;
    pthread_t tid;
    enum tstate state;
    int sleep_time;
} *thread;

static void *thread_func(void *arg)
{
    int idx = *((int *) arg);
    int s;

    sleep(thread[idx].sleep_time);
    printf("Thread %d terminating\n", idx);

    s = pthread_mutex_lock(&thread_mutex);
    if (s != 0)
        handle_error_en(s, "pthread_mutex_lock");
    
    num_unjoined++;
    thread[idx].state = TS_TERMINATED;

    s = pthread_mutex_unlock(&thread_mutex);
    if (s != 0)
        handle_error_en(s, "pthread_mutex_unlock");
    s = pthread_cond_signal(&thread_died);
    if (s != 0)
        handle_error_en(s, "pthread_cond_signal");

    return NULL;
}

int main(int argc, char *argv[])
{
    int s, idx;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s: seconds...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    thread = calloc(argc - 1, sizeof(*thread));
    if (thread == NULL)
        handle_error("calloc");

    for (idx = 0; idx < argc - 1; idx++) {
        thread[idx].idx = idx;
        thread[idx].sleep_time = atoi(argv[idx + 1]);
        thread[idx].state = TS_ALIVE;
        s = pthread_create(&thread[idx].tid, NULL, thread_func, &thread[idx].idx);
        if (s != 0)
            handle_error_en(s, "pthread_create");
    }

    total_threads = argc - 1;
    num_live = total_threads;

    while (num_live > 0) {
        s = pthread_mutex_lock(&thread_mutex);
        if (s != 0)
            handle_error_en(s, "pthread_mutex_lock");

        while (num_unjoined == 0) {
            s = pthread_cond_wait(&thread_died, &thread_mutex);
            if (s != 0)
                handle_error_en(s, "pthread_cond_wait");
        }

        for (idx = 0; idx < total_threads; idx++) {
            if (thread[idx].state == TS_TERMINATED) {
                s = pthread_join(thread[idx].tid, NULL);
                if (s != 0)
                    handle_error_en(s, "pthread_join");

                thread[idx].state = TS_JOINED;
                num_live--;
                num_unjoined--;

                printf("Reaped thread %d (num_live=%d)\n", idx, num_live);
            }
        }

        s = pthread_mutex_unlock(&thread_mutex);
        if (s != 0)
            handle_error_en(s, "pthread_mutex_unlock");
    }

    exit(EXIT_SUCCESS);
}
