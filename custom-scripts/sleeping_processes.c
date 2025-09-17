#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>

#define SYSCALL_SLEEPING_PROCESSES	386

int main(int argc, char** argv){  
    char buf[4096];
    long ret;

    printf("Invoking 'listProcessSleeping' system call.\n");
         
    ret = syscall(SYSCALL_SLEEPING_PROCESSES, buf, sizeof(buf)); 
         
    if(ret > 0) {
        /* Success, show the process info. */
        printf("%s\n", buf);
    }
    else {
        printf("System call 'listProcessSleeping' did not execute as expected error %ld\n", ret);
    }
          
    return 0;
}