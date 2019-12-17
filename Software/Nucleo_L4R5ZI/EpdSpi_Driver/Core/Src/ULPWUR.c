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

	Paint_NewImage(frameBuf, 196, 88, 0, 0xff);
	Paint_Clear(cal.color);
	Paint_DrawString_EN(5, 20, fach, &Font24, cal.color, 0x44 );
	Paint_DrawString_EN(5, 50, dozent, &Font24, cal.color, 0x66 );
	//EpdDriverDrawBox(frameBuf, 202, 195, 196, 88);
	EpdDriverDrawBox(frameBuf, cal.posX, cal.posY, 196, 88);
}
