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

/************************************************************************
*					STATIC FUNCTION PROTOTYPES
************************************************************************/
static void FLEX_FilterSet(void);
static void FLEX_MountingSet(void);
static void FLEX_TiltCmd(void);
static void FLEX_HeadingCmd(void);
/************************************************************************
*					VARIABLES
************************************************************************/
 __persistent volatile FLEX_t FLEX;
 /************************************************************************
*					GLOBAL FUNCTIONS
************************************************************************/
void FLEX_Init(void) {
  FLEX.SysMode = FL_NORMAL;
  FLEX.UART = pFL_UART;
  BufferC_Clear(&FLEX.UART->Buffer);
  
//  FLEX.UART->Timer->Timeout = 500u;
  FLEX.UART->Timer->Timeout = 500;
  FLEX.UART->Timer->TimeoutFlag = false;
  return;
}

void FLEX_ParseBuffer(void) {
  
FLEX.Mode = FLEX_TILT_CMD;
  
  switch(FLEX.Mode) {
    case FLEX_FILTER_SETUP:
      FLEX_FilterSet();
      break;
    case FLEX_MOUNT_SETUP:
      FLEX_MountingSet();
      break;
    case FLEX_TILT_CMD:
      FLEX_TiltCmd();
      break;
    case FLEX_DIRECTION_CMD:
      FLEX_HeadingCmd();
      break;
    default:
    
      break;
  }
}



static void FLEX_FilterSet(void) {
  const char filt[] = {SPARTON_FILTER_MSG};

  for(uint8_t i=0;i<5;i++) {
    TFLEX_putc(filt[i]);
  }
  return;
}


static void FLEX_MountingSet(void) {
  
  const char mtg[] = {SPARTON_MOUNTING_MSG};
  
  for(uint8_t i=0;i<5;i++) {
    TFLEX_putc(mtg[i]);
  }
  return;
}
 

static void FLEX_TiltCmd(void) {
  uint16_t pitch = 0.0;
  uint16_t roll = 0.0;
  char sendstr[7] = SPARTON_TILT_MSG;
  /* Retreive Current Pitch & Roll */
//  OS5000S_CurrentPandR(&pitch,&roll);
  OS5000S_CurrentPandR_Int(&pitch,&roll);
  sendstr[5] = (char) (roll & 0x00FF);
  sendstr[4] = (char) ((roll >> 8) & 0x00FF);
  sendstr[3] = (char) (pitch & 0x00FF);
  sendstr[2] = (char) ((pitch >> 8) & 0x00FF);
  
  /* Send Heading String */
  for(uint8_t i=0;i<7;i++) {
    TFLEX_putc(sendstr[i]);
    while(TFLEX_busy());
  }
  return;
}

static void FLEX_HeadingCmd(void){
  uint16_t heading = 0.0;
  char sendstr[5] = SPARTON_DIRECTION_MSG;
  
//  memset(&sendstr,0,5);
  //strcpy(sendstr,SPARTON_DIRECTION_MSG,5);
  
  /* Retreive Current Heading */
  OS5000S_CurrentHeading_Int(&heading);
  sendstr[3] = (char) (heading & 0x00FF);
  sendstr[2] = (char) ((heading>>8) & 0x00FF);
  
  /* Send Heading String */
  for(uint8_t i=0;i<5;i++) {
    TFLEX_putc(sendstr[i]);
    while(TFLEX_busy());
  }
  
  return;
}