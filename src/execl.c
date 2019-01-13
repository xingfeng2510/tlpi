#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "common.h"

int main(int argc, char *argv[])
{
    printf("Initial value of USER: %s\n", getenv("USER"));
    if (putenv("USER=xmb") != 0)
        handle_error("putenv");

    execl("/usr/bin/printenv", "printenv", "USER", "SHELL", (char *)NULL);
    handle_error("execl");
}
