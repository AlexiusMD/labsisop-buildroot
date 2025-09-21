#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SYSCALL_WRITETOLOG 387

void usage() {
    printf("Usage: writeToLog <message>\n");
    exit(0);
}

int main(int argc ,char* argv[]) {
    if (argc < 2) {
        usage();
    }

    char msg[1024];

    for (int i = 1; i < argc; i++) {
        strcat(msg, argv[i]);
        if (i < argc - 1) {
            strcat(msg, " ");
        }
    }

    long res = syscall(SYSCALL_WRITETOLOG, msg, strlen(msg) + 1);
    printf("Sent %ld bytes to kernel log\n", res);

    return 0;
}
