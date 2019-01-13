#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#include "common.h"

int main(int argc, char *argv[])
{
    int fd, flags;
    char template[] = "/tmp/testXXXXXX";

    setbuf(stdout, NULL); /* Disable buffering of stdout */

    fd = mkstemp(template);
    if (fd == -1)
        handle_error("mkstemp");

    printf("File offset before fork(): %lld\n", (long long) lseek(fd, 0, SEEK_CUR));

    flags = fcntl(fd, F_GETFL);
    if (flags == -1)
        handle_error("fcntl - F_GETFL");
    printf("O_APPEND flag before fork() is: %s\n", (flags & O_APPEND) ? "on" : "off");

    switch (fork()) {
    case -1:
        handle_error("fork");

    case 0:
        sleep(60);

        if (lseek(fd, 1000, SEEK_SET) == -1)
            handle_error("lseek");

        flags = fcntl(fd, F_GETFL);
        if (flags == -1)
            handle_error("fcntl - F_GETFL");
        flags |= O_APPEND;
        if (fcntl(fd, F_SETFL, flags) == -1)
            handle_error("fcntl - F_SETFL");

        _exit(EXIT_SUCCESS);

    default:
        if (wait(NULL) == -1)
            handle_error("wait");
        printf("Child has exited\n");

        printf("File offset in parent: %lld\n", (long long) lseek(fd, 0, SEEK_CUR));

        flags = fcntl(fd, F_GETFL);
        if (flags == -1)
            handle_error("fcntl - F_GETFL");
        printf("O_APPEND flag in parent is: %s\n", (flags & O_APPEND) ? "on" : "off");

        exit(EXIT_SUCCESS);
    }
}
