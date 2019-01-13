#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

static int sigcnt[NSIG];
static volatile sig_atomic_t got_sigint = 0;

static void handler(int sig)
{
    if (sig == SIGINT)
        got_sigint = 1;
    else
        sigcnt[sig]++;
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
    int n, num_secs;
    sigset_t pending_mask, blocking_mask, empty_mask;

    printf("%s: PID is %ld\n", argv[0], (long) getpid());

    for (n = 1; n < NSIG; n++)
        (void) signal(n, handler);

    if (argc > 1) {
        num_secs = atoi(argv[1]);

        sigfillset(&blocking_mask);
        if (sigprocmask(SIG_SETMASK, &blocking_mask, NULL) == -1) {
            perror("sigprocmask");
            exit(EXIT_FAILURE);
        }

        printf("%s: sleeping for %d seconds\n", argv[0], num_secs);
        sleep(num_secs);

        printf("%s: pending signals are: \n", argv[0]);
        sigpending(&pending_mask);
        print_sigset(&pending_mask);

        sigemptyset(&empty_mask);
        if (sigprocmask(SIG_SETMASK, &empty_mask, NULL) == -1) {
            perror("sigprocmask");
            exit(EXIT_FAILURE);
        }
    }

    while (!got_sigint)
        continue;

    for (n = 1; n < NSIG; n++)
        if (sigcnt[n] != 0)
            printf("%s: signal %d caught %d time%s\n", argv[0], n, 
                    sigcnt[n], (sigcnt[n] == 1) ? "" : "s");

    exit(EXIT_FAILURE);
}
