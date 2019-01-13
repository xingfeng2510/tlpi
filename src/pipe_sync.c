#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common.h"

int main(int argc, char *argv[])
{
    int pfd[2];
    int i, dummy;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s sleep-time...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    setbuf(stdout, NULL);

    printf("Parent started\n");

    if (pipe(pfd) == -1)
        handle_error("pipe");

    for (i = 1; i < argc; i++) {
        switch (fork()) {
        case -1:
            handle_error("fork");

        case 0:
            if (close(pfd[0]) == -1)
                handle_error("close");

            sleep(atoi(argv[i]));

            printf("Child %d (PID=%ld) closing pipe\n",
                    i, (long) getpid());
            if (close(pfd[1]) == -1)
                handle_error("close");

            _exit(EXIT_SUCCESS);

        default:
            break;
        }
    }

    if (close(pfd[1]) == -1)
        handle_error("close");

    if (read(pfd[0], &dummy, 1) != 0) {
        fprintf(stderr, "parent didn't get EOF\n");
        exit(EXIT_SUCCESS);
    }
    printf("Parent ready to go\n");

    exit(EXIT_SUCCESS);
}
