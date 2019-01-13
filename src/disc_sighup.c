#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

#include "common.h"

static void handler(int sig)
{
    printf("PID %ld: caught signal %2d (%s)\n", (long) getpid(),
            sig, strsignal(sig));
}

int main(int argc, char *argv[])
{
    pid_t pid, cpid;
    int i;
    struct sigaction sa;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s {d|s}... [ > sig.log 2>&1 ]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    setbuf(stdout, NULL);

    pid = getpid();
    printf("PID of parent process is: %ld\n", (long) pid);
    printf("Foreground process group ID is: %ld\n", (long) tcgetpgrp(STDIN_FILENO));

    for (i = 1; i < argc; i++) {
        cpid = fork();
        if (cpid == -1)
            handle_error("fork");

        if (cpid == 0) {
            if (argv[i][0] == 'd')
                if (setpgid(0, 0) == -1)
                    handle_error("setpgid");

            sigemptyset(&sa.sa_mask);
            sa.sa_flags = 0;
            sa.sa_handler = handler;
            if (sigaction(SIGHUP, &sa, NULL) == -1)
                handle_error("sigaction");
            break;
        }
    }

    alarm(60);

    printf("PID=%ld PGID=%ld\n", (long) getpid(), (long) getpgrp());
    for (;;)
        pause();
}
