/**
 ****************************************************************
 @file    hal_clock.h
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
#ifndef HAL_CLOCK_H_
#define HAL_CLOCK_H_

// --- Includes

// --- Defines

// --- Typedefs

// --- Function Prototypes
void hal_clk_init(void);

#endif /* HAL_CLOCK_H_ */
