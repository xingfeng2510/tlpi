#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#include "common.h"

int main(int argc, char *argv[])
{
    if (argc !=2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s shm-name\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (shm_unlink(argv[1]) == -1)
        handle_error("shm_unlink");

    exit(EXIT_SUCCESS);
}
