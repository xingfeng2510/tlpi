#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#include "common.h"

int main(int argc, char *argv[])
{
    // if (daemon(0, 0) == -1)
    //    handle_error("daemon");
    
    int fd;

    switch (fork()) {
    case -1:
        handle_error("fork");
    case 0:
        break;
    default:
        _exit(EXIT_SUCCESS);
    }

    if (setsid() == -1)
        handle_error("setsid");

    umask(0);
    chdir("/");

    fd = open("/dev/null", O_RDWR);
    if (fd == -1)
        handle_error("open");
    dup2(fd, 0);
    dup2(fd, 1);
    dup2(fd, 2);
    close(fd);

    pause();
}
