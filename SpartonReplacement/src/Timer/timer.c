/** @file timer.c
 *  @brief 	Timer configurations for MSP430FR5949
 *
 *  @author Matt Casari, matthew.casari@noaa.org
 *  @date March 23, 2016
 *  @version 0.1.2
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *
 *  @bug  No known bugs
 */

#include "timer.h"
//TimerMS_t UARTTimer;
//TimerMS_t SystemTimer;
//TimerMS_t *pUARTTimer = &UARTTimer;

__persistent volatile TimerMS_t OSTimer;
__persistent volatile TimerMS_t *pOSTimer = &OSTimer;
__persistent volatile TimerMS_t FLTimer;
__persistent volatile TimerMS_t *pFLTimer = &FLTimer;
/************************************************************************
*				GLOBAL FUNCTIONS
************************************************************************/

void TIMER_A0_Init ( void )
{
  /* Set up for 1 second using 32768 Crystal */
  TA0CCR0 = 0x7FFF;
  TA0CTL = TASSEL__ACLK | MC__UP | TACLR | TAIE; // ACLK, contmode, clear TAR

	return;
}

/************************************************************************/
void TIMER_A1_Init ( void )
{

  /* Set up for 1mS using 32768Hz Crystal */
  TA1CCR0 =32;
  TA1CTL = TASSEL__ACLK | MC__UP;
  TA1CCTL0 = CCIE;
  
  /* Set up for 1mS using 8MHz Clock Signal */
//  TA1CCR0 = 8000;
//  TA1CTL = TASSEL__SMCLK | MC__UP;
//  TA1CCTL0 = CCIE;

  return;
}

/************************************************************************/
void TIMER_B_MilliSecond( uint16_t length )
{
  
}



/************************************************************************
*					INTERRUPT VECTOR
************************************************************************/




#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
//  GPIO_TogglePin(1,0);
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
      
      break;
    default: break;
  }
  
}


#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
  if(FLTimer.Timeout > 0){
    FLTimer.Timeout--;
    if(FLTimer.Timeout == 0){
      FLTimer.TimeoutFlag = true;
    }
  }
  
//  if(FLTimer.ConfigTimeout > 0){
//    FLTimer.ConfigTimeout--;
//    if(FLTimer.ConfigTimeout == 0){
//      FLTimer.ConfigTimeoutFlag = true;
//    }
//  }
  
  if(OSTimer.Timeout > 0){
    OSTimer.Timeout--;
    if(OSTimer.Timeout == 0){
      OSTimer.TimeoutFlag = true;
    }
  }
  
  
  return;
}



