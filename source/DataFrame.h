#ifndef DATA_FRAME_H
#define DATA_FRAME_H
#include "DataTypes.h"

/********************************************************************************
*---STRUCT DATA_FRAME---
*
* Description:
*	Comprises one data field to be transferred to or from the communications board
*	to mainboard.
*
*********************************************************************************/	
typedef struct Data_Frame
{
	uint8 data[251];  
	uint8 dest_Addr[7];
	uint8 src_Addr[7];
	uint8 master_Frame_Count;
	uint8 vc_Frame_Count;
	uint8 time_Stamp[8];	
} Data_Frame;

/********************************************************************************
*---STRUCT DATA_QUEUE---
*
* Description:
*	Queue for buffering 251-Byte data field transfers to and from mainboard.
*
*********************************************************************************/	
typedef struct Data_Queue
{
	uint8       head;
	uint8       tail;
	uint8       size;
	uint8       capacity;
	Data_Frame* Data_Frame_Array_List;	
} Data_Queue;

//Data Frame Queue Operation
Data_Queue* create_Data_Queue   	(uint8 capacity);
uint8  	    isFullData          	(Data_Queue* queue);
uint8       isEmptyData         	(Data_Queue* queue);
uint8       enqueueData         	(Data_Queue* queue, Data_Frame* data);
Data_Frame* dequeueData                 (Data_Queue* queue);

#endif
