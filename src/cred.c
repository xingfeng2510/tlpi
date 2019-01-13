#define _GNU_SOURCE

#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

void print_cred(void)
{
    uid_t ruid, euid, suid;
    gid_t rgid, egid, sgid;

    getresuid(&ruid, &euid, &suid);
    getresgid(&rgid, &egid, &sgid);

    printf("ruid=%d, euid=%d, suid=%d\n", ruid, euid, suid);
    printf("rgid=%d, egid=%d, sgid=%d\n", rgid, egid, sgid);
}

void xsetuid(uid_t uid)
{
    printf("setuid(%d)...\n", uid);
    int ret = setuid(uid);
    if (ret == -1) {
        perror("setuid");
        exit(EXIT_FAILURE);
    }
}

void xseteuid(uid_t euid)
{
    printf("seteuid(%d)...\n", euid);
    int ret = seteuid(euid);
    if (ret == -1) {
        perror("seteuid");
        exit(EXIT_FAILURE);
    }
}

int main()
{
    print_cred();
    
    uid_t euid = geteuid(); // saved suid
    xsetuid(getuid());
    print_cred();

    xsetuid(euid);
    print_cred();

    return 0;
}
