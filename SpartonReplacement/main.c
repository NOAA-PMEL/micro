/** @file main.c
 *
 *  @brief Converts T-FLEX commands for Sparton SP-300x Sensor to OceanServer OS5000S
 *
 *  @author Matt Casari, matthew.casari@noaa.org
 *  @date Nov 8, 2016
 *  @version 0.0.1
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *	@mainpage
 * 
 *  
 *  T-FLEX RS-232 Info:
 *  Baud Rate - 9600
 *  Bits - 8
 *  Stop Bits - 1
 *  Parity - None 
 *
 *  Ocean Server RS-232 Info:
 *  Baud Rate - 9600
 *  Bits - 8
 *  Stop Bits - 1
 *  Parity - None
 *
 *
 *  @note The OceanServer 5000-S Compass can be calibrated without removing
 *        from the PCB.  Use the Ocean Server Digital Compass Demo Utility to
          calibrate the compass.  Note: When complete, you must cycle power 
          before the FLEX commands will work again.
 *  @note To add FRAM capability, you must enable MPU in IAR 
 *        (Project->Options->General Options->MPU/IPU->Support MPU)
 * 
 */


/*------------------- Connection Diagram------------------------------*/
/*
		       						 					
		                               
                             MSP430FR5949           
                           --------------------        
                           |                  |       
                           |                  |        
                           |                  |
(J1.3)T-FLEX UART Rx ------|P2.5/UCA1TXD      |
(J1.2)T-FLEX UART Tx ------|P2.6/UCA1RXD      |
                           |                  |
                           |                  | 	
                           |      P2.0/UCA0TXD|------ OS5000S UART Rx (J2.7,BE)
                           |      P2.1/UCA0RXD|------ OS5000S UART Tx (J2.6,GN)
                           |                  | 		  
                           |                  | 	
                           |                  | 	
                           |              PJ.4|----------------LFXIn
                           |                  |			32.768kHz Crystal
                           |              PJ.5|----------------LFXOut
                           |                  |
                           --------------------

*/
/*****************************  Includes  *********************************/
#include "includes.h"

/***********************  Persistents (In FRAM)  *****************************/
__persistent uint8_t version[] = VERSION;
__persistent uint8_t serialNumber[16] = "";


int main( void )
{
  /* Configure MPU */
  __low_level_init();				
  WDTCTL = WDTPW | WDTHOLD;	
  
  /* Setup the GPIO */
  SETUP_GPIO();
  
  /* Setup the Clock */
  SETUP_Clock();
  
  /* Setup Timer */
  TIMER_A1_Init();  
  
  /* Initialize the OS5000S Structure */
  OS5000S_Init();
  
  /* Initialize the FLEX Structure */
  FLEX_Init();
  
  /* Setup the Peripherals */
  SETUP_MicroPeripherals();
  OS5000S_Attach_Rx_Interrupt();
  
  /* Enable the interrupts */
  __bis_SR_register(GIE);

  FLEX.SysMode = FL_NORMAL;
  FLEX.UART->Timer->TimeoutFlag = false;
  FLEX.UART->Timer->Timeout = 50;
  

  for(;;) {

    
    if(FLEX.SysMode == FL_CONFIGURE) {
       SYS_ConfigurationRoutine();
      
    } else {
      /* Parse the Ocean Server Buffer */
      OS5000S_ParseBuffer();
      /* Parse the FLEX commands on timeout */
      if( (FLEX.UART->Timer->TimeoutFlag == true) && OS5000S.MSGValid == true) {
        FLEX.UART->Timer->TimeoutFlag = false;
        FLEX.UART->Timer->Timeout = 10;
        FLEX_ParseBuffer();
      }
    }
  }

}


/** @brief OS Configuration Routine 
 *
 *
 *
 * @param
 *
 * @ret None 
 */
void SYS_ConfigurationRoutine(void) {
  char val = 0;
  while(BufferC_Get(&FLEX.UART->Buffer,&val)!= BUFFER_C_IS_EMPTY){
      OS_putc(val);
    }  
}
 








/** @brief Configure the Micro Peripherals
 * 
 * Configure the SD Card (SPI), and UART 
 * 
 * @param None
 * 
 * @return None
 */
void SETUP_MicroPeripherals(void) {
    
  /* Configure the T-FLEX UART */
  TFLEX_UART_Init();

  /* Configure the OceanServer OS5000-S */
  OS5000S_UART_Init();
  


#ifdef DEBUG
  /* Clear the terminal */

#endif
  
  return;
}


/** @brief Configure GPIO
 *
 *  Initialize the GPIO and set an interrupt on P3.0
 *  Set selection bits for UART
 *  Set selection bits for 32kHz crystal
 * 
 *  @param None
 *
 *  @return Void
 */
void SETUP_GPIO(void)
{
  /* Set All GPIO settings to 0 */
  GPIO_Init();        
      
  /* Configure Selection bits for UART */
  P2SEL1 |= BIT5 | BIT6;                    /* USCI_A0 UART operation */
  P2SEL0 &= ~(BIT5 | BIT6);

  /* LFXIN */
  PJSEL1 &= ~BIT4;
  PJSEL0 |= BIT4;
  
    /* Setup RTC Output Pin */
  GPIO_ClearPin(1,0);
  GPIO_SetPinAsOutput(1,0);
  P1SEL1 |= BIT0;
  P1SEL0 &= ~BIT0;
  
  /* Set Switched Power Pin */
  P3DIR |= 0x01;
  
  /* Unlock GPIO */
  PM5CTL0 &= ~LOCKLPM5;		/* Needs to be done after config GPIO & Pins! */
  
  __delay_cycles(100);
  P3OUT |= 0x01;
  

  
#ifdef DEBUG
  GPIO_SetPinAsOutput(1,0);
#endif
  
   /* Set interrupts */
  P3IFG &= ~BIT0;

  return;
}


/** @brief Set the system clock
 *
 *  Set the system clock for DCO @ 1MHz, 
 *  SMCLK @500MHSz
 *  MCLK @ 1MHz
 *  ACLK @32.768kHz
 *  LFXT driver on Low Power
 *
 *  @param None
 *
 *  @return Void
 */
void SETUP_Clock(void)
{
  /* Configure the clock                */
  /* DCO running at 8MHz                */
  /* ACLK running on LFXT 32768Hz       */
  /* SMCLK running on DCOCLK, 4MHz    */
  /* MCLK running on DCOCLK, 1MHz       */
  /* LFXT Driver on low power           */
  CSCTL0_H = CSKEY >> 8;		/* Unlock registers */
  CSCTL1 = DCOFSEL_6;			/* Set DCO to 8Mhz */
  CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
  CSCTL3 = DIVA__1 | DIVS__2 | DIVM__1;	
  CSCTL4 =   LFXTDRIVE_0 | VLOOFF;
  CSCTL4 &= ~LFXTOFF;
  
  /* Wait for the clock to lock   */
  do
  {
    CSCTL5 &= ~LFXTOFFG;
    SFRIFG1 &= ~OFIFG;
  }while(SFRIFG1 & OFIFG);
  
  /* Lock CS Register */
  CSCTL0_H = 0;							
  
}
