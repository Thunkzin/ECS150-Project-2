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
	struct state thread_state;
	void* stack;
	uthread_ctx_t *thread_context;
};

struct state{
	bool running;
	bool ready;
	bool blocked;
	bool zombie;
};



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
 * "idle" thread. It returns once all the threads have finished running.
 *
 * If @preempt is `true`, then preemptive scheduling is enabled.
 *
 * Return: 0 in case of success, -1 in case of failure (e.g., memory allocation,
 * context creation).
 */
	if(preempt){
		preempt_enable();
	}
	struct uthread_tcb *idle_thread = (struct uthread_tcb*)malloc(sizeof(struct uthread_tcb));
	if(idle_thread == NULL){
		return -1;
	}
	current = idle_thread;
	//set the current stack's state
	current->thread_state.running = true;
	current->thread_state.ready = false;
	current->thread_state.blocked = false;
	current->thread_state.zombie = false;
	//allocate current stack's memory 
	current->stack = uthread_ctx_alloc_stack;
	//initialize the thread's context
	current->thread_context = (uthread_ctx_t *)malloc(sizeof(uthread_ctx_t));
	uthread_ctx_init(current->thread_context, current->stack, func, arg);


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

