#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

#include "common.h"

static int cmdNum;  /* Our position in pipeline */

static void handler(int sig)
{
    if (getpid() == getpgrp())
        fprintf(stderr, "Terminal FG process group: %ld\n",
                (long) tcgetpgrp(STDERR_FILENO));
    fprintf(stderr, "Process %ld (%d) received signal %d (%s)\n",
            (long) getpid(), cmdNum, sig, strsignal(sig));

    if (sig == SIGTSTP)
        raise(SIGSTOP);
}

int main(int argc, char *argv[])
{
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = handler;
    if (sigaction(SIGINT, &sa, NULL) == -1)
        handle_error("sigaction");
    if (sigaction(SIGTSTP, &sa, NULL) == -1)
        handle_error("sigaction");
    if (sigaction(SIGCONT, &sa, NULL) == -1)
        handle_error("sigaction");

    if (isatty(STDIN_FILENO)) {
        fprintf(stderr, "Terminal FG process group: %ld\n",
                (long) tcgetpgrp(STDIN_FILENO));
        fprintf(stderr, "Command   PID  PPID  PGRP   SID\n");
        cmdNum = 0;
    } else {
        if (read(STDIN_FILENO, &cmdNum, sizeof(cmdNum)) <= 0)
            handle_error("read");
    }

    cmdNum++;
    fprintf(stderr, "%4d    %5ld %5ld %5ld %5ld\n", cmdNum,
            (long) getpid(), (long) getppid(),
            (long) getpgrp(), (long) getsid(0));

    if (!isatty(STDOUT_FILENO)) /* If not tty, then should be pipe */
        if (write(STDOUT_FILENO, &cmdNum, sizeof(cmdNum)) == -1)
            handle_error("write");

    for (;;)
        pause();
}
