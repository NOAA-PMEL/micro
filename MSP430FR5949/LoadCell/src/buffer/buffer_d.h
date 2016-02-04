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



#ifndef BUFFER_D_H_
#define BUFFER_D_H_
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




typedef struct _CircularBufferD_s
{
	double buffer[BUFFER_F_SIZE];
	uint16_t size;
	uint16_t read;
	uint16_t write;

}CircularBufferD_s;


/************************************************************************
*					GLOBAL FUNCTION PROTOTYPES
************************************************************************/

int8_t BufferD_Put(CircularBufferD_s *buf, double value);
int8_t BufferD_Get(CircularBufferD_s *buf, double *value);
int8_t BufferD_Clear(CircularBufferD_s *buf);
int8_t BufferD_IsFull(CircularBufferD_s *buf);
int8_t BufferD_IsEmpty(CircularBufferD_s *buf);


#endif

