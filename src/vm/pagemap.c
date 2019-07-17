#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <limits.h>

#define PTRSIZE sizeof(void *)
#define BITVAL(X, Y) (X & ((uint64_t)1 << Y)) >> Y 

int main(int argc, char *argv[])
{
    long pid;
    unsigned long vaddr;
    char *endptr;
    int pagesize;
    char pathname[PATH_MAX];

    FILE *stream;
    long offset;
    int status;
    unsigned char buf[PTRSIZE];
    size_t nread;
    uint64_t value = 0, pfn;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <PID> <VADDR>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    pid = strtol(argv[1], &endptr, 10);
    vaddr = strtoll(argv[2], &endptr, 16);
    sprintf(pathname, "/proc/%ld/pagemap", pid);
    pagesize = getpagesize();

    stream = fopen(pathname, "rb");
    if (stream == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    offset = (vaddr / pagesize) * PTRSIZE;
    status = fseek(stream, offset, SEEK_SET);
    if (status == -1) {
        perror("fseek");
        exit(EXIT_FAILURE);
    }

    printf("read %d bytes from %s at %#x\n", PTRSIZE, pathname, offset);

    nread = fread(buf, 1, PTRSIZE, stream);
    if (nread != PTRSIZE) {
        fprintf(stderr, "fread: partial read\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < PTRSIZE; i++)
        value |= (uint64_t) buf[i] << (i * 8);

    printf("pte raw value: %#llx\n", value);

    if (BITVAL(value, 63)) {
        pfn = value & 0x7fffffffffffff;
        printf("va: %#llx, PFN: %#llx, pa: %#llx\n", vaddr, pfn, pfn * pagesize + vaddr % pagesize);
    } else {
        printf("page not present\n");
    }
    if (BITVAL(value, 62))
        printf("page swapped\n");

    fclose(stream);

    return 0;
}
