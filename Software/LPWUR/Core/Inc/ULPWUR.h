/*
 * ULPWUR.h
 *
 *  Created on: Nov 19, 2019
 *      Author: crenda
 */

#ifndef INC_ULPWUR_H_
#define INC_ULPWUR_H_


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "stm32l4xx_hal.h"
#include "main.h"
#include "spi.h"
#include "gpio.h"
#include "dma.h"
#include "usart.h"
#include "EpdDriver.h"
#include "GUI_Paint.h"

#define KALDARK 0xdd
#define KALBRIGHT 0xe0

typedef struct CalCellInfo
{
    uint8_t place; // 1-35 Calender fields
    uint8_t color; // 1-35 Calender fields
    uint16_t posX; //bpp
    uint16_t posY; //Rotate mode
    uint16_t cellLen; //Start address of source Frame buffer
    uint16_t cellhigh;//Base address of target image buffer
    uint32_t imBufsize;//Base address of target image buffer
}CalCellInfo;

void UpdateCalender(void);
void DrawCalenderField(uint8_t place, char* fach, char* dozent);
void receiveData(void);


#endif /* INC_ULPWUR_H_ */
