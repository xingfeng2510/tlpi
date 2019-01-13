#include <unistd.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#include "common.h"

int main(int argc, char *argv[])
{
    int value;
    sem_t *sem;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s sem-name\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    sem = sem_open(argv[1], 0);
    if (sem == SEM_FAILED)
        handle_error("sem_open");

    if (sem_post(sem) == -1)
        handle_error("sem_post");

    if (sem_getvalue(sem, &value) == -1)
        handle_error("sem_getvalue");

    printf("%d\n", value);
    
    exit(EXIT_SUCCESS);
}
