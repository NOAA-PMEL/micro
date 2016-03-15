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



#ifndef BUFFER32_H_
#define BUFFER32_H_
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

#define BUFFER32_SIZE				(64)
#define ACTUAL_BUFFER32_SIZE		(BUFFER32_SIZE + 1)


#define BUFFER32_ERROR_EMPTY			(1)
#define BUFFER32_ERROR_FULL			(2)
#define BUFFER32_NOT_FULL				(0)
#define BUFFER32_NOT_EMPTY			(BUFFER32_NOT_FULL)
#define BUFFER32_IS_EMPTY				(1)
#define BUFFER32_IS_FULL				(2)


typedef struct _CircularBuffer32_s
{
	uint32_t buffer[BUFFER32_SIZE];
	uint16_t size;
	uint16_t read;
	uint16_t write;

}CircularBuffer32_s;


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
int8_t Buffer32_Put(CircularBuffer32_s *buf, uint32_t value);

/** @brief Get char from buffer
 *
 *	Retreives one char from the buffer
 *
 *  @param *bug pointer to the char buffer
 *  @param *value pointer to the char variable
 *
 *  @return status
 */
int8_t Buffer32_Get(CircularBuffer32_s *buf, uint32_t *value);

/** @brief Clear the buffer
 *
 *	Clear the char buffer
 *
 *  @param *buf pointer to the char buffer
 *
 *  @return status
 */
int8_t Buffer32_Clear(CircularBuffer32_s *buf);

/** @brief Tests for full buffer
 *
 *	Checks the buffer to see if it is full
 *
 *  @param *buf pointer to the char buffer
 *
 *  @return status
 */
int8_t Buffer32_IsFull(CircularBuffer32_s *buf);

/** @brief Test for empty buffer
 *
 *	Checks the buffer to see if it is empty
 *
 *  @param *buf pointer to the char buffer
 *
 *  @return status
 */
int8_t Buffer32_IsEmpty(CircularBuffer32_s *buf);


#endif

