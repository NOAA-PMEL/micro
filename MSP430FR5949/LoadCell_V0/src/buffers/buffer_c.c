/** @file BufferC.c
 *  @brief Character circular buffer library
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

#include "buffer_c.h"


/************************************************************************
*					GLOBAL FUNCTIONS
************************************************************************/
int8_t BufferC_Put(CircularBufferC_s *buf, char value)
{
	uint8_t NextWrite = (buf->write + 1) % (ACTUAL_BUFFER_C_SIZE);

	if(NextWrite == buf->read)
	{
		return BUFFER_C_ERROR_FULL;
	}

	buf->buffer[buf->write] = value;
	buf->write = NextWrite;

	return 0;
}

int8_t BufferC_Get(CircularBufferC_s *buf, char *value)
{
	if(buf->read == buf->write)
	{
		return BUFFER_C_ERROR_EMPTY;
	}

	*value = buf->buffer[buf->read];
	buf->read = (buf->read + 1) % ACTUAL_BUFFER_C_SIZE;

	return 0;
}


int8_t BufferC_Clear(CircularBufferC_s *buf)
{
	uint16_t i = 0;

	buf->read = 0;
	buf->write = 0;

	for(i=0;i<ACTUAL_BUFFER_C_SIZE;i++)
	{
		buf->buffer[i] = 0;
	}

	return 0;
}

int8_t BufferC_IsFull(CircularBufferC_s *buf)
{
	uint16_t NextWrite = (buf->write + 1) %(ACTUAL_BUFFER_C_SIZE);

	if(NextWrite == buf->read)
	{
		return BUFFER_C_IS_FULL;
	}
	return BUFFER_C_NOT_FULL;
}

int8_t BufferC_IsEmpty(CircularBufferC_s *buf)
{
	if(buf->write == buf->read)
	{
		return BUFFER_C_IS_EMPTY;
	}

	return BUFFER_C_NOT_EMPTY;
}


