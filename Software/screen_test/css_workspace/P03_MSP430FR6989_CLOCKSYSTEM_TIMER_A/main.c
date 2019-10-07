/**
 ****************************************************************
 @file    main.c
 ****************************************************************
 @brief   Main for the Embedded-Systems Praktikum P03.
 ****************************************************************
 @author  Adrian Tuescher, IMES/HSR
 @version 01.20
 @date    2019-07-26
 ****************************************************************
 */
// --- Includes
#include <stdio.h>
#include <stdint.h>
#include <intrinsics.h>
#include <msp430fr6989.h>

#include "ESP_MSPEXP430FR6989.h"
#include "LP_LCD_MSPEXP430FR6989.h"

#include "HAL/CLK/hal_clock.h"
#include "HAL/TIM/hal_timer.h"

// --- Defines
#define MAX_SECONDS 60
#define MAX_MINUTES 60
#define MAX_HOURS   24

// --- Typedefs
typedef struct
{
  uint8_t sec;
  uint8_t min;
  uint8_t hour;
} time_t;

// --- Local Function Prototypes
static void _timerOverflowCallbackFct(void);
static void _timerCC0CallbackFct(void);
static void _timeGet(time_t* pTime);
static void _timeSet(time_t* pTime);

// --- Variables
static time_t _time;

/**
 ****************************************************************
 @brief   Main program
 @param   -
 @return  -
 ****************************************************************
 */
int main(void)
{
  uint8_t oldSec = 0;
  time_t timeLocal;

  WDTCTL = WDTPW | WDTHOLD;     //stop watchdog
  PM5CTL0 &= ~LOCKLPM5;         //disable high-impedance ports

  hal_clk_init();
  hal_tim_init();
  hal_tim_interruptCallbackFctRegister(TIMER_INTERRUPT_OVERFLOW, _timerOverflowCallbackFct);
  //register timerCC0 callbackFunction
  hal_tim_interruptCallbackFctRegister(TIMER_INTERRUPT_CC0, _timerCC0CallbackFct);

  lp_lcdInit();
  lp_lcdSpecSymb(LCD_TMR, true);

  // Configure pin on P4.2 as output
  P4OUT &= ~0x04;   //Set the corresponding Bit to zero
  P4DIR |= 0x04;    //configure P4.2 as output

  //setup time
  timeLocal.sec = 0;
  timeLocal.min = 23;
  timeLocal.hour = 9;
  _timeSet(&timeLocal);

  lp_lcdTime(timeLocal.hour, timeLocal.min, timeLocal.sec);

  __enable_interrupt();
  hal_tim_start();

  while(1)
  {
    //get current time and display it
    _timeGet(&timeLocal);

    //check if time has changed
    if(timeLocal.sec != oldSec)
    {
      oldSec = timeLocal.sec;
      lp_lcdTime(timeLocal.hour, timeLocal.min, timeLocal.sec);
    }
  }
}

/**
 ****************************************************************
 @brief  Registered CallbackFunction, called from the HAL_TIMER if
 overflow interrupt occurred.
 @param  -
 @return -
 ****************************************************************
 */
static void _timerOverflowCallbackFct(void)
{
  //toggle pin on P4.2
  P4OUT ^= 0x04;
}

/**
 ****************************************************************
 @brief  Registered CallbackFunction, called from the HAL_TIMER if
 CC0 interrupt occurred.
 @param  -
 @return -
 ****************************************************************
 */
static void _timerCC0CallbackFct(void)
{
  //toggle pin on P4.2
  P4OUT ^= 0x04;

  _time.sec++;

  if(_time.sec >= MAX_SECONDS)
  {
    _time.sec = 0;
    _time.min++;

    if(_time.min >= MAX_MINUTES)
    {
      _time.min = 0;

      _time.hour++;

      if(_time.hour >= MAX_HOURS)
      {
        _time.hour = 0;
      }
    }
  }
}

/**
 ****************************************************************
 @brief   Get the actual time of the timer module.
 @param   pTime Pointer to timer data structure.
 @return  -
 ****************************************************************
 */
static void _timeGet(time_t* pTime)
{
  //temporary block timer CC0 interrupts durring data copy
  hal_tim_interruptDisable(TIMER_INTERRUPT_CC0);

  pTime->sec = _time.sec;
  pTime->min = _time.min;
  pTime->hour = _time.hour;

  hal_tim_interruptEnable(TIMER_INTERRUPT_CC0);   //enable CC0 interrupt
}

/**
 ****************************************************************
 @brief   Set the actual time of the timer module with the inputTime.
 @param   pTime Pointer to timer input data structure.
 @return  -
 ****************************************************************
 */
static void _timeSet(time_t* pTime)
{
  //check input data
  if(pTime->sec >= MAX_SECONDS)
  {
    pTime->sec = 0;
  }
  if(pTime->min >= MAX_MINUTES)
  {
    pTime->min = 0;
  }
  if(pTime->hour >= MAX_HOURS)
  {
    pTime->hour = 0;
  }

  //temporary block timer CC0 interrupts durring data copy
  hal_tim_interruptDisable(TIMER_INTERRUPT_CC0);

  _time.sec = pTime->sec;
  _time.min = pTime->min;
  _time.hour = pTime->hour;

  hal_tim_interruptEnable(TIMER_INTERRUPT_CC0);   //enable CC0 interrupt
}
