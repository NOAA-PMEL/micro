
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
 *  This Rain Gauge program takes the counter ouput from PMEL Rain Gauge v4.0
 *  capacitance sensor, counts the number of toggles per second and stores
 *  the counts and the number of elapsed seconds in seperate buffers until 
 *  the user commands a data retreival by passing a 'D' or 'd' character over
 *  the UART.  
 *  Return message is:
 *  @@@COUNT,TIME\r\n
 *  Where:
 *  @@@ is the message header
 *  COUNT is the 10 digit sensor count
 *  TIME is the 10 digit number of seconds integrated over
 *  
 *  RS-232 Info:
 *  Buad Rate - 9600
 *  Bits - 8
 *  Stop Bits - 1
 *  Parity - None 
 *
 *  @note To add FRAM capability, you must enable MPU in IAR (Project->Options->General Options->MPU/IPU->Support MPU)
 * 
 */


/*------------------- Connection Diagram------------------------------*/
/*
		       						 					
		                               
		    MSP430FR5949             
		 --------------------        
		 |                  |       
		 |                  |        
		 |              P3.0|---------------------- To Capacitance Sensor
		 |                  |
		 |                  |
		 |              P2.5|---------------------- UART Tx
 		 |              P2.6|---------------------- UART Rx
		 |                  | 		  
		 |              PJ.4|----------------------   LFXIn
		 |                  |						  32.768kHz Crystal
         |              PJ.5|----------------------   LFXOut
		 |                  |
		 --------------------

*/

#define PMEL            // For compiling outside of Unity/Ceedling
#define VERSION     ("4.3.0")
/*****************************  Includes  *********************************/
#include "./inc/includes.h"

/************************ Function Prototypes *****************************/
void STATE_Sample(void);
void STATE_Transmit(uint32_t count, uint32_t seconds);

void SETUP_Clock(void);
void SETUP_GPIO(void);
void SETUP_RTC(void);

/***********************  Constants (In FRAM)  *****************************/
const uint8_t SensorPort = 3;
const uint8_t SensorPin = 0;

#ifdef DEBUG
const uint8_t DebugPort = 1;
const uint8_t DebugPin = 6;
#endif

/***********************  Persistents (In FRAM)  *****************************/
__persistent uint32_t SumOfCount = 0;
__persistent uint32_t SecondCounter = 0;

/*************************  Global variables  *****************************/
// Variables

// Counters
uint32_t SensorCounter;

// Structures

// Enum types
SystemState_t SystemState;

// Character Arrays


/*******************************  MAIN  **********************************/
int main(void) {

  // Configure MPU		
  __low_level_init();				// Setup FRAM
  WDTCTL = WDTPW | WDTHOLD;	

  // Configure GPIO
  SETUP_GPIO();

  // Setup the Clock
  SETUP_Clock();
  
  // Setup the RTC
  SETUP_RTC();
  
  // Configure the UART
  UART_Init(UART_A1,UART_BAUD_9600,CLK_32768,UART_CLK_SMCLK);

  // Initialize the timers
  TIMER_A1_Init();
  TIMER_A0_Init();

  // Enable the interrupts
  __bis_SR_register(GIE);
  __no_operation();                         // For debugger

  // Set the startup State
  SystemState = Sample;
  SensorCounter = 0;
  SecondCounter = 0;
  // Main loop
  for(;;)
  {
    uint32_t temp_SumOfCount = 0;
    uint32_t temp_SecondsCounter = 0;
    
    switch(SystemState)
    {
      case Sample:
        //__bis_SR_register(LPM3_bits | GIE); // Set LPM and wait for timer interrupt
        break;
      case Transmit:

        
        temp_SumOfCount = SumOfCount;
        temp_SecondsCounter = SecondCounter;
        SumOfCount = 0;
        SecondCounter = 0;
        STATE_Transmit(temp_SumOfCount,temp_SecondsCounter);
        SecondCounter = 0;
        SumOfCount = 0;
        break;
      default:
        break;
    }
    
  }
}


/** @brief Enters Transmit State
 *
 *  Preps the data for transmission and sends over the UART
 *
 *  @param None
 *
 *  @return Void
 */
void STATE_Transmit(uint32_t count, uint32_t seconds)
{
  char sendString[64] = {0};

  // Setup the Load report string
  sprintf(sendString,"@@@%10d,%10d\r\n",count,seconds);
  
  // Write the load string
  UART_Write(&sendString[0],64,UART_A1);
  
  return;
}


/** @brief Read data from FRAM
 *
 *
 *  @param None
 *
 *  @return Void
 */
void FRAM_RetreiveData(void)
{
	// Retreive metadata from FRAM persistent variables

	return;
}

/** @brief Saves data to FRAM
 *
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
  // SMCLK running on DCOCLK, 500MHz
  // MCLK running on DCOCLK, 1MHz
  // LFXT Driver on low power
  CSCTL0_H = CSKEY >> 8;		// Unlock registers
  CSCTL1 = DCOFSEL_1;			// Set DCO to 8Mhz
  CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
  CSCTL3 = DIVA__1 | DIVS__2 | DIVM__1;	
  CSCTL4 =   LFXTDRIVE_0 | VLOOFF;
  CSCTL4 &= ~LFXTOFF;
  
  //   Wait for the clock to lock
  do
  {
    CSCTL5 &= ~LFXTOFFG;
    SFRIFG1 &= ~OFIFG;
  }while(SFRIFG1 & OFIFG);
  CSCTL0_H = 0;							// Lock CS Register
  
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
  // Set All GPIO settings to 0
  GPIO_Init();        // Sets all Outputs Low and regs to 0
  
  // Set up the Capacitance Sensor (counter) Input Pin
  GPIO_ClearPin(SensorPort, SensorPin);
  GPIO_SetPinAsInput(SensorPort, SensorPin);
  GPIO_AttachInputInterrupt(SensorPort, SensorPin,GPIO_EDGE_LOW_TO_HIGH);
  
#ifdef DEBUG
  GPIO_ClearPin(DEBUG_PORT,DEBUG_PIN);
  GPIO_SetPinAsOutput(DEBUG_PORT,DEBUG_PIN);
#endif 
  
  // Configure Selection bits for UART
  P2SEL1 |= BIT5 | BIT6;                    // USCI_A0 UART operation
  P2SEL0 &= ~(BIT5 | BIT6);

  // LFXIN
  PJSEL1 &= ~BIT4;
  PJSEL0 |= BIT4;
  
  // Unlock GPIO
  PM5CTL0 &= ~LOCKLPM5;		// Needs to be done after config GPIO & Pins!
  
   // Set interrupts
  P3IFG &= ~BIT0;

  return;
}


/** @brief Configure RTC
 *
 *  Initialize Real-Time Clock C (RTC_C)
 *  Alarm every 1-second
 *  Temperature Compensated
 * 
 *  @param None
 *
 *  @return Void
 */
void SETUP_RTC(void)
{
  // Configure RTC C
  // RTCTEIVE - Event Interrupt Enable
  // RTCRDYIE - Ready Interrupt Enable
  // RTCBCD - Timer on BCD Mode
  // RTCHOLD - RTC Hold
  RTCCTL01 = RTCTEVIE | RTCRDYIE | RTCBCD | RTCHOLD;  


  RTCYEAR = 0x2016;
  RTCMON = 0x3;
  RTCDAY = 0x08;
  RTCDOW = 0x02;
  RTCHOUR = 0x16;
  RTCMIN = 0x57;
  RTCSEC = 0x00;

  RTCADOWDAY = 0x2;                       // RTC Day of week alarm = 0x2
  RTCADAY = 0x20;                         // RTC Day Alarm = 0x20
  RTCAHOUR = 0x10;                        // RTC Hour Alarm
  RTCAMIN = 0x23;                         // RTC Minute Alarm

  RTCCTL01 &= ~(RTCHOLD);                 // Start RTC
}



