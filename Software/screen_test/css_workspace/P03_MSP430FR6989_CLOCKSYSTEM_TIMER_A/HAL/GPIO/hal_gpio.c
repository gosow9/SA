/**
 ****************************************************************
 @file    hal_gpio.c
 ****************************************************************
 @brief   This module offers a set of functions to handle the buttons
 @brief   and the LEDs of the EXP-MSP430FR6989 LaunchPad.
 ****************************************************************
 @author  Adrian Tuescher, IMES/HSR
 @version 1.2
 @date    2019-08-02
 ****************************************************************
 */
// --- Includes
#include <stdint.h>
#include <stdbool.h>
#include <msp430fr6989.h>
#include <msp430fr5xx_6xxgeneric.h>

#include "hal_gpio.h"
#include "../../GLOB_Types.h"

// --- Defines
#define S1      0x02
#define S2      0x04
#define LED1    0x01
#define LED2    0x80
#define BP_T1   0x08
#define BP_T2   0x08

// --- Typedefs

// --- Local Function Prototypes
void _port1_ISR (void);
void _port3_ISR (void);

// --- Variables
static volatile pFctHandler _hal_gpio_callbackFct[4] =
{
  (pFctHandler) 0x00000000, //GPIO_S1 callback function pointer
  (pFctHandler) 0x00000000, //GPIO_S2 callback function pointer
  (pFctHandler) 0x00000000, //GPIO_BP_T1 callback function pointer
  (pFctHandler) 0x00000000,
//GPIO_BP_T2 callback function pointer
  };

/**
 ****************************************************************
 @brief   Initialize the two buttons and the two LEDs.
 @param   -
 @return  -
 ****************************************************************
 */
void hal_gpio_init (void)
{
  //LED1
  P1OUT &= ~LED1;   //Set the corresponding Bit to zero
  P1DIR |= LED1;    //configure P1.0 as output
  //LED2
  P9OUT &= ~LED2;   //Set the corresponding Bit to zero
  P9DIR |= LED2;    //configure P9.7 as output

  //Button S1
  P1DIR &= ~S1;     //configure P1.1 as input
  P1OUT |= S1;      //use pull-up resistor
  P1REN |= S1;      //enable pull-up/pull-down resistor
  //configure the button S1, with interrupts on push down
  P1IES |= S1;      //generate an interrupt on high to low transition
  P1IE |= S1;       //enable interrupt

  //Button S2
  P1DIR &= ~S2;     //configure P1.2 as input
  P1OUT |= S2;      //use pull-up resistor
  P1REN |= S2;      //enable pull-up/pull-down resistor
  //configure the button S1, with interrupts on push down
  P1IES |= S2;      //generate an interrupt on high to low transition
  P1IE |= S2;       //enable interrupt

  //Button BoosterPack Serial Communication T1
  P3DIR &= ~BP_T1;  //configure P3.3 as input
  //configure the button BP_T1, with interrupts on push down
  P3IES |= BP_T1;   //generate an interrupt on high to low transition
  P3IE |= BP_T1;    //enable interrupt

  //Button BoosterPack Serial Communication T2
  P1DIR &= ~BP_T2;  //configure P1.3 as input
  //configure the button BP_T2, with interrupts on push down
  P1IES |= BP_T2;   //generate an interrupt on high to low transition
  P1IE |= BP_T2;    //enable interrupt
}

/**
 ****************************************************************
 @brief   Get the pressed value of a button
 @param   gpio One of the possible GPIOs from enumeration
 @return  Boolean value. One if pressed, zero if not pressed
 ****************************************************************
 */
bool hal_gpio_get (gpioPeripherie_t gpio)
{
  bool pressed = true;

  switch (gpio)
  {
    case GPIO_S1:
      if (P1IN & S1)
      {
        pressed = false;
      }
      break;
    case GPIO_S2:
      if (P1IN & S2)
      {
        pressed = false;
      }
      break;
    case GPIO_BP_T1:
      if (P3IN & BP_T1)
      {
        pressed = false;
      }
      break;
    case GPIO_BP_T2:
      if (P1IN & BP_T2)
      {
        pressed = false;
      }
      break;
    default:
      pressed = false;
      break;
  }
  return pressed;
}

/**
 ****************************************************************
 @brief   Set or clear one of the possible outputs
 @param   gpio One of the possible GPIOs from enumeration
 @param   on If true, the led light turns on, else off
 @return  -
 ****************************************************************
 */
void hal_gpio_set (gpioPeripherie_t gpio, bool on)
{
  switch (gpio)
  {
    case GPIO_LED1:
      if (on == true)
      {
        P1OUT |= LED1;    //Set the corresponding Bit to one
      }
      else
      {
        P1OUT &= ~LED1;   //Set the corresponding Bit to zero
      }
      break;
    case GPIO_LED2:
      if (on == true)
      {
        P9OUT |= LED2;    //Set the corresponding Bit to one
      }
      else
      {
        P9OUT &= ~LED2;   //Set the corresponding Bit to zero
      }
      break;
    default:
      break;
  }
}

/**
 ****************************************************************
 @brief   Toggle one of the possible outputs
 @param   gpio One of the possible GPIOs from enumeration
 @return  -
 ****************************************************************
 */
void hal_gpio_toggle (gpioPeripherie_t gpio)
{
  switch (gpio)
  {
    case GPIO_LED1:
      //toggle this led
      P1OUT ^= LED1;
      break;
    case GPIO_LED2:
      //toggle this led
      P9OUT ^= LED2;
      break;
    default:
      break;
  }
}

/**
 ****************************************************************
 @brief  Register a callback function for one of the possible input GPIO's.
 @param  gpio    One of the possible GPIOs from enumeration
 @param  pCallbackFct Callback function for this input.
 @return -
 ****************************************************************
 */
void hal_gpio_interruptCallbackFctRegister (gpioPeripherie_t gpio, pFctHandler pCallbackFct)
{
  switch (gpio)
  {
    case GPIO_S1:
      _hal_gpio_callbackFct[0] = pCallbackFct;
      break;
    case GPIO_S2:
      _hal_gpio_callbackFct[1] = pCallbackFct;
      break;
    case GPIO_BP_T1:
      _hal_gpio_callbackFct[2] = pCallbackFct;
      break;
    case GPIO_BP_T2:
      _hal_gpio_callbackFct[3] = pCallbackFct;
      break;
    default:
      break;
  }
}

/**
 ****************************************************************
 @brief  Enable interrupt for one of the possible GPIOs from enumeration.
 @param  gpio    One of the possible GPIOs from enumeration
 @return -
 ****************************************************************
 */
void hal_gpio_interruptEnable (gpioPeripherie_t gpio)
{
  switch (gpio)
  {
    case GPIO_S1:
      P1IE |= S1;
      break;
    case GPIO_S2:
      P1IE |= S2;
      break;
    case GPIO_BP_T1:
      P3IE |= BP_T1;
      break;
    case GPIO_BP_T2:
      P1IE |= BP_T2;
      break;
    default:
      break;
  }
}

/**
 ****************************************************************
 @brief  Disable interrupt for one of the possible GPIOs from enumeration.
 @param  gpio    One of the possible GPIOs from enumeration
 @return -
 ****************************************************************
 */
void hal_gpio_interruptDisable (gpioPeripherie_t gpio)
{
  switch (gpio)
  {
    case GPIO_S1:
      P1IE &= ~S1;
      break;
    case GPIO_S2:
      P1IE &= ~S2;
      break;
    case GPIO_BP_T1:
      P3IE &= ~BP_T1;
      break;
    case GPIO_BP_T2:
      P1IE &= ~BP_T2;
      break;
    default:
      break;
  }
}

/**
 ****************************************************************
 @brief  Port_1 Interrupt Service Routine
 @param   -
 @return  -
 ****************************************************************
 */
#pragma vector=PORT1_VECTOR
__interrupt void _port1_ISR (void)
{
  uint8_t status = 0x00;

  //get masked interrupt-status from all pins
  status = P1IFG;
  //clear interrupt-flag
  P1IFG &= ~status;

  if (status & S1)
  {
    if (_hal_gpio_callbackFct[0] != 0x00000000)  //do function callback, if a callback is registered
    {
      _hal_gpio_callbackFct[0]();
    }
  }
  if (status & S2)
  {
    if (_hal_gpio_callbackFct[1] != 0x00000000)  //do function callback, if a callback is registered
    {
      _hal_gpio_callbackFct[1]();
    }
  }
  if (status & BP_T2)
  {
    if (_hal_gpio_callbackFct[3] != 0x00000000)  //do function callback, if a callback is registered
    {
      _hal_gpio_callbackFct[3]();
    }
  }
}

/**
 ****************************************************************
 @brief  Port_3 Interrupt Service Routine
 @param   -
 @return  -
 ****************************************************************
 */
#pragma vector=PORT3_VECTOR
__interrupt void _port3_ISR (void)
{
  uint8_t status = 0x00;

  //get masked interrupt-status from all pins
  status = P3IFG;
  //clear interrupt-flag
  P3IFG &= ~status;

  if (status & BP_T1)
  {
    if (_hal_gpio_callbackFct[2] != 0x00000000)  //do function callback, if a callback is registered
    {
      _hal_gpio_callbackFct[2]();
    }
  }
}
