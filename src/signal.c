#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include "common.h"

void print_sigset(const sigset_t *sigset)
{
    int sig, cnt;

    cnt = 0;
    for (sig = 1; sig < NSIG; sig++) {
        if (sigismember(sigset, sig)) {
            cnt++;
            printf("\t%d (%s)\n", sig, strsignal(sig));
        }
    }

    if (cnt == 0)
        puts("\t<empty signal set>");
}

int print_sigmask()
{
    sigset_t curr_mask;

    if (sigprocmask(SIG_BLOCK, NULL, &curr_mask) == -1)
        return -1;
    print_sigset(&curr_mask);

    return 0;
}

int print_pendingsigs()
{
    sigset_t pending_sigs;
    
    if (sigpending(&pending_sigs) == -1)
        return -1;
    print_sigset(&pending_sigs);

    return 0;
}

static int counter = 0;

static void handler(int sig)
{
    printf("caught %d (%s)\n", sig, strsignal(sig));

    counter++;
}

int main(int argc, char **argv)
{
    signal(SIGTERM, handler);
    signal(SIGQUIT, handler);

    for (int i = 0; i < 100000; i++) {
        raise(SIGTERM); // send synchronously
        puts("raised");
    }
    printf("counter: %d\n", counter);

    sleep(5);

    sigset_t block_set, prev_mask;

    signal(SIGINT, handler);
    signal(SIGTERM, handler);
    signal(SIGPIPE, SIG_IGN);

    sigemptyset(&block_set);
    sigaddset(&block_set, SIGINT);
    sigaddset(&block_set, SIGTERM);

    if (sigprocmask(SIG_BLOCK, &block_set, &prev_mask) == 1)
        handle_error("sigprocmask");

    puts("signal mask:");
    print_sigmask();

    sleep(60);

    puts("pending signals:");
    print_pendingsigs();

    puts("restore previous signal mask");
    if (sigprocmask(SIG_SETMASK, &prev_mask, NULL) == -1)
        handle_error("sigprocmask");

    sleep(5);
    puts("unreachable code");

    return 0;
}
