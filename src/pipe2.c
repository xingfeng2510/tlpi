#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int pfd[2];
    pid_t cpid;
    char buf;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (pipe(pfd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0) {
        if (dup2(pfd[1], STDOUT_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }

        for (int i = 3; i < 1024; i++)
            close(i);

        char *argv2[] = {"echo", argv[1], NULL};
        execvp(argv2[0], argv2);
        perror("execvp");
    } else {
        close(pfd[1]);
        char *output = malloc(strlen(argv[1]) + 1);
        output[strlen(argv[1])] = '\0';
        read(pfd[0], output, strlen(argv[1]));
        printf("output: %s\n", output);
        close(pfd[0]);
        wait(NULL);
        exit(EXIT_SUCCESS);
    }
}
