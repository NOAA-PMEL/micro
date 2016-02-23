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



#ifndef BUFFERS_H_
#define BUFFERS_H_
/************************************************************************
*							HEADER FILES
************************************************************************/
#include "../../inc/includes.h"
#ifndef PMEL
#include "../../test/msp430fr5969.h"
#else
#include <msp430fr5969.h>
#endif


//#include "buffer8.h"
//#include "buffer16.h"
//#include "buffer32.h"
//#include "buffer_c.h"
//#include "buffer_d.h"
//#include "buffer_f.h"

//#include "../inc/includes.h"

///************************************************************************
//*						STANDARD LIBRARIES
//************************************************************************/
#include <stdint.h>
/************************************************************************
*							MACROS
************************************************************************/
//#define BUFFER8_SIZE				(255)
//#define BUFFER16_SIZE				(255)
//#define BUFFER32_SIZE				(255)
////#define BUFFER_F_SIZE				(255)
//#define BUFFER_D_SIZE				(255)
////#define BUFFER_C_SIZE				(255)
//
//#define ACTUAL_BUFFER8_SIZE			(BUFFER8_SIZE + 1)
//#define ACTUAL_BUFFER16_SIZE		(BUFFER16_SIZE + 1)
//#define ACTUAL_BUFFER32_SIZE		(BUFFER32_SIZE + 1)
////#define ACTUAL_BUFFER_F_SIZE		(BUFFER_F_SIZE + 1)
//#define ACTUAL_BUFFER_D_SIZE		(BUFFER_D_SIZE + 1)
////#define ACTUAL_BUFFER_C_SIZE		(BUFFER_C_SIZE + 1)
//
//#define BUFFER_ERROR_EMPTY			(1)
//#define BUFFER_ERROR_FULL			(2)
//#define BUFFER_NOT_FULL				(0)
//#define BUFFER_NOT_EMPTY			(BUFFER_NOT_FULL)
//#define BUFFER_IS_EMPTY				(1)
//#define BUFFER_IS_FULL				(2)



#define BUFFER_F_SIZE				(64)
#define BUFFER_C_SIZE				(64)


#define ACTUAL_BUFFER_F_SIZE		(BUFFER_F_SIZE + 1)
#define ACTUAL_BUFFER_C_SIZE		(BUFFER_C_SIZE + 1)



#define BUFFER_F_ERROR_EMPTY			(1)
#define BUFFER_F_ERROR_FULL			(2)
#define BUFFER_F_NOT_FULL				(0)
#define BUFFER_F_NOT_EMPTY			(BUFFER_F_NOT_FULL)
#define BUFFER_F_IS_EMPTY				(1)
#define BUFFER_F_IS_FULL				(2)






#define BUFFER_C_ERROR_EMPTY			(1)
#define BUFFER_C_ERROR_FULL			(2)
#define BUFFER_C_NOT_FULL				(0)
#define BUFFER_C_NOT_EMPTY			(BUFFER_C_NOT_FULL)
#define BUFFER_C_IS_EMPTY				(1)
#define BUFFER_C_IS_FULL				(2)


typedef struct _CircularBufferF_s
{
	float buffer[BUFFER_F_SIZE];
	uint16_t size;
	uint16_t read;
	uint16_t write;

}CircularBufferF_s;


typedef struct _CircularBufferC_s
{
	char buffer[BUFFER_C_SIZE];
	uint16_t size;
	uint16_t read;
	uint16_t write;

}CircularBufferC_s;

/************************************************************************
*					GLOBAL FUNCTION PROTOTYPES
************************************************************************/

int8_t BufferF_Put(CircularBufferF_s *buf, float value);
int8_t BufferF_Get(CircularBufferF_s *buf, float *value);
int8_t BufferF_Clear(CircularBufferF_s *buf);
int8_t BufferF_IsFull(CircularBufferF_s *buf);
int8_t BufferF_IsEmpty(CircularBufferF_s *buf);
int8_t BufferF_Put_Circular(CircularBufferF_s *buf, float value);        

/** @brief Add char to buffer
 *
 *	Adds a single char to the buffer
 *
 *  @param *buf pointer to the char buffer
 *  @param value char value to add
 *
 *  @return status
 */
int8_t BufferC_Put(CircularBufferC_s *buf, char value);

/** @brief Get char from buffer
 *
 *	Retreives one char from the buffer
 *
 *  @param *bug pointer to the char buffer
 *  @param *value pointer to the char variable
 *
 *  @return status
 */
int8_t BufferC_Get(CircularBufferC_s *buf, char *value);

/** @brief Clear the buffer
 *
 *	Clear the char buffer
 *
 *  @param *buf pointer to the char buffer
 *
 *  @return status
 */
int8_t BufferC_Clear(CircularBufferC_s *buf);

/** @brief Tests for full buffer
 *
 *	Checks the buffer to see if it is full
 *
 *  @param *buf pointer to the char buffer
 *
 *  @return status
 */
int8_t BufferC_IsFull(CircularBufferC_s *buf);

/** @brief Test for empty buffer
 *
 *	Checks the buffer to see if it is empty
 *
 *  @param *buf pointer to the char buffer
 *
 *  @return status
 */
int8_t BufferC_IsEmpty(CircularBufferC_s *buf);

#endif

