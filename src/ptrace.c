#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>   /* For constants ORIG_RAX etc */
#include <sys/user.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void printStatus(int wstatus) {
    if WIFEXITED(wstatus) {
        printf("exited, status=%d\n", WEXITSTATUS(wstatus));
        _exit(wstatus);
    } else if WIFSIGNALED(wstatus) {
        printf("killed by signal %d\n", WTERMSIG(wstatus));
    } else if WIFSTOPPED(wstatus) {
        printf("stopped by signal %d\n", WSTOPSIG(wstatus));
    } else if WIFCONTINUED(wstatus) {
        printf("continued\n");
    }
}

int main() {
    pid_t cpid, w;
    int wstatus;
    int flag = 0;
    long scid, retval;

    switch(cpid = fork()) { 
        case -1: 
            perror("fork");
            exit(EXIT_FAILURE);
        case 0: 
            ptrace(PTRACE_TRACEME, 0, NULL, NULL); 
            execl("/bin/ls", "ls", NULL); 
        default: 
            w = waitpid(cpid, &wstatus, WUNTRACED | WCONTINUED);
            if (w == -1) {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }
            printStatus(wstatus);
            scid = ptrace(PTRACE_PEEKUSER, cpid, ORIG_RAX*8, NULL); 
            printf("Process executed system call ID = %ld\n", scid); 

            ptrace(PTRACE_SYSCALL, cpid, NULL, NULL); 
            while(1) { 
                w = waitpid(cpid, &wstatus, WUNTRACED | WCONTINUED);
                if (w == -1) {
                    perror("waitpid");
                    exit(EXIT_FAILURE);
                }
                printStatus(wstatus);
                if (flag == 0) { 
                    //scid = ptrace(PTRACE_PEEKUSER, cpid, ORIG_RAX*8, NULL); 
                    //printf("0. Process executed system call ID = %ld\n", scid);
                    struct user_regs_struct regs;
                    scid = ptrace(PTRACE_GETREGS, cpid, NULL, &regs);
                    printf("0, system call id = %ld, %llu, %llu, %llu\n", scid, regs.rbx, regs.rcx, regs.rdx);
                    flag = 1; 
                } else { 
                    retval = ptrace(PTRACE_PEEKUSER, cpid, RAX*8, NULL); 
                    printf("1. with return value= %ld\n", retval); 
                    flag = 0; 
                } 
                ptrace(PTRACE_SYSCALL, cpid, NULL, NULL); 
            } 
    } 

    return 0;
}
