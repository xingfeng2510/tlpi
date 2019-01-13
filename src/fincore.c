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
    int fd;
    struct stat sb;
    char *addr;
    unsigned char *vec;
    size_t page_size, total_pages, page_index;
    size_t cached_pages = 0;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    page_size = sysconf(_SC_PAGESIZE);
    if (page_size == -1)
        handle_error("sysconf");

    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
        handle_error("open");

    if (fstat(fd, &sb) == -1)
        handle_error("fstat");

    addr = mmap(NULL, sb.st_size, PROT_NONE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
        handle_error("mmap");
    
    total_pages = (sb.st_size + page_size - 1) / page_size;
    vec = malloc(total_pages);
    if (vec == NULL)
        handle_error("malloc");

    if (mincore(addr, sb.st_size, vec) == -1)
        handle_error("mincore");

    for (page_index = 0; page_index < total_pages; page_index++) {
        if (vec[page_index] & 1)
            cached_pages++;
    }

    printf("%s\n", argv[1]);
    printf("file size: %ld\n", sb.st_size);
    printf("total pages: %ld\n", total_pages);
    printf("cached pages: %ld\n", cached_pages);
    printf("cached sizes: %ld\n", cached_pages * page_size);
    printf("cache hit ratio: %.2f\n", 100 * (cached_pages / (double)total_pages));
    
    exit(EXIT_SUCCESS);
}
