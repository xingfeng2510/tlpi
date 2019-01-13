#include <sched.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common.h"

int main(int argc, char *argv[])
{
    int i, oldpol, newpol;
    struct sched_param sp;
    pid_t pid;

    if (argc < 3 || strchr("rfo", argv[1][0]) == NULL) {
        fprintf(stderr, "%s policy priority [pid...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    newpol = (argv[1][0] == 'r') ? SCHED_RR :
                (argv[1][0] == 'f') ? SCHED_FIFO : SCHED_OTHER;
    sp.sched_priority = atoi(argv[2]);

    for (i = 3; i < argc; i++) {
        pid = atoi(argv[i]);

        oldpol = sched_getscheduler(pid);
        if (oldpol == -1)
            handle_error("sched_getscheduler");

        if (sched_setscheduler(pid, newpol, &sp) == -1)
            handle_error("sched_setscheduler");

        printf("%s old policy: %-5s, new policy: %-5s\n", argv[i],
                (oldpol == SCHED_FIFO) ? "FIFO" :
                (oldpol == SCHED_RR) ? "RR" : "OTHER",
                (newpol == SCHED_FIFO) ? "FIFO" :
                (newpol == SCHED_RR) ? "RR" : "OTHER");
    }

    exit(EXIT_SUCCESS);
}
