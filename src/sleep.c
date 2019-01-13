#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

static void handler(int sig)
{
    printf("Caught signal %d (%s)\n", sig, strsignal(sig));
}

int main()
{
    __sighandler_t h;
    for (int i = 1; i < NSIG; i++) {
        h = signal(i, handler);
        if (h == SIG_ERR) {
            fprintf(stderr, "%2d: ", i);
            perror("signal");
        } else
            printf("%2d: %25s handler: %s\n", i, strsignal(i), h == SIG_DFL ? "SIG_DFL" : "OTHER");
    }

    int nsec = sleep(10);
    if (nsec > 0) {
        printf("Left %d seconds\n", nsec);
        perror("sleep");
        return -1;
    }

    return 0;
}
