#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"
#include "queue.h"



struct uthread_tcb *current;
queue_t alive_thread_queue;
queue_t zombie_thread_queue;

struct uthread_tcb {
	enum state thread_state;
	void* stack;
	uthread_ctx_t *thread_context;
};

enum state{running, ready, blocked, zombie};



struct uthread_tcb *uthread_current(void)
{
	return current;
}

void uthread_yield(void)
{
/*
 * uthread_yield - Yield execution
 *
 * This function is to be called from the currently active and running thread in
 * order to yield for other threads to execute.
 */
	perror("Exiting current thread.\n");
	/* set the current thread to zombie when exit func is called */
	current->thread_state = zombie;
	/* enqueue the current thread onto zombie_thread_queue */
	queue_enqueue(zombie_thread_queue,current);
	/* yield */
	uthread_yield();

	/* trigger an error and terminate in case current thread continue to execute after call to unthread_exit  */
	assert(false);
}

void uthread_exit(void)
{
/*
 * uthread_exit - Exit from currently running thread
 *
 * This function is to be called from the currently active and running thread in
 * order to finish its execution.
 *
 * This function shall never return.
 */

}

int uthread_create(uthread_func_t func, void *arg)
{
/*
 * uthread_create - Create a new thread
 * @func: Function to be executed by the thread
 * @arg: Argument to be passed to the thread
 *
 * This function creates a new thread running the function @func to which
 * argument @arg is passed.
 *
 * Return: 0 in case of success, -1 in case of failure (e.g., memory allocation,
 * context creation).
 */

	struct uthread_tcb *new_thread = (struct uthread_tcb*)malloc(sizeof(struct uthread_tcb));
	//set the current stack's state
	new_thread->thread_state = ready;
	//allocate current stack's memory 
	new_thread->stack = uthread_ctx_alloc_stack;
	//initialize the thread's context
	new_thread->thread_context = (uthread_ctx_t *)malloc(sizeof(uthread_ctx_t));
	uthread_ctx_init(current->thread_context, current->stack, func, arg);
	//enqueue the thread into the queue
	queue_enqueue(alive_thread_queue, new_thread);

	if(new_thread == NULL){
		return -1;
	}

	return 0;
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
/*
 * uthread_run - Run the multithreading library
 * @preempt: Preemption enable
 * @func: Function of the first thread to start
 * @arg: Argument to be passed to the first thread
 *
 * This function should only be called by the process' original execution
 * thread. It starts the multithreading scheduling library, and becomes the
 * "idle" thread. It returns once all the threads ha
 * ve finished running.
 *
 * If @preempt is `true`, then preemptive scheduling is enabled.
 *
 * Return: 0 in case of success, -1 in case of failure (e.g., memory allocation,
 * context creation).
 */
	if(preempt){
		preempt_enable();
	}
	//create the idle_thread
	struct uthread_tcb *idle_thread = (struct uthread_tcb*)malloc(sizeof(struct uthread_tcb));
	if(idle_thread == NULL){
		return -1;
	}
	current = idle_thread;
	//set the current stack's state
	current->thread_state = running;
	//allocate current stack's memory 
	current->stack = uthread_ctx_alloc_stack;
	//initialize the thread's context
	current->thread_context = (uthread_ctx_t *)malloc(sizeof(uthread_ctx_t));
	uthread_ctx_init(current->thread_context, current->stack, func, arg);

	// create the very first thread into the alive queue
	uthread_create(func, arg);

	while(alive_thread_queue != 0){
		while(zombie_thread_queue != 0){
			struct uthread_tcb *zombie_thread;
			//move the dequeued data into zombie_thread's context, then remove the thread
			queue_dequeue(zombie_thread_queue, zombie_thread->thread_context);
			free(zombie_thread->thread_context);
			uthread_ctx_destroy_stack(zombie_thread->stack);
		}
		//keep running different thread.
		uthread_yield();
	}
	return 0;
}

void uthread_block(void)
{
	/* TODO Phase 3 */
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	/* TODO Phase 3 */
}

