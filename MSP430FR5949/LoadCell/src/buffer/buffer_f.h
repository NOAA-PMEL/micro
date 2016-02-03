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



#ifndef BUFFER_F_H_
#define BUFFER_F_H_
/************************************************************************
*							HEADER FILES
************************************************************************/
#include <stdint.h>
#include "buffers.h"

#ifndef PMEL
#include "../test/msp430fr5969.h"
#else
#include <msp430fr5969.h>
#endif
/************************************************************************
*						STANDARD LIBRARIES
************************************************************************/

/************************************************************************
*							MACROS
************************************************************************/




typedef struct _CircularBufferF_s
{
	float buffer[BUFFER_F_SIZE];
	uint16_t size;
	uint16_t read;
	uint16_t write;

}CircularBufferF_s;


/************************************************************************
*					GLOBAL FUNCTION PROTOTYPES
************************************************************************/

int8_t BufferF_Put(CircularBufferF_s *buf, float value);
int8_t BufferF_Get(CircularBufferF_s *buf, float *value);
int8_t BufferF_Clear(CircularBufferF_s *buf);
int8_t BufferF_IsFull(CircularBufferF_s *buf);
int8_t BufferF_IsEmpty(CircularBufferF_s *buf);


#endif

