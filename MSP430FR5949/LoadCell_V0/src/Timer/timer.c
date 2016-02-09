/** @file timer.c
 *  @brief 	Timer configurations for MSP430FR5969
 *
 *  @author Matt Casari, matthew.casari@noaa.org
 *  @date October 7, 2015
 *  @version 0.1.1
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *
 *  @bug  No known bugs
 */

#include "timer.h"


/************************************************************************
*				STATIC FUNCTION PROTOTYPES
************************************************************************/

/************************************************************************
*				STATIC VARIABLES
************************************************************************/

/************************************************************************
*				GLOBAL FUNCTIONS
************************************************************************/

void TimerAInit ( void )
{

	// Enable Timer A on SMCLK
	TA0CCTL0 = CCIE;                          // TACCR0 interrupt enabled
	TA0CCR0 = TIMERA_DELAY_COUNT;
	TA0CTL = TASSEL__SMCLK | MC__UP |ID_3;          // SMCLK, UP mode

	return;
}

/************************************************************************/
void TimerBInit ( void )
{

	// Enable Timer B on SMCLK
	TB0CCTL0 = CCIE;
	TB0CCR0 = TIMERB_DELAY_COUNT;
	TB0CTL = TASSEL__SMCLK | MC__UP | ID_3;
	TB0CCTL0 &= ~CCIE;

	return;
}

/************************************************************************/
void TimerBms( uint16_t length )
{

	for(uint16_t i=0; i<length;i++)
	{
		TimerFlags.TimerB = true;
		ResetTimerB();
		SetTimerBInterrupt();
		while(TimerFlags.TimerB == true );
	}
}
