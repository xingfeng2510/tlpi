#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>

#include "common.h"

void bye(void)
{
    printf("That was all, folks\n");
}

static void handler(int sig)
{
    printf("Caught signal %d\n", sig);
}

int main(void)
{
    long a;
    int i;

    a = sysconf(_SC_ATEXIT_MAX);
    printf("ATEXIT_MAX = %ld\n", a);

    signal(SIGINT, handler);

    i = atexit(bye);
    if (i != 0)
        handle_error("atexit");

    sleep(30);
    if (errno == EINTR)
        fprintf(stderr, "Sleep interrupted\n");

    exit(EXIT_SUCCESS);
}
