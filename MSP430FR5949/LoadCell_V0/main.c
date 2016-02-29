
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
void STATE_Sample(void);
void STATE_Console(void);
void STATE_Compute(void);
void STATE_Transmit(void);

void sensorRead(PAXLDSensor_t *sensor);
void sensorProcessData(PAXLDSensor_t *sensor);

void FRAM_RetreiveData(void);
void FRAM_SaveData(void);


/***********************  Constants (In FRAM)  *****************************/
const uint8_t sensorAddress = 0x46;
const uint8_t sensorEOCPort = 3;
const uint8_t sensorEOCPin = 7;

__persistent uint8_t counterValTestWhee = 0;
__persistent metadata_t FRAM_Metadata;



/*************************  Global variables  *****************************/
// Variables
metadata_t	Metadata;
volatile uint16_t sampleCount = 0;
volatile uint32_t sampleTimer = 0;
volatile uint8_t errorCounter = 0;
volatile double pressure = 0.0;




//float Temperatures[BUFFER_F_SIZE] = {0};
float TemperatureMean = 0;

// Structures
PAXLDSensor_t pxSensor;
CircularBufferF_s PressureDataBuffer;
CircularBufferF_s TemperatureDataBuffer;
CircularBuffer_t ConsoleData;
CurrentData_t CurrentData;

// Enum types
SystemState_t SystemState;
CalibrationState_t CalibrationState;
state_t ConsoleState;
// Character Arrays
char sendData[128];

// Counters
//volatile uint8_t TimeoutCounter_1 = 0;
//volatile uint8_t TimeoutCounter_2 = 0;
volatile uint32_t msTimeoutCounter = 0;
volatile uint32_t ms2TimeoutCounter = 0;
volatile uint32_t MenuTimeoutA = 0;
volatile uint32_t ControlTimer = 0;
volatile uint32_t ControlCounter = 0;

/*******************************  MAIN  **********************************/
int main(void) {
  


	// Configure MPU		
  __low_level_init();				// Setup FRAM
	
	
	// Load Saved (FRAM) metadata into local RAM
	FRAM_RetreiveData();
	
  // Temporary fakeout values;
//  Metadata.Slope = 100;
//  Metadata.Intercept = 1000;
 
  
	// Set the sensor I2C address -> Change for production
  pxSensor.address = 0x46;
	
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
  //CSCTL2 = SELA__LFXTCLK | SELS__LFXTCLK | SELM__LFXTCLK;
  CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
  //CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;	// Divide 8 all reg
  CSCTL3 = DIVA__1 | DIVS__2 | DIVM__1;	// Divide 8 all reg
  CSCTL4 =   LFXTDRIVE_0 | VLOOFF;;
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
  TIMER_A0_Init();
  
  // Set interrupts
  P3IFG &= ~BIT7;
  
  __bis_SR_register(GIE);
  __no_operation();                         // For debugger

  // Initialize I2C
  I2CInit();
  
  // Initialize the Keller Sensor
  uint16_t tempA = 0xA8;
  I2CWrite( pxSensor.address, &tempA, 1);
  PAxLDInit(&pxSensor,sensorAddress,sensorEOCPort,sensorEOCPin);
  
  // Set interrupts
   __bis_SR_register(GIE);       // Enter LPM0 w/ interrupts
   
  // Reset sample timer
  sampleTimer = 1000;
   
  // Clear the data buffer
  BufferF_Clear(&PressureDataBuffer);
  BufferF_Clear(&TemperatureDataBuffer);
   
   // Set the startup State
   SystemState = Sample;
   
  // Main loop
  for(;;)
  {
    switch(SystemState)
    {
      case Sample:
      	if(sampleTimer > 800)
      	{
      		FET_ON();
      	}
				if(sampleTimer > 1000)
				{
                  
					sampleTimer = 0;
        	STATE_Sample();
        	FET_OFF();
	      }
        break;
      case Compute:
      	
          STATE_Compute();
          sampleCount = 0;
          SystemState = Transmit;
        break;
      case Transmit:
        STATE_Transmit();
        //FET_ON();
        SystemState = Sample;
        sampleCount = 0;
        sampleTimer = 0;
        break;
      case Console:
				STATE_Console();
				sampleTimer = 0;
        break;
      default:
        break;
      
    }
    
  }
}


void STATE_Sample(void)
{
    // Request data from sensor
    PAxLDRequestDataOnInterrupt(&pxSensor);
    
    // Read the sensors
    sensorRead(&pxSensor);
    
    // Process sensor data
    sensorProcessData(&pxSensor);
    
    //Pressures[sampleCount] = pxSensor.pressure;
    BufferF_Put_Circular(&PressureDataBuffer,pxSensor.pressure);
    BufferF_Put_Circular(&TemperatureDataBuffer,pxSensor.temperature);
 
}

void STATE_Compute(void)
{
  	float TempF[BUFFER_F_SIZE] = {0};
    
    
    // Retreive Pressures from Buffer
    sampleCount = 0;
    while(BufferF_IsEmpty(&PressureDataBuffer) == BUFFER_NOT_EMPTY)
    {
      BufferF_Get(&PressureDataBuffer,&TempF[sampleCount++]);
    }
    
    // Convert to Load
    for(uint8_t i=0;i<sampleCount;i++)
    {
      TempF[i] *= Metadata.Slope;
      TempF[i] += Metadata.Intercept;
    }
   
    
    // Run Stats on the pressures
    STATS_CalculateMean(&TempF[0],sampleCount, &CurrentData.MeanLoad);
    STATS_ComputeSTD(&TempF[0],sampleCount,CurrentData.MeanLoad,&CurrentData.STDLoad);
    STATS_FindMax(&TempF[0],sampleCount,&CurrentData.MaxLoad);
    STATS_FindMin(&TempF[0],sampleCount,&CurrentData.MinLoad);

    // Retreive Temperatures from buffer
    sampleCount = 0;
    while(BufferF_IsEmpty(&TemperatureDataBuffer) == BUFFER_NOT_EMPTY)
    {
    	BufferF_Get(&TemperatureDataBuffer,&TempF[sampleCount++]);
    }
    
    // Clear the buffers
    BufferF_Clear(&PressureDataBuffer);
    BufferF_Clear(&TemperatureDataBuffer);
    
    // Find mean of the temperature for reporting
    STATS_CalculateMean(&TempF[0],sampleCount,&CurrentData.MeanTemperature);
    

  
}

void STATE_Transmit(void)
{
  char sendString[64] = {0};
  uint8_t sendVal[64]= {0};
  

  // Setup the Load report string
  sprintf(sendString,"%3.4f,%3.4f,%3.4f,%3.4f,%3.1f\n\r",CurrentData.MeanLoad,CurrentData.STDLoad,CurrentData.MaxLoad,CurrentData.MinLoad,CurrentData.MeanTemperature);
  memcpy(sendVal,sendString,64);
  __delay_cycles(500000);
  
  // Write the load string
  UART_Write(&sendVal[0],64,UART_A1);
  
  return;
}


void STATE_Console(void)
{
  MenuTimeoutA = 0;
  CONSOLE_State_Main();

  return;
}







void FRAM_RetreiveData(void)
{
	
	Metadata.Slope = FRAM_Metadata.Slope;
	Metadata.Intercept = FRAM_Metadata.Intercept;
	Metadata.DataCounter = FRAM_Metadata.DataCounter;
	for(uint8_t i=0;i<METADATA_ARRAY_SIZE;i++)
	{
		Metadata.RecordedData[i] = FRAM_Metadata.RecordedData[i];
		Metadata.InputLoad[i] = FRAM_Metadata.InputLoad[i];
	}
	return;
}

void FRAM_SaveData(void)
{
	FRAM_Metadata.Slope = Metadata.Slope;
	FRAM_Metadata.Intercept = Metadata.Intercept;
	FRAM_Metadata.DataCounter = Metadata.DataCounter;
	for(uint8_t i=0;i<METADATA_ARRAY_SIZE;i++)
	{
		FRAM_Metadata.RecordedData[i] = Metadata.RecordedData[i];
		FRAM_Metadata.InputLoad[i] = Metadata.InputLoad[i];
	}
	return;
}

void sensorRead(PAXLDSensor_t *sensor)
{

  ms2TimeoutCounter = 0;
  while(sensor->dataAvailableFlag != true && ms2TimeoutCounter < 15);
  if(sensor->dataAvailableFlag != true)
  {
      sensor->badDataCount++;
  }
  else
  {
  		UCA1IE &= ~UCRXIE;
      PAxLDRequestDataRead(&sensor[0], 5);
      UCA1IE |= UCRXIE;
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
void sensorProcessData(PAXLDSensor_t *sensor)
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
  return;
}



