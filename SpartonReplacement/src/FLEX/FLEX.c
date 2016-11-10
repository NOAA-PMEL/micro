/** @file OS5000S.c
*  @brief OceanServer OS5000-S Drivers
*
*  @author Matt Casari, matthew.casari@noaa.org
 *  @date November 9, 2016
*  @version 0.0.1
*
*  @copyright National Oceanic and Atmospheric Administration
*  @copyright Pacific Marine Environmental Lab
*  @copyright Environmental Development Division
*
*	@note
*
*  @bug  No known bugs
*/
#include "FLEX.h"
 __persistent volatile FLEX_t FLEX;

void FLEX_Init(void) {
  FLEX.Mode = FL_NORMAL;
  FLEX.UART = pFL_UART;
  BufferC_Clear(&FLEX.UART->Buffer);
  
//  FLEX.UART->Timer->Timeout = 500u;
  FLEX.UART->Timer->Timeout = 500;
  FLEX.UART->Timer->TimeoutFlag = false;
  return;
}