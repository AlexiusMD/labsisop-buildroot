#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/syscalls.h>
#include "writeToLog.h"

#define MAX_LOG_SIZE 1024
  
asmlinkage long sys_writeToLog(const char __user *buf, int size) {
    char kbuf[MAX_LOG_SIZE];
    long copied;

    // Basic validation
    if (!buf || size <= 0) {
        return -1;
    }

    // Limit size to MAX_LOG_SIZE-1
    if (size > MAX_LOG_SIZE - 1) {
        size = MAX_LOG_SIZE - 1;
    }

    // Copy string from user space
    copied = strncpy_from_user(kbuf, buf, size);

    if (copied < 0) {
        return -1;
    }

    kbuf[copied] = '\0'; // Ensure null termination

    // Print to kernel log
    printk(KERN_INFO "User message: %s\n", kbuf);

    return copied; // Return number of bytes copied
}
