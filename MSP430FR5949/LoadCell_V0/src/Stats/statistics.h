/** @file statistics.h
 *  @brief Statistic functions
 *  
 *  @author Matt Casari, matthew.casari@noaa.org
 *  @date March 28, 2016
 *  @version 1.0.0
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab 
 *  @copyright Environmental Development Division
 *  
 *	@note 
 *	@note
 *
 *
 *  @bug  No known bugs
 */
#ifndef statistics_H
#define statistics_H

/************************************************************************
*							HEADER FILES
************************************************************************/


/************************************************************************
*							CONSTANTS
************************************************************************/
#define STATISTICS_OK		(0)
#define STATISTICS_FAIL	(1)

/************************************************************************
*						STANDARD LIBRARIES		
************************************************************************/
#include <stdint.h>
#include <math.h>


#include "../../inc/includes.h"

#ifdef PMEL
#include <msp430fr5949.h>
#else
#include "../../test/msp430fr5969.h"
#endif


/************************************************************************
*					GLOBAL FUNCTION PROTOTYPES
************************************************************************/
/** @brief Calculates Mean of float array.
 *
 *	Calculates the mean of an array of floats
 *
 *
 *  @param *vals Array of floats
 *  @param length Length of float Array
 *  @param *mean Calculated mean value
 *
 *  @return Error code
 */
 uint8_t STATS_CalculateMean(float *vals, uint16_t length, float *mean);
 
 
 /** @brief Find Max of Float array
 *
 *	Finds the maximum value of an array of floats
 *
 *
 *  @param *vals Array of floats
 *  @param length Length of float Array
 *  @param *max Maximum value register
 *
 *  @return Error code
 */
 uint8_t STATS_FindMax(float *vals, uint16_t length, float *max);
 
  /** @brief Find Min of Float array
 *
 *	Finds the minimum value of an array of floats
 *
 *
 *  @param *vals Array of floats
 *  @param length Length of float Array
 *  @param *min Minimum value register
 *
 *  @return Error code
 */
uint8_t STATS_FindMin(float *vals, uint16_t length, float *min);
 
/** @brief Compute Standard Deviation
*
*	 Computes the standard deviation of the array provided
*
*
*  @param *vals Array of floats
*  @param length Length of float Array
*  @param mean Mean of 
*  @param *std Standard Deviation register
*
*  @return Error code
*/
uint8_t STATS_ComputeSTD(float *vals, uint16_t length, float mean, float *std);


#endif // statistics_H
