#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#include "common.h"

int main(int argc, char *argv[])
{
    int fd;
    char *addr;
    struct stat sb;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s shm-name\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    fd = shm_open(argv[1], O_RDONLY, 0);
    if (fd == -1)
        handle_error("shm_open");
    
    if (fstat(fd, &sb) == -1)
        handle_error("fstat");

    addr = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
        handle_error("mmap");

    if (close(fd) == -1)
        handle_error("close");

    write(STDOUT_FILENO, addr, sb.st_size);
    printf("\n");

    exit(EXIT_SUCCESS);
}
