#include <syslog.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    openlog(argv[0], LOG_PID, LOG_USER);
    syslog(LOG_USER | LOG_INFO, "Exiting");
    closelog();

    exit(EXIT_SUCCESS);
}
