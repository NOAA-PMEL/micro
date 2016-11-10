/** @file timer.h
 *  @brief Drivers for MSP430FR5949 Timers
 *
 *  @author Matt Casari, matthew.casari@noaa.org
 *  @date March 23, 2016
 *  @version 0.1.2
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *
 *  @bug  No known bugs
 */

#ifndef TIMER_H
#define TIMER_H

/************************************************************************
*						STANDARD LIBRARIES
************************************************************************/
#include <stdint.h>
#include <msp430fr5949.h>

/************************************************************************
*							HEADER FILES
************************************************************************/
#include "main.h"
/************************************************************************
*							MACROS
************************************************************************/
#ifndef true
#define true				(1)
#endif

#ifndef false
#define	false				(0)
#endif


#define	SYS_FREQ			(8000000u)
#define SMCLK				(SYS_FREQ / 8)

// Timer A Definitions
#define TIMERA_FREQUENCY		(1000)							// 10millisecond timer
#define TIMERA_TEMP_CALC1		(SMCLK/TIMERA_FREQUENCY)
#define TIMERA_DELAY_COUNT		(TIMERA_TEMP_CALC1 / 8)

// Timer B Definitions
#define TIMERB_FREQUENCY		(1000)							// 1millisecond timer
#define TIMERB_TEMP_CALC1		(SMCLK/TIMERB_FREQUENCY)
#define TIMERB_DELAY_COUNT		(TIMERB_TEMP_CALC1 / 8)

/** 
 * 
 *
 *
 */
typedef struct _TimerMS_s {
//  uint32_t volatile Elapsed;
//  uint32_t volatile Total;
  uint32_t volatile Timeout;
  uint8_t volatile TimeoutFlag;
}TimerMS_t;


//extern TimerMS_t UARTTimer;
//extern TimerMS_t *pUARTTimer;

extern volatile TimerMS_t *pOSTimer;
extern volatile TimerMS_t *pFLTimer;

//extern TimerMS_t SystemTimer;
/************************************************************************
*						GLOBAL FUNCTION PROTOTYPES
************************************************************************/
/** @brief Initialize Timer A
 *
 *	Initializes Timer A for use (freq set above in TIMERA_FREQUENCY)
 *
 *  @param none
 *
 *  @return none
 */
void TIMER_A0_Init ( void );

/** @brief Initialize Timer B
 *
 *	Initializes Timer B for use (freq set above in TIMERB_FREQUENCY)
 *
 *  @param none
 *
 *  @return none
 */
void TIMER_A1_Init ( void );

/** @brief TimerB X milli-second timer
 *
 *	Starts a one-shot timer for length milli-seconds in timer B
 *
 *  @param length Number of milli-seconds to set timer B for
 *
 *  @return none
 *
 *  @todo This function is restricts processor until complete, should be rethought
 */
void TIMER_B_MilliSecond( uint16_t length );

#endif
