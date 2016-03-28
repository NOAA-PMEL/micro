
/** @file main.c
 *
 *  @brief Main Function
 *
 *  @author Matt Casari, matthew.casari@noaa.org
 *  @date March 28, 2016
 *  @version 1.0.0
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
 *	@todo 
 *	@todo 
 *	
 *	@todo 
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
		 |              PJ.4|---------------------- LFXIn
		 |                  |						    32.768kHz Crystal
         |              PJ.5|---------------------- LFXOut
		 |                  |
		 --------------------

 */

#define PMEL            /* For compiling outside of Unity/Ceedling */
#define VERSION     ("1.0.0")
/*****************************  Includes  *********************************/
#include "./inc/includes.h"

/************************ Function Prototypes *****************************/
void STATE_Sample(void);
void STATE_Console(void);
void STATE_Compute(void);
void STATE_Transmit(void);

void sensorInit(void);
void sensorRead(PAXLDSensor_t *sensor);
void sensorProcessData(PAXLDSensor_t *sensor);

void FRAM_RetreiveData(void);
void FRAM_SaveData(void);

void SETUP_Clock(void);
void SETUP_GPIO(void);

/***********************  Constants (In FRAM)  *****************************/
const uint8_t sensorAddress = 0x46;
const uint8_t sensorEOCPort = 3;
const uint8_t sensorEOCPin = 7;

/***********************  Persistents (In FRAM)  *****************************/
__persistent uint8_t counterValTestWhee = 0;
__persistent metadata_t FRAM_Metadata;

/*************************  Global variables  *****************************/
/* Variables */
metadata_t	Metadata;
volatile uint16_t sampleCount = 0;
volatile uint32_t sampleTimer = 0;
volatile uint8_t errorCounter = 0;
volatile double pressure = 0.0;
float TemperatureMean = 0;

/* Counters */
volatile uint32_t msTimeoutCounter = 0;
volatile uint32_t ms2TimeoutCounter = 0;
volatile uint32_t MenuTimeoutA = 0;
volatile uint32_t ControlTimer = 0;
volatile uint32_t ControlCounter = 0;
volatile uint8_t NANCounter = 0;

/* Structures */
PAXLDSensor_t pxSensor;
CircularBufferF_s PressureDataBuffer;
CircularBufferF_s TemperatureDataBuffer;
CircularBuffer_t ConsoleData;
CurrentData_t CurrentData;

/* Enum types */
SystemState_t SystemState;
CalibrationState_t CalibrationState;
state_t ConsoleState;

/* Character Arrays */


/*******************************  MAIN  **********************************/
int main(void) {

  /* Configure MPU & FRAM */
  __low_level_init();				

  /* Load Saved (FRAM) metadata into local RAM */
  FRAM_RetreiveData();

  /* Set the sensor I2C address -> Change for production */
  pxSensor.address = 0x46;

  /* Configure GPIO */
  SETUP_GPIO();

  /* Configure the UART */
  UART_Init(UART_A1,UART_BAUD_4800,CLK_32768,UART_CLK_SMCLK);

  /* Turn the Keller ON */
  FET_ON();

  /* Initialize the timer for 1 Second */
  TIMER_A1_Init();
  TIMER_A0_Init();

  /* Set interrupts */
  P3IFG &= ~BIT7;

  /* Enable the interrupts */
  __bis_SR_register(GIE);
  __no_operation();                         /* For debugger */

  /* Initialize I2C */
  I2CInit();

  /* Initialize the Keller Sensor */
  sensorInit();

  /* Reset sample timer */
  sampleTimer = 1000;

  /* Clear the data buffer */
  BufferF_Clear(&PressureDataBuffer);
  BufferF_Clear(&TemperatureDataBuffer);

  /* Set the startup State */
  SystemState = Sample;
   
  /* Main loop */
  for(;;)
  {
    /* State Machine for Sensor */
    /* Sample Mode, Compute Stats Mode, Transmit mode & Console Mode */
    switch(SystemState)
    {
      case Sample:
        /* Polls the Keller for data and stores in buffer */
        I2CInit();
        FET_ON();
        sampleTimer = 0;
        while(sampleTimer < 10);
        sampleTimer = 0;
        STATE_Sample();
        FET_OFF();
        I2CClose();
        __bis_SR_register(LPM3_bits | GIE); /* Set LPM and wait for timer interrupt */
        break;
        
      case Compute:
        /* Computes statistical analysis on data in buffer */
        STATE_Compute();
        sampleCount = 0;
        SystemState = Transmit;
        break;
        
      case Transmit:
        /* Transmits statistical data back to user */
        STATE_Transmit();
        SystemState = Sample;
        sampleCount = 0;
        sampleTimer = 0;
        break;
        
      case Console:
        /* Enters console mode */
        STATE_Console();
        sampleTimer = 0;
        break;
        
      default:
        break;
    }
  }
}

/** @brief Enters Sampling State
 *
 *  Requests data from the PAxLD sensor and puts it in the circular buffer
 *
 *  @param None
 *
 *  @return Void
 */
void STATE_Sample(void)
{
    /* Request data from sensor */
    PAxLDRequestDataOnInterrupt(&pxSensor);
    
    /* Read the sensors */
    sensorRead(&pxSensor);
    
    /* Process sensor data */
    sensorProcessData(&pxSensor);
    
    /*Pressures[sampleCount] = pxSensor.pressure; */
    BufferF_Put_Circular(&PressureDataBuffer,pxSensor.pressure);
    BufferF_Put_Circular(&TemperatureDataBuffer,pxSensor.temperature);
 
}

/** @brief Enters Compute State
 *
 *  Retreives all the stored data, performs statistical calculations. 
 *  Also looks for error in the data, and resets the sensor if there
 *  is a problem.
 *
 *  @param None
 *
 *  @return Void
 */
void STATE_Compute(void)
{
  	float TempF[BUFFER_F_SIZE] = {0};
    
    /* Retreive Pressures from Buffer */
    sampleCount = 0;
    while(BufferF_IsEmpty(&PressureDataBuffer) == BUFFER_NOT_EMPTY)
    {
      BufferF_Get(&PressureDataBuffer,&TempF[sampleCount++]);
    }
    
    /* Convert to Load */
    for(uint8_t i=0;i<sampleCount;i++)
    {
      
      TempF[i] *= Metadata.Slope;
      TempF[i] += Metadata.Intercept;

    }

    /* Run Stats on the pressures */
    STATS_CalculateMean(&TempF[0],sampleCount, &CurrentData.MeanLoad);
    STATS_ComputeSTD(&TempF[0],sampleCount,CurrentData.MeanLoad,&CurrentData.STDLoad);
    STATS_FindMax(&TempF[0],sampleCount,&CurrentData.MaxLoad);
    STATS_FindMin(&TempF[0],sampleCount,&CurrentData.MinLoad);

    /* Look for NAN in data and reset the sensor if there is one */
    if(CurrentData.MeanLoad != CurrentData.MeanLoad )
    {
      sensorInit();
      BufferC_Clear(&ConsoleData);
    }

    /* Retreive Temperatures from buffer */
    sampleCount = 0;
    while(BufferF_IsEmpty(&TemperatureDataBuffer) == BUFFER_NOT_EMPTY)
    {
    	BufferF_Get(&TemperatureDataBuffer,&TempF[sampleCount++]);
    }
    
    /* Clear the buffers */
    BufferF_Clear(&PressureDataBuffer);
    BufferF_Clear(&TemperatureDataBuffer);
    
    /* Find mean of the temperature for reporting */
    STATS_CalculateMean(&TempF[0],sampleCount,&CurrentData.MeanTemperature);
    
}

/** @brief Enters Transmit State
 *
 *  Preps the data for transmission and sends over the UART
 *
 *  @param None
 *
 *  @return Void
 */
void STATE_Transmit(void)
{
  char sendString[64] = {0};

  /* Setup the Load report string */
  sprintf(sendString,"@@@%6.1f,%6.1f,%6.1f,%6.1f,%6.1f\r\n",CurrentData.MeanLoad,CurrentData.STDLoad,CurrentData.MaxLoad,CurrentData.MinLoad,CurrentData.MeanTemperature);
  
  /* Write the load string */
  UART_Write(&sendString[0],64,UART_A1);
  
  return;
}

/** @brief Enter Console 
 *
 *  Initalizes and enters the console
 *
 *  @param None
 *
 *  @return Void
 */
void STATE_Console(void)
{
  MenuTimeoutA = 0;
  
  /* Show the console */
  CONSOLE_State_Main();

  return;
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
	/* Retreive metadata from FRAM persistent variables */
    Metadata.SerialNumber = FRAM_Metadata.SerialNumber;
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
    /* Save metadata to FRAM persistent variables */
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

/** @brief Read the PAxLD Sensor
 *
 *  Reads the data from the PAxLD sensor or waits for a timeout
 *
 *  @param None
 *
 *  @return Void
 */
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
         sensor->pressure = NAN;
     }
     else
     {
         PAxLDProcessReceivedData(&sensor[0]);
     }
  }
  return;
}

/** @brief Init the PAxLD sensor
 *
 *  Reinitializes the I2C bus and the Keller PAxLD sensor
 *  and 
 *
 *  @param None
 *
 *  @return Void
 */
void sensorInit(void)
{
  I2CInit();
  sampleTimer = 0;
  FET_ON();
  while(sampleTimer < 10);
  
  uint16_t tempA = 0xA8;
  I2CWrite( pxSensor.address, &tempA, 1);
  PAxLDInit(&pxSensor,sensorAddress,sensorEOCPort,sensorEOCPin);
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
  /*
   Configure the clock 
   DCO running at 1MHz
   ACLK running on LFXT 32768Hz
   SMCLK running on DCOCLK, 500MHz
   MCLK running on DCOCLK, 1MHz
   LFXT Driver on low power
  */
  CSCTL0_H = CSKEY >> 8;		/* Unlock registers */
  CSCTL1 = DCOFSEL_1;			/* Set DCO to 8Mhz */
  CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
  CSCTL3 = DIVA__1 | DIVS__2 | DIVM__1;	
  CSCTL4 =   LFXTDRIVE_0 | VLOOFF;
  CSCTL4 &= ~LFXTOFF;
  
  /*   Wait for the clock to lock */
  do
  {
    CSCTL5 &= ~LFXTOFFG;
    SFRIFG1 &= ~OFIFG;
  }while(SFRIFG1 & OFIFG);
  CSCTL0_H = 0;							/* Lock CS Register */
  
}


void SETUP_GPIO(void)
{
  /* Set All GPIO settings to 0 */
  GPIO_Init();        /* Sets all Outputs Low and regs to 0 */
   
  /* Set up the PAxLD Input Pin */
  GPIO_ClearPin(sensorEOCPort, sensorEOCPin);
  GPIO_SetPinAsInput(sensorEOCPort, sensorEOCPin);
  GPIO_AttachInputInterrupt(sensorEOCPort,sensorEOCPin,GPIO_EDGE_HIGH_TO_LOW);
  
  /* Set up the I2C Pins */
  GPIO_ClearPin(1,6);
  GPIO_ClearPin(1,7);
  GPIO_SetPinAsOutput(1,6);
  GPIO_SetPinAsOutput(1,7);

  /* Configure the FET driving power to the Keller Sensor */
  FET_OFF();
  FET_INIT();
  
  /* Configure Selection bits for UART */
  P2SEL1 |= BIT5 | BIT6;                    /* USCI_A0 UART operation */
  P2SEL0 &= ~(BIT5 | BIT6); 
  
  /* Configure I2C Pins */
  P1SEL1 |= BIT6 | BIT7;

  /* LFXIN */
  PJSEL1 &= ~BIT4;
  PJSEL0 |= BIT4;
  
  /* Unlock GPIO */
  PM5CTL0 &= ~LOCKLPM5;		/* Needs to be done after config GPIO & Pins! */

  /* Setup the Clock */
  SETUP_Clock();
  
}







