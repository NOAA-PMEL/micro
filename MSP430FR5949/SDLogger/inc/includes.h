
/** @file includes.h
 *  @brief Drivers for MSP430FR5969 GPIO
 *
 *  @author Matt Casari, matthew.casari@noaa.org
 *  @date October 7, 2015
 *  @version 0.1.1
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *  @bug  No known bugs
 */

#ifndef INCLUDES_H
#define INCLUDES_H


/*****************************  DEBUG  *********************************/
#define DEBUG 	(true)
//#define PMEL
/************************************************************************
*						STANDARD LIBRARIES
************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>

#ifndef PMEL
#include "../test/msp430fr5969.h"
#else
#include <msp430fr5969.h>		// Device Specific Header File
#endif

/************************************************************************
*							HEADER FILES
************************************************************************/
#include "../src/GPIO/gpio.h"				// GPIO Driver Header file
#include "../src/UART/uart.h"	  		// UART Driver Header file
#include "../src/Timer/timer.h" 		// Timer Driver Header file
#include "../src/SPI/SPI.h"
#include "../src/buffers/buffer_c.h"
#include "../src/buffers/buffer8.h"
#include "../src/buffers/buffer16.h"
#include "../src/buffers/buffer32.h"

/************************************************************************
*							CONSTANTS
************************************************************************/
#ifndef true
#define true				(1)
#endif

#ifndef false
#define	false				(0)
#endif

/************************************************************************
*							MACROS
************************************************************************/
// Generic functions
#define LENGTH_OF(x)				(sizeof(x)/sizeof(x[0]))
#define BIT_TO_MASK(a) 			 ( 1u << (a) )

// Set up the wait loop delay values
#define DELAY					(1000)


// Macro for Power FET Pin
#define FET_PORT					(3)
#define FET_PIN						(6)
#define FET_INIT()					(GPIO_SetPinAsOutput(FET_PORT,FET_PIN))
#define FET_ON()					(GPIO_SetPin(FET_PORT,FET_PIN))
#define FET_OFF()					(GPIO_ClearPin(FET_PORT,FET_PIN))
#define FET_TOGGLE()				(GPIO_TogglePin(FET_PORT,FET_PIN))

// Macro for SPI Chip Select
#define CS_PORT                     (3)
#define CS_PIN                      (7)
#define CS_INIT()                   (GPIO_SetPinAsOutput(CS_PORT,CS_PIN))
#define CS_ON()                     (GPIO_SetPin(CS_PORT,CS_PIN))
#define CS_OFF()                    (GPIO_ClearPin(CS_PORT,CS_PIN))
#define CD_TOGGLE()                 (GPIO_TogglePin(CS_PORT,CS_PIN))

/************************************************************************
*							STRUCTURES
************************************************************************/


/************************************************************************
*							GLOBAL VARIABLES
************************************************************************/

//extern console_t console;

#ifdef PMEL


// Flags and counters

// Structures
typedef struct _CircularBuffer8_s CircularBuffer_s;

extern CircularBuffer_s Buf8_Input;
extern CircularBuffer_s Buf8_Output;

// functions


#endif

#endif
