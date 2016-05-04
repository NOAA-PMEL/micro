/** @file DMA.h
 *  @brief These functions provide access the Direct Memory Allocation (DMA)
 *  ability of the MSP430.  
 *
 *  @author Matt Casari, matthew.casari@noaa.org
 *  @date May 2, 2016
 *  @version 0.0.1
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *	@note
 *
 *  @bug  No known bugs
 */



#ifndef _DMA_H
#define _DMA_H
/************************************************************************
*							HEADER FILES
************************************************************************/

/************************************************************************
*						STANDARD LIBRARIES
************************************************************************/
#include <stdint.h>
#include "../../inc/includes.h"

#ifdef PMEL
#include <msp430fr5949.h>
#else
#include "../../test/msp430fr5969.h"
#endif
/************************************************************************
*							MACROS
************************************************************************/


/************************************************************************
*					GLOBAL FUNCTION PROTOTYPES
************************************************************************/
/** @brief Initialize the GPIO
 *
 *	Clears all GPIO and sets to outputs
 *
 *  @param none
 *
 *  @return OKs
 */
uint8_t DMA_Init(uint16_t src_addr, uint16_t dest_addr, uint16_t size);

/** @brief Set Output Pin
 *
 *	Sets the selected port/pin to output
 *
 *  @param Port GPIO port pin is located on
 *  @param Pin GPIO pin to set as output
 *
 *  @return status
 */
uint8_t DMA_SetSourceAddress(uint16_t src_addr);

/** @brief Set Input Pin
 *
 *	Sets the selected port/pin as input
 *
 *  @param Port GPIO Port pin is located on
 *  @param Pin GPIO Pin to set as input
 *
 *  @return status
 */
uint8_t DMA_SetDestinationAddress( uint16_t dest_addr );

/** @brief Set Pin Output HIGH
 *
 *	Sets the selected pin output to HIGH
 *
 *  @param Port GPIO Port pin is located on
 *  @param Pin GPIO Pin to set high
 *
 *  @return status
 */
uint8_t DMA_Restart(void);

uint8_t DMA_TriggerBlock(void);

uint8_t DMA_CompleteTransmit(uint8_t channel);
#endif
