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
	//TA0CTL = TASSEL__ACLK | MC__CONTINUOUS | TACLR | TAIE; // ACLK, contmode, clear TAR
	
	// Enable Timer A on SMCLK
	//TA0CCTL0 = CCIE;                          // TACCR0 interrupt enabled
	//TA0CCR0 = TIMERA_DELAY_COUNT;
	TA0CTL = TASSEL__ACLK | MC__CONTINUOUS | TACLR | TAIE; // ACLK, contmode, clear TAR

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



/************************************************************************
*					INTERRUPT VECTOR
************************************************************************/
#ifdef PMEL


#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void)
{
  switch(__even_in_range(TA0IV, TA0IV_TAIFG))
  {
    case TA0IV_NONE:   break;               // No interrupt
    case TA0IV_TACCR1: break;               // CCR1 not used
    case TA0IV_TACCR2: break;               // CCR2 not used
    case TA0IV_3:      break;               // reserved
    case TA0IV_4:      break;               // reserved
    case TA0IV_5:      break;               // reserved
    case TA0IV_6:      break;               // reserved
    case TA0IV_TAIFG:                       // overflow
      FET_TOGGLE();
      break;
    default: break;
  }
}
#endif