/** @file OS5000S.h
 *  @brief OceanServer OS5000-S Drivers
 *
 *  @author Matt Casari, matthew.casari@noaa.org
 *  @date November 9, 2016
 *  @version 0.0.1
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *  @note 
 *
 *  @bug  No known bugs
 */


/************************************************************************
*						HEADER FILES
************************************************************************/
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
/************************************************************************
*						STANDARD LIBRARIES
************************************************************************/
#include "UART.h"
#include "DMA.h"
#include "timer.h"
#include "buffer_c.h"

/************************************************************************
*						MACROS
************************************************************************/


/************************************************************************
*						ENUMS
************************************************************************/
/** @brief Flex String Validation
 * 
 * FLEX String Validation enum's
 *
 */
typedef enum {
  FLEX_VALID,
  FLEX_INVALID_STRING
}FLSTAT_t;


/** @brief Flex Command Modes
 * 
 * FLEX Command Modes
 *
 */
typedef enum {
  FL_DEBUG,
  FL_CONFIGURE,
  FL_NORMAL
}FLMODE_t;  


/************************************************************************
*						STRUCTS
************************************************************************/
typedef struct {
//  CircularBufferC_s buf;    /** Receive Buffer */
  UART_t *UART;             /** Received UART Data */
  FLMODE_t Mode;            /** COMMANDED Mode */
}FLEX_t;


extern volatile FLEX_t FLEX;

/************************************************************************
*					GLOBAL FUNCTION PROTOTYPES
************************************************************************/
/** @brief Parse the FLEX buffer
 *
 *	Parse the buffer for commands
 *
 *  @param None
 *
 *  @return None
 */
void FLEX_Init(void);