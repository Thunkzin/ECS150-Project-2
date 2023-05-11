/*
    Simple test for preempt
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>
#include <preempt.c>
#include <private.h>




void thread2(void *arg)
{
	(void)arg;
	uthread_yield();
	printf("thread2\n");
    
}

void thread1(void *arg)
{
	(void)arg;
    uthread_create(thread2, NULL);
	uthread_yield();
	printf("thread1\n");
    uthread_block();
    printf("I'm blocked\n");
}

int main(void)
{
	uthread_run(true, thread1, NULL);
	return 0;
}
