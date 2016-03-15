
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
 *  @note To add FRAM capability, you must enable MPU in IAR (Project->Options->General Options->MPU/IPU->Support MPU)
 * 
 *	@todo watchdog is not setup
 *	@todo Add console
 *	
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
		 |              PJ.4|----------------------     LFXIn
		 |                  |							32.768kHz Crystal
         |              PJ.5|----------------------     LFXOut
		 |                  |
		 --------------------

 */

#define PMEL         // For compiling outside of Unity/Ceedling
#define VERSION     ("0.0.1")
/*****************************  Includes  *********************************/
#include "../inc/includes.h"

/************************ Function Prototypes *****************************/
void FRAM_RetreiveData(void);
void FRAM_SaveData(void);

void SETUP_Clock(void);
void SETUP_GPIO(void);

/***********************  Constants (In FRAM)  *****************************/

/***********************  Persistents (In FRAM)  *****************************/


/*************************  Global variables  *****************************/
// Variables

// Counters

// Structures
CircularBuffer8_s Buf8_Output;
CircularBuffer8_s Buf8_Input;
// Enum types

// Character Arrays


/*******************************  MAIN  **********************************/
int main(void) {

  // Configure MPU		
  __low_level_init();				// Setup FRAM

  // Configure GPIO
  SETUP_GPIO();

  // Configure the UART
  UART_Init(UART_A1,UART_BAUD_9600,CLK_32768,UART_CLK_SMCLK);

  // Configure SPI
  SPI_Master_3W_Init(SPI_B0, 250000,8000000, UCSSEL__SMCLK);

  // Initialize the timer for 1 Second
  TIMER_A1_Init();
  TIMER_A0_Init();

  // Enable the interrupts
  __bis_SR_register(GIE);
  __no_operation();                         // For debugger

  Buffer8_Clear(&Buf8_Output);
  Buffer8_Clear(&Buf8_Input);

  for(uint8_t i=0;i<255;i++)
  {
    Buffer8_Put(&Buf8_Output,i);
  }
  uint8_t value = 0;
  
  while(Buffer8_IsEmpty(&Buf8_Output) != BUFFER8_IS_EMPTY)
  {
    Buffer8_Get(&Buf8_Output,&value);
    UCB0TXBUF = value;
  }
    
    
  // Main loopD
  for(;;)
  {
    //__delay_cycles(10000);
    
    Buffer8_Clear(&Buf8_Output);
    while(Buffer8_IsEmpty(&Buf8_Input) != BUFFER8_IS_EMPTY)
    {
      Buffer8_Get(&Buf8_Input,&value);
      Buffer8_Put(&Buf8_Output,value);
      __delay_cycles(1000);
    }
    Buffer8_Clear(&Buf8_Input);
    while(Buffer8_IsEmpty(&Buf8_Output) != BUFFER8_IS_EMPTY)
    {    
      Buffer8_Get(&Buf8_Output,&value);
      UCB0TXBUF = value;
    }
    
  }
}



/** @brief Read data from FRAM
 *
 *  Retreives the Metadata to FRAM 
 *  Slope
 *  Intercept
 *  Data in Array
 *  DataCounter value
 *
 *  @param None
 *
 *  @return Void
 */
void FRAM_RetreiveData(void)
{

	return;
}

/** @brief Saves data to FRAM
 *
 *  Saves the Metadata to FRAM 
 *  Slope
 *  Intercept
 *  Data in Array
 *  DataCounter value
 *
 *  @param None
 *
 *  @return Void
 */
void FRAM_SaveData(void)
{

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
  // Configure the clock 
  // DCO running at 1MHz
  // ACLK running on LFXT 32768Hz
  // SMCLK running on DCOCLK, 8MHz
  // MCLK running on DCOCLK, 1MHz
  // LFXT Driver on low power
  CSCTL0_H = CSKEY >> 8;		// Unlock registers
  CSCTL1 = DCOFSEL_6;			// Set DCO to 8Mhz
  CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
  CSCTL3 = DIVA__1 | DIVS__1 | DIVM__8;	
//  CSCTL4 =   LFXTDRIVE_0 | VLOOFF;
  CSCTL4 &= ~LFXTOFF;
  
  //   Wait for the clock to lock
  do
  {
    CSCTL5 &= ~LFXTOFFG;
    SFRIFG1 &= ~OFIFG;
  }while(SFRIFG1 & OFIFG);
  CSCTL0_H = 0;							// Lock CS Register
  
}

void SETUP_GPIO(void)
{
  // Set All GPIO settings to 0
  GPIO_Init();        // Sets all Outputs Low and regs to 0
  
  GPIO_SetPinAsInput(1,7);

  // Configure Selection bits for UART
  P2SEL1 |= BIT5 | BIT6;                    // USCI_A0 UART operation
  P2SEL0 &= ~(BIT5 | BIT6);
 
  /* Configuration of SPI */
  P1SEL1 |= BIT6 | BIT7;        /* MOSI & MISO */
  P1SEL0 &= ~(BIT6 | BIT7);
  
  P2SEL1 |= BIT2;
  P2SEL0 &= ~BIT2;
  

  // LFXIN
  PJSEL1 &= ~BIT4;
  PJSEL0 |= BIT4;
  
  // Unlock GPIO
  PM5CTL0 &= ~LOCKLPM5;		// Needs to be done after config GPIO & Pins!

  // Setup the Clock
  SETUP_Clock();
  
}







