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
	if(queue == NULL || data == NULL || queue_length(queue) == 0){
		return -1;
	}
	element_t element_to_be_dequeued;	
	element_to_be_dequeued = queue->first;
	*data = element_to_be_dequeued->element_data_address;
	queue->first = element_to_be_dequeued->next_element;
	queue->queue_size -= 1;
	return 0;
}

int queue_delete(queue_t queue, void *data)
{
/*
 * queue_delete - Delete data item
 * @queue: Queue in which to delete item
 * @data: Data to delete
 *
 * Find in queue @queue, the first (ie oldest) item equal to @data and delete
 * this item.
 *
 * Return: -1 if @queue or @data are NULL, of if @data was not found in the
 * queue. 0 if @data was found and deleted from @queue.
 */
	if(queue == NULL || data == NULL){
        	return -1;
  	  }

	element_t current = queue->first;
	element_t previous = NULL;
	int current_node_number = 0;
	int found = 0;

	/* iterate through the queue to find the data */
	while (current_node_number < queue_length(queue)) {
		if(*current->element_data_address == data){
			found = 1;
			break;
		}
		/* move to next node */
		previous = current;
		current = current->next_element;
		current_node_number++;

	}
	if(!found){
		return -1;
	}
	/* delete the node with matching data */
	/* current is at first node */
	if(previous == NULL){
		queue->first = current->next_element; // the node next to current become first node.
	}
	/* current is not at first node and it's somewhere in between */
	else {
		previous->next_element = current->next_element;
	}
	/* if current is at the last node */
	if (current == queue->last) {
		queue->last = previous;
	}
	queue->queue_size =- 1;
	free(current);
	return 0;
}
}

int queue_iterate(queue_t queue, queue_func_t func)
{
/*
 * queue_iterate - Iterate through a queue
 * @queue: Queue to iterate through
 * @func: Function to call on each queue item
 *
 * This function iterates through the items in the queue @queue, from the oldest
 * item to the newest item, and calls the given callback function @func on each
 * item. The callback function receives the current data item as parameter.
 *
 * Note that this function should be resistant to data items being deleted
 * as part of the iteration (ie in @func).
 *
 * Return: -1 if @queue or @func are NULL, 0 otherwise.
 */
	if(queue == NULL || func == NULL){
		return -1;
	}
	element_t current_element = queue->first;
	for(int current_element_number = 0; current_element_number < queue_length(queue); ++current_element_number){
		func(queue, current_element->element_data_address);
		current_element = current_element->next_element;
	}
	return 0;
}
/*
 * typedef void (*queue_func_t)(queue_t queue, void *data);

 * queue_func_t - Queue callback function type
 * @queue: Queue to which item belongs
 * @data: Data item
 *
 * Function to be run on each item using queue_iterate(). The current item is
 * received as @data.
 */


int queue_length(queue_t queue)
{
/*
 * queue_length - Queue length
 * @queue: Queue to get the length of
 *
 * Return the length of queue @queue.
 *
 * Return: -1 if @queue is NULL. Length of @queue otherwise.
 */
	if(queue == NULL){
		return -1;
	}
	return queue->queue_size;
}

