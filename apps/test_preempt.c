#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>
#include <preempt.c>
#include <private.h>

void thread3(void *arg)
{
	(void)arg;
	printf("\nI'm in thread 3, exit.\n");
	exit(0);
}

void thread2(void *arg)
{
	(void)arg;
	uthread_create(thread3, NULL);
	while (1)
	{
		printf("2");
	}
}

void thread1(void *arg)
{
	(void)arg;
	uthread_create(thread2, NULL);
	while (1)
	{
		printf("1");
	}
}

int main(void)
{
	uthread_run(true, thread1, NULL);
	return 0;
}
