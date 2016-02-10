
/** @file main.c
 *
 *  @brief Main Function
 *
 *  @author Matt Casari, matthew.casari@noaa.org
 *  @date November 2, 2015
 *  @version 0.1.2
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *	@mainpage
 *
 *
 *  This Load Cell program polls a single Keller-Druck PAxLD sensor,
 *	converts the pressure to load and averages the samples until the user requests the data.  At that time the
 *  the average, max, min and standard deviation are computed.  
 *	
 *	A console program allows the user to configure the Load Cell
 * 
 *	@todo watchdog is not setup
 *	@todo Add console
 *	@todo sprintf is taking too long to be useful
 *	@todo remove __delay_cycles() and replace with timer call
 */


/*------------------- Connection Diagram------------------------------*/
/*
		       						 						3V3
		                                |
		    MSP430FR5949             -------
		 --------------------        |     |
		 |                  |       4.7k  4.7k
		 |                  |        |     |
		 |              P1.6|--------------|------- To PAxLD SCL
		 |              P1.7|---------------------- To PAxLD SDA
		 |              P3.7|---------------------- To PAxLD EOC
		 |                  |
		 |              P3.6|---------------------- To Control FET
		 |                  |
		 |              P2.5|---------------------- UART Tx
 		 |              P2.6|---------------------- UART Rx
		 |                  | 		  
		 |              PJ.4|----------------------   LFXIn
		 |                  |													32.768kHz Crystal
	   |              PJ.5|---------------------- 	LFXOut
		 |                  |
		 --------------------

 */

#define PMEL            // For compiling outside of Unity/Ceedling

/*****************************  Includes  *********************************/
#include "./inc/includes.h"

/************************ Function Prototypes *****************************/

/*************************  Global variables  *****************************/




/*******************************  MAIN  **********************************/
int main(void) {
  char sendChar = 'A';
  
  // Pause the watchdog
  WDTCTL = WDTPW | WDTHOLD;		
	__low_level_init();				// Setup FRAM
	
  // Set All GPIO settings to 0
  GPIO_Init();        // Sets all Outputs Low and regs to 0

  // Configure the FET driving power to the Keller Sensor
  FET_OFF();
  FET_INIT();

    // Configure GPIO
  P2SEL1 |= BIT5 | BIT6;                    // USCI_A0 UART operation
  P2SEL0 &= ~(BIT5 | BIT6);

  // LFXIN
  PJSEL1 &= ~BIT4;
  PJSEL0 |= BIT4;
  
  // Unlock GPIO
  PM5CTL0 &= ~LOCKLPM5;		// Needs to be done after config GPIO & Pins!

	// Configure the clock 
  CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
  CSCTL1 = 0;                       // Set DCO to 8MHz
  CSCTL2 = SELA__LFXTCLK | SELS__LFXTCLK | SELM__LFXTCLK;  // Set SMCLK = MCLK = SELM = LFXTCLK
  CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // set all dividers
  CSCTL4 =  VLOOFF | LFXTDRIVE_0;
  CSCTL4 &= ~LFXTOFF;
  
  // Wait for the clock to lock
  do
  {
    CSCTL5 &= ~LFXTOFFG;
    SFRIFG1 &= ~OFIFG;
  }while(SFRIFG1 & OFIFG);
  
  CSCTL0_H = 0;                             // Lock CS registers
  
    // Configure the UART
  UART_Init(UART_A1,UART_BAUD_9600,CLK_32768,UART_CLK_SMCLK);

	
  // Turn the Keller ON
  FET_ON();
  
  // Initialize the timer
  TimerAInit();
  
  
  // Set interrupts
  __bis_SR_register(GIE);
  __no_operation();                         // For debugger



  __delay_cycles(5000);



  // Main loop
  for(;;)
  {
    //UART_WriteChar(sendChar,UART_A1);
    UCA1TXBUF = sendChar;
    
    __delay_cycles(5000);
      
  }
}




