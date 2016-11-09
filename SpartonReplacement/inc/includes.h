
/** @file includes.h
 *  @brief Drivers for MSP430FR5969 GPIO
 *
 *  @author Matt Casari, matthew.casari@noaa.org
 *  @date October 7, 2015
 *  @version 0.1.1
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *  @bug  No known bugs
 */

#ifndef _INCLUDES_H
#define _INCLUDES_H


//#define PMEL
/************************************************************************
*						STANDARD LIBRARIES
************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
#include <time.h>
#include <msp430fr5969.h>		/* Device Specific Header File */


/************************************************************************
*							HEADER FILES
************************************************************************/
#include "main.h"
#include "gpio.h"
#include "timer.h"
#include "UART.h"
#include "buffer_c.h"
#include "DMA.h"




#endif
