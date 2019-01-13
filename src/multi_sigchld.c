#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#include "common.h"

static void sigchld_handler(int sig)
{
    printf("handler: caught SIGCHLD\n");

    sleep(5);
    printf("handler: returning\n");
}

int main(int argc, char *argv[])
{
    int i, sigcnt;
    sigset_t block_mask, empty_mask;
    struct sigaction sa;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "Usage: %s child-sleep-time...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    setbuf(stdout, NULL);

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigchld_handler;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
        handle_error("sigaction");

    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGCHLD);
    if (sigprocmask(SIG_SETMASK, &block_mask, NULL) == -1)
        handle_error("sigprocmask");
    
    for (i = 1; i< argc; i++) {
        switch (fork()) {
        case -1:
            handle_error("fork");

        case 0:
            sleep(atoi(argv[i]));
            printf("Child %d (PID=%ld) exiting\n", i, (long) getpid());
            _exit(EXIT_SUCCESS);

        default:
            break;
        }
    }
    
    sigemptyset(&empty_mask);
    for (;;) {
        if (sigsuspend(&empty_mask) == -1 && errno != EINTR)
            handle_error("sigsuspend");
        sigcnt++;
    }
    exit(EXIT_SUCCESS);
}
