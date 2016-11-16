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
#define VALID_STRING_LENGTH         (12)


/************************************************************************
*						STRUCTS
************************************************************************/

typedef struct {
//  CircularBufferC_s buf;    /** Receive Buffer */
  UART_t *UART;             /** Received UART Data */
  float heading;            /** Current Heading */
  float pitch;              /** Current Pitch */
  float roll;               /** Current Roll */
  uint8_t Calibrate;        /** Calibrate Mode 0-Off, 1-On */
}OS5000S_t;


extern OS5000S_t OS5000S;

/************************************************************************
*						ENUMS
************************************************************************/
typedef enum {
  OS_VALID,
  OS_INVALID_STRING
}OSSTAT_t;


/************************************************************************
*					GLOBAL FUNCTION PROTOTYPES
************************************************************************/
/** @brief Parse the OS5000-S buffer
 *
 *	Parse the buffer for heading, pitch and roll info
 *
 *  @param None
 *
 *  @return None
 */
void OS5000S_ParseBuffer(void);

/** @brief Return Current Heading value
 *
 *	Returns the current heading value
 *
 *  @param *heading Pointer to current heading
 *
 *  @return None
 */
void OS5000S_CurrentHeading(float *val);

/** @brief Return the current pitch & roll value
 *
 *	Return the current pitch & roll values
 *
 *  @param *pitch Pointer to Pitch value
 *  @param *roll Pointer to Roll value
 *
 *  @return None
 */
void OS5000S_CurrentPandR(float *pitch,float *roll);

/** @brief Return Current Heading value
 *
 *	Returns the current heading value
 *
 *  @param *heading Pointer to current heading
 *
 *  @return None
 */
void OS5000S_CurrentHeading_Int(uint16_t *heading);

/** @brief Return the current pitch & roll value
 *
 *	Return the current pitch & roll values
 *
 *  @param *pitch Pointer to Pitch value
 *  @param *roll Pointer to Roll value
 *
 *  @return None
 */
void OS5000S_CurrentPandR_Int(uint16_t *pitch,uint16_t *roll);