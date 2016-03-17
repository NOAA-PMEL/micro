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
