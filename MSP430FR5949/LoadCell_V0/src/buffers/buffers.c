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
#include "buffers.h"
/************************************************************************
*					STATIC VARIABLES
************************************************************************/

/************************************************************************
*					FLOAT Buffer Functions
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



/************************************************************************
*					CHAR Buffer Functions
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


int8_t BufferC_CheckForNewline(CircularBufferC_s *buf)
{
	uint16_t readNum = 0;
	uint16_t length = 0;
	
	if(buf->write == buf->read)
	{
		return BUFFER_C_IS_EMPTY;
	}
	
	if(buf->read > buf->write)
	{
		length = ACTUAL_BUFFER_C_SIZE - buf->read;
		length += buf->write;
	}
	else 
	{
		length = buf->write - buf->read;
	}

	for(uint16_t i=0;i<length;i++)
	{
		readNum = ((buf->read+i) % (ACTUAL_BUFFER_C_SIZE));
		
		if(buf->buffer[readNum] == 0x0A)
		{
			return BUFFER_C_NEWLINE_DETECTED;
		}
	}
	
	return BUFFER_C_NEWLINE_NOT_DETECTED;
}