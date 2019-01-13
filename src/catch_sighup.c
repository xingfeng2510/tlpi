#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include "common.h"

static void handler(int sig)
{
}

int main(int argc, char *argv[])
{
    pid_t cpid;
    struct sigaction sa;

    setbuf(stdout, NULL);

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(SIGHUP, &sa, NULL) == -1)
        handle_error("sigaction");

    cpid = fork();
    if (cpid == -1)
        handle_error("fork");

    if (cpid == 0 && argc > 1) {
        if (setpgid(0, 0) == -1)
            handle_error("setpgid");
        
        if (fork() == -1)
            handle_error("fork");
    }

    printf("PID=%ld, PPID=%ld, PGID=%ld, SID=%ld\n", (long) getpid(),
            (long) getppid(), (long) getpgrp(), (long) getsid(0));

    alarm(60);

    for (;;) {
        pause();
        printf("%ld: caught SIGHUP\n", (long) getpid());
    }
}
