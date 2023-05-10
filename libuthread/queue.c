#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "queue.h"

typedef struct element* element_t;

struct element{
	void *element_data_address;
	struct element *next_element;
};


struct queue {
	int queue_size;
	element_t first;
	element_t last;

};

queue_t queue_create(void)
{
	queue_t que = (queue_t)malloc(sizeof(struct queue));
	que->queue_size = 0;
	que->first = NULL;
	que->last = NULL;
	return que;
}

int queue_destroy(queue_t queue)
{
	/*return -1 if the queue is NULL or if the queue is not empty*/
	if(queue == NULL || queue->first != NULL || queue->last != NULL){
		return -1;
	}
	free(queue);
	return 0;
}

int queue_enqueue(queue_t queue, void *data)
{

	if(queue == NULL || data == NULL){
		return -1;
	}

	element_t new_element = (element_t)malloc(sizeof(struct element));
	new_element->element_data_address = data;
	new_element->next_element = NULL;

	if(queue->queue_size == 0){
		/*If the item to be enqueue is the first item*/
		queue->first = new_element;
		queue->last = new_element;
	}else{
		queue->last->next_element = new_element;
		queue->last = new_element;
	}
	queue->queue_size++;
	return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	if(queue == NULL || queue_length(queue) == 0 || queue->first->element_data_address == NULL || queue->first == NULL){
		return -1;
	}

	/*FIFO requires that first element will be dequeue everytime*/
	element_t dequeue_element;
	if(queue_length(queue) == 1){
		dequeue_element = queue->first;
		*data = dequeue_element->element_data_address;
		queue->first = NULL;
		queue->last = NULL;
		queue->queue_size = 0;
	}else{
		dequeue_element = queue->first;
		*data = dequeue_element->element_data_address;
		queue->first = dequeue_element->next_element;
		queue->queue_size = queue->queue_size - 1;
	}
	return 0;	
}

int queue_delete(queue_t queue, void *data)
{
	if(queue == NULL || data == NULL){
		return -1;
	}

	bool data_found = false;
	element_t previous_element;
	element_t current_element;

	previous_element = NULL; 
	current_element = queue->first;
	for(int i = 0; i < queue_length(queue); ++i){
		if(current_element->element_data_address == data){
			data_found = true;
			break;
		}
		previous_element = current_element;
		current_element = previous_element->next_element;
	}

	/*if data is found, current element is the element to be deleted*/
	if(data_found == false){
		return -1;
	}

	if(previous_element == NULL && current_element->next_element != NULL){
		/*the element to be deleted is the first element*/
		queue->first = current_element->next_element;
		free(current_element);
		queue->queue_size--;
	}else if(previous_element != NULL && current_element->next_element == NULL){
		/*the element to be deleted is the last element*/
		queue->last = previous_element;
		free(current_element);
		queue->queue_size--;
	}else if(previous_element == NULL && current_element->next_element == NULL){
		/*the element to be deleted is the only element in the queue*/
		queue->first = NULL;
		queue->last = NULL;
		free(current_element);
		queue->queue_size = 0;
	}else{
		previous_element->next_element = current_element->next_element;
		current_element->next_element = NULL;
		free(current_element);
		queue->queue_size--;
	}
	return 0;
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	if(queue == NULL || func == NULL){
		return -1;
	}

	element_t element_iterator = queue->first;
	element_t element_iterator_2 = queue->first;
	for(int i = 0; i < queue_length(queue); ++i){
		func(element_iterator->element_data_address);
		element_iterator = element_iterator_2->next_element;
		element_iterator_2 = element_iterator;
	}
	return 0;
}

int queue_length(queue_t queue)
{
	if(queue == NULL){
		return -1;
	}
	return queue->queue_size;
}
