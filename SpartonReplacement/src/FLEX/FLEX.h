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
#define SPARTON_FILTER_MSG      ("\xA4\x90\x00\x20\xA0")
#define SPARTON_MOUNTING_MSG    ("\xA4\x4A\x00\xA0")
#define SPARTON_DIRECTION_MSG   ("\xA4\x09\x00\x00\xA0")
#define SPARTON_TILT_MSG        ("\xA4\x06\x00\x00\x00\x00\xA0");

#define FLEX_START_CHAR         (0xA4)
#define FLEX_END_CHAR           (0xA0)
#define FLEX_FILTER_CMD         (0x90)
#define FLEX_HEADING_CMD        (0x09)
#define FLEX_PANDR_CMD          (0x06)
#define FLEX_MOUNTING_CMD       (0x4A)
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


/** @brief Flex Commands
 *
 * FLEX to Sparton Commands
 *
 */
typedef enum {
  
  FLEX_FILTER_SETUP,
  FLEX_MOUNT_SETUP,
  FLEX_TILT_CMD,
  FLEX_DIRECTION_CMD,
  FLEX_IDLE
}FLCMD_t;


/************************************************************************
*						STRUCTS
************************************************************************/
typedef struct {
//  CircularBufferC_s buf;    /** Receive Buffer */
  UART_t *UART;             /** Received UART Data */
  FLMODE_t SysMode;            /** COMMANDED Mode */
  FLCMD_t Mode;
}FLEX_t;


extern FLEX_t FLEX;

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