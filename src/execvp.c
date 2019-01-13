#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

extern char **environ;

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s command [arg]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    for (char **p = environ; *p; p++)
        printf("%s\n", *p);
    
    execvp(argv[optind], argv+optind); 
    perror("execvp");

    return 0;
}
