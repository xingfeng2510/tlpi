#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#include "common.h"

void handler(int sig)
{
    printf("Caught signal %d (%s)\n", sig, strsignal(sig));
}

int main(int argc, char *argv[])
{
    struct sigaction sa, sa2;

    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGINT, &sa, NULL) == -1)
        handle_error("sigaction");

    sa2.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGALRM, &sa2, NULL) == -1)
        handle_error("sigaction");

    alarm(10);

    int nread;
    char buf[128];
    /*
    while ((nread = read(0, buf, 128)) == -1 && errno == EINTR) {
        puts("Read interrupted, continue...");
        continue;
    }
    */
    nread = read(0, buf, 128);
    if (nread == -1)
        handle_error("read");
    printf("Readed: %s", buf);

    exit(EXIT_SUCCESS);
}
