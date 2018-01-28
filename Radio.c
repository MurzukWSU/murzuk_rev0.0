#include "cc1110.h"
#include "hal_bui_lcd.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

//Define RFST constant values
#define SFSTXON 0x00
#define SCAL    0x01
#define SRX     0x02
#define STX     0x03
#define SIDLE   0x04

struct AX25_Frame
{
	//---START---
	unsigned char frame_Start;  //Start of AX25 Frame identifier 0xFE
	
	//Header information
	unsigned char dest_Addr[7];         //Callsign of destination station in ASCII
	unsigned char src_Addr[7];          //Callsign of source station in ASCII
	unsigned char control;              //Packet configuration bits fixed to 0x03
	unsigned char proto_Ident;          //Packet configuration bits fixed to 0xF0

	//Information Field
	unsigned char frame_ID;     	    //Packet configuration bits fixed to 0x00
	unsigned char master_Frame_Count;   //Number of current packet being sent
	unsigned char vc_Frame_Count;       //Number of current packet in current virtual channel being sent
	unsigned char first_Header_Pointer; //Not used, hard fixed to 0xFE
	unsigned char data[251];            //Contains message bits provided by mainboard
	unsigned char frame_Status;         //Configures time stamp options
	unsigned char time_Stamp[8];        //Time stamp 	

	unsigned char frame_End;            //End of AX25 Frame identifier 0xFE
	//---END---
};

//---FUNCTION DECLARTIONS---
int decomm_AX25_Packet(struct AX25_Frame *frame);

void main(void)
{
	unsigned char rxBuffer[2] = {0x00, 0x00};
		 char rxBufferByte1[9] = {'.', '.', '.', '.', '.', '.', '.', '.', '\0'};
		 char rxBufferByte2[9] = {'.', '.', '.', '.', '.', '.', '.', '.', '\0'};
	unsigned char i = 0;
	unsigned char j = 0;
	unsigned char k = 0;
	unsigned char powOf2[8] = {1, 2, 4, 8, 16, 32, 64, 128};

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
	
	CLKCON &= ~0x40;
	while(CLKCON & 0x40);

	//Issue SIDLE command strobe to put radio in idle-mode
	RFST = SIDLE;

	//Issue freq. synt. calibration command
	RFST = SCAL; //Note MCSM0 register is set to auto-calibrate freq. synt.
		     //on every transition from idle-mode to rx-mode/tx-mode

	//Clear RFTXRXIF (RX/TX interrupt flag)
	RFTXRXIF = 0;

	halBuiInitLcd();

	while(1)
	{	
		RFST = SRX;

		for(i = 0; i < 2; i++)
		{
			rxBuffer[i] = 0x00;
			while(!RFTXRXIF);
			RFTXRXIF = 0;
			rxBuffer[i] = RFD;

			for(j = 0; j < 8; j++)
			{
				if((rxBuffer[0] & powOf2[j]) == 0)
				{
					rxBufferByte1[7 - j] = '0';
				}
				else
				{
					rxBufferByte1[7 - j] = '1';
				}
			}
			
			for(k = 0; k < 8; k++)
			{
				if((rxBuffer[1] & powOf2[k]) == 0)
				{
					rxBufferByte2[7 - k] = '0';
				}
				else
				{
					rxBufferByte2[7 - k] = '1';
				}
			}

			halBuiLcdUpdate((char*) rxBufferByte1, (char*) rxBufferByte2); 

		}

		//Wait for IRQ_DONE interrupt flag
		while((RFIF & 0x10) == 0);
		RFIF &= ~0x10;
		P1_0 = 1;
		RFTXRXIF = 0;

				
	}		
}

int decomm_AX25_Packet(struct AX25_Frame *frame)
{
	//Extract data bits from AX25 Packet
	unsigned char data_bits[251];
	memcpy(data_bits, frame->data, sizeof(frame->data));	
	return 0;	
}
