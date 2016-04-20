/** @file spi_H
  *  @brief
  *
  *  @author Matt Casari, matthew.casari@noaa.org
  *  @date 
  *  @version
  *
  *  @copyright National Oceanic and Atmospheric Administration
  *  @copyright Pacific Marine Environmental Lab
  *  @copyright Environmental Development Division
  *
  *	 @note
  *
  *  @bug  No known bugs
  */
#ifndef SPI_H
#define SPI_
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
*							CONSTANTS
************************************************************************/

/************************************************************************
*							MACROS
************************************************************************/
#define SPI_VALID       (0)
#define SPI_INVALID     (1)

/************************************************************************
*							STRUCTURES
************************************************************************/
/** @brief 
 * 
 * 
 *
 */

/************************************************************************
*							ENUMERATIONS
************************************************************************/
/** @brief System State Enumeration
 *
 *  
 */
 
/************************************************************************
*					GLOBAL FUNCTION PROTOTYPES
************************************************************************/
/** @brief Init SPI Channel
 *
 *	Sets all internals for SPI comms
 *
 *  @return Status
 */
uint8_t SPI_Init(void);

/** @brief Write to SPI
 *
 *	Write a single char to SPI channel
 *
 *  @return Status
 */
uint8_t SPI_Write(uint8_t value);
    
    
    #endif // spi_H
