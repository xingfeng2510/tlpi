#include <stdlib.h>
#include <stdio.h>

#include "common.h"

int main(void)
{
    FILE *fp;
    char buf[128];
    int status;

    fp = popen("ls", "r");
    if (fp == NULL)
        handle_error("popen");

    while (fgets(buf, 128, fp) != NULL)
        printf("%s", buf);

    status = pclose(fp);
    printf("pclose() status = %#x\n", (unsigned int) status);

    exit(EXIT_SUCCESS);
}
