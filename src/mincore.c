#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common.h"

static void show_mincore(char *addr, size_t length)
{
    unsigned char *vec;
    long page_size, num_pages, j;

    page_size = sysconf(_SC_PAGESIZE);

    num_pages = (length + page_size - 1) / page_size;
    vec = malloc(num_pages);
    if (vec == NULL)
        handle_error("malloc");
    if (mincore(addr, length, vec) == -1)
        handle_error("mincore");

    for (j = 0; j < num_pages; j++) {
        if (j % 64 == 0)
            printf("%s%10p: ", (j == 0) ? "" : "\n", addr + (j * page_size));
        printf("%c", (vec[j] & 1) ? '*' : '.');
    }
    printf("\n");

    free(vec);
}

int main(int argc, char *argv[])
{
    char *addr;
    size_t total_len, lock_len;
    long page_size, step_size, j;

    if (argc != 4 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s num-pages lock-page-step lock-page-len\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    page_size = sysconf(_SC_PAGESIZE);
    if (page_size == -1)
        handle_error("sysconf(_SC_PAGESIZE)");

    total_len = atoi(argv[1]) * page_size;
    step_size = atoi(argv[2]) * page_size;
    lock_len = atoi(argv[3]) * page_size;

    addr = mmap(NULL, total_len, PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (addr == MAP_FAILED)
        handle_error("mmap");
    
    printf("Allocated %ld (%#lx) bytes starting at %p\n",
            (long) total_len, (unsigned long) total_len, addr);

    printf("Before mlock:\n");
    show_mincore(addr, total_len);

    sleep(30);

    for (j = 0; j + lock_len <= total_len; j += step_size)
        if (mlock(addr + j, lock_len) == -1)
            handle_error("mlock");

    printf("After mlock:\n");
    show_mincore(addr, total_len);

    sleep(30);
    
    exit(EXIT_SUCCESS);
}
