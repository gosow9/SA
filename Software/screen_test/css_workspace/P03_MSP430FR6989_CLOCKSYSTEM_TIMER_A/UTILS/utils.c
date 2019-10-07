/**
 ****************************************************************
 @file    utils.c
 ****************************************************************
 @brief   This module offers some general utility functions.
 ****************************************************************
 @author  Adrian Tuescher, IMES/HSR
 @version 01.01
 @date    2019-05-08
 ***************************************************************
 */
// --- Includes
#include <intrinsics.h>
#include "utils.h"
#include "../ESP_MSPEXP430FR6989.h"

// --- Defines

// --- Typedefs

// --- Local Function Prototypes

// --- Variables

/**
 ****************************************************************
 @brief   Busy-Wait function for a delay.
 @param   ms  Busy-Delay time in milliseconds
 @return  -
 ****************************************************************
 */
void utils_busyDelayMs(unsigned long ms)
{
  volatile unsigned long delay = 0;
  //volatile unsigned long sysClkMHz = esp_sysCtlClockGet()/1000000;  //sysClock is 8MHz
  //delay = sysClkMHz*ms;

  delay = ms << 3;  //sysClock is 8MHz
  while(delay)
  {
    __delay_cycles(976);	//976 counts for 1ms (including the overhead)
    delay--;
  }
}
