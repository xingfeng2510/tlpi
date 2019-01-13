#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "common.h"

int main(int argc, char *argv[])
{
    printf("Hello world\n");
    write(STDOUT_FILENO, "Ciao\n", 5);

    switch (fork()) {
    case -1:
        handle_error("fork");

    case 0:
        sleep(30);
        _exit(EXIT_SUCCESS);
    
    default:
        exit(EXIT_SUCCESS);
    }
}
