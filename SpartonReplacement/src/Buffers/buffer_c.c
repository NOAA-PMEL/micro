/** @file buffer_c.c
 *  @brief Character circular buffer library
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date March 23, 2016
 *  @version 0.1.0
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

   
static void BufferC_Size(CircularBufferC_s *buf);
//static uint8_t BufferC_Size_Calc(uint8_t read, uint8_t write);
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
    
    BufferC_Size(buf);

	return 0;
}

int8_t BufferC_Get(CircularBufferC_s *buf, char *value)
{
	if(buf->read == buf->write)
	{
      buf->size = 0;
		return BUFFER_C_ERROR_EMPTY;
	}

	*value = buf->buffer[buf->read];
	buf->read = (buf->read + 1) % ACTUAL_BUFFER_C_SIZE;

    BufferC_Size(buf);
    
	return 0;
}

int8_t BufferC_Backspace(CircularBufferC_s *buf)
{
  uint16_t PrevWrite = 0;

  if(buf->read == buf->write)
  {
    return BUFFER_C_ERROR_EMPTY;
  }
  
  if(buf->write == 0) {
    PrevWrite = BUFFER_C_SIZE -1;
  } else {
    PrevWrite = buf->write - 1;
  }
  
  buf->write=PrevWrite;
  
  BufferC_Size(buf);
  return 0;
}

int8_t BufferC_HasNewline(CircularBufferC_s *buf)
{
  uint8_t read = 0;
  
  if(buf->read == buf->write)
  {
      return BUFFER_C_ERROR_EMPTY;
  }

  read = buf->read;
  while(read != buf->write){
    if(buf->buffer[read] == '\n' || buf->buffer[read] == '\r' || buf->buffer[read] == 0x18) {
      return BUFFER_C_HAS_NEWLINE;
    }
    read = (read + 1) % ACTUAL_BUFFER_C_SIZE;
  }

	return 0;
}
    
int8_t  BufferC_HasChar(CircularBufferC_s *buf, char val) {

  uint8_t read = 0;
  
  if(buf->read == buf->write)
  {
      return BUFFER_C_ERROR_EMPTY;
  }

  read = buf->read;
  while(read != buf->write){
    if(buf->buffer[read] == val) {
      return BUFFER_C_HAS_CHAR;
    }
    read = (read + 1) % ACTUAL_BUFFER_C_SIZE;
  }

	return 0;
}

int8_t BufferC_HasSequence(CircularBufferC_s *buf, const char *seq,uint8_t len) {
  uint8_t read = 0;     /* Read index */
//  uint8_t subIdx = 0;   /* Read substring index */
  uint8_t sIdx = 0;     /* sequence index */
  uint8_t validFlag = false; /* Valid sequence flag */
//  uint8_t length = len;
  if(buf->read == buf->write) {
    return BUFFER_C_ERROR_EMPTY;
  }
  
  /* Populate the tempoary read index */
  read = buf->read;
 
  while(read != buf->write) {
    /* If we see the first sequence value, check for the rest */
    if(buf->buffer[read] == seq[sIdx]) {
      /* Copy buffer string to temp */
      char temp[8];
      memset(&temp[0],0,8);
      for(uint8_t i=0;i<len;i++) {
        temp[i] = buf->buffer[read];
        read = (read + 1) % ACTUAL_BUFFER_C_SIZE;
        if(read == buf->write){
          break;
        }
        
      }
      
      if(memcmp(temp,seq,len)==0) {
        validFlag = true;
      }
      
    } else {
      /* Increment the temporary read index and rollover if necessary */
      read = (read + 1) % ACTUAL_BUFFER_C_SIZE;
    }
  }
  
  if(validFlag == true) {
    return BUFFER_C_HAS_SEQ;
  } else {
    return BUFFER_C_NO_SEQ;
  }
}

int8_t BufferC_Clear(CircularBufferC_s *buf)
{
	uint16_t i = 0;

	buf->read = 0;
	buf->write = 0;
    buf->size = 0;
    
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

static void BufferC_Size(CircularBufferC_s *buf){
  if(buf->write == buf->read){
    buf->size = 0;
  } else if(buf->write > buf->read){
    buf->size = (buf->write - buf->read);
  } else {
    buf->size = BUFFER_C_SIZE - buf->read + buf->write + 1;
  }
}

//static uint8_t BufferC_Size_Calc(uint8_t read, uint8_t write) {
//  uint8_t size;
//  if(write == read) {
//    size = 0;
//  } else if (write > read) {
//    size = write - read;
//  } else {
//    size = BUFFER_C_SIZE - read + write + 1;
//  }
//  
//  return size;
//}
