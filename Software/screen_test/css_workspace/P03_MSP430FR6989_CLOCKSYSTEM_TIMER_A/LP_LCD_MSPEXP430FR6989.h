/**
 ****************************************************************
 @file    LP_LCD_MSPEXP430FR6989.h
 ****************************************************************
 @brief   This module offers some functions to handle the lcd of the
 @brief   MSP-EXP430FR6989 Launchpad.
 ****************************************************************
 @author  Adrian Tuescher, IMES/HSR
 @version 01.004.000
 @date    2018-07-26
 ****************************************************************
 */
#ifndef LP_LCD_MSPEXP430FR6989_H_
#define LP_LCD_MSPEXP430FR6989_H_

// --- Includes
#include <stdint.h>
#include "GLOB_Types.h"

// --- Defines

// --- Typedefs
//! A enumeration which represent all possible symbols on the LCD
typedef enum
{
  LCD_ANT = 0,
  LCD_RX,
  LCD_TX,
  LCD_HRT,
  LCD_NOTE,
  LCD_TMR,
  LCD_REC,
  LCD_BAT,
  LCD_BAT_CLAMP,
  LCD_BAT_LEVEL_1,
  LCD_BAT_LEVEL_2,
  LCD_BAT_LEVEL_3,
  LCD_BAT_LEVEL_4,
  LCD_BAT_LEVEL_5,
  LCD_BAT_LEVEL_6,
  LCD_LAST_SYMB
} lcdSymb_t;

// --- Function Prototypes
uint32_t lp_lcdInit(void);

void lp_lcdClrAll(void);
void lp_lcdClrTxt(void);

void lp_lcdSpecSymb(lcdSymb_t symb, bool_t on);

void lp_lcdTxtWrite(char* txt);

void lp_lcdHex8Bit(uint8_t value);
void lp_lcdHex16Bit(uint16_t value);
void lp_lcdInt(int value);
void lp_lcdFloat(float value);

void lp_lcdTime(uint8_t hour, uint8_t min, uint8_t sec);

#endif /* LP_LCD_MSPEXP430FR6989_H_ */
