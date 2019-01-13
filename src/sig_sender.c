#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int num_sigs, sig, i;
    pid_t pid;

    if (argc < 4 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "Usage: %s pid num-sigs sig-num [sig-num-2]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    pid = atoi(argv[1]);
    num_sigs = atoi(argv[2]);
    sig = atoi(argv[3]);

    printf("%s: sending signal %d to process %ld %d times\n",
            argv[0], sig, (long) pid, num_sigs);

    for (i = 0; i < num_sigs; i++) {
        if (kill(pid, sig) == -1) {
            perror("kill");
            exit(EXIT_FAILURE);
        }
    }

    if (argc > 4) {
        int sig2 = atoi(argv[4]);
        if (kill(pid, sig2) == -1) {
            perror("kill");
            exit(EXIT_FAILURE);
        }
    }
    
    printf("%s: exiting\n", argv[0]);

    exit(EXIT_SUCCESS);
}
