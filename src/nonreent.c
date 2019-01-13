#define _GNU_SOURCE
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *str2;
static int handled = 0;

static void handler(int sig)
{
    crypt(str2, "xx");
    handled++;
}

int main(int argc, char *argv[])
{
    char *cr1;
    int call_num, mismatch;
    struct sigaction sa;

    if (argc != 3) {
        fprintf(stderr, "%s str1 str2\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    str2 = argv[2];
    cr1 = strdup(crypt(argv[1], "xx"));

    if (cr1 == NULL) {
        perror("strdup");
        exit(EXIT_FAILURE);
    }

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    for (call_num = 1, mismatch = 0; ; call_num++) {
        if (strcmp(crypt(argv[1], "xx"), cr1) != 0) {
            mismatch++;
            printf("Mismatch on call %d (mismatch=%d handled=%d)\n", 
                    call_num, mismatch, handled);
        }
    }

    exit(EXIT_SUCCESS);
}
