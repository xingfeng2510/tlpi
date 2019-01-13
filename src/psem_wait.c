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
    int flags;
    mode_t perms;
    unsigned int value;
    sem_t *sem;

    if (argc < 3 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s name value\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    flags = O_CREAT | O_EXCL;
    perms = S_IRUSR | S_IWUSR;
    value = atoi(argv[2]);
    sem = sem_open(argv[1], flags, perms, value);
    if (sem == SEM_FAILED)
        handle_error("sem_open");

    if (sem_wait(sem) == -1)
        handle_error("sem_wait");

    printf("%ld sem_wait() succeeded\n", (long) getpid());
    
    exit(EXIT_SUCCESS);
}
