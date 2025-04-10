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
 *  @copyright Engineering Development Division
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


#define VERSION     ("4.1.3")
/*****************************  Includes  *********************************/
#include "./inc/includes.h"

/************************ Function Prototypes *****************************/
void STATE_CheckRxBuffer(void);
void STATE_Sample(void);
void STATE_MinuteTimerRoutine(void);
void STATE_Transmit(uint32_t count, uint32_t seconds);
void STATE_TransmitVolume(float volume, uint32_t count, uint32_t seconds);
void STATE_TransmitReport(SampleData_t *Data);
void STATE_TransmitIridium(SampleData_t *Data);
void STATE_TransmitCurrentTime(void);
void SETUP_Clock(void);
void SETUP_GPIO(void);
void ClearBuffers(void);
float CalculateVolume(uint32_t count, uint32_t seconds);

void populatehour(void);

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
__persistent SampleData_t HourData;
__persistent RTCStruct_t RTC;
__persistent uint8_t version[] = VERSION;
__persistent uint8_t serialNumber[16] = "";
__persistent uint8_t ClearBufferFlag = false;

/*************************  Global variables  *****************************/
/* Counters */
volatile uint32_t SensorCounter;
volatile uint8_t ConsoleTimeoutCounter;
volatile uint8_t ConsoleCounter;

/* Structures */
CircularBufferC_s ConsoleData;
CircularBufferC_s UartData;
CurrentData_t MinuteData;

/* Enum types */
SystemState_t SystemState;
TransSubState_t TxSubState;



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

  /* Clear the buffers */
  ClearBuffers();
  
   /* TESTING ONLY!!!!!!!!!!!!!!!!!!!!!!!!!!*/
  populatehour();
  
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
   
  /* Main loop */
  for(;;)
  {
    uint32_t temp_SumOfCount = 0;
    uint32_t temp_SecondsCounter = 0;
    float volume = 0.0;
    
    if(ClearBufferFlag == true)
    {
      ClearBuffers();
      ClearBufferFlag = false;
    }
    
    switch(SystemState)
    {
      case Sample:
        __bis_SR_register(LPM3_bits | GIE); /* Set LPM and wait for timer interrupt */
        while(BufferC_IsEmpty(&UartData) == BUFFER_C_NOT_EMPTY)
        {
          STATE_CheckRxBuffer();
        }
        break;
      case Console:
        CONSOLE_Main();
        if(SystemState != MinuteTimerRoutine)
        {
          SystemState = Sample;
        }
        SumOfCount = 0;
        SecondCounter = 0;
        ConsoleCounter = 0;
        break;
      case MinuteTimerRoutine:
        /* Grab the date/time */
        STATE_MinuteTimerRoutine();
        /* Set back to sampling state */
        SystemState = Sample;
        break;
      case Transmit:
        temp_SecondsCounter = SecondCounter + 1;
        while(SecondCounter < temp_SecondsCounter);
        temp_SumOfCount = SumOfCount;
        SumOfCount = 0;
        SecondCounter = 0;
        switch(TxSubState)
        {
          case Counts:
            STATE_Transmit(temp_SumOfCount,temp_SecondsCounter);
            break;
          case Volume:
            volume = CalculateVolume(temp_SumOfCount, temp_SecondsCounter);
            STATE_TransmitVolume(volume,temp_SumOfCount,temp_SecondsCounter);
            break;
          case Report:
            STATE_TransmitReport(&HourData);
            if(SystemState != MinuteTimerRoutine) {
              SystemState = Sample;
            }
            break;
          case Iridium:
            STATE_TransmitIridium(&HourData);
            break;
          case CurrentTime:
            STATE_TransmitCurrentTime();
            break;
          default:
            SystemState = Sample;
            break;
        }
        SecondCounter = 0;
        SumOfCount = 0;
        SystemState = Sample;
        break;
      case Offset:
        
        SystemState = Sample;
        break;
      default:
        break;
    }
    
  }
}

/** @brief Enters Minute Timer State    
 *
 *  Updates RTC values (if requested) and calculates stats
 *  on last 60 seconds of data.  Saves to associated buffer.
 *
 *  @param count Frequency count sum from sensor
 *  @param seconds Number of seconds of integration
 *
 *  @return Void
 */
void STATE_MinuteTimerRoutine(void)
{
  float vol[60] = {0.0};
  float Mean = 0.0;
  float STD = 0.0;
  float Min = 0.0;
  float Max = 0.0;

  int8_t StartIdx = 0;
  uint8_t numSamples = 0;
  uint8_t idx = 0;
  
  uint8_t minIdx = MinuteData.min;
    
  /* Calculate the Statistical Data */
  /* Find the start index of the temp data */
  if(MinuteData.numSamples > (4))
  {
    numSamples = 5;
  } else {
    numSamples = MinuteData.numSamples;
  }

  /* Check for index wrap-around */
  if((minIdx - numSamples) >= 0)
  {
    StartIdx = minIdx - numSamples;
  } else {
    StartIdx = (5 + minIdx - numSamples);
  }

  /* Update index of last sampled */
  MinuteData.lastSampleRecorded = StartIdx;
  
  /* Prep all the data to save in the buffers */
  for(uint8_t i=0;i<numSamples;i++)
  {
    MinuteData.numSamples--;
    if((StartIdx + i) > 4)
    {
      idx = StartIdx + i - 5;
    } else {
      idx = StartIdx + i;
    }
    
    /* Convert array to volumes */
    for(uint8_t j=0;j<60;j++)
    {
      vol[j] = CalculateVolume(MinuteData.Counts[idx][j],1);
      
      /* Clear the Buffer for next go-round */
      MinuteData.Counts[idx][j] = 0;
    }
    
    /* Calculate Stats */
    STATS_CalculateMean(&vol[0], 60, &Mean);
    STATS_ComputeSTD(&vol[0],60,Mean,&STD);
    STATS_FindMin(&vol[0],60,&Min);
    STATS_FindMax(&vol[0],60,&Max);

     /* Stuff the buffers */
    BufferF_Put_Circular(&HourData.Mean, Mean);
    BufferF_Put_Circular(&HourData.STD, STD);
    BufferF_Put_Circular(&HourData.Min, Min);
    BufferF_Put_Circular(&HourData.Max, Max);
    Buffer16_Put_Circular(&HourData.Year, MinuteData.Year[idx]);
    Buffer8_Put_Circular(&HourData.Month, MinuteData.Mon[idx]);
    Buffer8_Put_Circular(&HourData.Day, MinuteData.Day[idx]);
    Buffer8_Put_Circular(&HourData.Hour, MinuteData.Hour[idx]);
    Buffer8_Put_Circular(&HourData.Minute, MinuteData.Min[idx]);

    
  }
 
  /* Send a . to indicate minute elapse, and then CR/NL */
#ifdef DEBUG
  UART_WriteChar('.',UART_A1);
  __delay_cycles(5000);
  UART_WriteChar('\r',UART_A1);
  __delay_cycles(5000);
  UART_WriteChar('\n',UART_A1);
#endif
}


                             
                              
/** @brief Enters Transmit State
 *
 *  Preps the data for transmission and sends over the UART
 *
 *  @param count Frequency count sum from sensor
 *  @param seconds Number of seconds of integration
 *
 *  @return Void
 */
void STATE_Transmit(uint32_t count, uint32_t seconds){
  char sendString[64] = {0};
  uint8_t sendString_u[64] = {0};
  
  /* Setup the Load report string */
  sprintf(sendString,"@@@%10lu,",count);
  memcpy(sendString_u,sendString,64);
  UART_Write(&sendString_u[0],64,UART_A1);
  /* Write the second part of the load string */
  sprintf(sendString,"%10lu\r\n",seconds);
  memcpy(sendString_u,sendString,64);
  UART_Write(&sendString_u[0],64,UART_A1);
  return;
}

/** @brief Enters Transmit State
 *
 *  Preps the data for transmission and sends over the UART
 *
 *  @param volume Volume (in mL)
 *  @param seconds Numer of seconds of integration
 *
 *  @return Void
 */
void STATE_TransmitVolume(float volume, uint32_t count, uint32_t seconds){
  char sendString[64] = {0};
  uint8_t sendString_u[64] = {0};
  
  /*  Send the volume */
  sprintf(sendString,"@@@%7.2fmL,",volume);
  memcpy(sendString_u,sendString,64);
  UART_Write(&sendString_u[0],64,UART_A1);
  
  /*  Send the counts */
  sprintf(sendString,"%10lu,",count);
  memcpy(sendString_u,sendString,64);
  UART_Write(&sendString_u[0],64,UART_A1);

  /*  Send the seconds */
  sprintf(sendString,"%10lu\r\n",seconds);
  memcpy(sendString_u,sendString,64);
  UART_Write(&sendString_u[0],64,UART_A1);
  
}


/** @brief Enters Transmit Iridium Report State
 *
 *  Preps last hour of data to transmit over Iridium
 *  
 *
 *  Format is:
 *      Header -
 *              RAIN YYYY/MM/DD HH:00:00 INTERVAL\r\n
 *      Data -
 *              Mean0 Std0 Mean1 Std1 Mean2 Std2 Mean3 Std4\r\n
 *              Mean5 Std   ...
 *              ...
 *              Mean55 Std55 Mean56 Std56 Mean57 Std57 Mean58 Std58 Mean59 Std59\r\n
 *
 *
 *  @param *Data Hour Data Structure
 *
 *  @return Void
 */
void STATE_TransmitIridium(SampleData_t *Data)
{
  uint16_t year = 0;
  uint8_t mon = 0;
  uint8_t day = 0;
  int8_t currentIdx = 0;
  int8_t stopIdx = 0;
  int8_t idx = 0;
  float mean = 0.0;
  float std = 0.0;
  char line[128] = {0};  
  uint8_t line_u[128] = {0};

  /* Grab the First Time Recorded */
  uint8_t now = 0;
  uint8_t last = 0xFF;
  
  /* Set the index up*/
  currentIdx = HourData.Hour.write;
  currentIdx = currentIdx % 60;
  stopIdx = currentIdx;
  if(stopIdx < 0) {
    stopIdx += 60;
  }
  
  idx = currentIdx;
  /* Retreive buffered data and transmit to UART A1 */
  do
  {
    Buffer8_GetRequested(&HourData.Hour,idx,&now);
    
    if(now==0)
    {
      now = 0xFF;
    }
    
    if(now < last)
    {
      last = now;
      Buffer16_GetRequested(&HourData.Year, idx, &year);
      Buffer8_GetRequested(&HourData.Month,idx,&mon);
      Buffer8_GetRequested(&HourData.Day,idx,&day);
    }
    ++idx;
    idx = idx % 60;
  } while(idx != stopIdx);
  
//  sprintf(line,"RAIN %04x%02x%02x,%02x:00:00\r\n",year,mon,day,last);
  sprintf(line,"RAIN %02x/%02x/%04x,%02x:00:00,00:01:00\r\n",mon,day,year,last);
  memcpy(line_u,line,128);
  UART_Write(&line_u[0],LENGTH_OF(line_u),UART_A1);
  
  /* Retreive buffered data and transmit to UART A1 */
  uint8_t counter = 5;
  uint8_t endline[] = "\r\n";
  
  /* Set the index up*/
//  currentIdx = HourData.Hour.write + 1;
//  currentIdx = current
//  stopIdx = currentIdx;
  idx = currentIdx;
  do
  {
    
    if(counter == 0)
    {
      counter = 4;
      UART_Write(&endline[0],LENGTH_OF(endline),UART_A1);
    } else {
      counter--;
    }
   
    /* Retreive the last 60 minutes worth of data from buffers */
    BufferF_GetRequested(&HourData.Mean,idx,&mean);
    BufferF_GetRequested(&HourData.STD,idx,&std);

    
    /* RTC values are in hex, so print hex values to UART*/
    sprintf(line,"%7.2f %7.2f",mean,std);
    memcpy(line_u,line,128);
    UART_Write(&line_u[0],LENGTH_OF(line_u),UART_A1);
    
    ++idx;
    idx = idx % 60;
  } while(idx != stopIdx);
  
  UART_Write(&endline[0],LENGTH_OF(endline),UART_A1);
  UART_Write(&endline[0],LENGTH_OF(endline),UART_A1);
  return;
}


/** @brief Enters Transmit Report State
 *
 *  Preps 60 minutes of data to transmit over UART
 *
 *  Format is:
 *    YYYYMMDD,HH:MM,Mean,STD,Min,Max\r\n
 *    Where:
 *      YYYY - 4-digit Year
 *      MM   - 2-digit Month
 *      DD   - 2-digit Day
 *      HH   - 2-digit Hour (24 hour clock)
 *      MM   - 2-digit Minute
 *      Mean - 2 decimal point, 6 character Mean Volume (mL)
 *      STD - 2 decimal point, 6 character Volume Standard Deviation (mL)
 *      Min - 2 decimal point, 6 character Minimum Volume (mL)
 *      Max - 2 decimal point, 6 character Maximum Volume (mL)
 *
 *  @param volume Volume (in mL)
 *  @param seconds Numer of seconds of integration
 *
 *  @return Void
 */
void STATE_TransmitReport(SampleData_t *Data)
{
  uint16_t year;
  uint8_t mon;
  uint8_t day;
  uint8_t hr;
  uint8_t minute;
  int8_t currentIdx = 0;
  int8_t stopIdx = 0;
  int8_t idx = 0;
  float mean;
  float std;
  float min;
  float max;
  char line[128] = {0};  
  uint8_t line_u[128] = {0};

  /* Set the index up*/
  currentIdx = HourData.Hour.write;
  currentIdx = currentIdx % 60;
  stopIdx = currentIdx;
  if(stopIdx < 0) {
    stopIdx += 60;
  }
  idx = currentIdx;
  /* Retreive buffered data and transmit to UART A1 */
  do
  {
    /* Retreive the last 60 minutes worth of data from buffers */
    Buffer16_GetRequested(&HourData.Year, idx, &year);
    Buffer8_GetRequested(&HourData.Month, idx, &mon);
    Buffer8_GetRequested(&HourData.Day, idx, &day);
    Buffer8_GetRequested(&HourData.Hour, idx, &hr);
    Buffer8_GetRequested(&HourData.Minute, idx, &minute);
    BufferF_GetRequested(&HourData.Mean,idx,&mean);
    BufferF_GetRequested(&HourData.STD,idx,&std);
    BufferF_GetRequested(&HourData.Min,idx,&min);
    BufferF_GetRequested(&HourData.Max,idx,&max);
    
    /* RTC values are in hex, so print hex values to UART*/
    sprintf(line,"@@@%04x%02x%02x,%02x:%02x,%7.2f,%7.2f,%7.2f,%7.2f\r\n",year,mon,day,hr,minute,mean,std,min,max);
    memcpy(line_u,line,128);
    UART_Write(&line_u[0],LENGTH_OF(line_u),UART_A1);
    
    /* Update report index */
    ++idx;
    idx = idx % 60;
  } while(idx != stopIdx);
  
  return;
}

/** @brief Enters Transmit Current Time
 *
 *  Reports the RTC Date & time to user
 *
 *  @return Void
 */
void STATE_TransmitCurrentTime(void)
{
  char OutputStr[64] = {0};
  uint8_t OutputStr_u[64] = {0};
  
  //sprintf(OutputStr, "%04x,%02x,%02x,%02x:%02x:%02x\r\n",RTCYEAR,RTCMON,RTCDAY,RTCHOUR,RTCMIN,RTCSEC);
  sprintf(OutputStr, "%04x,%02x,%02x,%02x:%02x:%02x\r\n",RTC.Year,RTC.Mon,RTC.Day,RTC.Hour,RTC.Min,RTC.Sec);
  memcpy(OutputStr_u,OutputStr,64);
  UART_Write(&OutputStr_u[0],LENGTH_OF(OutputStr_u),UART_A1);
  
  return;
}


void ClearBuffers(void)
{
  /* Clear Minute Data Buffers */
  for(uint8_t i=0;i<5;i++) {
    MinuteData.Day[i] = 0;
    MinuteData.Hour[i] = 0;
    MinuteData.Min[i] = 0;
    MinuteData.Mon[i] = 0;
    MinuteData.Year[i] = 0;
    
    for(uint8_t j=0;j<60;j++) {
      MinuteData.Counts[i][j] = 0;
    }
    
  }
  MinuteData.min = 0;
  MinuteData.sec = 0;
  MinuteData.numSamples = 0;
  MinuteData.lastSampleRecorded = 0;
  
  /* Clear Hour Data Buffers */
  Buffer16_Clear(&HourData.Year);
  Buffer8_Clear(&HourData.Month);
  Buffer8_Clear(&HourData.Day);
  Buffer8_Clear(&HourData.Hour);
  Buffer8_Clear(&HourData.Minute);
  BufferF_Clear(&HourData.Mean);
  BufferF_Clear(&HourData.STD);
  BufferF_Clear(&HourData.Min);
  BufferF_Clear(&HourData.Max);
}

/** @brief Calculates the Volume 
 *
 *  Calculates volume based on formula:
 *  dm = n * 60/tsec
 *  Volume = (A1 /(dm^2)) + A0
 * 
 *  where:
 *  A0 = Sensor Offset
 *  A1 = Sensor Slope
 *  n = Sum of Counts
 *  tsec = Total number of seconds
 *
 *  @param count Frequency count from sensor
 *  @param seconds Number of seconds integrated over
 *
 *  @return Volume
 */
float CalculateVolume(uint32_t count, uint32_t seconds){
  float dm = 0;
  float vol = 0.0;
  
  dm = (float) count * 60;
  dm /= (float) seconds;
 
  /* Look for invalid dms */
  if(( dm > (dmMax) ) || (dm < (dmMin)))
  {
    return NAN;
  }
    
  dm *= dm;
  
  vol = (float) (slope / dm);
  vol += intercept;
  
  if(vol == -0.00)
  {
    vol = 0.0;
  }
  return vol;
  
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
void BufferTest(void)
{
  
  for(uint8_t i=0;i<59;i++)
  {
    Buffer16_Put(&HourData.Year,i);
    Buffer8_Put(&HourData.Month,i);
    Buffer8_Put(&HourData.Day,i);
    Buffer8_Put(&HourData.Hour,i);
    Buffer8_Put(&HourData.Minute,i);
    BufferF_Put(&HourData.Mean,(i*10.0));
    BufferF_Put(&HourData.STD,(i*1.0));
    BufferF_Put(&HourData.Min,(i*0.1));
    BufferF_Put(&HourData.Max,(i*2.0));
    
  }
  
}
#endif



void STATE_CheckRxBuffer(void)
{
  char value;
  char string[32] = {0};
  //uint8_t stringValid = false;
  
  SystemState = Sample;
  
  while(BufferC_IsEmpty(&UartData) == BUFFER_C_NOT_EMPTY)
  {
    BufferC_Get(&UartData,&value);
    switch(value)
    {
        case 'D':
          SystemState = Transmit;
          TxSubState = Volume;
          break;
        case 'd':
          SystemState = Transmit;
          TxSubState = Counts;
          break;
        case 'I':
        case 'i':
          SystemState = Transmit;
          TxSubState = Iridium;
          break;
        case 'R':
        case 'r':
          SystemState = Transmit;
          TxSubState = Report;
          break;
        case 'O':
        case 'o':
          RTC.TimeAtCommand = SecondCounter;
          SystemState = Offset;
          break;
        case 'T':
        case 't':
          /* Added for testing*/
          RTC.Sec = RTCSEC;
          RTC.Min = RTCMIN;
          RTC.Hour = RTCHOUR;
          RTC.Day = RTCDAY;
          RTC.Mon = RTCMON;
          RTC.Year = RTCYEAR;
          SystemState = Transmit; 
          TxSubState = CurrentTime;
          break;
        case 0x03:
          if(++ConsoleCounter >= 3)
          {
            SystemState = Console;
          }

          break;
        default:
            break;
    }          
  }
  
  if(SystemState == Offset)
  {
    while(BufferC_HasNewline(&UartData) != BUFFER_C_HAS_NEWLINE) // || ADD TIMEOUT!
    {
    }
    
    uint8_t idx = 0;
    uint8_t cnt = 0;
    while(idx < 32 && BufferC_IsEmpty(&UartData) == BUFFER_C_NOT_EMPTY)
    {
      BufferC_Get(&UartData,&value);
      if((value >= '0' && value <= '9') || (value == '-'))
      {
        string[cnt] = value;
        cnt++;
      } else if(value == '=' || value == 'o' || value == 'O' )  {

      }else {
        idx = 32;
        UART_WriteNACK(UART_A1);
        
      }
      idx++;
    }
    
    int32_t offsetVal = 0;
    offsetVal = atol(string);

    if(offsetVal != 0)
    {
      UART_WriteACK(UART_A1);
      /* Offset the time */
      RTC_Offset(offsetVal);
      __delay_cycles(10);
    }
    
    
    SystemState = Sample;
    __delay_cycles(10);
  }
}
 

void populatehour(void) {
  
  HourData.Minute.buffer[0] = 0x00;
  HourData.Minute.buffer[1] = 0x01;
  HourData.Minute.buffer[2] = 0x02;
  HourData.Minute.buffer[3] = 0x03;
  HourData.Minute.buffer[4] = 0x04;
  HourData.Minute.buffer[5] = 0x05;
  HourData.Minute.buffer[6] = 0x06;
  HourData.Minute.buffer[7] = 0x07;
  HourData.Minute.buffer[8] = 0x08;
  HourData.Minute.buffer[9] = 0x09;
  HourData.Minute.buffer[10] = 0x10;
  HourData.Minute.buffer[11] = 0x11;
  HourData.Minute.buffer[12] = 0x12;
  HourData.Minute.buffer[13] = 0x13;
  HourData.Minute.buffer[14] = 0x14;
  HourData.Minute.buffer[15] = 0x15;
  HourData.Minute.buffer[16] = 0x16;
  HourData.Minute.buffer[17] = 0x17;
  HourData.Minute.buffer[18] = 0x18;
  HourData.Minute.buffer[19] = 0x19;
  HourData.Minute.buffer[20] = 0x20;
  HourData.Minute.buffer[21] = 0x21;
  HourData.Minute.buffer[22] = 0x22;
  HourData.Minute.buffer[23] = 0x23;
  HourData.Minute.buffer[24] = 0x24;
  HourData.Minute.buffer[25] = 0x25;
  HourData.Minute.buffer[26] = 0x26;
  HourData.Minute.buffer[27] = 0x27;
  HourData.Minute.buffer[28] = 0x28;
  HourData.Minute.buffer[29] = 0x29;
  HourData.Minute.buffer[30] = 0x30;
  HourData.Minute.buffer[31] = 0x31;
  HourData.Minute.buffer[32] = 0x32;
  HourData.Minute.buffer[33] = 0x33;
  HourData.Minute.buffer[34] = 0x34;
  HourData.Minute.buffer[35] = 0x35;
  HourData.Minute.buffer[36] = 0x36;
  HourData.Minute.buffer[37] = 0x37;
  HourData.Minute.buffer[38] = 0x38;
  HourData.Minute.buffer[39] = 0x39;
  HourData.Minute.buffer[40] = 0x40;
  HourData.Minute.buffer[41] = 0x41;
  HourData.Minute.buffer[42] = 0x42;
  HourData.Minute.buffer[43] = 0x43;
  HourData.Minute.buffer[44] = 0x44;
  HourData.Minute.buffer[45] = 0x45;
  HourData.Minute.buffer[46] = 0x46;
  HourData.Minute.buffer[47] = 0x47;
  HourData.Minute.buffer[48] = 0x48;
  HourData.Minute.buffer[49] = 0x49;
  HourData.Minute.buffer[50] = 0x50;
  HourData.Minute.buffer[51] = 0x51;
  HourData.Minute.buffer[52] = 0x52;
  HourData.Minute.buffer[53] = 0x53;
  HourData.Minute.buffer[54] = 0x54;
  HourData.Minute.buffer[55] = 0x55;
  HourData.Minute.buffer[56] = 0x56;
  HourData.Minute.buffer[57] = 0x57;
  HourData.Minute.buffer[58] = 0x58;
  HourData.Minute.buffer[59] = 0x59;
  
  
  HourData.Mean.buffer[0] = 0;
  HourData.Mean.buffer[1] = 1;
  HourData.Mean.buffer[2] = 2;
  HourData.Mean.buffer[3] = 3;
  HourData.Mean.buffer[4] = 4;
  HourData.Mean.buffer[5] = 5;
  HourData.Mean.buffer[6] = 6;
  HourData.Mean.buffer[7] = 7;
  HourData.Mean.buffer[8] = 8;
  HourData.Mean.buffer[9] = 9;
  HourData.Mean.buffer[10] = 10;
  HourData.Mean.buffer[11] = 11;
  HourData.Mean.buffer[12] = 12;
  HourData.Mean.buffer[13] = 13;
  HourData.Mean.buffer[14] = 14;
  HourData.Mean.buffer[15] = 15;
  HourData.Mean.buffer[16] = 16;
  HourData.Mean.buffer[17] = 17;
  HourData.Mean.buffer[18] = 18;
  HourData.Mean.buffer[19] = 19;
  HourData.Mean.buffer[20] = 20;
  HourData.Mean.buffer[21] = 21;
  HourData.Mean.buffer[22] = 22;
  HourData.Mean.buffer[23] = 23;
  HourData.Mean.buffer[24] = 24;
  HourData.Mean.buffer[25] = 25;
  HourData.Mean.buffer[26] = 26;
  HourData.Mean.buffer[27] = 27;
  HourData.Mean.buffer[28] = 28;
  HourData.Mean.buffer[29] = 29;
  HourData.Mean.buffer[30] = 30;
  HourData.Mean.buffer[31] = 31;
  HourData.Mean.buffer[32] = 32;
  HourData.Mean.buffer[33] = 33;
  HourData.Mean.buffer[34] = 34;
  HourData.Mean.buffer[35] = 35;
  HourData.Mean.buffer[36] = 36;
  HourData.Mean.buffer[37] = 37;
  HourData.Mean.buffer[38] = 38;
  HourData.Mean.buffer[39] = 39;
  HourData.Mean.buffer[40] = 40;
  HourData.Mean.buffer[41] = 41;
  HourData.Mean.buffer[42] = 42;
  HourData.Mean.buffer[43] = 43;
  HourData.Mean.buffer[44] = 44;
  HourData.Mean.buffer[45] = 45;
  HourData.Mean.buffer[46] = 46;
  HourData.Mean.buffer[47] = 47;
  HourData.Mean.buffer[48] = 48;
  HourData.Mean.buffer[49] = 49;
  HourData.Mean.buffer[50] = 50;
  HourData.Mean.buffer[51] = 51;
  HourData.Mean.buffer[52] = 52;
  HourData.Mean.buffer[53] = 53;
  HourData.Mean.buffer[54] = 54;
  HourData.Mean.buffer[55] = 55;
  HourData.Mean.buffer[56] = 56;
  HourData.Mean.buffer[57] = 57;
  HourData.Mean.buffer[58] = 58;
  HourData.Mean.buffer[59] = 59;
  
  HourData.Day.write = 0;
  HourData.Hour.write = 0;
  HourData.Max.write = 0;
  HourData.Mean.write = 0;
  HourData.Min.write = 0;
  HourData.Minute.write = 0;
  HourData.Month.write = 0;
  HourData.STD.write = 0;
  HourData.Year.write = 0;
  
  RTCHOUR=0;
  RTCMIN=0x59;
  RTCSEC=0x55;
  
}