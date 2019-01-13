#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common.h"

#define BUF_SIZE 10

int main(int argc, char *argv[])
{
    int pfd[2];
    char buf[BUF_SIZE];
    ssize_t num_read;

    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s string\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (pipe(pfd) == -1)
        handle_error("pipe");

    switch (fork()) {
    case -1:
        handle_error("fork");

    case 0: /* Child - reads from pipe */
        if (close(pfd[1]) == -1)
            handle_error("close - child");

        for (;;) {
            num_read = read(pfd[0], buf, BUF_SIZE);
            if (num_read == -1)
                handle_error("read");
            if (num_read == 0)
                break;
            if (write(STDOUT_FILENO, buf, num_read) != num_read)
                handle_error("child - partial write");
        }

        write(STDOUT_FILENO, "\n", 1);
        if (close(pfd[0]) == -1)
            handle_error("close");
        _exit(EXIT_SUCCESS);
    
    default: /* Parent - writes to pipe */
        if (close(pfd[0]) == -1)
            handle_error("close - parent");
        if (write(pfd[1], argv[1], strlen(argv[1])) != strlen(argv[1])) 
            handle_error("write - partial write");
    
        if (close(pfd[1]) == -1) /* Child will see EOF */
            handle_error("close");
        wait(NULL);
        exit(EXIT_SUCCESS);
    }
}
