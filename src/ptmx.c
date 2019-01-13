#define _XOPEN_SOURCE
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#include "common.h"

int main(int argc, char *argv[])
{
    int ptm, pts;
    char *p;
    char buf[20];
    int nwrite;
    int cpid;

    ptm = open("/dev/ptmx", O_RDWR | O_NOCTTY);
    if (ptm == -1)
        handle_error("open");

    if (grantpt(ptm) == -1) {
        close(ptm);
        handle_error("grantpt");
    }

    if (unlockpt(ptm) == -1) {
        close(ptm);
        handle_error("unlockpt");
    }
    
    if ((p = ptsname(ptm)) == NULL) {
        close(ptm);
        handle_error("ptsname");
    }
    printf("tty: %s\n", p);

    cpid = fork();
    if (cpid == -1) {
        close(ptm);
        handle_error("fork");
    }

    switch (cpid) {
    case -1:
        close(ptm);
        handle_error("fork");

    case 0:
        if (setsid() == -1)
            handle_error("setsid");

        close(ptm);

        pts = open(p, O_RDWR);
        if (pts == -1)
            handle_error("open");

        if (dup2(pts, STDIN_FILENO) != STDIN_FILENO)
            handle_error("dup2 - STDIN_FILENO");
        if (dup2(pts, STDOUT_FILENO) != STDOUT_FILENO)
            handle_error("dup2 - STDOUT_FILENO");
        if (dup2(pts, STDERR_FILENO) != STDERR_FILENO)
            handle_error("dup2 - STDERR_FILENO");

        if (pts > STDERR_FILENO)
            close(pts);

        sleep(30);

        _exit(0);

    default:
        nwrite = write(ptm, "hello\nworld\n", 12);
        printf("%d bytes written\n", nwrite);

        wait(NULL);
        exit(0);
    }
}
