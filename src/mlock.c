#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>

#include "common.h"

int main(int argc, char *argv[])
{
    char *addr;
    size_t len = 20 * 1024;

    addr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (addr == MAP_FAILED)
        handle_error("mmap");
    printf("mmap() returned address is 0x%p\n", addr);

    sleep(30);

    if (mlock(addr, len) == -1)
        handle_error("mlock");
    printf("after mlock()\n");

    sleep(30);
    exit(EXIT_SUCCESS);
}
