#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>

#include "common.h"

int main(void)
{
    int *addr;

    addr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
                MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (addr == MAP_FAILED)
        handle_error("mmap");

    *addr = 1;

    switch (fork()) {
    case -1:
        handle_error("fork");

    case 0:
        printf("Child startd, value = %d\n", *addr);
        (*addr)++;

        if (munmap(addr, sizeof(int)) == -1)
            handle_error("munmap");
        exit(EXIT_SUCCESS);

    default:
        if (wait(NULL) == -1)
            handle_error("wait");
        printf("In parent, value = %d\n", *addr);
        if (munmap(addr, sizeof(int)) == -1)
            handle_error("munmap");
        exit(EXIT_SUCCESS);
    }
}
