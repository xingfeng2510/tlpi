#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

#include "common.h"

static void handler(int sig)
{
    printf("Caught signal %d (%s)\n", sig, strsignal(sig));
}

int main(int argc, char *argv[])
{
    char c;

    setbuf(stdout, NULL);

    signal(SIGHUP, handler);

    switch (fork()) {
    case -1:
        handle_error("fork");

    case 0:
        printf("Child PID=%ld\n", (long) getpid());
        kill(getpid(), SIGTSTP);
        printf("Child continued\n");
        if (read(STDIN_FILENO, &c, 1) == -1)
            perror("read");
        sleep(30);
        _exit(EXIT_SUCCESS);

    default:
        printf("Parent PID=%ld\n", (long) getpid());
        sleep(5);
        exit(EXIT_SUCCESS);
    }
}
