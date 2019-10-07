/**
 ****************************************************************
 @file    hal_clock.c
 ****************************************************************
 @brief   This module offers a function to setup all the clocks.
 @brief   MCLK:      8MHz from DCOCLK
 @brief   SMCLK:     4MHz from DCOCLK
 @brief   ACLK: 16.384kHz from LFXTCLK
 ****************************************************************
 @author  Adrian Tuescher, IMES/HSR
 @version 1.0
 @date    2019-07-26
 ****************************************************************
 */
// --- Includes
#include <stdint.h>
#include <stdbool.h>
#include <msp430fr6989.h>
#include <msp430fr5xx_6xxgeneric.h>

#include "hal_clock.h"

// --- Defines

// --- Typedefs

// --- Local Function Prototypes

// --- Variables

/**
 ****************************************************************
 @brief	  Initialize all the clocks.
 @brief   MCLK:      8MHz from DCOCLK
 @brief   SMCLK:     8MHz from DCOCLK
 @brief   ACLK: 32.768kHz from LFXTCLK
 @param   -
 @return  -
 ****************************************************************
 */
void hal_clk_init(void)
{
  volatile uint16_t regValue;

  CSCTL0 = CSKEY;       // Unlock CS registers

  ///@todo Aufgabe 2.1.1 Setup DCO
  //DCO Clock Setup


  ///@todo Aufgabe 2.1.1 Setup MCLK with 1:1 DCO clock
  //MCLK Clock setup


  ///@todo Aufgabe 2.1.2 Setup SMCLK with 1/2 of DCO clock (optional in code)
  //SMCLK Clock setup


  //LFX Clock Setup
  //Setup LFX I/Os to use by external oscillator
  PJSEL0 |= BIT4 | BIT5;                    // For LFX (32.768kHz)
  PJSEL1 &= ~(BIT4 | BIT5);
  //Turn LFXT on
  CSCTL4 &= ~LFXTOFF;
  //wait until clock is running without fault
  do
  {
    CSCTL5 &= ~LFXTOFFG;        //clear LFX oscillator fault flag
    SFRIFG1 &= ~OFIFG;          //clear also oscillator fault flag in special function register
    __delay_cycles(25000);      // time for flag to get set again
    regValue = CSCTL5 & LFXTOFFG; //read back oszillator fault flag
  } while(regValue);           // Test oscillator fault flag

  ///@todo Aufgabe 2.2.1 Setup ACLK with 1/2 of LFXTCLK clock
  //ACLK Clock setup


  // Output ACLK on P4.1
  P4DIR |= BIT1;
  P4SEL0 |= BIT1;
  P4SEL1 |= BIT1;

  // Output MCLK on P4.0
  P4DIR |= BIT0;
  P4SEL0 |= BIT0;
  P4SEL1 |= BIT0;
}
