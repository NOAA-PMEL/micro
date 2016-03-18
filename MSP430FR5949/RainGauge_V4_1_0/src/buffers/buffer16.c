/** @file BUFFER16.c
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
#include "buffer16.h"
/************************************************************************
*					STATIC VARIABLES
************************************************************************/

/************************************************************************
*					GLOBAL FUNCTIONS
************************************************************************/
int8_t Buffer16_Put(CircularBuffer16_s *buf, uint16_t value)
{
	uint16_t NextWrite = (buf->write + 1) % (ACTUAL_BUFFER16_SIZE);

	if(NextWrite == buf->read)
	{
		return BUFFER16_ERROR_FULL;
	}

	buf->buffer[buf->write] = value;
	buf->write = NextWrite;

	return 0;
}

int8_t Buffer16_Put_Circular(CircularBuffer16_s *buf, float value)
{
	uint8_t NextWrite = (buf->write + 1) % (ACTUAL_BUFFER16_SIZE);                                                                          
	uint8_t NextRead = (buf->read + 1) % (ACTUAL_BUFFER16_SIZE);
	if(NextWrite == buf->read)
	{
     buf->write = buf->read;
		 buf->read = NextRead;   
	}      
	
	buf->buffer[buf->write] = value;
	buf->write = NextWrite;      
    
    if(buf->read >= ACTUAL_BUFFER16_SIZE)
    {
      buf->read = 0;
    }
	
	return 0;
}


int8_t Buffer16_Get(CircularBuffer16_s *buf, uint16_t *value)
{
	if(buf->read == buf->write)
	{
		return BUFFER16_ERROR_EMPTY;
	}

	*value = buf->buffer[buf->read];
	buf->read = (buf->read + 1) % ACTUAL_BUFFER16_SIZE;

	return 0;
}

int8_t Buffer16_GetRequested(CircularBuffer16_s *buf, uint8_t requested, uint16_t *value)
{
  uint8_t readIdx = 0;
  
  if(requested > buf->write)
  {
    readIdx = BUFFER16_SIZE - (buf->write) - requested;
  } else {
    readIdx = (buf->write) - requested;
  }
  
  *value = buf->buffer[readIdx];
  
  return 0;
}

int8_t Buffer16_Clear(CircularBuffer16_s *buf)
{
	uint16_t i = 0;

	buf->read = 0;
	buf->write = 0;

	for(i=0;i<ACTUAL_BUFFER16_SIZE;i++)
	{
		buf->buffer[i] = 0;
	}

	return 0;
}

int8_t Buffer16_IsFull(CircularBuffer16_s *buf)
{
	uint16_t NextWrite = (buf->write + 1) %(ACTUAL_BUFFER16_SIZE);

	if(NextWrite == buf->read)
	{
		return BUFFER16_IS_FULL;
	}
	return BUFFER16_NOT_FULL;
}

int8_t Buffer16_IsEmpty(CircularBuffer16_s *buf)
{
	if(buf->write == buf->read)
	{
		return BUFFER16_IS_EMPTY;
	}

	return BUFFER16_NOT_EMPTY;
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

