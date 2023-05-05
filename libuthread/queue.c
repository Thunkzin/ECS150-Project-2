#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"


typedef struct element* element_t;

struct element {
	void *element_data_address;
	struct element *next_element;
};


struct queue {
/*
 * queue_t - Queue type
 *
 * A queue is a FIFO data structure. Data items are enqueued one after the
 * other.  When dequeueing, the queue must returned the oldest enqueued item
 * first and so on.
 *
 * Apart from delete and iterate operations, all operations should be O(1).
 */
	int queue_size;
	element_t first;
	element_t last;
};

queue_t queue_create(void)
{
/*
 * queue_create - Allocate an empty queue
 *
 * Create a new object of type 'struct queue' and return its address.
 *
 * Return: Pointer to new empty queue. NULL in case of failure when allocating
 * the new queue.
 */
	queue_t created_queue = (queue_t)malloc(sizeof(struct queue));
	created_queue->queue_size = 0;
	created_queue->first = NULL;
	created_queue->last = NULL;
	if(perror() == -1){
		return NULL;
	}
	return created_queue;
}

int queue_destroy(queue_t queue)
{
/*
 * queue_destroy - Deallocate a queue
 * @queue: Queue to deallocate
 *
 * Deallocate the memory associated to the queue object pointed by @queue.
 *
 * Return: -1 if @queue is NULL or if @queue is not empty. 0 if @queue was
 * successfully destroyed.
 */
	if(queue == NULL || queue->first != NULL || queue->last != NULL){
		return -1;
	}
	free(queue);
	return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
/*
 * queue_enqueue - Enqueue data item
 * @queue: Queue in which to enqueue item
 * @data: Address of data item to enqueue
 *
 * Enqueue the address contained in @data in the queue @queue.
 *
 * Return: -1 if @queue or @data are NULL, or in case of memory allocation error
 * when enqueing. 0 if @data was successfully enqueued in @queue.
 */
	if(queue == NULL || data == NULL){
		return -1;
	}
	element_t element_to_be_enqueued;
	element_to_be_enqueued->element_data_address = data;
	queue->last->next_element = element_to_be_enqueued;
	queue->last = element_to_be_enqueued;
	
	queue->queue_size ++;
	return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
/*
 * queue_dequeue - Dequeue data item
 * @queue: Queue in which to dequeue item
 * @data: Address of data pointer where item is received
 *
 * Remove the oldest item of queue @queue and assign this item (the value of a
 * pointer) to @data.
 *
 * Return: -1 if @queue or @data are NULL, or if the queue is empty. 0 if @data
 * was set with the oldest item available in @queue.
 */
	element_t element_to_be_dequeued;	
	*data = element_to_be_dequeued->element_data_address;

}

int queue_delete(queue_t queue, void *data)
{
	/* TODO Phase 1 */
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	/* TODO Phase 1 */
}

int queue_length(queue_t queue)
{
	/* TODO Phase 1 */
}

