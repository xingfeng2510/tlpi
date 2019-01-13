#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

int main(int argc, char *argv[])
{
    FILE *tfile;
    char *ptr;
    int wrote;

    tfile = tmpfile();
    if (tfile == NULL)
        handle_error("tmpfile");
    setbuf(tfile, NULL);

    ptr = "hello world";
    wrote = fwrite((void *) ptr, sizeof(ptr[0]), strlen(ptr), tfile);
    if (wrote != strlen(ptr))
        handle_error("fwrite");

    sleep(60);

    exit(EXIT_SUCCESS);
}
