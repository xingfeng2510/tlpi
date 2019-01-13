#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

extern char **environ;

int main()
{
    printf("address of environ=%p\n", environ);

    for (char **p = environ; *p != NULL; p++)
        puts(*p);

    pid_t cid = fork();
    if (cid > 0) {
        printf("parent PID=%d, environ=%p\n", getpid(), environ);
        sleep(60);
        printf("parent envrion=%p\n", environ);
        _exit(1);
    }

    printf("child PID=%d\n", getpid());
    printf("address of environ=%p\n", environ);
    sleep(30);
    
    char *s = "XMELON=hello";
    if (putenv(s)) {
        printf("cannot set %s\n", s);
        exit(EXIT_FAILURE);
    }
    printf("address of environ=%p\n", environ);
    
    char *argv[] = {"tailf", "-n3", "/home/xingfeng2510/cred.c", NULL};
    execve("/bin/tailf", argv, environ);

    printf("after modifying environment...\n");
    sleep(60);

    char *value = getenv("XMELON");
    printf("getenv(\"XMELON\")=%s\n", value);
    unsetenv("XMELON");
    value = getenv("XMELON");
    printf("getenv(\"XMELON\")=%s\n", value == NULL ? "" : value);

    return 0;
}
