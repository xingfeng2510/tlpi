#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <time.h>

static void handler(int sig)
{
    printf("Caught signal %d (%s)\n", sig, strsignal(sig));
}

static void print_sigset(const sigset_t *sigset)
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

int main(int argc, char *argv[])
{
    sigset_t orig_mask, block_mask, pending_sigs;
    time_t start_time;

    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGINT);
    sigaddset(&block_mask, SIGQUIT);
    if (sigprocmask(SIG_BLOCK, &block_mask, &orig_mask) == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }
    puts("Original signal mask:");
    print_sigset(&orig_mask);

    signal(SIGINT, handler);
    signal(SIGQUIT, handler);

    for (start_time = time(NULL); time(NULL) < start_time + 4; ) 
        continue;
    
    sigpending(&pending_sigs);
    puts("Before sigsuspend, pending signals:");
    print_sigset(&pending_sigs);

    sigsuspend(&orig_mask);

    sigpending(&pending_sigs);
    puts("After sigsuspend, pending signals:");
    print_sigset(&pending_sigs);

    puts("Restore orignal signal mask");
    if (sigprocmask(SIG_SETMASK, &orig_mask, NULL) == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}
