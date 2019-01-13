#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

#include "common.h"

static void segv_handler(int sig, siginfo_t *siginfo, void *ucontext)
{
    int x;

    printf("Caught signal %d (%s)\n", sig, strsignal(sig));
    if (siginfo->si_signo == SIGSEGV) {
        switch (siginfo->si_code) {
            case 1: 
                puts("SEGV_MAPERR"); 
                break;
            case 2: 
                puts("SEGV_ACCERR"); 
                break;
            default: 
                puts("Unknown signal code");
        }
    }
    printf("Top of handler stack near %p\n", &x);
    fflush(NULL);

    _exit(EXIT_FAILURE);
}

static void overflow_stack(int depth)
{
    char a[100000];

    printf("Call %d - top of stack near %p\n", depth, &a[0]);
    overflow_stack(depth + 1);
}

int main(int argc, char *argv[])
{
    stack_t ss;
    struct sigaction sa;
    int j;

    printf("Top of standard stack near %p\n", &j);

    ss.ss_sp = malloc(SIGSTKSZ);
    if (ss.ss_sp == NULL)
        handle_error("malloc");
    ss.ss_size = SIGSTKSZ;
    ss.ss_flags = 0;
    if (sigaltstack(&ss, NULL) == -1)
        handle_error("sigaltstack");
    printf("Alternate stack is at %p-%p\n", ss.ss_sp, (char *) sbrk(0) - 1);

    sa.sa_sigaction = segv_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_ONSTACK | SA_SIGINFO;
    if (sigaction(SIGSEGV, &sa, NULL) == -1)
        handle_error("sigaction");

    overflow_stack(1);
}
