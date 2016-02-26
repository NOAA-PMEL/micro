
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
#define PMEL
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
#include "../src/I2C/i2c.h"					// I2C Driver Header file
#include "../src/Keller/PAxLD.h"		// Keller PAxLD Sensor Driver Header file
#include "../src/Stats/statistics.h"    // Statistics functions
#include "../src/buffers/buffers.h"
#include "../src/Console/console.h"


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

// I2C bus being used (only on on the MSP430FR5969)
#define I2C_B0

// Set up the wait loop delay values
#define DELAY					(1000)

// Timer B Values
#define ResetTimerB()				(TB0R = 0)
#define SetTimerBInterrupt()		(TB0CCTL0 = CCIE)

// Define PAXLD Power FET Pin
#define FET_PORT					(3)
#define FET_PIN						(6)
#define FET_INIT()					(GPIO_SetPinAsOutput(FET_PORT,FET_PIN))
#define FET_ON()					(GPIO_SetPin(FET_PORT,FET_PIN))
#define FET_OFF()					(GPIO_ClearPin(FET_PORT,FET_PIN))
#define FET_TOGGLE()				(GPIO_TogglePin(FET_PORT,FET_PIN))

// Number of PAXLD sensors attached to the I2C Bus
#define NUM_KELLER_SENSORS	(1)

// Number of samples to average over
#define NUM_SAMPLES (5)
#define NUMBER_OF_SAMPLES       (5)

// Size of the metadata arrays
#define METADATA_ARRAY_SIZE (6)



/************************************************************************
*							STRUCTURES
************************************************************************/

// PAXLD structure definition
typedef struct PAXLD PAXLDSensor_t;
typedef struct _CircularBufferC_s CircularBuffer_t ;

//typedef struct{
//	union {
//
//		struct {
//			uint16_t	StateA : 1,	/* Temperature Ready to Convert	*/
//						StateB : 1,			/* Timer A has been hit */
//						StateC : 1,	// Unused
//						StateD : 1,	// Unused
//						StateE : 1,	// Unused
//						DebounceFlag : 1,	// Unused
//						TimerB : 1,		// Unused
//						spare8 : 1,		// Unused
//						spare7 : 1,		// Unused
//						spare6 : 1,		// Unused
//						spare5 : 1,		// Unused
//						spare4 : 1,		// Unused
//						spare3 : 1,		// Unused
//						spare2 : 1,		// Unused
//						spare1 : 1,		// Unused
//						spare0 : 1;		// Unused
//		};
//		uint16_t	all_flags;
//	};
//}FLAGS;

typedef struct SystemValues {
    float slope;
    float intercept;
}SystemValues_t;

typedef enum mode {
    Display,
    Read,
    Continue,
    Exit
} modes_t;

typedef enum state {
		Hold,
    Main,
    Calibration,
    ManualCal,
    DisplayCal,
    DisplayMetadata,
    UpdateSN,
    AutoSample
} state_t;

typedef enum _CalibrationState {
	CalHold,
	Input,
	CalculateSandI,
	DisplayCalValues,
	DisplaySandI,
	ClearAllData,
	ReturnToMain
}CalibrationState_t;

typedef struct console {
    modes_t mode;
//    state_t state;
//    state_t previousState;
    uint8_t inputChar;
    uint16_t SerialNumber;
}console_t;

typedef enum SysState {
  Sample,
  Compute,
  Transmit,
  Console
} SystemState_t;


typedef struct _Metadata {
	float Slope;
	float Intercept;
	float RecordedData[METADATA_ARRAY_SIZE];
	float InputLoad[METADATA_ARRAY_SIZE];
	uint8_t DataCounter;
}metadata_t;


/************************************************************************
*							GLOBAL VARIABLES
************************************************************************/

//extern console_t console;

#ifdef PMEL
extern float slope;
extern float intercept;

// Flags and counters
//extern volatile FLAGS TimerFlags;
extern volatile uint32_t msTimeoutCounter;
extern volatile uint32_t ms2TimeoutCounter;
extern volatile uint32_t sampleTimer;
extern volatile uint32_t MenuTimeoutA;
extern volatile uint32_t ControlTimer;
extern volatile uint32_t ControlCounter;

// Structures
extern PAXLDSensor_t pxSensor;
extern SystemState_t SystemState;
extern CircularBuffer_t ConsoleData;
extern state_t ConsoleState;
extern metadata_t Metadata;
extern metadata_t FRAM_Metadata;
extern CalibrationState_t CalibrationState;
// functions

extern void sensorRead(PAXLDSensor_t *sensor);
extern void sensorProcessData(PAXLDSensor_t *sensor);

#endif

#endif
