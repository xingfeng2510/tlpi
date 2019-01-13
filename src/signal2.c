#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

#include "common.h"

static void handler(int sig)
{
    printf("Caught signal %d (%s)\n", sig, strsignal(sig));
    sleep(30);
    printf("handler done\n");
}

int main(int argc, char *argv[])
{
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(SIGINT, &sa, NULL) == -1)
        handle_error("sigaction");

    pause();
    printf("pause returned\n");

    exit(EXIT_SUCCESS);
}
