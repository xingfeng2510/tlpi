#include <unistd.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "common.h"

pid_t gettid() 
{ 
    return syscall(__NR_gettid); 
}

static void *child_func(void *arg)
{
    int tno = *((int *) arg);
    printf("%d: thread %ld terminating\n", tno, (long) gettid());
}

int main(int argc, char *argv[])
{
    int nthread;
    int *tnop;
    int s;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s: thread-count\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    nthread = atoi(argv[1]);
    tnop = malloc(nthread * sizeof(int));

    for (int i = 0; i < nthread; i++) {
        pthread_t t;
        tnop[i] = i;
        s = pthread_create(&t, NULL, child_func, &tnop[i]);
        if (s != 0)
            handle_error_en(s, "pthread_create");
        s = pthread_detach(t);
        if (s != 0)
            handle_error_en(s, "pthread_detach");
        if (i != 0 && i % 100 == 0)
            sleep(1);
    }
    pause();

    exit(EXIT_SUCCESS);
}
