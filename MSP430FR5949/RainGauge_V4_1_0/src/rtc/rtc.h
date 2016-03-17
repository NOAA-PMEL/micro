#ifndef rtc_H
#define rtc_H

/************************************************************************
*							HEADER FILES
************************************************************************/
#include "../../inc/includes.h"

#ifndef PMEL
#include "../test/msp430fr5969.h"
#else
#include <msp430fr5949.h>
#endif


/************************************************************************
*						STANDARD LIBRARIES
************************************************************************/
#include <stdint.h>

#define RTC_OK      (0)
#define RTC_FAIL    (1)

typedef struct _RTCStruct_s
{
  uint16_t Year;
  uint8_t Mon;
  uint8_t Day;
  uint8_t Hour;
  uint8_t Min;
  uint8_t Sec;
  uint8_t UpdateFlag;
  uint8_t TimeAtCommand;
}RTCStruct_s;


/** @brief Initialize the RTC
 *
 *	Sets the RTC up based on user provided time
 *
 *
 *  @return none
 */
void RTC_Init(void);


/** @brief Set the RTC 
 *  
 *  Sets the RTC to the Date/Time in String
 *  String format should be YYYYMMDDHHMMSS
 *
 * @param DateTime String
 *
 * @return Status
 */
uint8_t RTC_Set(uint8_t *RTCString);

#endif // rtc_H
