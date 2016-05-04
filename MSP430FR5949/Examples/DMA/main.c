
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
		 |              P3.0|---------------------- From Capacitance Sensor
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


#define VERSION     ("4.1.2")
/*****************************  Includes  *********************************/
#include "./inc/includes.h"

/************************ Function Prototypes *****************************/
void SETUP_Clock(void);
void SETUP_GPIO(void);
float CalculateVolume(uint32_t count, uint32_t seconds);

#ifdef DEBUG
void BufferTest(void);
#endif

/***********************  Constants (In FRAM)  *****************************/
const uint8_t SensorPort = 3;
const uint8_t SensorPin = 0;

#ifdef DEBUG
const uint8_t DebugPort = 1;
const uint8_t DebugPin = 6;
#endif

/***********************  Persistents (In FRAM)  *****************************/
__persistent uint32_t SumOfCount = 0;
__persistent uint32_t MinuteSensorCounter = 0;
__persistent uint32_t SecondCounter = 0;
__persistent float slope = 1.412e12;
__persistent float intercept = -493.25;
__persistent float dmMin = 3.5540e4;    /* Value dm connot be less than */
__persistent float dmMax = 5.454e4;    /*  Value dm cannot exceed */

__persistent RTCStruct_t RTC;
__persistent uint8_t version[] = VERSION;
__persistent uint8_t serialNumber[16] = "";
/*************************  Global variables  *****************************/
/* Counters */
volatile uint32_t SensorCounter;
volatile uint8_t ConsoleTimeoutCounter;
volatile uint8_t ConsoleCounter;

/* Structures */

CurrentData_t MinuteData;

/* Enum types */
SystemState_t SystemState;
TransSubState_t TxSubState;

/* Constants */
__persistent char line0[256] = {0x21};//"This is the first\r\n";
__persistent char line1[256] = {0x22};//"This is the second\r\n\n";
unsigned short line0_addr = 0;
unsigned short line1_addr = 0;
unsigned short destination_addr = 0;
uint16_t SendSize = 0;
uint8_t DMATransmitComplete = false;

uint8_t writeline = 0;

char startline[] = "\r\nSTARTING\r\n\n";
/*******************************  MAIN  **********************************/
int main(void) {

  /* Configure MPU */
  __low_level_init();				// Setup FRAM
  WDTCTL = WDTPW | WDTHOLD;	

  
  
  /* Debug Conditions */
#ifdef DEBUG
  BufferTest();
  MinuteData.lastSampleRecorded  = 0;
  MinuteData.numSamples = 0;
  MinuteData.min = 4;
  
  slope = 1.412e12;
  intercept = -493.25;
  dmMax = 5.304e4;
  dmMin = 3.704e4;
#endif 
  
  /* Configure GPIO */
  SETUP_GPIO();

  /* Setup the Clock */
  SETUP_Clock();
  
  /* Setup the RTC */
  RTC_Init();

  /* Configure the UART */
  UART_Init(UART_A1,UART_BAUD_9600,CLK_32768,UART_CLK_SMCLK);
 
  /* Initialize the timers */
  TIMER_A1_Init();
  TIMER_A0_Init();

  /* Enable the interrupts */
  __bis_SR_register(GIE);
  __no_operation();                         // For debugger

  /* Set the startup State */
  SystemState = Sample;
  SensorCounter = 0;
  SecondCounter = 0;
  RTC.TimeAtCommand = SecondCounter;
  
  UART_Write(&startline[0],LENGTH_OF(startline),UART_A1);
  
    /* Setup DMA Channel */
  //__data16_write_addr((unsigned short) &DMA0SA,(unsigned long) &line0[0]);
  __data16_write_addr((unsigned short) &line0_addr,(unsigned long) &line0[0]);
  __data16_write_addr((unsigned short) &line1_addr,(unsigned long) &line1[0]);
  __data16_write_addr((unsigned short) &destination_addr,(unsigned long) &UCA1TXBUF); 
  
  SendSize = sizeof(line0)/sizeof(line0[0]);
  
  DMA_Init(line0_addr, destination_addr, SendSize);
  DMA_TriggerBlock();
  //DMA0SZ = sizeof(line0)/sizeof(line0[0]);
  
  //DMA0SA = line0_addr;
  //DMA0CTL = DMADT_0 | DMADSTINCR_0 | DMASRCINCR_3 | DMASRCBYTE ;
  //DMA0CTL |= DMAEN;
  
  //DMA0CTL |= DMAREQ;                      // Trigger block transfer
  /* Main loop */
  for(;;)
  {
    
    if(DMA0SZ == 1)
    {
      __delay_cycles(50);
    }
    //if((DMA0CTL & 0x10) == 0)
    if(DMATransmitComplete==true)
    {
      DMATransmitComplete = false;
      writeline ^= 0x01;
      
      if(writeline == 0)
      {
        DMA_SetSourceAddress(line0_addr);
      }
      else {
        DMA_SetSourceAddress(line1_addr);
      }
      
      DMA_Restart();
      DMA_TriggerBlock();
      
      //DMA0CTL = DMADT_0 | DMADSTINCR_0 | DMASRCINCR_3 | DMASRCBYTE ;
      //DMA0CTL |= DMAEN;
      //DMA0CTL |= DMAREQ;
    }
    
    //DMA0CTL |= DMAREQ;                      // Trigger block transfer
    //__delay_cycles(5000);
  }
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
  CSCTL0_H = CSKEY >> 8;		// Unlock registers
  CSCTL1 = DCOFSEL_6;			// Set DCO to 8Mhz
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
  
  /* Set up the Capacitance Sensor (counter) Input Pin  */
  GPIO_ClearPin(SensorPort, SensorPin);
  GPIO_SetPinAsInput(SensorPort, SensorPin);
  GPIO_AttachInputInterrupt(SensorPort, SensorPin,GPIO_EDGE_LOW_TO_HIGH);
  
#ifdef DEBUG
  GPIO_ClearPin(DEBUG_PORT,DEBUG_PIN);
  GPIO_SetPinAsOutput(DEBUG_PORT,DEBUG_PIN);
#endif 
  
  /* Configure Selection bits for UART */
  P2SEL1 |= BIT5 | BIT6;                    /* USCI_A0 UART operation */
  P2SEL0 &= ~(BIT5 | BIT6);

  /* LFXIN */
  PJSEL1 &= ~BIT4;
  PJSEL0 |= BIT4;
  
  /* Unlock GPIO */
  PM5CTL0 &= ~LOCKLPM5;		/* Needs to be done after config GPIO & Pins! */
  
   /* Set interrupts */
  P3IFG &= ~BIT0;

  return;
}


#ifdef DEBUG

#endif

