/**
 ****************************************************************
 @file    hal_timer.h
 ****************************************************************
 @brief   This module offers a set of functions to handle the TimerB0.
 ****************************************************************
 @author  Adrian Tuescher, IMES/HSR
 @version 1.1
 @date    2019-07-26
 ****************************************************************
 */
#ifndef HAL_TIMER_H_
#define HAL_TIMER_H_

// --- Includes
#include "../../GLOB_Types.h"

// --- Defines

// --- Typedefs
//! A enumeration which represent all timerInterrupts
typedef enum
{
  TIMER_INTERRUPT_OVERFLOW = 0,
  TIMER_INTERRUPT_CC0,
  TIMER_LAST_INTERRUPT
} timerInterrupt_t;

// --- Function Prototypes
void hal_tim_init(void);
void hal_tim_start(void);
void hal_tim_stop(void);

void hal_tim_interruptCallbackFctRegister(timerInterrupt_t timInterrupt, pFctHandler pCallbackFct);
void hal_tim_interruptEnable(timerInterrupt_t timInterrupt);
void hal_tim_interruptDisable(timerInterrupt_t timInterrupt);

#endif /* HAL_TIMER_H_ */
