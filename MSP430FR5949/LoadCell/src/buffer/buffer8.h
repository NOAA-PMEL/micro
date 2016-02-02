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



#ifndef BUFFER8_H_
#define BUFFER8_H_
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




typedef struct _CircularBuffer8_s
{
	uint8_t *buffer[BUFFER8_SIZE];
	uint16_t size;
	uint16_t read;
	uint16_t write;

}CircularBuffer8_s;


/************************************************************************
*					GLOBAL FUNCTION PROTOTYPES
************************************************************************/

int8_t Buffer8_Put(CircularBuffer8_s *buf, uint8_t value);
int8_t Buffer8_Get(CircularBuffer8_s *buf, uint8_t *value);
int8_t Buffer8_Clear(CircularBuffer8_s *buf);
int8_t Buffer8_IsFull(CircularBuffer8_s *buf);
int8_t Buffer8_IsEmpty(CircularBuffer8_s *buf);


#endif

