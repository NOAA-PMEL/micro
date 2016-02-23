/** @file BUFFER_F.c
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
#include "buffer_f.h"
/************************************************************************
*					STATIC VARIABLES
************************************************************************/

/************************************************************************
*					GLOBAL FUNCTIONS
************************************************************************/
int8_t BufferF_Put(CircularBufferF_s *buf, float value)
{
	uint8_t NextWrite = (buf->write + 1) % (ACTUAL_BUFFER_F_SIZE);

	if(NextWrite == buf->read)
	{
		return BUFFER_F_ERROR_FULL;
	}

	buf->buffer[buf->write] = value;
	buf->write = NextWrite;

	return 0;
}

int8_t BufferF_Put_Circular(CircularBufferF_s *buf, float value)
{
	uint8_t NextWrite = (buf->write + 1) % (ACTUAL_BUFFER_F_SIZE);                                                                          
	uint8_t NextRead = (buf->read + 1) % (ACTUAL_BUFFER_F_SIZE);
	if(NextWrite == buf->read)
	{
     buf->write = buf->read;
		 buf->read = NextRead;   
//         buf->write = NextWrite;
//         buf->buffer[buf->write]=value;
	}      
	
	buf->buffer[buf->write] = value;
	buf->write = NextWrite;      
    
    if(buf->read >= ACTUAL_BUFFER_F_SIZE)
    {
      buf->read = 0;
    }
	
	return 0;
}
int8_t BufferF_Get(CircularBufferF_s *buf, float *value)
{
	if(buf->read == buf->write)
	{
		return BUFFER_F_ERROR_EMPTY;
	}

	*value = buf->buffer[buf->read];
	buf->read = (buf->read + 1) % ACTUAL_BUFFER_F_SIZE;

	return 0;
}


int8_t BufferF_Clear(CircularBufferF_s *buf)
{
	uint16_t i = 0;

	buf->read = 0;
	buf->write = 0;

	for(i=0;i<ACTUAL_BUFFER_F_SIZE;i++)
	{
		buf->buffer[i] = 0;
	}

	return 0;
}

int8_t BufferF_IsFull(CircularBufferF_s *buf)
{
	uint16_t NextWrite = (buf->write + 1) %(ACTUAL_BUFFER_F_SIZE);

	if(NextWrite == buf->read)
	{
		return BUFFER_F_IS_FULL;
	}
	return BUFFER_F_NOT_FULL;
}

int8_t BufferF_IsEmpty(CircularBufferF_s *buf)
{
	if(buf->write == buf->read)
	{
		return BUFFER_F_IS_EMPTY;
	}

	return BUFFER_F_NOT_EMPTY;
}

