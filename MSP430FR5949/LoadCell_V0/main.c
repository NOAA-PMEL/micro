
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

    WDTCTL = WDTPW | WDTHOLD;		// pause watchdog

    //Configure the GPIO
    GPIO_Init();        // Sets all Outputs Low and regs to 0
//    P3OUT = 0;
//    P3DIR = 0x40;
    
//
      FET_OFF();
      FET_INIT();

      
    // Unlock GPIO
    PM5CTL0 &= ~LOCKLPM5;		// Needs to be done after config GPIO & Pins!

    
    // Start the Clock
    CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
    CSCTL1 = DCOFSEL_6;                       // Set DCO to 8MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;  // Set SMCLK = MCLK = DCO
                                            // ACLK = VLOCLK
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // set all dividers
    CSCTL0_H = 0;                             // Lock CS registers
    
    __delay_cycles(DELAY);



    //P3OUT = 0x40;
   
    FET_ON();

    // Set interrupts
    __bis_SR_register(GIE);
    __no_operation();                         // For debugger



   __delay_cycles(5000);
    
   
 
    // Main loop
    for(;;)
    {
      
      FET_TOGGLE();
      __delay_cycles(5000);
        
    }
}




/*****************************  INTERRUPTS  ********************************/
/*******************************  UART A1  *********************************/
