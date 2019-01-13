#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    for (;;) {
        puts("hello");
    }

    return 0;

    setvbuf(stdout, NULL, _IOFBF, 0);

    puts("hello world");
    sleep(5);

    puts("hello world 2");
    sleep(5);

    exit(EXIT_SUCCESS); 
}
