#include "rtc.h"

static uint8_t RTC_ParseString(char *RTCString,uint16_t *Year,uint8_t *Mon,uint8_t *Day,uint8_t *Hour,uint8_t *Min,uint8_t *Sec);
void RTC_Init(void)
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
  RTCSEC = 0x58;
  
  RTCAMIN = 0x01;                         // Set 1 Minute Alarm
  RTCAMIN |= 0x80;                        // Enable Alaarm
  
  RTCCTL01 &= ~(RTCHOLD);                 // Start RTC
}
  
  
  
  
uint8_t RTC_Set(uint8_t *RTCString)
{
  uint8_t InvalidString[] = "\r\nInvalid Date/Time String\r\n";
  uint16_t Year;
  uint8_t Mon;
  uint8_t Day;
  uint8_t Hour;
  uint8_t Min;
  uint8_t Sec;
  uint8_t RTCStatus = false;
  uint8_t diffSec = 0;
  uint8_t tempSec = 0;
  uint8_t tempOffset = 0;
  uint16_t HighCentury = 0;
  uint16_t LowCentury = 0;
  uint16_t Decade = 0;
  uint16_t LowYear = 0;
  uint16_t low;
  uint16_t high;
  
  /*  Parse the Input String */
  RTCStatus = RTC_ParseString(RTCString,&Year,&Mon,&Day,&Hour,&Min,&Sec);
  
  /* Return Failure if Bad String */
  if(RTCStatus == RTC_FAIL)
  {
    UART_Write(&InvalidString[0],LENGTH_OF(InvalidString),UART_A1);
    return false;
  }
  
  /* Wait for the top of the second to race through calculations to reset on next second */
  tempSec = SecondCounter;
  while(SecondCounter == tempSec);
  
  
  /* Calculate Number of Seconds Difference between when message came in and present */
  diffSec = SecondCounter - RTC.TimeAtCommand + 1; /* +1 for reset at next second */
  
  Sec += diffSec;
  if(Sec > 59)
  {
    tempOffset = Sec / 60;
    Sec = Sec % 60;
  } else {
    tempOffset = 0;
  }
  
  Min = Min + tempOffset;
  if(Min > 59)
  {
    tempOffset = Min / 60;
    Min = Min % 60;
  } else {
    tempOffset = 0;
  }
  
  Hour = Hour + tempOffset;
  if(Hour > 23)
  {
    tempOffset = Hour / 24;
    Hour = Hour % 24;
  } else {
    tempOffset = 0;
  }
  
  Day = Day + tempOffset;
  switch(Mon)
  {
    case 4:
      case 6:
      case 9:
      case 11:
        if(Day > 30)
        {
          Day = 1;
          Mon++;
        }
        break;
      case 2:
        if( (Year % 4) == 0)
        {
          if(Day > 29)
          {
            Day = 1; 
            Mon++;
          }
          
        } else {
          if(Day > 28)
          {
            Day = 1;
            Mon++;
          }
        }
        break;
      case 1:
      case 3:
      case 5:
      case 7:
      case 10:
      case 12:
        if(Day > 31)
        {
          Day = 1;
          Mon++;
        }
        break;
      default:
        return RTC_FAIL;
        break;
      
    }
  
  if(Mon > 12)
  {
    Mon = 1;
    Year++;
  }
  low = Sec %  10;
  high = Sec / 10;
  high = high << 4;
  RTC.Sec = high + low;
  
  low = Min % 10;
  high = Min / 10;
  high = high << 4;
  RTC.Min = high + low;
  
  low = Hour % 10;
  high = Hour / 10;
  high = high << 4;
  RTC.Hour = high + low;
  
  low = Day % 10;
  high = Day / 10;
  high = high << 4;
  RTC.Day = high + low;
  
  low = Mon % 10;
  high = Mon / 10;
  high = high << 4;
  RTC.Mon = high + low;
 
  
  // Convert Year to Hex
  LowYear = Year % 10;
  Decade = (Year - LowYear) / 10;
  LowCentury = Decade / 10;
  HighCentury = LowCentury / 10;
  Decade = Decade % 10;
  LowCentury = LowCentury % 10;
  HighCentury = HighCentury % 10;
  
  Decade = Decade << 4;
  LowCentury = LowCentury << 8;
  HighCentury = HighCentury << 12;
  
  RTC.Year = HighCentury + LowCentury + Decade + LowYear;

  // Set the flag to update
  RTC.UpdateFlag = true;
  
  return true;
}




static uint8_t RTC_ParseString(char *RTCString,uint16_t *Year,uint8_t *Mon,uint8_t *Day,uint8_t *Hour,uint8_t *Min,uint8_t *Sec)
{
  char temp[4];
  uint16_t tYear = 0;
  uint8_t t0 = 0;
  
  
  for(uint8_t i=0;i<14;i++)
  {
    if( ( RTCString[i] < '0') || (RTCString[i] > '9') )
    {
      return RTC_FAIL;
    }
    
  }
  
  // Parse Year & look for valid (Anything greater than last year, 2016+)
  memcpy(&temp,&RTCString[0],4);
  tYear = (uint16_t) atoi(temp);
  if(tYear > 2015)
  {
    *Year = tYear;
  } else {
    return RTC_FAIL;
  }
  memset(&temp,0,4);
  
  // Parse Month & look for valid (1 - 12)
  memcpy(&temp,&RTCString[4],2);
  t0 = (uint8_t) atoi(temp);
  if(t0 > 12 || t0 < 1)
  {
    return RTC_FAIL;
  }
  *Mon = t0;
  memset(&temp,0,4);
  
  // Parse Day & look for valid (based on month & year)
  memcpy(&temp,&RTCString[6],2);
  t0 = (uint8_t) atoi(temp);
  switch(*Mon)
  {
    case 4:
    case 6:
    case 9:
    case 11:
      if(t0 > 30 || t0 < 1)
      {
        return RTC_FAIL;
      }
      break;
    case 2:
      if( (*Year % 4) == 0)
      {
        if(t0 > 29 || t0 < 1)
        {
          return RTC_FAIL;
        }
        
      } else {
        if(t0> 28 || t0 < 1)
        {
          return RTC_FAIL;
        }
      }
      break;
    case 1:
    case 3:
    case 5:
    case 7:
    case 10:
    case 12:
      if(t0 > 31 || t0 < 1)
      {
        return RTC_FAIL;
      }
      break;
    default:
      return RTC_FAIL;
      break;
    
  }
  *Day = t0;
  memset(&temp,0,4);

  // Parse Hour & look for valid (0 - 23)
  memcpy(&temp,&RTCString[8],2);
  t0 = (uint8_t) atoi(temp);
  if(t0 > 23)
  {
    return RTC_FAIL;
  }
  *Hour = t0;
  memset(&temp,0,4);

  // Parse Min & look for valid (0 - 59)
  memcpy(&temp,&RTCString[10],2);
  t0 = (uint8_t) atoi(temp);
  if(t0 > 59 )
  {
    return RTC_FAIL;
  }
  *Min = t0;
  memset(&temp,0,4);
  
  // Parse Sec & look for valid (0 - 59)
  memcpy(&temp,&RTCString[12],2);
  t0 = (uint8_t) atoi(temp);
  if(t0 > 59)
  {
    return RTC_FAIL;
  }
  *Sec = t0;
  memset(&temp,0,4);
   
   
  return RTC_OK;
}



#pragma vector=RTC_VECTOR
__interrupt void RTC_ISR(void)
{
  switch(__even_in_range(RTCIV, RTCIV_RT1PSIFG))
  {
    case RTCIV_NONE:
      break;
    case RTCIV_RTCOFIFG:    // Oscillator Failure
      break;
    case RTCIV_RTCRDYIFG:   // RTC Ready
      // Check for overflow and lock to max
      if( (0xFFFFFFFF - SumOfCount) > SensorCounter)
      {
        SumOfCount += SensorCounter;
      }
      else
      {
        SumOfCount = 0xFFFFFFFF;
      }
      // Update Counters
      SensorCounter = 0;
      SecondCounter ++;
      ConsoleTimeoutCounter++;
      
      // Change RTC values if flagged
      if(RTC.UpdateFlag == true)
      {
        RTCYEAR = RTC.Year;
        RTCMON = RTC.Mon;
        RTCDAY = RTC.Day;
        RTCHOUR = RTC.Hour;
        RTCMIN = RTC.Min;
        RTCSEC = RTC.Sec;
        RTC.UpdateFlag = false;
      }
#ifdef DEBUG
      GPIO_TogglePin(DEBUG_PORT,DEBUG_PIN);
#endif
      break;
    case RTCIV_RTCTEVIFG:   // RTC Interval Timer Flag
      // Minute timer
      __no_operation();
      break;
    case RTCIV_RTCAIFG:     // RTC User Alarm
      __no_operation();
      break;
    case RTCIV_RT0PSIFG:    // RTC Prescaler 0
      break;
    case RTCIV_RT1PSIFG:    // RTC Prescalser 1
      break;
    default:
      break;   
  }
}