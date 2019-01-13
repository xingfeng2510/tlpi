#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "common.h"

int main(int argc, char *argv[])
{
    int which, prio;
    id_t who;

    if (argc != 4 || strchr("pgu", argv[1][0]) == NULL) {
        fprintf(stderr, "%s {p|g|u} who priority\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    which = (argv[1][0] == 'p') ? PRIO_PROCESS :
                (argv[1][0] == 'g') ? PRIO_PGRP : PRIO_USER;
    who = atoi(argv[2]);
    prio = atoi(argv[3]);

    if (setpriority(which, who, prio) == -1)
        handle_error("setpriority");

    errno = 0;
    prio = getpriority(which, who);
    if (prio == -1 && errno != 0)
        handle_error("getpriority");

    printf("Nice value is: %d\n", prio);
    
    exit(EXIT_SUCCESS);
}
