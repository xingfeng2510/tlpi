#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#include "common.h"

#define MYFILE "myfile"
#define MYDIR "mydir"

int main(int argc, char *argv[])
{
    int fd;
    struct stat sb;
    mode_t u;

    umask(033);
    
    fd = open(MYFILE, O_RDWR | O_CREAT | O_EXCL, 0666);
    if (fd == -1)
        handle_error("open");
    if (mkdir(MYDIR, 0777) == -1)
        handle_error("mkdir");

    u = umask(0);
    printf("umask: %o\n", u);
    if (stat(MYFILE, &sb) == -1)
        handle_error("stat");
    printf("myfile perms: %o\n", sb.st_mode);
    if (stat(MYDIR, &sb) == -1)
        handle_error("stat");
    printf("mydir perms: %o\n", sb.st_mode);

    if (unlink(MYFILE) == -1)
        handle_error("unlink");
    if (rmdir(MYDIR) == -1)
        handle_error("rmdir");

    return 0;
}
