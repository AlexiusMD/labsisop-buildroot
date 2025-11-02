#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <linux/sched.h>
#include <thread_profiler.h>

char *buffer;
char *ptr;
size_t buffer_size;
sem_t buffer_lock;

void* work(void* arg) {
    thread_info_t* info = (thread_info_t*) arg;

    while (1) {
        sem_wait(&buffer_lock);

        if (ptr >= buffer + buffer_size) {
            sem_post(&buffer_lock);
            break;
        }

        *ptr = info->id;
        ptr++;
        info->count++;

        sem_post(&buffer_lock);
        sched_yield();
    }

    return NULL;
}

char decode_policy(char* policy) {
    if (strcmp(policy, "SCHED_NORMAL") == 0) {
        return SCHED_NORMAL;
    } else if (strcmp(policy, "SCHED_FIFO") == 0) {
        return SCHED_FIFO;
    } else if (strcmp(policy, "SCHED_RR") == 0) {
        return SCHED_RR;
    } else if (strcmp(policy, "SCHED_BATCH") == 0) {
        return SCHED_BATCH;
    } else if (strcmp(policy, "SCHED_IDLE") == 0) {
        return SCHED_IDLE;
    } else if (strcmp(policy, "SCHED_DEADLINE") == 0) {
        return SCHED_DEADLINE;
    } else if (strcmp(policy, "SCHED_LOW_IDLE") == 0) {
        return SCHED_LOW_IDLE;
    } else if (strcmp(policy, "SCHED_OTHER") == 0) {
        return SCHED_OTHER;
    }

    return -1;
}

void usage() {
    printf("Usage: thread_profiler <thread_num> <buffer_size_in_kb> [<policy_1> <priority_1> ... <policy_n> <priority_n>]\n");
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        usage();
        return EXIT_FAILURE;
    }

    short num_threads = atoi(argv[1]);
    if (num_threads < 1) {
        perror("At least one thread must be allocated.\n");
        return EXIT_FAILURE;
    }

    // Read global buffer size in kb
    buffer_size = atoi(argv[2]);
    if (buffer_size < 1) {
        printf("Using default buffer size (1kb).\n");
        buffer_size = DEFAULT_BUFFER_SIZE;
    } else {
        buffer_size *= DEFAULT_BUFFER_SIZE;
    }

    buffer = (char *) malloc(buffer_size);
    if(!buffer) {
        perror("Failed to allocate buffer.\n");
        return EXIT_FAILURE;
    }
    ptr = buffer;

    // Initialize semaphore
    sem_init(&buffer_lock, 0, 1);

    // Allocate worker thread buffers
    pthread_t threads[num_threads];
    thread_info_t info[num_threads];

    for (int i = 0; i < num_threads; i++) {
        info[i].id = 'A' + i;
        info[i].count = 0;
        info[i].policy = SCHED_OTHER;
        info[i].priority = 0;

        unsigned short idx = 3 + i * 2;

        // Decode user input policy and priority
        if (argc > idx + 1) {
            int policy = decode_policy(argv[idx]);
            int priority = atoi(argv[idx + 1]);

            if (priority > sched_get_priority_max(policy) || priority < sched_get_priority_min(policy)) {
                printf("Invalid priority value for %s: (min: %d) (max: %d)\n", argv[idx], sched_get_priority_min(policy), sched_get_priority_max(policy));
                return EXIT_FAILURE;
            }

            info[i].policy = policy;
            info[i].priority = priority;
        }

        // Set thread scheduling policy
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setschedpolicy(&attr, info[i].policy);

        // Set thread scheduling priority
        struct sched_param param;
        param.sched_priority = info[i].priority;
        pthread_attr_setschedparam(&attr, &param);
        pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

        // Creates thread
        if (pthread_create(&threads[i], &attr, work, &info[i]) != 0) {
            perror("Failed to create thread.\n");
            return EXIT_FAILURE;
        }

        // Frees thread attributes
        pthread_attr_destroy(&attr);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Show buffer content after all writes ended
    printf("Thread scheduling results:\n\n");
    fwrite(buffer, 1, buffer_size, stdout);
    printf("\n\n");

    // Displays every context switch once
    printf("Compacting the buffer:\n");
    char current_char = buffer[0];
    for (size_t i = 1; i < buffer_size; i++) {
        if (buffer[i] != current_char) {
            printf("%c", current_char);
            current_char = buffer[i];
        }
    }
    printf("%c\n", current_char);


    // Show how many times each thread worked
    printf("Thread work counts:\n");
    for (int i = 0; i < num_threads; i++) {
        printf("%c: %d\n", info[i].id, info[i].count);
    }

    free(buffer);
    sem_destroy(&buffer_lock);
    return EXIT_SUCCESS;
}