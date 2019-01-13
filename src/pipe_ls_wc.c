#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

#include "common.h"

int main(void)
{
    int pfd[2];

    if (pipe(pfd) == -1)
        handle_error("pipe");

    switch (fork()) {
    case -1:
        handle_error("fork");

    case 0:
        if (close(pfd[0]) == -1)
            handle_error("close 1");

        if (pfd[1] != STDOUT_FILENO) { /* Defensive check */
            if (dup2(pfd[1], STDOUT_FILENO) == -1)
                handle_error("dup2 1");
            if (close(pfd[1]) == -1)
                handle_error("close 2");
        }

        execlp("ls", "ls", (char *) NULL); /* Writes to pipe */
        handle_error("execlp ls");

    default:
        break;
    }

    switch (fork()) {
    case -1:
        handle_error("fork");

    case 0:
        if (close(pfd[1]) == -1)
            handle_error("close 3");

        if (pfd[0] != STDIN_FILENO) { /* Defensive check */
            if (dup2(pfd[0], STDIN_FILENO) == -1)
                handle_error("dup2 2");
            if (close(pfd[0]) == -1)
                handle_error("close 4");
        }

        execlp("wc", "wc", "-l", (char *) NULL); /* Reads from pipe */
        handle_error("execlp wc");

    default:
        break;
    }

    if (close(pfd[0]) == -1)
        handle_error("close 5");
    if (close(pfd[1]) == -1)
        handle_error("close 6");
    if (wait(NULL) == -1)
        handle_error("wait 1");
    if (wait(NULL) == -1)
        handle_error("wait 2");

    exit(EXIT_SUCCESS);
}
