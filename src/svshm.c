#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>

#include "common.h"

int main(int argc, char *argv[])
{
    int shmid;
    void *addr, *addr2;
    size_t page_size;

    page_size = sysconf(_SC_PAGESIZE);
    if (page_size == -1)
        handle_error("sysconf");

    shmid = shmget(IPC_PRIVATE, 8*page_size, IPC_CREAT | S_IRUSR | S_IWUSR);
    if (shmid == -1)
        handle_error("shmget");
    if (shmctl(shmid, SHM_LOCK, NULL) == -1)
        handle_error("shmctl - lock");

    printf("shmid=%d\n", shmid);

    addr = shmat(shmid, NULL, 0);
    if (addr == (void *) -1)
        handle_error("shmat 1");
    printf("%d -> %p\n", shmid, addr);

    for (int i = 0; i < 8; i++)
        *((char *) addr + i * page_size) = 'a';

    addr2 = shmat(shmid, NULL, SHM_RDONLY);
    if (addr2 == (void *) -1)
        handle_error("shmat 2");
    printf("%d -> %p\n", shmid, addr2);

    printf("sleeping 5 seconds\n");
    sleep(5);

    if (shmdt(addr2) == -1)
        handle_error("shmdt 2");
    if (shmdt(addr) == -1)
        handle_error("shmdt");
    if (shmctl(shmid, IPC_RMID, 0) == -1)
        handle_error("shmctl - rmid");

    printf("shmdt/shmctl done\n");
    sleep(5);

    exit(EXIT_SUCCESS);
}
