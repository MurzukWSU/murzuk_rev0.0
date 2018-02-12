#include "cc1110.h"
#include "DataFrame.h"
#include "DataTypes.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/********************************************************************************
*---FUNCTION---
* Name: create_Data_Queue()
* Description:
*	Creates and allocates memory for a Data RX or TX queue.
* Parameters:
*	uint8 capacity - the capacity of the queue
* Returns:
*	Data_Queue* - pointer to the created data queue
*********************************************************************************/	
Data_Queue* create_Data_Queue(uint8 capacity)
{
	//Allocate memory for the new Data_Queue
	Data_Queue* queue = (Data_Queue *) malloc(sizeof(Data_Queue));
	
	//Set the capacity of the queue
	queue->capacity = capacity;
	
	//Set the counter for the head and tail of the queue, set the size of the queue
	queue->head = 0;
	queue->size = 0;
	queue->tail = capacity - 1;
	
	//Allocate memory for the list of Data_Frames in the queue
	queue->Data_Frame_Array_List = (Data_Frame *) malloc(sizeof(Data_Frame) * queue->capacity);	

	return queue;

}

/********************************************************************************
*---FUNCTION---
* Name: isFullData()
* Description:
*	Returns true if the size of the queue is equal to the capacity of the
*       queue.
* Parameters:
*	Data_Queue* - pointer to the queue	
* Returns:
*	uint8 - boolean value, 1 if queue is full, 0 if queue is not
*	full	
*********************************************************************************/	
uint8 isFullData(Data_Queue* queue)
{
	return(queue->size == queue->capacity);
}

/********************************************************************************
*---FUNCTION---
* Name: isEmptyData()
* Description:
*	Returns true if the size of the queue is equal to 0.
* Parameters:
*	Data_Queue* - pointer to the queue	
* Returns:
*	uint8 - boolean value, 1 if queue is empty, 0 if queue is not
*	empty	
*********************************************************************************/	
uint8 isEmptyData(Data_Queue* queue)
{
	return(queue->size == 0);
}

/********************************************************************************
*---FUNCTION---
* Name: enqueueData()
* Description:
*	Enqueues the passed Data_Frame at the tail of the queue.
* Parameters:
*	Data_Queue* - pointer to the queue
*	Data_Frame* - pointer to the frame being enqueued	
* Returns:
*	uint8 - boolean value, 1 if enqueue fails, 0 if it succeeds
*********************************************************************************/	
uint8 enqueueData(Data_Queue* queue, Data_Frame* data)
{
	//Check to make sure the queue has available space to enqueue the new item
	if(isFullData(queue))
	{
		return 1;
	}
	
	//Adjust the position of the tail counter
	queue->tail = (queue->tail + 1) % queue->capacity;

	//Copy the bits from the Data_Frame into the queue
	memcpy(&(queue->Data_Frame_Array_List[queue->tail]), data, sizeof(*data));

	//Update the size of the queue
	queue->size = queue->size + 1;

	//Free the memory for the original Data_Frame
	free(data);
	
	return 0;
}

/********************************************************************************
*---FUNCTION---
* Name: dequeueData()
* Description:
*	Dequeues the the Data_Frame at the head of the queue.
* Parameters:
*	Data_Queue* - pointer to the queue
* Returns:
*	Data_Frame* - dequeued frame
*********************************************************************************/	
Data_Frame* dequeueData(Data_Queue* queue)
{
	Data_Frame* data;
	
	//Check to make sure the queue has at least element that can be dequeued
	if(isEmptyData(queue))
	{
		return NULL;
	}

	//Allocate space to return the dequeued Data_Frame
	data = (Data_Frame *) malloc(sizeof(Data_Frame));

	//Copy over the bits from the element in the queue to new Data_Frame	
	memcpy(data, &(queue->Data_Frame_Array_List[queue->head]), sizeof(queue->Data_Frame_Array_List[queue->head]));
	
	//Set the bits in the queue (since the queue is fixed capacity memory will not be freed, this "clears" the memory)
	memset(&(queue->Data_Frame_Array_List[queue->head]), 0x00, sizeof(Data_Frame));
	
	//Adjust the head pointer of the queue
	queue->head = (queue->head + 1) % queue->capacity;
	
	//Update the size of the queue
	queue->size = queue->size - 1;

	return data;
}
