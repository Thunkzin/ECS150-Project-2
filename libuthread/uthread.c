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
//#include "preempt.c"



struct uthread_tcb *current;
queue_t alive_thread_queue;
queue_t zombie_thread_queue;

struct uthread_tcb {
	enum { running, ready, blocked, zombie} thread_state;
	void* stack;
	uthread_ctx_t *thread_context;
};




struct uthread_tcb *uthread_current(void){
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

	if(current->thread_state == running){
		//set the currently running thread's state into ready
		current->thread_state = ready;
		//put the ready thread into the alive_queue to wait for next run
		queue_enqueue(alive_thread_queue, current);
	}

	struct uthread_tcb *popped_out_thread = NULL;
	//pop out the oldest thread from the queue, and store it into popped_out_thread
	if(queue_dequeue(alive_thread_queue, (void**)popped_out_thread) != 0){
		//if error occurs, return
		return;
	}
	struct uthread_tcb *current_thread_snap_shot = uthread_current();

	if(popped_out_thread->thread_state == ready){
		//set the popped_out_thread into running
		popped_out_thread->thread_state = running;	
		//turn current thread into the popped_out_thread (the snap shot is taken so it's fine)
		current = popped_out_thread;
		//switch the context between thread_snap_shot and popped_out_thread.
		uthread_ctx_switch(current_thread_snap_shot->thread_context, popped_out_thread->thread_context);
	}

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
	perror("Exiting current thread.\n");
	struct uthread_tcb *exiting_thread;
	exiting_thread = uthread_current();
	/* set the current thread to zombie when exit func is called */
	exiting_thread->thread_state = zombie;
	/* enqueue the current thread onto zombie_thread_queue */
	queue_enqueue(zombie_thread_queue,exiting_thread);
	/* yield */
	uthread_yield();

	/* trigger an error and terminate in case current thread continue to execute after call to unthread_exit  */
	assert(false);

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
		int i = 0;
		i++;
	}


	//create the idle_thread
	struct uthread_tcb *idle_thread = (struct uthread_tcb*)malloc(sizeof(struct uthread_tcb));
	if(idle_thread == NULL){
		return -1;
	}
	
	//initialize the thread's context
	idle_thread->thread_context = (uthread_ctx_t *)malloc(sizeof(uthread_ctx_t));

	//set the current stack's state
	idle_thread->thread_state = running;
	//allocate current stack's memory 
	idle_thread->stack = uthread_ctx_alloc_stack;
	current = idle_thread;
	uthread_ctx_init(current->thread_context, current->stack, func, arg);

	// create the very first thread into the alive queue
	uthread_create(func, arg);

	while(alive_thread_queue != 0){
		while(zombie_thread_queue != 0){
			struct uthread_tcb *zombie_thread;
			//move the dequeued data into zombie_thread's context, then remove the thread
			queue_dequeue(zombie_thread_queue, (void**)zombie_thread->thread_context);
			free(zombie_thread->thread_context);
			uthread_ctx_destroy_stack(zombie_thread->stack);
		}
		//keep running different thread.
		uthread_yield();
	}
	return 0;
}

// void uthread_block(void)
// {
// 	current->thread_state = blocked;
// }

// void uthread_unblock(struct uthread_tcb *uthread)
// {
// 	preempt_enable();
// 	preempt_disable();
// 	queue_enqueue(alive_thread_queue, uthread);
// 	preempt_enable();
// 	return;
// }
