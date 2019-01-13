#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

#include "common.h"

void print_wait_status(int status)
{
    if (WIFEXITED(status)) {
        printf("child exited, status=%d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("child killed by signal %d (%s)",
                WTERMSIG(status), strsignal(WTERMSIG(status)));
#ifdef WCOREDUMP
        if (WCOREDUMP(status))
            printf(" (core dumped)");
#endif
        printf("\n");
    } else if (WIFSTOPPED(status)) {
        printf("child stopped by signal %d (%s)\n",
                WSTOPSIG(status), strsignal(WSTOPSIG(status)));
    } else if (WIFCONTINUED(status)) {
        printf("child continued\n");
    } else {
        printf("what happened to this child? (status=%x)\n",
                (unsigned int) status);
    }
}

int main(int argc, char *argv[])
{
    int status;
    pid_t cid;
    
    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "Usage: %s [exit-status]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    switch (fork()) {
    case -1:
        handle_error("fork");

    case 0:
        printf("Child started with PID %ld\n", (long) getpid());
        if (argc > 1)
            _exit(atoi(argv[1]));
        else
            for (;;)
                pause();
        _exit(EXIT_FAILURE);

    default:
        for (;;) {
            cid = waitpid(-1, &status, WUNTRACED | WCONTINUED);
            if (cid == -1)
                handle_error("waitpid");

            printf("waitpid() returned: PID=%ld; status=0x%04x (%d,%d)\n",
                    (long) cid, status, status >> 8, status & 0xff);
            print_wait_status(status);

            if (WIFEXITED(status) || WIFSIGNALED(status))
                exit(EXIT_SUCCESS);
        }
    }
}
