/** @file i2c.c
 *  @brief I2C driver for MSP430FR5969
 *
 *  @author Matt Casari, matthew.casari@noaa.org
 *  @date Oct 2, 2015
 *  @version A.0.1
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *	@note Tested on MSP430FR5969
 *
 *  @bug  No known bugs
 */
 /************************************************************************
*						STATIC FUNCTION PROTOTYPES
************************************************************************/

/************************************************************************
*						GLOBAL VARIABLES
************************************************************************/

/************************************************************************
*						GLOBAL FUNCTIONS
************************************************************************/
#include "statistics.h"


uint8_t STATS_CalculateMean(float *vals, uint16_t length, float *mean)
{
	float sum = 0.0;
	//float len = (float) length;
	float CalculatedMean = 0.0;
	uint16_t i = 0;
	
	if(length <1)
 	{
 		return STATISTICS_FAIL;
 	}
 	
	for(i=0;i<length;i++)
	{
		sum += vals[i];
		
	}
	CalculatedMean = (sum/length);
	*mean = CalculatedMean;

	return STATISTICS_OK;	
}


uint8_t STATS_FindMax(float *vals, uint16_t length, float *max)
{
 	float MaxVal = 0;
 	uint16_t i = 0;
 	
 	if(length <1)
 	{
 		return STATISTICS_FAIL;
 	}
 	
 	MaxVal = vals[0];
 	
 	for(i=1;i<length;i++)
 	{
 		if(vals[i]>MaxVal)
 		{
 			MaxVal = vals[i];
 		}
 	}
 	
 	*max = MaxVal;
 	
 	
 	return STATISTICS_OK;
}


uint8_t STATS_FindMin(float *vals, uint16_t length, float *min)
{
 	float MinVal = 0;
 	uint16_t i = 0;
 	
 	if(length <1)
 	{
 		return STATISTICS_FAIL;
 	}
 	
 	MinVal = vals[0];
 	
 	for(i=1;i<length;i++)
 	{
 		if(vals[i]<MinVal)
 		{
 			MinVal = vals[i];
 		}
 	}
 	
 	*min = MinVal;
 	
 	
 	return STATISTICS_OK;
}


uint8_t STATS_ComputeSTD(float *vals, uint16_t length, float mean, float *std)
{
	float VarianceSum = 0;
	float Diff = 0;
	uint16_t i;
	
	if(length <= 0)
	{
		return STATISTICS_FAIL;	
	}
	
	for(i=0;i<length;i++)
	{
		
		Diff = vals[i] - mean;
		Diff *= Diff;
		VarianceSum += Diff;
		
	}
	
	VarianceSum /= length;
	
	*std = sqrt(VarianceSum);
	
	return STATISTICS_OK;
}
/************************************************************************
*						STATIC FUNCTIONS
************************************************************************/