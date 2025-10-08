#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/sched.h>

#define SCHED_LOW_IDLE 7

volatile int running = 1;

void *run(void *data)
{
	while (running);

	return 0;
}

void print_sched(int policy)
{
	int priority_min, priority_max;

	switch(policy){
		case SCHED_DEADLINE:
			printf("SCHED_DEADLINE");
			break;
		case SCHED_FIFO:
			printf("SCHED_FIFO");
			break;
		case SCHED_RR:
			printf("SCHED_RR");
			break;
		case SCHED_NORMAL:
			printf("SCHED_OTHER");
			break;
		case SCHED_BATCH:
			printf("SCHED_BATCH");
			break;
		case SCHED_IDLE:
			printf("SCHED_IDLE");
			break;
		case SCHED_LOW_IDLE:
			printf("SCHED_LOW_IDLE");
			break;
		default:
			printf("unknown\n");
	}
	priority_min = sched_get_priority_min(policy);
	priority_max = sched_get_priority_max(policy);
	printf(" PRI_MIN: %d PRI_MAX: %d\n", priority_min, priority_max);
}

int setpriority(pthread_t *thr, int newpolicy, int newpriority)
{
	int policy, ret;
	struct sched_param param;

	printf("Trying to set policy to: ");
	print_sched(newpolicy);

	printf("Min prio: %d - Max prio: %d\n", sched_get_priority_min(newpolicy), sched_get_priority_max(newpolicy));
	
	if (newpriority > sched_get_priority_max(newpolicy) || newpriority < sched_get_priority_min(newpolicy)){
		printf("Invalid priority: MIN: %d, MAX: %d\n", sched_get_priority_min(newpolicy), sched_get_priority_max(newpolicy));

		return -1;
	}

	pthread_getschedparam(*thr, &policy, &param);
	printf("current: ");
	print_sched(policy);

	param.sched_priority = newpriority;
	ret = pthread_setschedparam(*thr, newpolicy, &param);
	if (ret != 0)
		perror("perror(): ");

	pthread_getschedparam(*thr, &policy, &param);
	printf("new: ");
	print_sched(policy);

	return 0;
}

int main(int argc, char **argv)
{
	int timesleep, priority;
	pthread_t thr;

	if (argc < 3){
		printf("usage: ./%s <execution_time>\n\n", argv[0]);

		return 0;
	}

	timesleep = atoi(argv[1]);
	priority = atoi(argv[2]);
	pthread_create(&thr, NULL, run, NULL);
	setpriority(&thr, SCHED_LOW_IDLE, priority);
	sleep(timesleep);
	running = 0;
	pthread_join(thr, NULL);

	return 0;
}
