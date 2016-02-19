
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
#define VERSION     ("0.0.1")
/*****************************  Includes  *********************************/
#include "./inc/includes.h"

/************************ Function Prototypes *****************************/
void sensorRead(PAXLD_t *sensor);
void sensorProcessData(PAXLD_t *sensor);
/***********************  Constants (In FRAM)  *****************************/
const uint8_t sensorAddress = 0x46;
const uint8_t sensorEOCPort = 3;
const uint8_t sensorEOCPin = 7;

/*************************  Global variables  *****************************/
// Variables
volatile uint8_t sampleCount = 0;
volatile uint8_t errorCounter = 0;
volatile double pressure = 0.0;

// Structures
PAXLDSensor_t pxSensor;

// Character Arrays
char sendData[128];

// Counters
volatile uint8_t TimeoutCounter_1 = 0;
volatile uint8_t TimeoutCounter_2 = 0;
volatile uint32_t msTimeoutCounter = 0;
volatile uint32_t ms2TimeoutCounter = 0;

/*******************************  MAIN  **********************************/
int main(void) {
  
  float Pressures[32] = {0};
  float PressureMean = 0;
  float PressureMax = 0;
  float PressureMin = 0;
  float PressureSTD = 0;
  
  char sendChar = 'C';
  char sendString[32] = {0};
  uint8_t sendVal[32]= {0};
  uint8_t splashVer[32] = "v0.0.1\r\n";
  
  pxSensor.address = 0x46;
  
  // Pause the watchdog
  WDTCTL = WDTPW | WDTHOLD;		
	__low_level_init();				// Setup FRAM
	
  // Set All GPIO settings to 0
  GPIO_Init();        // Sets all Outputs Low and regs to 0
  
  // Set up the PAxLD Input Pin
  GPIO_ClearPin(sensorEOCPort, sensorEOCPin);
  GPIO_SetPinAsInput(sensorEOCPort, sensorEOCPin);
  GPIO_AttachInputInterrupt(sensorEOCPort,sensorEOCPin,GPIO_EDGE_HIGH_TO_LOW);
  
  // Set up the I2C Pins
  GPIO_ClearPin(1,6);
  GPIO_ClearPin(1,7);
  GPIO_SetPinAsOutput(1,6);
  GPIO_SetPinAsOutput(1,7);

  
  // Configure the FET driving power to the Keller Sensor
  FET_OFF();
  FET_INIT();

    // Configure GPIO
  P2SEL1 |= BIT5 | BIT6;                    // USCI_A0 UART operation
  P2SEL0 &= ~(BIT5 | BIT6);
  
  // Configure I2C Pins
  P1SEL1 |= BIT6 | BIT7;

  // LFXIN
  PJSEL1 &= ~BIT4;
  PJSEL0 |= BIT4;
  
  // Unlock GPIO
  PM5CTL0 &= ~LOCKLPM5;		// Needs to be done after config GPIO & Pins!

  // Configure the clock 
  // DCO running at 1MHz
  // ACLK running on LFXT 32768Hz
  // SMCLK running on DCOCLK, 500MHz
  // MCLK running on DCOCLK, 1MHz
  // LFXT Driver on low power
  CSCTL0_H = CSKEY >> 8;					// Unlock registers
  CSCTL1 = DCOFSEL_1;			// Set DCO to 8Mhz
  CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
  CSCTL3 = DIVA__1 | DIVS__2 | DIVM__1;	// Divide 8 all reg
  CSCTL4 =   LFXTDRIVE_0;
  CSCTL4 &= ~LFXTOFF;
  
//   Wait for the clock to lock
  do
  {
    CSCTL5 &= ~LFXTOFFG;
    SFRIFG1 &= ~OFIFG;
  }while(SFRIFG1 & OFIFG);
  CSCTL0_H = 0;							// Lock CS Register
     
    // Configure the UART
  UART_Init(UART_A1,UART_BAUD_9600,CLK_32768,UART_CLK_SMCLK);

	
  // Turn the Keller ON
  FET_ON();
  
  // Initialize the timer for 1 Second
  TIMER_A1_Init();
  //TIMER_A0_Init();
  

  // Set interrupts
  P3IFG &= ~BIT7;
  
  __bis_SR_register(GIE);
  __no_operation();                         // For debugger

  // Write Splash Screen
//  for(uint8_t i=0;i<32;i++)
//  {
//    UART_WriteChar(splashVer[i],UART_A1);
//  }

  // Initialize I2C
  I2CInit();
  
  // Initialize the Keller Sensor
  uint16_t tempA = 0xA8;
  I2CWrite( pxSensor.address, &tempA, 1);
  PAxLDInit(&pxSensor,sensorAddress,sensorEOCPort,sensorEOCPin);
  
  //FET_OFF();
   __bis_SR_register(GIE);       // Enter LPM0 w/ interrupts
  // Main loop
  for(;;)
  {
    // Transmit some junk on the UART
    //UART_WriteChar(sendChar,UART_A1);
    for(uint8_t i=0;i<32;i++)
    {
      // Request data from sensor
      PAxLDRequestDataOnInterrupt(&pxSensor);
    
    // Read the sensors
      sensorRead(&pxSensor);
    
    // Process sensor data
      sensorProcessData(&pxSensor);
      Pressures[i] = pxSensor.pressure;
      
    }
    FET_OFF();
    
    STATS_CalculateMean(&Pressures[0],LENGTH_OF(Pressures),&PressureMean);
    STATS_ComputeSTD(&Pressures[0],LENGTH_OF(Pressures),PressureMean,&PressureSTD);
    STATS_FindMax(&Pressures[0],LENGTH_OF(Pressures),&PressureMax);
    STATS_FindMin(&Pressures[0],LENGTH_OF(Pressures),&PressureMin);
    
    sprintf(sendString,"%3.2f,%3.4f,%3.2f,%3.2f\n",PressureMean,PressureSTD,PressureMax,PressureMin);
    memcpy(sendVal,sendString,32);
    __delay_cycles(500000);
    UART_Write(&sendVal[0],32,UART_A1);
    for(uint8_t i=0;i<32;i++)
    {
      sendVal[i] = 0;
      sendString[i] = 0;
    }
    FET_ON();
    __delay_cycles(500000);  
  }
}


void sensorRead(PAXLD_t *sensor)
{

  ms2TimeoutCounter = 0;
  while(sensor->dataAvailableFlag != true && ms2TimeoutCounter < 15);
  if(sensor->dataAvailableFlag != true)
  {
      sensor->badDataCount++;
  }
  else
  {
      PAxLDRequestDataRead(&sensor[0], 5);
      sensor->badDataCount = 0;
  }

}


/** @brief Request data from PAxLD sensor on I2C Interrupts
 *
 *  writes to PAxLD sensor with a request for the sensor
 *  to return the data.  Uses I2C STOP and RX interrupts
 *
 *  @param *sensor Sensor data structure
 *
 *  @return Void
 */
void sensorProcessData(PAXLD_t *sensor)
{

  ms2TimeoutCounter = 0;
  while((sensor->dataIndex < 5) && ms2TimeoutCounter < 15);
  {
     if((sensor->dataIndex < 5))
     {
         sensor->pressure = -999.9;
     }
     else
     {
         PAxLDProcessReceivedData(&sensor[0]);
     }
  }

}



