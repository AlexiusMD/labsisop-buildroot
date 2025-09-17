#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include "processSleeping.h"
  
asmlinkage long sys_listProcessSleeping(const char __user *buf, int size) {
    struct task_struct *process;
    unsigned char kbuf[4096] = {0};
    unsigned char tempbuf[256];
    int bufsz = 0;
    int tmpsz;
    int ret;

    if (!buf || size <= 0) {
        return -1;
    }

    for_each_process(process) {
       if (process->state & (TASK_INTERRUPTIBLE | TASK_UNINTERRUPTIBLE)) {
            snprintf(tempbuf, sizeof(tempbuf), "Process: %s\n PID_Number: %ld\n Process State: %ld\n\n", 
                process->comm, 
                (long)task_pid_nr(process), 
                (long)process->state);

            tmpsz = strlen(tempbuf);

            if (tmpsz + bufsz + 1 > sizeof(kbuf)) {
                break;
            }

            strncat((char*)kbuf, (char*)tempbuf, sizeof(kbuf) - bufsz - 1);
            bufsz += tmpsz;
        }   
    }

    if (bufsz + 1 > size) {
        return -1;
    }

    ret = copy_to_user((void*)buf, (void*)kbuf, bufsz + 1);

    return bufsz - ret;
}