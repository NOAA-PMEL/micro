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
#ifndef PMEL
#include "../test/msp430fr5969.h"
#else
#include <msp430fr5969.h>
#endif

//#include "../inc/includes.h"

/************************************************************************
*						STANDARD LIBRARIES
************************************************************************/
#include <stdint.h>
/************************************************************************
*							MACROS
************************************************************************/
#define BUFFER8_SIZE				(255)
#define BUFFER16_SIZE				(255)
#define BUFFER32_SIZE				(255)
#define BUFFER_F_SIZE				(255)
#define BUFFER_D_SIZE				(255)
#define BUFFER_C_SIZE				(255)

#define ACTUAL_BUFFER8_SIZE			(BUFFER8_SIZE + 1)
#define ACTUAL_BUFFER16_SIZE		(BUFFER16_SIZE + 1)
#define ACTUAL_BUFFER32_SIZE		(BUFFER32_SIZE + 1)
#define ACTUAL_BUFFER_F_SIZE		(BUFFER_F_SIZE + 1)
#define ACTUAL_BUFFER_D_SIZE		(BUFFER_D_SIZE + 1)
#define ACTUAL_BUFFER_C_SIZE		(BUFFER_C_SIZE + 1)

#define BUFFER_ERROR_EMPTY			(1)
#define BUFFER_ERROR_FULL			(2)
#define BUFFER_NOT_FULL				(0)
#define BUFFER_NOT_EMPTY			(BUFFER_NOT_FULL)
#define BUFFER_IS_EMPTY				(1)
#define BUFFER_IS_FULL				(2)









#endif

