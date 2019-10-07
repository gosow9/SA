/**
 ****************************************************************
 @file    hal_gpio.h
 ****************************************************************
 @brief   This module offers a set of functions to handle the buttons
 @brief   and the LEDs of the EXP-MSP430FR6989 LaunchPad.
 ****************************************************************
 @author  Adrian Tuescher, IMES/HSR
 @version 1.2
 @date    2019-08-02
 ****************************************************************
 */
#ifndef HAL_GPIO_H_
#define HAL_GPIO_H_

// --- Includes
#include <stdbool.h>
#include "../../GLOB_Types.h"

// --- Defines

// --- Typedefs
//! A enumeration which represent all GPIO peripherals
typedef enum
{
  GPIO_S1 = 0,
  GPIO_S2,
  GPIO_BP_T1,
  GPIO_BP_T2,
  GPIO_LED1,
  GPIO_LED2,
  GPIO_LAST_PERIPHERIE
} gpioPeripherie_t;

// --- Function Prototypes
void hal_gpio_init(void);

bool hal_gpio_get(gpioPeripherie_t gpio);
void hal_gpio_set(gpioPeripherie_t gpio, bool on);
void hal_gpio_toggle(gpioPeripherie_t gpio);

void hal_gpio_interruptCallbackFctRegister(gpioPeripherie_t gpio, pFctHandler pCallbackFct);
void hal_gpio_interruptEnable(gpioPeripherie_t gpio);
void hal_gpio_interruptDisable(gpioPeripherie_t gpio);

#endif /* HAL_GPIO_H_ */
