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



#ifndef BUFFER16_H_
#define BUFFER16_H_
/************************************************************************
*							HEADER FILES
************************************************************************/
#include <stdint.h>
#include "../inc/msp430fr5969.h"
#include "buffers.h"
/************************************************************************
*						STANDARD LIBRARIES
************************************************************************/

/************************************************************************
*							MACROS
************************************************************************/


typedef struct _CircularBuffer16_s
{
	uint16_t *buffer[BUFFER16_SIZE];
	uint16_t size;
	uint16_t read;
	uint16_t write;

}CircularBuffer16_s;


/************************************************************************
*					GLOBAL FUNCTION PROTOTYPES
************************************************************************/

int8_t Buffer16_Put(CircularBuffer16_s *buf, uint16_t value);
int8_t Buffer16_Get(CircularBuffer16_s *buf, uint16_t *value);
int8_t Buffer16_Clear(CircularBuffer16_s *buf);
int8_t Buffer16_IsFull(CircularBuffer16_s *buf);
int8_t Buffer16_IsEmpty(CircularBuffer16_s *buf);

#endif

