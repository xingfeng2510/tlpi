#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common.h"

int main(int argc, char *argv[])
{
    int num_children, i;
    pid_t cid;

    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "Usage: %s [num-children]\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    num_children = (argc > 1) ? atoi(argv[1]) : 1;

    setbuf(stdout, NULL); /* Make stdout unbuffered */

    for (i = 0; i < num_children; i++) {
        switch (cid = fork()) {
        case -1:
            handle_error("fork");

        case 0:
            printf("%d child\n", i);
            _exit(EXIT_FAILURE);

        default:
            printf("%d parent\n", i);
            wait(NULL); /* Wait for child to terminate */
            break;
        }
    }

    exit(EXIT_SUCCESS);
}
