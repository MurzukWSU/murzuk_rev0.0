#ifndef AX25_FRAME_H
#define AX25_FRAME_H

#include "DataTypes.h"

//Define AX25 Frame Constants
#define AX25_FRAME_START    0xFE
#define AX25_CONTROL        0x03
#define AX25_PROTO_IDENT    0xF0
#define AX25_FRAME_ID       0x00
#define AX25_1ST_HEADER_PTR 0xFE
#define AX25_FRAME_END 	    0xFE

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
	uint8 frame_Start;          //Start of AX25 Frame identifier 0xFE
	
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

//AX25 Queue Operation
AX25_Queue* create_AX25_Queue   	(uint8 capacity);
uint8       isFullAX25          	(AX25_Queue* queue);
uint8       isEmptyAX25         	(AX25_Queue* queue);
uint8       enqueueAX25         	(AX25_Queue* queue, AX25_Frame* frame);
AX25_Frame* dequeueAX25         	(AX25_Queue* queue);

#endif
