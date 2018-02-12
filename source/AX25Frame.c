#include "cc1110.h"
#include "AX25Frame.h"
#include "DataTypes.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/********************************************************************************
*---FUNCTION---
* Name: create_AX25_Queue()
* Description:
*	Creates and allocates memory for an AX25 RX or TX queue.
* Parameters:
*	uint8 capacity - capacity of the queue
* Returns:
*	AX25_Queue* - pointer to the create AX25_Queue object
*********************************************************************************/	
AX25_Queue* create_AX25_Queue(uint8 capacity)
{
	//Allocate space for an AX25_Queue
	AX25_Queue* queue = (AX25_Queue *) malloc(sizeof(AX25_Queue));
	
	//Set the capacity of the queue
	queue->capacity = capacity;
	
	//Set the counters for head and tail and set the size of the queue to 0
	queue->head = 0;
	queue->size = 0;
	queue->tail = capacity - 1;

	//Allocate space for the list of AX25_Frames in the queue
	queue->AX25_Frame_Array_List = (AX25_Frame *) malloc(sizeof(AX25_Frame) * queue->capacity);	

	return queue;
}

/********************************************************************************
*---FUNCTION---
* Name: isFullAX25()
* Description:
*	Returns true if the size of the queue is equal to the capacity of the
*       queue.
* Parameters:
*	AX25_Queue* - pointer to the queue	
* Returns:
*	uint8 - boolean value, 1 if queue is full, 0 if queue is not
*	full	
*********************************************************************************/	
uint8 isFullAX25(AX25_Queue* queue)
{
	return(queue->size == queue->capacity);
}

/********************************************************************************
*---FUNCTION---
* Name: isEmptyAX25()
* Description:
*	Returns true if the size of the queue is equal to 0.
* Parameters:
*	AX25_Queue* - pointer to the queue	
* Returns:
*	uint8 - boolean value, 1 if queue is empty, 0 if queue is not
*	empty	
*********************************************************************************/	
uint8 isEmptyAX25(AX25_Queue* queue)
{
	return(queue->size == 0);
}

/********************************************************************************
*---FUNCTION---
* Name: enqueueAX25()
* Description:
*	Enqueues the passed AX25_Frame at the tail of the queue.
* Parameters:
*	AX25_Queue* - pointer to the queue
*	AX25_Frame* - pointer to the frame being enqueued	
* Returns:
*	uint8 - boolean value, 1 if enqueue fails, 0 if it succeeds
*********************************************************************************/	
uint8 enqueueAX25(AX25_Queue* queue, AX25_Frame* frame)
{
	//Check to make sure the queue has available space for an additional frame
	if(isFullAX25(queue))
	{
		return 1;
	}
		
	//Adjust the tail counter for the queue
	queue->tail = (queue->tail + 1) % queue->capacity;

	//Copy the memory from the frame into the queue
	memcpy(&(queue->AX25_Frame_Array_List[queue->tail]), frame, sizeof(*frame));
	
	//Update the size of the queue
	queue->size = queue->size + 1;

	//Free the memory of the element since it is now in the queue
	free(frame);
	
	return 0;
}

/********************************************************************************
*---FUNCTION---
* Name: dequeueAX25()
* Description:
*	Dequeues the the AX25_Frame at the head of the queue.
* Parameters:
*	AX25_Queue* - pointer to the queue
* Returns:
*	AX25_Frame* - dequeued frame
*********************************************************************************/	
AX25_Frame* dequeueAX25(AX25_Queue* queue)
{
	AX25_Frame* frame;
	
	//Check to make sure the queue has an available element to be dequeued
	if(isEmptyAX25(queue))
	{
		return NULL;
	}

	//Allocate memory to store the dequeued frame bits
	frame = (AX25_Frame *) malloc(sizeof(AX25_Frame));	

	//Copy the frame bits from the element in the queue to the new element
	memcpy(frame, &(queue->AX25_Frame_Array_List[queue->head]), sizeof(queue->AX25_Frame_Array_List[queue->head]));

	//Set the bits of the element in the queue (the queue is fixed capacity so its memory will not be reclaimed)
	memset(&(queue->AX25_Frame_Array_List[queue->head]), 0x00, sizeof(AX25_Frame));

	//Update the head counter and size values for the queue
	queue->head = (queue->head + 1) % queue->capacity;
	queue->size = queue->size - 1;

	return frame;
}
