/*
 * msp430_hal.c
 *
 *  Created on: 08.10.2019
 *      Author: mtischha
 */

#include "msp430_hal.h"

void HAL_GPIO_WritePin(uint8_t _port, uint16_t _pin, int _value)
{
    if(_value == 1)
    {
        GPIO_setOutputHighOnPin(_port, _pin);
    }
    else
    {
        GPIO_setOutputLowOnPin(_port, _pin);
    }
}

uint8_t HAL_GPIO_ReadPin(uint8_t _port, uint16_t _pin)
{
    return GPIO_getInputPinValue(_port, _pin);
}

void HAL_Delay(uint32_t __xms)
{
    //clock = 4MHZ (aus main.c!)
    int i = 4000*__xms; //delay in ms

    for(i; i>0; i--);
}