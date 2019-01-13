#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "common.h"

int main(int argc, char *argv[])
{
    int num_dead;
    pid_t cid;
    int i, w;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "Usage: %s sleep-time...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    setbuf(stdout, NULL);

    for (i = 1; i < argc; i++) {
        switch (fork()) {
        case -1:
            handle_error("fork");

        case 0:
            printf("Child %d started with PID %ld, sleeping %s seconds\n",
                    i, (long) getpid(), argv[i]);
            sleep(atoi(argv[i]));
            _exit(EXIT_SUCCESS);

        default:
            break;
        }
    }
    
    num_dead = 0;
    for (;;) {
        w = wait(NULL);
        if (w == -1) {
            if (errno == ECHILD) {
                printf("No more children - bye\n");
                exit(EXIT_SUCCESS);
            } else {
                handle_error("wait");
            }
        }

        num_dead++;
        printf("Wait() returned child PID %ld (num_dead=%d)\n",
                (long) w, num_dead);
    }
}
