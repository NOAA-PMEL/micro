/** @file buffers.h
 *  @brief Combination of float and char buffers for load cell project
 *
 *  @author Matt Casari, matthew.casari@noaa.org
 *  @date March 28, 2016
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

/************************************************************************
*						STANDARD LIBRARIES
************************************************************************/
#include <stdint.h>
/************************************************************************
*							MACROS
************************************************************************/
#define BUFFER_F_SIZE				(120)
#define BUFFER_C_SIZE				(32)


#define ACTUAL_BUFFER_F_SIZE		(BUFFER_F_SIZE + 1)
#define ACTUAL_BUFFER_C_SIZE		(BUFFER_C_SIZE + 1)

#define BUFFER_ERROR_EMPTY	(1)
#define BUFFER_ERROR_FULL		(2)
#define BUFFER_NOT_FULL			(0)
#define BUFFER_NOT_EMPTY		(BUFFER_NOT_FULL)
#define BUFFER_IS_EMPTY			(1)
#define BUFFER_IS_FULL			(2)

#define BUFFER_C_NEWLINE_DETECTED	(3)
#define BUFFER_C_NEWLINE_NOT_DETECTED	(1)

 /** @brief float circular buffer
 * 
 * This is the circular buffer type that stores float values
 *
 */
typedef struct _CircularBufferF_s
{
	float buffer[BUFFER_F_SIZE];
	uint16_t size;
	uint16_t read;
	uint16_t write;

}CircularBufferF_s;

/** @brief Char circular buffer
 * 
 * This is the circular buffer type that stores characters
 *
 */
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

/** @brief Add float value to buffer
 *
 *	Adds a single float value to the buffer
 *
 *  @param *buf pointer to the float buffer
 *  @param value float value to add
 *
 *  @return status
 */
int8_t BufferF_Put(CircularBufferF_s *buf, float value);

/** @brief Get float value from buffer
 *
 *	Retreives one float value from the buffer
 *
 *  @param *bug pointer to the float buffer
 *  @param *value pointer to the float variable
 *
 *  @return status
 */
int8_t BufferF_Get(CircularBufferF_s *buf, float *value);

/** @brief Clear the buffer
 *
 *	Clear the char buffer
 *
 *  @param *buf pointer to the float buffer
 *
 *  @return status
 */
int8_t BufferF_Clear(CircularBufferF_s *buf);

/** @brief Tests for full buffer
 *
 *	Checks the buffer to see if it is full
 *
 *  @param *buf pointer to the float buffer
 *
 *  @return status
 */
int8_t BufferF_IsFull(CircularBufferF_s *buf);

/** @brief Test for empty buffer
 *
 *	Checks the buffer to see if it is empty
 *
 *  @param *buf pointer to the float buffer
 *
 *  @return status
 */
int8_t BufferF_IsEmpty(CircularBufferF_s *buf);

/** @brief Overwrites buffer when full
 *
 *	Continues to write to buffer even when full, 
 *  updating the read and write pointers
 *
 *  @param *buf pointer to the float buffer
 *
 *  @return status
 */
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

/** @brief Look in Buffer for newline
 *
 *	Checks the buffer to see if a newline has been entered
 *
 *  @param *buf pointer to the char buffer
 *
 *  @return status
 */
int8_t BufferC_CheckForNewline(CircularBufferC_s *buf);


#endif

