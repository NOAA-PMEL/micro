
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
// #define DEBUG 	(true)

/************************************************************************
*						STANDARD LIBRARIES
************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
//#include <inttypes.h>

#ifndef PMEL
//#include "../test/msp430fr5969.h"
#else
#include <msp430fr5949.h>		// Device Specific Header File
#endif

/************************************************************************
*							HEADER FILES
************************************************************************/
#include "../src/GPIO/gpio.h"				// GPIO Driver Header file
#include "../src/UART/uart.h"	  		// UART Driver Header file
#include "../src/Timer/timer.h" 		// Timer Driver Header file
#include "../src/Console/console.h"     // Console header file
#include "../src/buffers/buffer_c.h"    // Char buffer header file
#include "../src/buffers/buffer_f.h"
#include "../src/buffers/buffer8.h"
#include "../src/buffers/buffer16.h"
#include "../src/rtc/rtc.h"
#include "../src/Stats/statistics.h"
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

// Debug Pin
#ifdef DEBUG
#define DEBUG_PORT  (1)
#define DEBUG_PIN   (6)
#endif 

/************************************************************************
*							STRUCTURES
************************************************************************/
typedef struct _CircularBufferC_s CircularBuffer_t ;

typedef struct _RTCStruct_s RTCStruct_t;

typedef struct _SampleData_s {
  CircularBufferF_s Mean;
  CircularBufferF_s STD;
  CircularBufferF_s Min;
  CircularBufferF_s Max;
  CircularBuffer16_s Year;
  CircularBuffer8_s Month;
  CircularBuffer8_s Day;
  CircularBuffer8_s Hour;
  CircularBuffer8_s Minute;
}SampleData_t;

typedef struct _CurrentData_s {
  uint32_t Counts[5][60];
  uint16_t Year[5];
  uint8_t Mon[5];
  uint8_t Day[5];
  uint8_t Hour[5];
  uint8_t Min[5];
  uint8_t min;
  uint8_t sec;
  uint8_t numSamples;
}CurrentData_t;

typedef enum SysState {
  Sample,
  Console,
  MinuteTimerRoutine,
  Transmit,
} SystemState_t;


typedef enum TransSubState{
  Counts,
  Volume,
  Report
} TransSubState_t;
/************************************************************************
*							GLOBAL VARIABLES
************************************************************************/
#ifdef PMEL

// Flags and counters
extern volatile uint32_t SensorCounter;
extern uint32_t SecondCounter;
extern uint32_t SumOfCount;
extern uint32_t MinuteSensorCounter;
extern volatile uint8_t ConsoleTimeoutCounter;
extern volatile uint8_t ConsoleCounter;
extern float slope;
extern float intercept;

// Structures
extern SystemState_t SystemState;
extern TransSubState_t TxSubState;
extern CircularBuffer_t ConsoleData;
extern RTCStruct_t RTC; 
extern CurrentData_t MinuteData;
// Functions


#endif

#endif
