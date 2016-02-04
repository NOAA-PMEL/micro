
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
#include "../src/GPIO/gpio.h"		// GPIO Header file
//#include "./Timer/timer.h"
#include "../src/UART/uart.h"
#include "../src/Hardware/lowlevel.h"
#include "../src/I2C/i2c.h"
#include "../src/Keller/PAxLD.h"
#include "../src/console/console.h"
#include "../src/buffer/buffers.h"
//#include "./Analog/analog.h"

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
#include LENGTH_OF(x)				(sizeof(x[0])/sizeof(x))

// I2C bus being used (only on on the MSP430FR5969)
#define I2C_B0

// Debounce timer
//#define DEBOUNCE_TIMER_MAX			(5)			// milliseconds

// Timer B
#define ResetTimerB()				(TB0R = 0)
#define SetTimerBInterrupt()		(TB0CCTL0 = CCIE)

// For DEBUG set the Dev Board LEDs up
#ifdef DEBUG

// LED 1 Macros
#define LED1_PORT					(GPIO_PORTP4)
#define LED1_PIN					(6)
#define LED1_INIT()					(GpioSetOutputPins(LED1_PORT,LED1_PIN))
#define LED1_OFF()					(GpioSetOutputLow(LED1_PORT,LED1_PIN))
#define LED1_ON()					(GpioSetOutputHigh(LED1_PORT,LED1_PIN))
#define LED1_TOGGLE()	      		(GpioTogglePin(LED1_PORT,LED1_PIN))
// LED 2 Macros
#define LED2_PORT					(GPIO_PORTP1)
#define LED2_PIN					(0)
#define LED2_INIT()					(GpioSetOutputPins(LED2_PORT,LED2_PIN))
#define LED2_OFF()					(GpioSetOutputLow(LED2_PORT,LED2_PIN))
#define LED2_ON()					(GpioSetOutputHigh(LED2_PORT,LED2_PIN))
#define LED2_TOGGLE()	      		(GpioTogglePin(LED2_PORT,LED2_PIN))

#endif


// Location of input pin of the PAXLD Sensor EOC pin
#define PAXLD_SENSOR0_EOC_PORT		(GPIO_PORTP1)
#define PAXLD_SENSOR0_EOC_PIN		(2)

#define PAXLD_SENSOR1_EOC_PORT		(GPIO_PORTP3)
#define PAXLD_SENSOR1_EOC_PIN		(4)

#define PAXLD_SENSOR2_EOC_PORT		(GPIO_PORTP1)
#define PAXLD_SENSOR2_EOC_PIN		(5)

#define PAXLD_SENSOR3_EOC_PORT		(GPIO_PORTP3)
#define PAXLD_SENSOR3_EOC_PIN		(5)

#define PAXLD_SENSOR4_EOC_PORT		(GPIO_PORTP1)
#define PAXLD_SENSOR4_EOC_PIN		(4)

#define PAXLD_SENSOR5_EOC_PORT		(GPIO_PORTP3)
#define PAXLD_SENSOR5_EOC_PIN		(6)

#define PAXLD_SENSOR6_EOC_PORT		(GPIO_PORTP1)
#define PAXLD_SENSOR6_EOC_PIN		(3)



// Interrupt mapping
#define INT_P1_2                    (pxSensor[0].dataAvailableFlag = true)
#define INT_P3_4                    (pxSensor[1].dataAvailableFlag = true)
#define INT_P1_5                    (pxSensor[2].dataAvailableFlag = true)
#define INT_P3_5                    (pxSensor[3].dataAvailableFlag = true)
#define INT_P1_4                    (pxSensor[4].dataAvailableFlag = true)
#define INT_P3_6                    (pxSensor[5].dataAvailableFlag = true)
#define INT_P1_3                    (pxSensor[6].dataAvailableFlag = true)

#define INT_P1_0                    (__no_operation())
#define INT_P1_1                    (__no_operation())
#define INT_P1_6                    (__no_operation())
#define INT_P1_7                    (__no_operation())

#define INT_P2_0                    (__no_operation())
#define INT_P2_1                    (__no_operation())
#define INT_P2_2                    (__no_operation())
#define INT_P2_3                    (__no_operation())
#define INT_P2_4                    (__no_operation())
#define INT_P2_5                    (__no_operation())
#define INT_P2_6                    (__no_operation())
#define INT_P2_7                    (__no_operation())

#define INT_P3_0                    (__no_operation())
#define INT_P3_1                    (__no_operation())
#define INT_P3_2                    (__no_operation())
#define INT_P3_3                    (__no_operation())
#define INT_P3_7                    (__no_operation())

#define INT_P4_0                    (__no_operation())
#define INT_P4_1                    (__no_operation())
#define INT_P4_2                    (__no_operation())
#define INT_P4_3                    (__no_operation())
#define INT_P4_4                    (__no_operation())
#define INT_P4_5                    (__no_operation())
#define INT_P4_6                    (__no_operation())
#define INT_P4_7                    (__no_operation())

// Define PAXLD Power FET Pin
#define FET_PORT					(GPIO_PORTP3)
#define FET_PIN						(0)
#define FET_INIT()					(GpioSetOutputPins(FET_PORT,FET_PIN))
#define FET_OFF()					(GpioSetOutputHigh(FET_PORT,FET_PIN))
#define FET_ON()					(GpioSetOutputLow(FET_PORT,FET_PIN))
#define FET_TOGGLE()				(GpioTogglePin(FET_PORT,FET_PIN))

// Number of PAXLD sensors attached to the I2C Bus
#define NUM_SENSORS					(7)
#define DELAY                           (500)

// Number of samples to average over
#define NUM_SAMPLES (5)

// PAXLD structure definition
typedef struct PAXLD PAXLDSensor_t;

/************************************************************************
*							STRUCTURES
************************************************************************/
typedef struct{
	union {

		struct {
			uint16_t	StateA : 1,	/* Temperature Ready to Convert	*/
						StateB : 1,			/* Timer A has been hit */
						StateC : 1,	// Unused
						StateD : 1,	// Unused
						StateE : 1,	// Unused
						DebounceFlag : 1,	// Unused
						TimerB : 1,		// Unused
						spare8 : 1,		// Unused
						spare7 : 1,		// Unused
						spare6 : 1,		// Unused
						spare5 : 1,		// Unused
						spare4 : 1,		// Unused
						spare3 : 1,		// Unused
						spare2 : 1,		// Unused
						spare1 : 1,		// Unused
						spare0 : 1;		// Unused
		};
		uint16_t	all_flags;
	};
}FLAGS;

/************************************************************************
*							GLOBAL VARIABLES
************************************************************************/
#ifdef PMEL
extern volatile FLAGS TimerFlags;
extern volatile uint8_t msTimeoutCounter;
extern const uint8_t sensorAddress[NUM_SENSORS];
extern const uint8_t sensorEOCPort[NUM_SENSORS];
extern const uint16_t sensorEOCPin[NUM_SENSORS];
extern PAXLDSensor_t pxSensor[NUM_SENSORS];
extern volatile uint8_t sensorCount;
#endif

#endif
