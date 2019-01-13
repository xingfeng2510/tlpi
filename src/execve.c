#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common.h"

int main(int argc, char *argv[])
{
    char *args[10];
    char *envs[] = { "GREET=salut", "BYE=adieu", NULL };

    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "Usage: %s pathname\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    args[0] = strrchr(argv[1], '/');
    if (args[0] != NULL)
        args[0]++;
    else
        args[0] = argv[1];
    args[1] = "hello world";
    args[2] = "goodbye";
    args[3] = NULL;

    execve(argv[1], args, envs);
    handle_error("execve");

    exit(EXIT_SUCCESS);
}
