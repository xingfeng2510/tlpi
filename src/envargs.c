#include <stdlib.h>
#include <stdio.h>

extern char **environ;

int main(int argc, char *argv[])
{
    int i;
    char **p;

    for (i = 0; i < argc; i++)
        printf("argv[%d]=%s\n", i, argv[i]);

    for (p = environ; *p != NULL; p++)
        printf("environ: %s\n", *p);

    exit(EXIT_SUCCESS);
}
