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



#ifndef _BUFFER_C_H
#define _BUFFER_C_H
/************************************************************************
*							HEADER FILES
************************************************************************/
#ifndef PMEL
#include "../test/msp430fr5969.h"
#else
#include <msp430fr5969.h>
#endif

#include "../inc/includes.h"
#include "buffers.h"
/************************************************************************
*						STANDARD LIBRARIES
************************************************************************/
#include <stdint.h>
/************************************************************************
*							MACROS
************************************************************************/




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

