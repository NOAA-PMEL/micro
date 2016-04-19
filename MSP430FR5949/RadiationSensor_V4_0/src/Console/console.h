/** @file console.h
 *  @brief
 *
 *  @author Matt Casari, matthew.casari@noaa.org
 *  @date March 23, 2016
 *  @version 0.1.0
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *	@note
 *
 *  @bug  No known bugs
 */

#ifndef console_H
#define console_H
/************************************************************************
*							HEADER FILES
************************************************************************/
#include "../../inc/includes.h"

#ifndef PMEL
#include "../test/msp430fr5969.h"
#else
#include <msp430fr5949.h>
#endif

/************************************************************************
*						STANDARD LIBRARIES
************************************************************************/
#include <stdint.h>
/************************************************************************
*							MACROS
************************************************************************/
#define CONSOLE_TIMEOUT     (30)

/** @brief Input Value Enumeration
 *
 *  Console state enumeration 
 */
typedef enum InputValue
{
  ConsoleWait,          /** Normal Console State  */
  ConstantsInput,       /** Waiting for constant value input */
  DateTimeInput,        /** Waiting for DateTime input */
  SerialInput
}InputValue_t;

/************************************************************************
*					GLOBAL FUNCTION PROTOTYPES
************************************************************************/
/** @brief Console Main State
 *
 *  Enter console read/parse routine
 *
 *  @param none
 *
 *  @return None
 */
void CONSOLE_Main(void);
#endif // console_H
