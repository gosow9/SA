/**
 ****************************************************************
 @file    GLOB_Types.h
 ****************************************************************
 @brief   Defines common types to be used in projects.
 ****************************************************************
 @author  Adrian Tuescher, IMES/HSR
 @version 1.0
 @date    2015-08-31
 ****************************************************************
 */
#ifndef GLOB_TYPES_H
#define GLOB_TYPES_H

// --- Includes
#include <stdbool.h>

// --- Defines
#define FALSE		false
#define TRUE		true

//*****************************************************************************
//
// Macros for hardware access
//
//*****************************************************************************
#define HWREG32(x)                                                              \
    (*((volatile uint32_t *)((uint16_t)x)))
#define HWREG16(x)                                                             \
    (*((volatile uint16_t *)((uint16_t)x)))
#define HWREG8(x)                                                             \
    (*((volatile uint8_t *)((uint16_t)x)))


// --- Typedefs
typedef float		float32_t;
typedef bool		bool_t;

typedef void (*pFctHandler)(void);
typedef bool_t (*pFctHandlerBoolVoid)(void);

#endif /* #ifndef GLOB_TYPES_H */
