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


#ifndef BUFFER32_H_
#define BUFFER32_H_
/************************************************************************
*							HEADER FILES
************************************************************************/
#include <stdint.h>
#include "buffers.h"

#ifndef PMEL
#include "../../test/msp430fr5969.h"
#else
#include <msp430fr5969.h>
#endif
/************************************************************************
*						STANDARD LIBRARIES
************************************************************************/

/************************************************************************
*							MACROS
************************************************************************/




typedef struct _CircularBuffer32_s
{
	uint32_t *buffer[BUFFER32_SIZE];
	uint16_t size;
	uint16_t read;
	uint16_t write;

}CircularBuffer32_s;



/************************************************************************
*					GLOBAL FUNCTION PROTOTYPES
************************************************************************/

int8_t Buffer32_Put(CircularBuffer32_s *buf, uint32_t value);
int8_t Buffer32_Get(CircularBuffer32_s *buf, uint32_t *value);
int8_t Buffer32_Clear(CircularBuffer32_s *buf);
int8_t Buffer32_IsFull(CircularBuffer32_s *buf);
int8_t Buffer32_IsEmpty(CircularBuffer32_s *buf);

#endif

