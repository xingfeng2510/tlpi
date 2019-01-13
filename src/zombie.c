#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h> 

#define CMD_SIZE 200

int main(int argc, char *argv[]) {
    char cmd[CMD_SIZE]; 
    pid_t childPid;

    setbuf(stdout, NULL);

    printf("Parent PID=%ld\n", (long) getpid());

    switch (childPid = fork()) { 
        case -1:
            perror("fork");
            _exit(EXIT_FAILURE);

        case 0: 
            printf("Child (PID=%ld) exiting\n", (long) getpid()); 
            _exit(EXIT_SUCCESS);

        default:
            sleep(3); /* Give child a chance to start and exit */ 
            snprintf(cmd, CMD_SIZE, "ps | grep %s", basename(argv[0])); 
            cmd[CMD_SIZE - 1] = '\0'; /* Ensure string is null-terminated */ 
            system(cmd);

            if (kill(childPid, SIGKILL) == -1) 
                perror("kill");

            sleep(3);
            printf("After sending SIGKILL to zombie (PID=%ld):\n", (long) childPid);
            system(cmd);

            exit(EXIT_SUCCESS);
    }
}
