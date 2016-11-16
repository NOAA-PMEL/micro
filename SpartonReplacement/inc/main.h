/** @file main.h
 *  @brief Main header for TAO Compass 
 *
 *  @author Matt Casari, matthew.casari@noaa.org
 *  @date Nov 8, 2016
 *  @version 0.0.1
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *  @bug  No known bugs
 */

#ifndef _MAIN_H
#define _MAIN_H

/************************************************************************
*						STANDARD LIBRARIES
************************************************************************/
#include <stdint.h>

/**************************** COMPILER  ********************************/
#define COMPILER    ("IAR EW430 v6.5.0")
#define VERSION     ("v0.0.1")
/*****************************  DEBUG  *********************************/
#define DEBUG 	(true)

#ifdef DEBUG
//#define INCLUDE_TESTS
#endif

void SETUP_Clock(void);
void SETUP_GPIO(void);
void SETUP_MicroPeripherals(void);
void SETUP_OceanServer(void);
void MAIN_SD_Unavailable(void);
void SYSTEM_RESET(void);
void SD_InitPins(void);
void SYS_ConfigurationRoutine(void);
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

#ifdef DEBUG
#define STATIC			
#else
#define STATIC              (static)
#endif
 
#define STR_HELPER(x)       #x
#define STR(x)              STR_HELPER(x)

/* Generic functions */
#define LENGTH_OF(x)		    (sizeof(x)/sizeof(x[0]))
#define BIT_TO_MASK(a) 			 ( 1u << (a) )

#define puts(x)                  (UART_Write((uint8_t*)&x[0],LENGTH_OF(x),UART_A1));

/* Set up the wait loop delay values */
#define DELAY					(1000)

/* Comm Ports MACROs */
#define NONE    (0)
#define I2C     (1)
#define SPI     (2)
//#define UART    (3)

#ifndef UCA0COMS
#define UCA0COMS    (SPI)
#endif

#ifndef UCA1COMS
#define UCA1COMS    (UART)
#endif

#ifndef UCB0COMS
#define UCB0COMS    (NONE)
#endif
/************************************************************************
*							STRUCTURES
************************************************************************/
typedef struct {
  volatile uint16_t SerialRxTimer;
  volatile uint8_t SerialRxTimerFlag:1;
}SystemTimer_t;


/************************************************************************
*							GLOBAL VARIABLES
************************************************************************/



/* Flags  */

/* Timers */

/* Structures */
typedef struct _CircularBuffer8_s CircularBuffer_s;
typedef struct _TimerMS_s MainTimerMS_s;



/* Externs */
//extern SystemStatus_t System;
extern CircularBuffer_s Buf8_Input;
extern CircularBuffer_s Buf8_Output;
extern MainTimerMS_s MainTimer;

//extern console_t console;

extern char buff[128];
extern char *pbuff;

#endif