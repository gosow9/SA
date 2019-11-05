/**
 ****************************************************************
 @file    hal_timer.c
 ****************************************************************
 @brief   This module offers a set of functions to handle the TimerB0.
 ****************************************************************
 @author  Adrian Tuescher, IMES/HSR
 @version 1.1
 @date    2019-07-26
 ****************************************************************
 */
// --- Includes
#include <stdint.h>
#include <stdbool.h>
#include <msp430fr6989.h>
#include <msp430fr5xx_6xxgeneric.h>

#include "hal_timer.h"
#include "../../GLOB_Types.h"

// --- Defines

// --- Typedefs

// --- Local Function Prototypes
void _timer0BCC0_ISR(void);
void _timer0B_TB_CC1_6_ISR(void);

// --- Variables
static volatile pFctHandler _hal_timerInterrupt_callbackFct[TIMER_LAST_INTERRUPT] =
{
  (pFctHandler)0x00000000,
  (pFctHandler)0x00000000
};

/**
 ****************************************************************
 @brief   Initialize the one second timer (TimerB0).
 @param   -
 @return  -
 ****************************************************************
 */
void hal_tim_init(void)
{
  ///@todo Register initialisieren (Aufgabe 2.3.1)
  //Setup timer


  ///@todo Register initialisieren (Aufgabe 2.3.1)


  ///@todo Register initialisieren (Aufgabe 2.3.1)


  ///@todo Setup one second interrupt with CaptureCompareUnit0 (Aufgabe 2.4)


  ///@todo Setup one second interrupt with CaptureCompareUnit0 (Aufgabe 2.4)

}

/**
 ****************************************************************
 @brief   Start the timer in upMode.
 @param   -
 @return  -
 ****************************************************************
 */
void hal_tim_start(void)
{
  ///@todo Register initialisieren (Aufgabe 2.3.2)


  ///@todo Register initialisieren (Aufgabe 2.3.2)


  ///@todo Register initialisieren (Aufgabe 2.4)

}

/**
 ****************************************************************
 @brief   Stops the timer.
 @param   -
 @return  -
 ****************************************************************
 */
void hal_tim_stop(void)
{
  ///@todo Register initialisieren (Aufgabe 2.3.3)

}

/**
****************************************************************
@brief  Register a callback function for one of the possible timer
        interrupts.
@param  timInterrupt  one of the possible timerInterrupts from enumeration
@param  pCallbackFct  callback function
@return -
****************************************************************
*/
void hal_tim_interruptCallbackFctRegister(timerInterrupt_t timInterrupt, pFctHandler pCallbackFct)
{
  if(timInterrupt < TIMER_LAST_INTERRUPT)
  {
    _hal_timerInterrupt_callbackFct[timInterrupt] = pCallbackFct;
  }
}

/**
****************************************************************
@brief  Enable one of the possible timerInterrupts from enumeration.
@param  timInterrupt  One of the possible timerInterrupts from enumeration
@return -
****************************************************************
*/
void hal_tim_interruptEnable(timerInterrupt_t timInterrupt)
{
  switch(timInterrupt)
  {
    case TIMER_INTERRUPT_OVERFLOW:
      TB0CTL |= 0x0002;
      break;
    case TIMER_INTERRUPT_CC0:
      TB0CCTL0 |= 0x0010;
      break;
    default:
      break;
  }
}

/**
****************************************************************
@brief  Disable one of the possible timerInterrupts from enumeration.
@param  timInterrupt  One of the possible timerInterrupts from enumeration
@return -
****************************************************************
*/
void hal_tim_interruptDisable(timerInterrupt_t timInterrupt)
{
  switch(timInterrupt)
  {
    case TIMER_INTERRUPT_OVERFLOW:
      TB0CTL &= ~0x0002;
      break;
    case TIMER_INTERRUPT_CC0:
      TB0CCTL0 &= ~0x0010;
      break;
    default:
      break;
  }
}

/**
 ****************************************************************
 @brief  TimerB0 CC0 Interrupt Service Routine
 @param   -
 @return  -
 ****************************************************************
 */
#pragma vector=TIMER0_B0_VECTOR
__interrupt void _timer0BCC0_ISR(void)
{
  //note: interrupt-flag is automatic cleared (UserManual, Cap. 17.2.6.1)

  if(_hal_timerInterrupt_callbackFct[TIMER_INTERRUPT_CC0] != 0x00000000)  //do function callback, if a callback is registered
  {
    _hal_timerInterrupt_callbackFct[TIMER_INTERRUPT_CC0]();
  }
}

/**
 ****************************************************************
 @brief  TimerB0 TB, CC1-6 Interrupt Service Routine
 @param   -
 @return  -
 ****************************************************************
 */
#pragma vector=TIMER0_B1_VECTOR
__interrupt void _timer0B_TB_CC1_6_ISR(void)
{
  //read status (automatic clear the highest pending interrupt)
  uint8_t status = TB0IV;

  if(status == 0x000E) //Timer overflow
  {
    if(_hal_timerInterrupt_callbackFct[TIMER_INTERRUPT_OVERFLOW] != 0x00000000)  //do function callback, if a callback is registered
    {
      _hal_timerInterrupt_callbackFct[TIMER_INTERRUPT_OVERFLOW]();
    }
  }
}
