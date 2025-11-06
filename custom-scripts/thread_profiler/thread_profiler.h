#ifndef THREAD_PROFILER_H
#define THREAD_PROFILER_H

// Scheduling policy (low idle) definition
#define SCHED_LOW_IDLE  7

// Default buffer size in bytes
#define DEFAULT_BUFFER_SIZE 1024

typedef struct thread_info_t
{
    char id;
    unsigned int sched_count;
    unsigned int write_count;
    unsigned int priority;
    unsigned int policy;
} thread_info_t;


#endif