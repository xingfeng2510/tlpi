#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    time_t t;
    struct timeval tv;

    t = time(NULL);
    printf("Seconds since the Epoch (1 Jan 1970): %ld\n", (long) t);

    if (gettimeofday(&tv, NULL) == -1) {
        perror("gettimeofday");
        exit(EXIT_FAILURE);
    }
    printf("gettimeofday() returned %ld secs, %ld microsecs\n",
        (long) tv.tv_sec, (long) tv.tv_usec);

    exit(EXIT_SUCCESS);
}
