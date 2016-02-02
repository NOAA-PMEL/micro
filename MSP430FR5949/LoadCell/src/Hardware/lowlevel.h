/** @file fuses.h
 *  @brief
 *
 *  @author Matt Casari, matthew.casari@noaa.org
 *  @date Dec 4, 2015
 *  @version
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *	@note
 *
 *  @bug  No known bugs
 */

#ifndef LOWLEVEL_H_
#define LOWLEVEL_H_
/************************************************************************
*							HEADER FILES
************************************************************************/

#ifndef PMEL
#include "../test/msp430fr5969.h"
#endif

/************************************************************************
*						STANDARD LIBRARIES
************************************************************************/
#include <stdint.h>

/************************************************************************
*					GLOBAL FUNCTION PROTOTYPES
************************************************************************/
/** @brief Setup the FRAM in memory
 *
 *	Initializes the micro FRAM
 *
 *  @param none
 *
 *  @return none
 */
void FRAM_Init(void);


/** @brief Setup the watchdog
 *
 *	Initializes the watchdog with ~3 second period
 *
 *  @param none
 *
 *  @return none
 */
void WD_Init(void );

/** @brief Stops the watchdog
 *
 *	Stops the watchdog with HALT
 *
 *  @param none
 *
 *  @return none
 */
void WD_Halt(void);

/** @brief Hit the watchdog
 *
 *	Clears the watchdog timer to prevent reset
 *
 *  @param none
 *
 *  @return none
 */
void WD_Hit(void);

#endif

