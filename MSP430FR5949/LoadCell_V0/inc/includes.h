
/** @file includes.h
 *  @brief Top level include file for Load Cell firmware
 *
 *  @author Matt Casari, matthew.casari@noaa.org
 *  @date March 28, 2016
 *  @version 1.0.0
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
#include <msp430fr5949.h>		/* Device Specific Header File */
#endif

/************************************************************************
*							HEADER FILES
************************************************************************/
#include "../src/GPIO/gpio.h"				/* GPIO Driver Header file */
#include "../src/UART/uart.h"	  		/* UART Driver Header file */
#include "../src/Timer/timer.h" 		/* Timer Driver Header file */
#include "../src/I2C/i2c.h"					/* I2C Driver Header file */
#include "../src/Keller/PAxLD.h"		/* Keller PAxLD Sensor Driver Header file */
#include "../src/Stats/statistics.h"    /* Statistics functions */
#include "../src/buffers/buffers.h"     /* Buffer header file */
#include "../src/Console/console.h"     /* Console Header File */


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
/* Generic functions */
#define LENGTH_OF(x)				(sizeof(x)/sizeof(x[0]))
#define BIT_TO_MASK(a) 			 ( 1u << (a) )

/* I2C bus being used (only on on the MSP430FR5969) */
#define I2C_B0

/* Set up the wait loop delay values */
#define DELAY					(1000)

/* Timer B Values */
#define ResetTimerB()				(TB0R = 0)
#define SetTimerBInterrupt()		(TB0CCTL0 = CCIE)

/* Define PAXLD Power FET Pin */
#define FET_PORT					(3)
#define FET_PIN						(6)
#define FET_INIT()					(GPIO_SetPinAsOutput(FET_PORT,FET_PIN))
#define FET_ON()					(GPIO_SetPin(FET_PORT,FET_PIN))
#define FET_OFF()					(GPIO_ClearPin(FET_PORT,FET_PIN))
#define FET_TOGGLE()				(GPIO_TogglePin(FET_PORT,FET_PIN))

/* Number of PAXLD sensors attached to the I2C Bus */
#define NUM_KELLER_SENSORS	(1)

/* Number of samples to average over */
#define NUM_SAMPLES (5)
#define NUMBER_OF_SAMPLES       (5)

/* Size of the metadata arrays */
#define METADATA_ARRAY_SIZE (6)



/************************************************************************
*							STRUCTURES
************************************************************************/

/* PAXLD structure definition */
typedef struct PAXLD PAXLDSensor_t;
typedef struct _CircularBufferC_s CircularBuffer_t ;

/** @brief System Metadata Structure
 *
 * Metadata used for calculating Load Values
 *
 */
typedef struct SystemValues {
    float slope;        /** Pressure to load Slope constant */
    float intercept;    /** Pressure to load Intercept constant */
}SystemValues_t;

/** @brief System Mode Enumeration
 *
 * System Mode states 
 *
 */
typedef enum mode {
    Display,    /** Display state */
    Read,       /** Read state */
    Continue,   /** Continue state */
    Exit        /** Exit console state */
} modes_t;
 
/** @brief Console State Enumeration
 *
 *  Console state enumerations
 *
 */
typedef enum state {
    Hold,               /**  */
    Main,               /** Main State */
    Calibration,        /** Calibration State */
    ManualCal,          /** Manual Calibration State */
    DisplayCal,         /** Display Calibration State */
    DisplayMetadata,    /** Display Metatdata State */
    UpdateSN,           /** Update Serial Number State */ 
    AutoSample          /** Autosample State */
} state_t;

/** @brief Calibration State Enumeration
 *
 *  Calibration state enumerations
 *
 */
typedef enum _CalibrationState {
	CalHold,            /** Calibration Hold state */
	Input,              /** Calibration Input state */
	CalculateSandI,     /** Calculate Slope & Intercept State */
	DisplayCalValues,   /** Dispay Calibration Values State */
	DisplaySandI,       /** Display Slope & Interecpt State */
	ClearAllData,       /** Clear All Data State */
	ReturnToMain        /** Return to Main */
}CalibrationState_t;

/** @brief Console structure
 *
 *  Temporary console data
 *
 */
typedef struct console {
    modes_t mode;           /** current mode */
    uint8_t inputChar;      /** current input char */
    uint16_t SerialNumber;  /** current serial number */
}console_t;


/** @brief System state enumeration
 *
 *  Current state of the system
 *
 */
typedef enum SysState {
  Sample,           /** Sampling State */
  Compute,          /** Compute Stats State */
  Transmit,         /** Transmit Stats State */
  Console           /** Console State */
} SystemState_t;

/** @brief Console structure
 *
 *  All data stored in FRAM to report back to user
 *
 */
typedef struct _Metadata {
	float Slope;                                /** SLOPE Metadata value */
	float Intercept;                            /** INTERCEPT Metadata value */
	float RecordedData[METADATA_ARRAY_SIZE];    /** Recorded Data */
	float InputLoad[METADATA_ARRAY_SIZE];       /** Recent load data */
    uint16_t SerialNumber;                      /** Serial Number */
	uint8_t DataCounter;                        /** Current counter value */
}metadata_t;

/** @brief Console structure
 *
 *
 *
 */
typedef struct _CurrentData {
	float MeanPressure;         /** Current MEAN pressure values */
	float MaxPressure;          /** Current MAX pressure values */
	float MinPressure;          /** Current MIN pressure values */
	float STDPressure;          /** Current STD pressure values */
	float MeanTemperature;      /** Current MEAN temperature values */
	float MeanLoad;             /** Current MEAN load values */
	float MaxLoad;              /** Current MAX load values */
	float MinLoad;              /** Current MIN load values */
	float STDLoad;              /** Current STD load values */
	uint32_t crc;               /** Current CRC values */
}CurrentData_t;

/************************************************************************
*							GLOBAL VARIABLES
************************************************************************/
#ifdef PMEL


/* Flags and counters */
extern volatile uint32_t msTimeoutCounter;
extern volatile uint32_t ms2TimeoutCounter;
extern volatile uint32_t sampleTimer;
extern volatile uint32_t MenuTimeoutA;
extern volatile uint32_t ControlTimer;
extern volatile uint32_t ControlCounter;

/* Structures */
extern PAXLDSensor_t pxSensor;
extern SystemState_t SystemState;
extern CircularBuffer_t ConsoleData;
extern state_t ConsoleState;
extern metadata_t Metadata;
extern metadata_t FRAM_Metadata;
extern CalibrationState_t CalibrationState;

/* Functions */
extern void sensorRead(PAXLDSensor_t *sensor);
extern void sensorProcessData(PAXLDSensor_t *sensor);

#endif

#endif
