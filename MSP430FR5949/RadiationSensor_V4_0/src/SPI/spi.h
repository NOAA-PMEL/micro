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
*						STANDARD LIBRARIES
************************************************************************/

/************************************************************************
*							CONSTANTS
************************************************************************/

/************************************************************************
*							MACROS
************************************************************************/


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
uint8_t SPI_Write(void);
    
    
    #endif // spi_H
