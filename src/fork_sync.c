#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#include "common.h"

#define SYNC_SIG SIGUSR1

static void handler(int sig)
{
    // Does nothing but return
}

int main(int argc, char *argv[])
{
    pid_t cid;
    sigset_t block_mask, orig_mask, empty_mask;
    struct sigaction sa;

    setbuf(stdout, NULL);

    sigemptyset(&block_mask);
    sigaddset(&block_mask, SYNC_SIG);
    if (sigprocmask(SIG_BLOCK, &block_mask, &orig_mask) == -1)
        handle_error("sigprocmask");

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = handler;
    if (sigaction(SYNC_SIG, &sa, NULL) == -1)
        handle_error("sigaction");

    switch (cid = fork()) {
    case -1:
        handle_error("fork");

    case 0:
        printf("[%ld] Child started - doing some work\n", (long) getpid());
        sleep(60);

        printf("[%ld] Child about to signal parent\n", (long) getpid());
        if (kill(getppid(), SYNC_SIG) == -1)
            handle_error("kill");

        _exit(EXIT_SUCCESS);

    default:
        printf("[%ld] Parent about to wait for signal\n", (long) getpid());

        sigemptyset(&empty_mask);
        if (sigsuspend(&empty_mask) == -1 && errno != EINTR)
            handle_error("sigsuspend");
        printf("[%ld] Parent got signal\n", (long) getpid());

        if (sigprocmask(SIG_SETMASK, &orig_mask, NULL) == -1)
            handle_error("sigprocmask");

        exit(EXIT_SUCCESS);
    }
}
