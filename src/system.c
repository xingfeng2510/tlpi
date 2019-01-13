#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#include "common.h"

#define MAX_CMD_LEN 200

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

int system(const char *command)
{
    int status;
    pid_t cpid;

    switch (cpid = fork()) {
    case -1:
        return -1;

    case 0:
        execl("/bin/sh", "sh", "-c", command, (char *) NULL);
        _exit(127);

    default:
        if (waitpid(cpid, &status, 0) == -1)
            return -1;
        else
            return status;
    }
}

int system2(const char *command)
{
    sigset_t block_mask, orig_mask;
    struct sigaction ignore_sa, orig_quit_sa, orig_int_sa, default_sa;
    pid_t cpid;
    int status;

    if (command == NULL)
        return system2(":") == 0;

    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGCHLD);
    sigprocmask(SIG_BLOCK, &block_mask, &orig_mask);

    ignore_sa.sa_handler = SIG_IGN;
    ignore_sa.sa_flags = 0;
    sigemptyset(&ignore_sa.sa_mask);
    sigaction(SIGINT, &ignore_sa, &orig_int_sa);
    sigaction(SIGQUIT, &ignore_sa, &orig_quit_sa);

    switch (cpid = fork()) {
    case -1:
        status = -1;
        break;

    case 0:
        default_sa.sa_handler = SIG_DFL;
        default_sa.sa_flags = 0;
        sigemptyset(&default_sa.sa_mask);

        if (orig_int_sa.sa_handler != SIG_IGN)
            sigaction(SIGINT, &default_sa, NULL);
        if (orig_quit_sa.sa_handler != SIG_IGN)
            sigaction(SIGQUIT, &default_sa, NULL);

        sigprocmask(SIG_SETMASK, &orig_mask, NULL);

        execl("/bin/bash", "bash", "-c", command, (char *) NULL);
        _exit(127);
    
    default:
        while (waitpid(cpid, &status, 0) == -1) {
            if (errno != EINTR) {
                status = -1;
                break;
            }
        }
        break;
    }

    sigprocmask(SIG_SETMASK, &orig_mask, NULL);
    sigaction(SIGINT, &orig_int_sa, NULL);
    sigaction(SIGQUIT, &orig_quit_sa, NULL);

    return status;
}

int main(int argc, char *argv[])
{
    char str[MAX_CMD_LEN];
    int status;

    for (;;) {
        printf("Command: ");
        fflush(stdout);
        if (fgets(str, MAX_CMD_LEN, stdin) == NULL)
            break; /* end-of-file */
        
        status = system2(str);
        printf("system() returned: status=0x%04x (%d,%d)\n",
                (unsigned int) status, status >> 8, status & 0xff);

        if (status == -1)
            handle_error("system");
        else {
            if (WIFEXITED(status) && WEXITSTATUS(status) == 127)
                printf("(Probably) could not invoke shell\n");
            else
                print_wait_status(status);
        }
    }

    exit(EXIT_SUCCESS);
}
