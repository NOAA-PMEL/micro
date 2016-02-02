/** @file BUFFERS.c
 *  @brief
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date Dec 4, 2015
 *  @version 0.0.1
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *	@note
 *
 *  @bug  No known bugs
 */

/************************************************************************
*						INCLUDES
************************************************************************/
#include "lowlevel.h"

/************************************************************************
*					GLOBAL FUNCTIONS
************************************************************************/
void FRAM_Init(void)
{
	MPUCTL0 = MPUPW;
	MPUSEGB1 = 0x05A6;
	MPUSEGB2 = 0x05A4;

	MPUSAM = (MPUSEG1WE | MPUSEG1RE | MPUSEG2RE | MPUSEG3RE | MPUSEG3XE);
	MPUCTL0 = MPUPW | MPUENA | MPUSEGIE;
}


void WD_Init(void )
{
	// Use the ACLK with a Divisor of 5
	WDTCTL = WDTPW | WDTHOLD;
	WDTCTL = WDTPW | WDTSSEL__ACLK | WDTIS_5;
	return;
}

void WD_Halt(void)
{
	WDTCTL |= WDTPW | WDTHOLD;		// halt watchdog
}

void WD_Hit(void)
{

	WDTCTL = WDTPW | WDTSSEL__ACLK | WDTIS_5 | WDTCNTCL;
	return;
}

