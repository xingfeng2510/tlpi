#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>

int main(int argc, char *argv[])
{
    int fd;
    
    if (argc > 1)
        fd = atoi(argv[1]);
    else
        fd = 2;

    printf("PID=%ld, TPGRP=%ld, SID=%ld\n", (long) getpid(), 
            (long) tcgetpgrp(fd), (long) tcgetsid(fd));

    exit(EXIT_SUCCESS);
}
