#define _GNU_SOURCE
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

static int opendb(void)
{
    int fd = open("/var/lib/mlocate/mlocate.db", O_RDONLY);
    if (fd >= 0) {
        if (fcntl(fd, F_SETFD, FD_CLOEXEC) != 0) {
            close(fd);
            fd = -1;
        }
    } else {
        puts("failed to open dbfile");
        exit(EXIT_FAILURE);
    }
    return fd;
}

void print_cred(void)
{
    uid_t ruid, euid, suid;
    gid_t rgid, egid, sgid;

    getresuid(&ruid, &euid, &suid);
    getresgid(&rgid, &egid, &sgid);

    printf("ruid=%d, euid=%d, suid=%d\n", ruid, euid, suid);
    printf("rgid=%d, egid=%d, sgid=%d\n", rgid, egid, sgid);
}

static void drop_privs(void)
{
    if (setuid(getuid()) != 0) {
        puts("failed to drop setuid privileges");
        exit(EXIT_FAILURE);
    }

    if (setresgid(-1, getgid(), getgid()) != 0) {
        puts("failed to drop setgid privileges");
        exit(EXIT_FAILURE);
    }

    print_cred();
}

int main()
{
    print_cred();

    sleep(15);

    int dbfd = opendb();
    drop_privs();
    sleep(15);
    close(dbfd);

    dbfd = opendb();

    return 0;
}
