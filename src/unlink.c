#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#include "common.h"

#define CMD_SIZE 200
#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
    int fd;
    char buf[BUF_SIZE];
    char shell_cmd[CMD_SIZE];

    if (argc < 2) {
        fprintf(stderr, "Usage: %s tmpfile\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    fd = open(argv[1], O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (fd == -1) 
        handle_error("open");

    sleep(20);

    if (unlink(argv[1]) == -1)
        handle_error("unlink");
    
    sleep(20);

    for (int i = 0; i < 100000; i++)
        write(fd, buf, BUF_SIZE);

    snprintf(shell_cmd, CMD_SIZE, "df -k `dirname %s`", argv[1]);
    system(shell_cmd);

    if (close(fd) == -1)
        handle_error("close");

    system(shell_cmd);
    exit(EXIT_SUCCESS);
}
