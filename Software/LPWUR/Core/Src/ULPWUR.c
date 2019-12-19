/*
 * ULPWUR.c
 *
 *  Created on: Nov 19, 2019
 *      Author: crenda
 */

#include "ULPWUR.h"



// ****************************************************************************************
// Function name: DrawCalenderField( )
//
// Description:
//   Draws a string to the Calender Field 1-35
//
// Arguments:
//      uint8_t place:	Select the field 1-35
//      char* fach:		String which subject has to be written
//      char* dozent:	String which teacher
// Return Values:
//   NULL.
// Note:
//
// ****************************************************************************************
void DrawCalenderField(uint8_t place, char* fach, char* dozent)
{
	CalCellInfo cal;
	cal.imBufsize= 17336*2;
	uint8_t frameBuf[cal.imBufsize];
	cal.place = place;
	if(cal.place%5!=0)
		cal.posX = 202 + (cal.place%5-1) * 200;
	else
		cal.posX = 1002;

	if(cal.place%5!=0)
	cal.posY = cal.place/5 *90 +195;
	else
		cal.posY = (cal.place/5 - 1) *90 +195;

	if((cal.place%5 != 0))
	{
		if(cal.place/5%2 != 0)
			cal.color = KALBRIGHT;
		else
			cal.color = KALDARK;
	}
	else
	{
		if(cal.place/5%2 != 0)
					cal.color = KALDARK;
				else
					cal.color = KALBRIGHT;
	}

	setNewImage(frameBuf, 196, 88, 0, 0xff);
	clear_Image(cal.color);
	//clear_Image(cal.color);
	drawString(5, 20, fach, &Font24, cal.color, 0x44 );
	EpdDriverDrawBox(frameBuf, cal.posX, cal.posY, 196, 88);


	setNewImage(frameBuf, 196, 88, 0, 0xff);
	clear_Image(cal.color);
	drawString(5, 15, dozent, &Font24, cal.color, 0x44 );
	//EpdDriverDrawBox(frameBuf, 202, 195, 196, 88);
	EpdDriverDrawBox(frameBuf, cal.posX, cal.posY+44, 196, 88);
}


void receiveData()
{
	uint8_t rxBuf[50]={0};
	char  fach[14];
	char  doz[4];
	HAL_UART_Receive(&huart4, rxBuf, 20,HAL_MAX_DELAY);
		 	 if(rxBuf[0]==0)
		 	 {
		 		 uint8_t arrcal[rxBuf[1]][20];
		 		 for(int i=0; i< rxBuf[1]; i++)
		 		 {
		 			HAL_UART_Receive(&huart4, &arrcal[i][0], 20,HAL_MAX_DELAY);
		 		 }
		 		 for(int p=0; p< rxBuf[1]; p++)
		 		 {
		 			 for(int i=15; i<18;i++)
		 				{doz[i-15]= arrcal[p][i];}
		 			 for(int i=1; i<11;i++)
		 				{fach[i-1]=arrcal[p][i];}
		 			 doz[3]='\0';
		 			 DrawCalenderField(arrcal[p][0], fach, doz);
		 		 }
		 		 //EpdDriverLoadTemplate();
//		 		 for(int i=0; i<50;i++)
//		 		 {rxBuf[i]=0;}
		 		 //HAL_UART_Transmit(&huart4, (uint8_t*)tempLoad, 16, HAL_MAX_DELAY);
		 		// HAL_UART_Transmit(&huart4, end, 1, HAL_MAX_DELAY);
		 	 }
		 	 if((0<rxBuf[0]) & (rxBuf[0] <=35))
		 	 {
		 		 for(int i=15; i<18;i++)
		 			 {doz[i-15]=rxBuf[i];}
		 		 for(int i=1; i<11;i++)
		 		 	 {fach[i-1]=rxBuf[i];}
		 		 doz[3]='\0';
		 		 DrawCalenderField(rxBuf[0], fach, doz);
		 		 //EpdDriverShowDisp();
		 		 for(int i=0; i<50;i++)
		 		 	 {rxBuf[i]=0;}

		 		 //HAL_UART_Transmit(&huart4, (uint8_t*)tempLoad, 16, HAL_MAX_DELAY);
		 		 //HAL_UART_Transmit(&huart4, trans, 6, HAL_MAX_DELAY);
		 		 //HAL_UART_Receive(&huart4, rxBuf, 2,HAL_MAX_DELAY);

		 	 }
		 	 if(37 == rxBuf[0])
		 	 	 {
		 		 EpdDriverShowDisp();
		 	 	 }
}
