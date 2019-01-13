#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

#include "common.h"

void handler(int sig)
{
    printf("%d (%s)\n", sig, strsignal(sig));
}

int main(void)
{
    signal(SIGINT, handler);

    if (pause() == -1)
        handle_error("pause");

    return 0;
}
