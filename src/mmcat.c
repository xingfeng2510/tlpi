#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common.h"

int main(int argc, char *argv[])
{
    char *addr;
    int fd;
    struct stat sb;

    if (argc != 2  || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s file\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
        handle_error("open");

    if (fstat(fd, &sb) == -1)
        handle_error("fstat");

    addr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED)
        handle_error("mmap");

    /*
    sleep(30);
    if (mlock(addr, sb.st_size) == -1)
        handle_error("mlock");
    printf("after mlock()");
    sleep(30);
    */

    if (write(STDOUT_FILENO, addr, sb.st_size) != sb.st_size)
        fprintf(stderr, "partial write");
    
    sleep(30);

    exit(EXIT_SUCCESS);
}
