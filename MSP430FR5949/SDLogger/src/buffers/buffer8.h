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



#ifndef BUFFER8_H_
#define BUFFER8_H_
/************************************************************************
*							HEADER FILES
************************************************************************/
#include <stdint.h>

#include "../../inc/includes.h"
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

#define BUFFER8_SIZE				(255)
#define ACTUAL_BUFFER8_SIZE		(BUFFER8_SIZE + 1)


#define BUFFER8_ERROR_EMPTY			(1)
#define BUFFER8_ERROR_FULL			(2)
#define BUFFER8_NOT_FULL				(0)
#define BUFFER8_NOT_EMPTY			(BUFFER8_NOT_FULL)
#define BUFFER8_IS_EMPTY				(1)
#define BUFFER8_IS_FULL				(2)


typedef struct _CircularBuffer8_s
{
	char buffer[BUFFER8_SIZE];
	uint16_t size;
	uint16_t read;
	uint16_t write;

}CircularBuffer8_s;


/************************************************************************
*					GLOBAL FUNCTION PROTOTYPES
************************************************************************/
/** @brief Add char to buffer
 *
 *	Adds a single char to the buffer
 *
 *  @param *buf pointer to the char buffer
 *  @param value char value to add
 *
 *  @return status
 */
int8_t Buffer8_Put(CircularBuffer8_s *buf, uint8_t value);

/** @brief Get char from buffer
 *
 *	Retreives one char from the buffer
 *
 *  @param *bug pointer to the char buffer
 *  @param *value pointer to the char variable
 *
 *  @return status
 */
int8_t Buffer8_Get(CircularBuffer8_s *buf, uint8_t *value);

/** @brief Clear the buffer
 *
 *	Clear the char buffer
 *
 *  @param *buf pointer to the char buffer
 *
 *  @return status
 */
int8_t Buffer8_Clear(CircularBuffer8_s *buf);

/** @brief Tests for full buffer
 *
 *	Checks the buffer to see if it is full
 *
 *  @param *buf pointer to the char buffer
 *
 *  @return status
 */
int8_t Buffer8_IsFull(CircularBuffer8_s *buf);

/** @brief Test for empty buffer
 *
 *	Checks the buffer to see if it is empty
 *
 *  @param *buf pointer to the char buffer
 *
 *  @return status
 */
int8_t Buffer8_IsEmpty(CircularBuffer8_s *buf);


#endif

