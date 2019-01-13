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
    size_t len;
    void *addr;

    if (argc < 3 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s shm-name string\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    fd = shm_open(argv[1], O_RDWR, 0);
    if (fd == -1)
        handle_error("shm_open");
    
    len = strlen(argv[2]);
    if (ftruncate(fd, len) == -1)
        handle_error("ftruncate");
    printf("Resized to %ld bytes\n", (long) len);

    addr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
        handle_error("mmap");

    if (close(fd) == -1)
        handle_error("close");

    printf("Copying %ld bytes\n", (long) len);
    memcpy(addr, argv[2], len);

    exit(EXIT_SUCCESS);
}
