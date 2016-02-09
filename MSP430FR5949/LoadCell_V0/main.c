
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
 *	This program polls up to seven (7) Keller-Druck PAxLD sensors and
 *	sends the values back to over the UART output
 *      Major updates include:
 *          1) Faster reads via EOC interrupts
 *          2) Faster write over UART
 *          3) Removal of while loops within functions
 *          4) Addition of FET control to sensor Power
 *
 *	\n\n Return message is:
 *	\n    $Sx,YYY.YY,ZZZ.Z,DDDDDD\\r\\n
 *	\n\n Where:
 *		\t x is the sensor number\n
 *		YYY.YY is the Pressure (in bar)\n
 *		ZZZ.Z is the temperature (in deg C)\n
 *		DDDDDD is the native pressure counts\n
 *

 *	@todo watchdog is not setup
 *	@todo Not optimized for low-power by sleeping in wait states
 *	@todo sprintf is taking too long to be useful
 *	@todo remove __delay_cycles() and replace with timer call
 */


/*------------------- Connection Diagram------------------------------*/
/*
		       						 3V3
		                                |
		    MSP430FR5969             -------
		 --------------------        |     |
		 |                  |       4.7k  4.7k
		 |                  |        |     |
		 |              P1.6|--------------|------- To PAxLD SCL
		 |              P1.7|---------------------- To PAxLD SDA
		 |              P1.5|---------------------- To PAxLD EOC
		 |                  |
                 |              P3.0|---------------------- To Control FET
		 |                  |
		 |              P2.5|---------------------- UART Tx
		 |                  |
		 --------------------

 */

#define PMEL            // For compiling outside of Unity/Ceedling
/************************************************************************
 *						STANDARD LIBRARIES
 ************************************************************************/
#include <stdint.h>
#include <msp430fr5949.h>
/*****************************  Includes  *********************************/
#include "./inc/includes.h"

/************************ Function Prototypes *****************************/

/*************************  Global variables  *****************************/




/*******************************  MAIN  **********************************/
int main(void) {
  char sendChar = 'A';
  
  // Pause the watchdog
  WDTCTL = WDTPW | WDTHOLD;		

  // Set All GPIO settings to 0
  GPIO_Init();        // Sets all Outputs Low and regs to 0

  // Configure the FET driving power to the Keller Sensor
  FET_OFF();
  FET_INIT();

    // Configure GPIO
  P2SEL1 |= BIT5 | BIT6;                    // USCI_A0 UART operation
  P2SEL0 &= ~(BIT5 | BIT6);

  // Unlock GPIO
  PM5CTL0 &= ~LOCKLPM5;		// Needs to be done after config GPIO & Pins!

  // Start the Clock
//  CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
//  CSCTL1 = DCOFSEL_6;                       // Set DCO to 8MHz
//  CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;  // Set SMCLK = MCLK = DCO
//                                          // ACLK = VLOCLK
//  CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // set all dividers
//  CSCTL0_H = 0;                             // Lock CS registers

  
  CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
  CSCTL1 = 0;                       // Set DCO to 8MHz
  CSCTL2 = SELA__LFXTCLK | SELS__LFXTCLK | SELM__LFXTCLK;  // Set SMCLK = MCLK = DCO
                                          // ACLK = VLOCLK
  CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // set all dividers
  CSCTL4 = LFXTOFF | VLOOFF | LFXTDRIVE_3;
  CSCTL0_H = 0;                             // Lock CS registers
  
    // Configure the UART
  //UART_Init(UART_A1,UART_BAUD_9600,CLK_8000000,UART_CLK_SMCLK);
  UART_Init(UART_A1,UART_BAUD_9600,CLK_32768,UART_CLK_SMCLK);


  // Turn the Keller ON
  FET_ON();
  
  
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




