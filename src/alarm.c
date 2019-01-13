#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

static void alarm_handler(int sig)
{
    printf("alarmed\n");
    alarm(3);
}

int main(int argc, char *argv[])
{
    signal(SIGALRM, alarm_handler);
    alarm(3);

    pause();
    printf("waiting for next alarm\n");
    pause();
    
    exit(EXIT_SUCCESS);
}
