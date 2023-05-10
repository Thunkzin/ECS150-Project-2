#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

struct semaphore {
	size_t sem_count;
	queue_t *waiting_queue;
};

sem_t sem_create(size_t count)
{
/* TODO Phase 3 */
/*
 * sem_create - Create semaphore
 * @count: Semaphore count
 *
 * Allocate and initialize a semaphore of internal count @count.
 *
 * Return: Pointer to initialized semaphore. NULL in case of failure when
 * allocating the new semaphore.
 */
	/* create semaphore */
	sem_t sem = (sem_t)malloc(sizeof(struct semaphore));

	/* check if allocation is succeed */
	if(sem == NULL){
		return NULL;
	}

	/* initialized semaphore count */
	sem->sem_count = count;
	/* create an empty blocked_queue */
	sem->waiting_queue = queue_create();
	return sem;

}

int sem_destroy(sem_t sem)
{
/* TODO Phase 3 */
/*
 * sem_destroy - Deallocate a semaphore
 * @sem: Semaphore to deallocate
 *
 * Deallocate semaphore @sem.
 *
 * Return: -1 if @sem is NULL or if other threads are still being blocked on
 * @sem. 0 is @sem was successfully destroyed.
 */
	/* check if @sem is NULL */
	if(sem == NULL){
		return -1;
	}

	/* check if there are threads in the waiting queue */
	if(sem->waiting_queue != NULL){
		return -1;
	}

	/* deallocate semaphore */
	queue_destroy(sem->waiting_queue);
	free(sem);

	return 0;
}

int sem_down(sem_t sem)
{
/* TODO Phase 3 */
/*
 * sem_down - Take a semaphore
 * @sem: Semaphore to take
 *
 * Take a resource from semaphore @sem.
 *
 * Taking an unavailable semaphore will cause the caller thread to be blocked
 * until the semaphore becomes available.
 *
 * Return: -1 if @sem is NULL. 0 if semaphore was successfully taken.
 */
	/* check if @sem is NULL */
	if(sem == NULL){
		return -1;
	}

	struct uthread_tcb *current_thread = uthread_current();

	/* block semaphore when count is zero */
	while(sem->sem_count == 0){
		queue_enqueue(sem->waiting_queue,(void*)current_thread);
		uthread_yield(); // 
	}
	return 0;
}

int sem_up(sem_t sem)
{
/* TODO Phase 3 */
/*
 * sem_up - Release a semaphore
 * @sem: Semaphore to release
 *
 * Release a resource to semaphore @sem.
 *
 * If the waiting list associated to @sem is not empty, releasing a resource
 * also causes the first thread (i.e. the oldest) in the waiting list to be
 * unblocked.
 *
 * Return: -1 if @sem is NULL. 0 if semaphore was successfully released.
 */

	if(sem == NULL){
		return -1;
	}
	/*internal count increase by one and the first thread gets unblocked*/
	sem->sem_count += 1;
	if(queue_length(sem->waiting_queue) > 0){
		struct uthread_tcb *first_thread;
		preempt_disable();
		queue_dequeue(sem->waiting_queue, (void**)&first_thread);
		preempt_enable();
		uthread_unblock(first_thread);
	}
	return 0;
}

