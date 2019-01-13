#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

#include "common.h"

static void handler(int sig)
{
    printf("PID=%ld: caught signal %d (%s)\n", (long) getpid(),
            sig, strsignal(sig));
}

int main(int argc, char *argv[])
{
    int i;
    struct sigaction sa;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s {s|p} ...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    setbuf(stdout, NULL);

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(SIGHUP, &sa, NULL) == -1)
        handle_error("sigaction");
    if (sigaction(SIGCONT, &sa, NULL) == -1)
        handle_error("sigaction");

    printf("child: PID=%ld, PPID=%ld, PGID=%ld, SID=%ld\n",
            (long) getpid(), (long) getppid(),
            (long) getpgrp(), (long) getsid(0));

    for (i = 1; i < argc; i++) {
        switch (fork()) {
        case -1:
            handle_error("fork");

        case 0:
            printf("child: PID=%ld, PPID=%ld, PGID=%ld, SID=%ld\n",
                    (long) getpid(), (long) getppid(),
                    (long) getpgrp(), (long) getsid(0));

            if (argv[i][0] == 's') {
                printf("PID=%ld stopping\n", (long) getpid());
                raise(SIGSTOP);
            } else {
                alarm(60);
                printf("PID=%ld pausing\n", (long) getpid());
                pause();
            }

            sleep(30);
            _exit(EXIT_SUCCESS);

        default:
            break;
        }
    }

    sleep(3);
    printf("parent exiting\n");
    exit(EXIT_SUCCESS);
}
