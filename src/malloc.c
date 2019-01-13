#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    char *ptr[1000];
    
    printf("initial program break: %10p\n", sbrk(0));

    sleep(30);

    for (int j = 0; j < 1000; j++) { 
        ptr[j] = malloc(10240); 
        if (ptr[j] == NULL) 
            perror("malloc"); 
    }

    printf("program break is now: %10p\n", sbrk(0));
    
    for (int j = 0; j < 1000; j++)
        free(ptr[j]);

    printf("after free(), program break is: %10p\n", sbrk(0));
}
