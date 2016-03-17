#include "rtc.h"


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
  RTCSEC = 0x00;

  RTCADOWDAY = 0x2;                       // RTC Day of week alarm = 0x2
  RTCADAY = 0x20;                         // RTC Day Alarm = 0x20
  RTCAHOUR = 0x10;                        // RTC Hour Alarm
  RTCAMIN = 0x23;                         // RTC Minute Alarm

  RTCCTL01 &= ~(RTCHOLD);                 // Start RTC
}
  
  
  
  
uint8_t RTC_Set(uint8_t *RTCString)
{
  uint16_t Year;
  uint8_t Mon;
  uint8_t Day;
  uint8_t Hour;
  uint8_t Min;
  uint8_t Sec;
  uint8_t ValidStringFlag = false;
  uint8_t InvalidString[] = "\r\nInvalid Date/Time String\r\n";
  
  
  ValidStringFlag = RTC_ParseString(RTCString,&Year,&Mon,&Day,&Hour,&Min,&Sec);
  
  if(ValidStringFlag == false)
  {
    UART_Write(&InvalidString[0],LENGTH_OF(InvalidString),UART_A1);
    return ValidStringFlag;
  }
  

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
      SensorCounter = 0;
      SecondCounter ++;
      ConsoleTimeoutCounter++;
#ifdef DEBUG
      GPIO_TogglePin(DEBUG_PORT,DEBUG_PIN);
#endif
      break;
    case RTCIV_RTCTEVIFG:   // RTC Interval Timer Flag
      break;
    case RTCIV_RTCAIFG:     // RTC User Alarm
      break;
    case RTCIV_RT0PSIFG:    // RTC Prescaler 0
      break;
    case RTCIV_RT1PSIFG:    // RTC Prescalser 1
      break;
    default:
      break;   
  }
}