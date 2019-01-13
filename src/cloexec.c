#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#include "common.h"

int main(int argc, char *argv[])
{
    int fd, fd2;
    int flags;
    int accmode;

    fd = open("xxx", O_RDONLY | O_CLOEXEC);
    if (fd == -1)
        handle_error("open");
    printf("fd[%d] flags=%o\n", fd, fcntl(fd, F_GETFD));

    // fd2 = dup2(fd, fd+1);
    fd2 = fcntl(fd, F_DUPFD_CLOEXEC, fd+1);
    if (fd2 == -1)
        handle_error("dup2");
    if (lseek(fd2, 5, SEEK_SET) == -1)
        handle_error("lseek");
    printf("fd[%d] flags=%o\n", fd2, fcntl(fd2, F_GETFD));

    sleep(30);

    flags = fcntl(STDOUT_FILENO, F_GETFL);
    accmode = flags & O_ACCMODE;
    if (accmode == O_WRONLY || accmode == O_RDWR)
        printf("stdout is writable\n");
    printf("flags=%o\n", flags);

    if (argc > 1) {
        flags = fcntl(STDOUT_FILENO, F_GETFD);
        if (flags == -1)
            handle_error("fcntl - F_GETFD");

        if (fcntl(STDOUT_FILENO, F_DUPFD, 111) == -1)
            handle_error("fcntl - F_DUPFD");

        flags |= FD_CLOEXEC;
        if (fcntl(STDOUT_FILENO, F_SETFD, flags) == -1)
            handle_error("fcntl - F_SETFD");
    }

    execlp("tailf", "tailf", "-n1", argv[0], (char *)NULL);
    perror("execlp");
}
