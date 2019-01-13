#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#include "common.h"

int main(int argc, char *argv[])
{
    int fd;
    size_t size;
    void *addr;

    if (argc < 3 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s shm-name shm-size\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    size = atoi(argv[2]);
    fd = shm_open(argv[1], O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (fd == -1)
        handle_error("shm_open");

    if (ftruncate(fd, size) == -1)
        handle_error("ftruncate");

    addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
        handle_error("mmap");

    exit(EXIT_SUCCESS);
}
