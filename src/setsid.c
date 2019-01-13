#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#include "common.h"

int main(int argc, char *argv[])
{
    int fd;
    int bytes;

    fd = open("/dev/tty", O_RDWR);
    if (fd == -1)
        handle_error("open");

    bytes = write(fd, "hello\n", 6); 
    if (bytes == -1)
        handle_error("write");

    if (ioctl(fd, TIOCNOTTY) == -1)
        handle_error("ioctl");

    if (fork() != 0) {
        printf("parent PID=%ld\n", (long) getpid());
        _exit(EXIT_SUCCESS);
    }

    if (setsid() == -1)
        handle_error("setsid");

    printf("PID=%ld, PGID=%ld, SID=%ld\n", (long) getpid(),
           (long) getpgrp(), (long) getsid(0));
    
    fd = open("/dev/tty", O_RDWR);
    if (fd == -1)
        handle_error("open");

    exit(EXIT_SUCCESS);
}
