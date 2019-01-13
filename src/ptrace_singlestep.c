#include <sys/ptrace.h>  
#include <sys/types.h>  
#include <sys/wait.h>  
#include <unistd.h>  
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])  
{  
    pid_t cpid;
    int wstatus;
    int count = 0;
    cpid = fork(); 
    if(cpid == 0) { 
        execl("./test", "test", NULL); 
    } else { 
        ptrace(PTRACE_ATTACH, cpid, NULL, NULL); 
        while (1) { 
            wait(&wstatus); 
            if (WIFEXITED(wstatus)) 
                break; 
            else if (WIFSTOPPED(wstatus)) {
                printf("stopped by signal %d\n", WSTOPSIG(wstatus));
            }
            count++; 
            ptrace(PTRACE_SINGLESTEP, cpid, NULL, NULL); 
        }
        printf("Total Instruction number= %d\n", count); 
    } 
}
