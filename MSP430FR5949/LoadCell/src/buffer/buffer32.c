/** @file BUFFER8.c
 *  @brief
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date Dec 4, 2015
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

/************************************************************************
*					STATIC FUNCTION PROTOTYPES
************************************************************************/
#include "buffer32.h"
/************************************************************************
*					STATIC VARIABLES
************************************************************************/

/************************************************************************
*					GLOBAL FUNCTIONS
************************************************************************/
int8_t Buffer32_Put(CircularBuffer32_s *buf, uint32_t value)
{
	uint16_t NextWrite = (buf->write + 1) % (ACTUAL_BUFFER16_SIZE);

	if(NextWrite == buf->read)
	{
		return BUFFER_ERROR_FULL;
	}

	buf->buffer[buf->write] = value;
	buf->write = NextWrite;

	return 0;
}

int8_t Buffer32_Get(CircularBuffer32_s *buf, uint32_t *value)
{
	if(buf->read == buf->write)
	{
		return BUFFER_ERROR_EMPTY;
	}

	*value = buf->buffer[buf->read];
	buf->read = (buf->read + 1) % ACTUAL_BUFFER32_SIZE;

	return 0;
}


int8_t Buffer32_Clear(CircularBuffer32_s *buf)
{
	uint16_t i = 0;

	buf->read = 0;
	buf->write = 0;

	for(i=0;i<ACTUAL_BUFFER32_SIZE;i++)
	{
		buf->buffer[i] = 0;
	}

	return 0;
}

int8_t Buffer32_IsFull(CircularBuffer32_s *buf)
{
	uint16_t NextWrite = (buf->write + 1) %(ACTUAL_BUFFER32_SIZE);

	if(NextWrite == buf->read)
	{
		return BUFFER_IS_FULL;
	}
	return BUFFER_NOT_FULL;
}

int8_t Buffer32_IsEmpty(CircularBuffer32_s *buf)
{
	if(buf->write == buf->read)
	{
		return BUFFER_IS_EMPTY;
	}

	return BUFFER_NOT_EMPTY;
}


/************************************************************************
*					STATIC FUNCTIONS
************************************************************************/
/** @brief
 *
 *	Add full description here
 *
 *  @param none
 *
 *  @return none
 */




/************************************************************************
*					INTERRUPT VECTOR
************************************************************************/

