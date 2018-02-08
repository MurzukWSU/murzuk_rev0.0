#include "cc1110.h"
#include "hal_files/hal_bui_lcd.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

//Define RFST constant values
#define SFSTXON             0x00
#define SCAL                0x01
#define SRX                 0x02
#define STX                 0x03
#define SIDLE               0x04

//Define AX25 Frame Constants
#define AX25_FRAME_START    0xFE
#define AX25_CONTROL        0x03
#define AX25_PROTO_IDENT    0xF0
#define AX25_FRAME_ID       0x00
#define AX25_1ST_HEADER_PTR 0xFE
#define AX25_FRAME_END 	    0xFE

typedef unsigned char uint8;

/********************************************************************************
*---STRUCT AX25_FRAME---
*
* Description:
*	Comprises one data frame according to the AX25 specification. All data
*	fields are uint8s. Total length = 251 Bytes.
*
*********************************************************************************/	
typedef struct AX25_Frame
{
	//---START---
	uint8 frame_Start;  //Start of AX25 Frame identifier 0xFE
	
	//Header information
	uint8 dest_Addr[7];         //Callsign of destination station in ASCII
	uint8 src_Addr[7];          //Callsign of source station in ASCII
	uint8 control;              //Packet configuration bits fixed to 0x03
	uint8 proto_Ident;          //Packet configuration bits fixed to 0xF0

	//Information Field
	uint8 frame_ID;     	    //Packet configuration bits fixed to 0x00
	uint8 master_Frame_Count;   //Number of current packet being sent
	uint8 vc_Frame_Count;       //Number of current packet in current virtual channel being sent
	uint8 first_Header_Pointer; //Not used, hard fixed to 0xFE
	uint8 data[251];            //Contains message bits provided by mainboard
	uint8 frame_Status;         //Configures time stamp options
	uint8 time_Stamp[8];        //Time stamp 	

	uint8 frame_End;            //End of AX25 Frame identifier 0xFE
	//---END---
} AX25_Frame;

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
	uint8 master_Frame_Count;
	uint8 vc_Frame_Count;
	uint8 time_Stamp[8];	
} Data_Frame;

/********************************************************************************
*---STRUCT AX25_QUEUE---
*
* Description:
*	Queue for buffering AX25_Frame TX and RX transfers between ground and 
*	the satellite.
*
*********************************************************************************/	
typedef struct AX25_Queue
{
	uint8       head;
	uint8       tail;
	uint8       size;
	uint8       capacity;
	AX25_Frame* AX25_Frame_Array_List;	
} AX25_Queue;

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


//---FUNCTION DECLARTIONS---

//Set-Up
void        initConfigRegisters (void);
void        initClock           (void);
void        initRFStateMach     (void);

//Frame Manipulation
Data_Frame*      decomm_AX25_Packet  (struct AX25_Frame *frame);

//AX25 Queue Operation
AX25_Queue* create_AX25_Queue   (uint8 capacity);
uint8       isFullAX25          (AX25_Queue* queue);
uint8       isEmptyAX25         (AX25_Queue* queue);
uint8       enqueueAX25         (AX25_Queue* queue, AX25_Frame* frame);
AX25_Frame* dequeueAX25         (AX25_Queue* queue);

//Data Frame Queue Operation
Data_Queue* create_Data_Queue   (uint8 capacity);
uint8  	    isFullData          (Data_Queue* queue);
uint8       isEmptyData         (Data_Queue* queue);
uint8       enqueueData         (Data_Queue* queue, Data_Frame* data);
Data_Frame* dequeueData         (Data_Queue* queue);

//Transmit Operation
uint8       transmit_AX25_Frame (AX25_Frame* frame);

//Receive Operation
AX25_Frame* receive_AX25_Frame  (void);

void main(void)
{
	//Create AX25 Frame and Data Frame RX/TX queues
	AX25_Queue* AX25_TX_Queue = create_AX25_Queue(10);			
	AX25_Queue* AX25_RX_Queue = create_AX25_Queue(10);	

	Data_Queue* Data_TX_Queue = create_Data_Queue(10);
	Data_Queue* Data_RX_Queue = create_Data_Queue(10);

	//Call Initialization functions
	initConfigRegisters();
	initClock();
	initRFStateMach();

	while(1)
	{	
				
	}
	
}

/********************************************************************************
*---FUNCTION---
* Name: initConfigRegisters()
* Description:
*	Configures the the SoC control registers as imported from SmartRFStudio.
* Parameters:
*	NONE
* Returns:
*	NONE
*********************************************************************************/	
void initConfigRegisters(void)
{
	//Set up control and radio registers for operation
	PKTCTRL0  = 0x04; //Packet control register
	PKTCTRL1  = 0x00; //Packet control register
	FSCTRL1   = 0x06; //Frequency synthesizer control register
	FREQ2     = 0x10; //Frequency control word, high byte
	FREQ1     = 0xCE; //Frequency control word, middle byte
	FREQ0     = 0xC4; //Frequency control word, low byte
	MDMCFG4   = 0xF5; //Modem configuration
	MDMCFG3   = 0x83; //Modem configuration
	MDMCFG2   = 0x03; //Modem configuration
	DEVIATN   = 0x15; //Modem deviation setting
	MCSM0	  = 0x18; //Main radio control state machine configuration
	FOCCFG    = 0x17; //Frequency offset compensation configuration
	FSCAL3    = 0xE9; //Frequency synthesizer calibration
	FSCAL2    = 0x2A; //Frequency synthesizer calibration
	FSCAL1    = 0x00; //Frequency synthesizer calibration
	FSCAL0    = 0x1F; //Frequency synthesizer calibration
	TEST1     = 0x31; //Various test settings
	TEST0     = 0x09; //Various test settings
	PA_TABLE0 = 0x60; //PA power setting 0
	IOCFG0    = 0x06; //Radio test signal configuration (P1_5)
	PKTLEN    = 2;	  //Packet length

}

/********************************************************************************
*---FUNCTION---
* Name: initClock()
* Description:
*	Configures the clock to run off the 26 MHz HS-XOSC.
* Parameters:
*	NONE
* Returns:
*	NONE
*********************************************************************************/	
void initClock(void)
{
	//Configure clock to run off of HS-XOSC (26 MHz) oscillator
	CLKCON &= ~0x40;
	//Wait for clock to be stable
	while(CLKCON & 0x40);
}

/********************************************************************************
*---FUNCTION---
* Name: initRFStateMach()
* Description:
*	Issues SIDLE and SCAL command strobes to set-up RFST for operation.
* Parameters:
*	NONE
* Returns:
*	NONE
*********************************************************************************/	
void initRFStateMach(void)
{
	//Issue SIDLE command strobe to put radio in idle-mode
	RFST = SIDLE;

	//Issue freq. synt. calibration command
	RFST = SCAL; //Note MCSM0 register is set to auto-calibrate freq. synt.
		     //on every transition from idle-mode to rx-mode/tx-mode

	//Clear RFTXRXIF (RX/TX interrupt flag)
	RFTXRXIF = 0;
}

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

/********************************************************************************
*---FUNCTION---
* Name: decomm_AX25_Packet()
* Description:
*	Extracts the data field from an AX25 packet and other pertinent fields.
* Parameters:
*	AX25_Frame* - pointer to the frame to be decommutated
* Returns:
*	Data_Frame* - data field (and other fields) from the AX25 frame
*********************************************************************************/	
Data_Frame* decomm_AX25_Packet(AX25_Frame *frame)
{
	//Extract data bits from AX25 Packet
	Data_Frame* dataFrame = (Data_Frame *) malloc (sizeof(Data_Frame));
	
	//Copy data bits from AX25_Frame to Data_Frame	
	memcpy(dataFrame->data, frame->data, sizeof(frame->data));
	
	//Copy master frame count byte from AX25_Frame to Data_Frame
	memcpy(&(dataFrame->master_Frame_Count), &(frame->master_Frame_Count), sizeof(uint8));

	//Copy vc frame count byte from AX25_Frame to Data_Frame
	memcpy(&(dataFrame->vc_Frame_Count), &(frame->vc_Frame_Count), sizeof(uint8));

	//Copy time stamp information from AX25_Frame to Data_Frame
	memcpy(dataFrame->time_Stamp, frame->time_Stamp, sizeof(uint8) * 8);	
	
	//Decomm'd AX25_Frame is no longer needed so free the memory
	free(frame);

	return dataFrame;	
}

/********************************************************************************
*---FUNCTION---
* Name: transmit_AX25_Frame()
* Description:
*	Transmits an AX25 from satellite to ground byte-by-byte through the RFD
*	register.
* Parameters:
*	AX25_Frame* - pointer to the frame to be transmitted
* Returns:
*	uint8 - returns 0 if transmission was completed successfully,
*	returns 1 if an error occured
*********************************************************************************/	
uint8 transmit_AX25_Frame(AX25_Frame* frame)
{
	uint8 da_Ind   = 0;
	uint8 sa_Ind   = 0;
	uint8 data_Ind = 0;
	uint8 ts_Ind   = 0;
	
	//Issue STX command strobe to put radio in tx-mode
	RFST = STX;
	
	//Transmit frame_Start byte
	while(!RFTXRXIF);
	RFTXRXIF = 0;
	RFD = frame->frame_Start;

	//Transmit dest_Addr bytes (7 bytes)
	for(da_Ind = 0; da_Ind < 7; da_Ind++)
	{
		while(!RFTXRXIF);
		RFTXRXIF = 0;
		RFD = frame->dest_Addr[da_Ind];
	}
				
	//Transmit src_Addr bytes (7 bytes)
	for(sa_Ind = 0; sa_Ind < 7; sa_Ind++)
	{
		while(!RFTXRXIF);
		RFTXRXIF = 0;
		RFD = frame->src_Addr[sa_Ind];
	}

	//Transmit control byte
	while(!RFTXRXIF);
	RFTXRXIF = 0;
	RFD = frame->control;
	
	//Transmit proto_Ident byte
	while(!RFTXRXIF);
	RFTXRXIF = 0;
	RFD = frame->proto_Ident;

	//Transmit frame_ID byte
	while(!RFTXRXIF);
	RFTXRXIF = 0;
	RFD = frame->frame_ID;

	//Transmit master_Frame_Count byte
	while(!RFTXRXIF);
	RFTXRXIF = 0;
	RFD = frame->master_Frame_Count;

	//Transmit vc_Frame_Count byte
	while(!RFTXRXIF);
	RFTXRXIF = 0;
	RFD = frame->vc_Frame_Count;

	//Transmit first_Header_Pointer byte
	while(!RFTXRXIF);
	RFTXRXIF = 0;
	RFD = frame->first_Header_Pointer;

	//Transmit data bytes (251 bytes)
	for(data_Ind = 0; data_Ind < 251; data_Ind++)
	{
		while(!RFTXRXIF);
		RFTXRXIF = 0;
		RFD = frame->data[data_Ind];
	}
	
	//Transmit frame_Status byte 
	while(!RFTXRXIF);
	RFTXRXIF = 0;
	RFD = frame->frame_Status;


	//Transmit time_Stamp bytes (8 bytes) 
	for(ts_Ind = 0; ts_Ind < 8; ts_Ind++)
	{
		while(!RFTXRXIF);
		RFTXRXIF = 0;
		RFD = frame->time_Stamp[ts_Ind];
	}

	//Transmit frame_End byte 
	while(!RFTXRXIF);
	RFTXRXIF = 0;
	RFD = frame->frame_End;

	//Wait for IRQ_DONE interrupt flag
	while((RFIF & 0x10) == 0);
	RFIF &= ~0x10;
	RFTXRXIF = 0;
	
	return 0;
}

/********************************************************************************
*---FUNCTION---
* Name: receive_AX25_Frame()
* Description:
*	Receives an AX25 from ground to satellite byte-by-byte through the RFD
*	register.
* Parameters:
*	NONE
* Returns:
*	AX25_Frame* - Pointer to the received AX25_Frame
*********************************************************************************/	
AX25_Frame* receive_AX25_Frame(void)
{
	AX25_Frame* frame = (AX25_Frame *) malloc(sizeof(AX25_Frame));
	uint8 da_Ind   = 0;
	uint8 sa_Ind   = 0;
	uint8 data_Ind = 0;
	uint8 ts_Ind   = 0;
	
	
	//Receive frame_Start byte
	while(!RFTXRXIF);
	RFTXRXIF = 0;
	frame->frame_Start = RFD;

	//Receive dest_Addr bytes (7 bytes)
	for(da_Ind = 0; da_Ind < 7; da_Ind++)
	{
		while(!RFTXRXIF);
		RFTXRXIF = 0;
		frame->dest_Addr[da_Ind] = RFD;
	}
				
	//Receive src_Addr bytes (7 bytes)
	for(sa_Ind = 0; sa_Ind < 7; sa_Ind++)
	{
		while(!RFTXRXIF);
		RFTXRXIF = 0;
		frame->src_Addr[sa_Ind] = RFD;
	}

	//Receive control byte
	while(!RFTXRXIF);
	RFTXRXIF = 0;
	frame->control = RFD;
	
	//Receive proto_Ident byte
	while(!RFTXRXIF);
	RFTXRXIF = 0;
	frame->proto_Ident = RFD;

	//Receive frame_ID byte
	while(!RFTXRXIF);
	RFTXRXIF = 0;
	frame->frame_ID = RFD;

	//Receive master_Frame_Count byte
	while(!RFTXRXIF);
	RFTXRXIF = 0;
	frame->master_Frame_Count = RFD;

	//Receive vc_Frame_Count byte
	while(!RFTXRXIF);
	RFTXRXIF = 0;
	frame->vc_Frame_Count = RFD;

	//Receive first_Header_Pointer byte
	while(!RFTXRXIF);
	RFTXRXIF = 0;
	frame->first_Header_Pointer = RFD;

	//Receive data bytes (251 bytes)
	for(data_Ind = 0; data_Ind < 251; data_Ind++)
	{
		while(!RFTXRXIF);
		RFTXRXIF = 0;
		frame->data[data_Ind] = RFD;
	}
	
	//Receive frame_Status byte 
	while(!RFTXRXIF);
	RFTXRXIF = 0;
	frame->frame_Status = RFD;


	//Receive time_Stamp bytes (8 bytes) 
	for(ts_Ind = 0; ts_Ind < 8; ts_Ind++)
	{
		while(!RFTXRXIF);
		RFTXRXIF = 0;
		frame->time_Stamp[ts_Ind] = RFD;
	}

	//Receive frame_End byte 
	while(!RFTXRXIF);
	RFTXRXIF = 0;
	frame->frame_End = RFD;

	//Wait for IRQ_DONE interrupt flag
	while((RFIF & 0x10) == 0);
	RFIF &= ~0x10;
	RFTXRXIF = 0;

	return frame;
}

