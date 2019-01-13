#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
    int total, i;
    ssize_t nread;
    char buf[BUF_SIZE];
    FILE *fp;
    char cwd[PATH_MAX];

    snprintf(cwd, PATH_MAX, "/proc/%s/cwd", argv[1]);
    chdir(cwd);

    fp = fopen("core.info", "w+");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    fprintf(fp, "argc=%d\n", argc);
    for (i = 0; i < argc; i++)
        fprintf(fp, "argv[%d]=<%s>\n", i, argv[i]);
    
    total = 0;
    while ((nread = read(STDIN_FILENO, buf, BUF_SIZE)) > 0)
        total += nread;
    fprintf(fp, "Total bytes in core dump: %d\n", total);
    
    fclose(fp);
    exit(EXIT_SUCCESS);
}
