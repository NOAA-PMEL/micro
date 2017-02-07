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
static void FLEX_SplitSubstring(char *str, FLEX_t *f);
/************************************************************************
*					VARIABLES
************************************************************************/
 __persistent FLEX_t FLEX;
const char OSSeq[] = OS5000S_ESCAPE_SEQUENCE;
uint8_t OSSeq_len = 3;
 /************************************************************************
*					GLOBAL FUNCTIONS
************************************************************************/
void FLEX_Init(void) {
  FLEX.SysMode = FL_NORMAL;
  FLEX.UART = pFL_UART;
  BufferC_Clear(&FLEX.UART->Buffer);
  
  FLEX.UART->Timer->Timeout = 500;
  FLEX.UART->Timer->TimeoutFlag = false;
  return;
}

void FLEX_ParseBuffer(void) {
  
  uint8_t stopFlag = false;
  uint16_t idx = 0;
  char str[BUFFER_C_SIZE];
  memset(&str[0],0,BUFFER_C_SIZE);
  
  /* Look for Ocean-Server Software Escape Sequence */
  if(BufferC_HasSequence(&FLEX.UART->Buffer, &OSSeq[0],OSSeq_len) == BUFFER_C_HAS_SEQ) {
    /* Halt UART Interrupt */
    OS5000S_UART_Halt();
    TFLEX_UART_Halt();
    
    /* Change mode to DMA */
    DMA0_Init();
    DMA1_Init();
    
    /* Go into low power mode */
    __low_power_mode_3();       /* <--- Program "hangs" here intentionally */
    
    
  } else {
    /* Otherwise look for FLEX commands */
    do{
      if(BufferC_HasChar(&FLEX.UART->Buffer,0xA0) == BUFFER_C_HAS_CHAR ){

        BufferC_Get(&FLEX.UART->Buffer,&str[idx++]);
      } else {
        stopFlag = true;

      }
    }while(stopFlag == false);
    
    if(str[idx-1] == 0xA0) {
      FLEX_SplitSubstring(&str[0], &FLEX);
    }
  
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
      case FLEX_DIRECTION_AND_TILT_CMD:
        FLEX_HeadingCmd();
        FLEX_TiltCmd();
        break;
      case FLEX_IDLE:
      default:
        __low_power_mode_3();
        break;
    }
  }
  /* Return to Idle Mode */
  FLEX.Mode = FLEX_IDLE;
  
  return;
}



static void FLEX_FilterSet(void) {
  const char filt[] = {SPARTON_FILTER_MSG};

  for(uint8_t i=0;i<5;i++) {
    TFLEX_putc(filt[i]);
    while(TFLEX_busy());
  }
  return;
}


static void FLEX_MountingSet(void) {
  
  const char mtg[] = {SPARTON_MOUNTING_MSG};

  for(uint8_t i=0;i<4;i++) {
    TFLEX_putc(mtg[i]);
    while(TFLEX_busy());
  }
  return;
}
 

static void FLEX_TiltCmd(void) {
  uint16_t pitch = 0.0;
  uint16_t roll = 0.0;
  char sendstr[7] = SPARTON_TILT_MSG;
  /* Retreive Current Pitch & Roll */
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


static void FLEX_SplitSubstring(char *str, FLEX_t *f) {
  const char DirAndPitch[] = "\xA4\x09\xA0\xA4\x06\xA0";
  /* Check for dual command Direction & Tilt */
  if(strstr(str,DirAndPitch)!=0){
    f->Mode = FLEX_DIRECTION_AND_TILT_CMD;
    return;
  }
  
  /* Check for Filter Setup */
  if(strstr(str,SPARTON_FILTER_MSG)!=0) {
    f->Mode = FLEX_FILTER_SETUP;
    return;
  }
  
  /* Check for Mount SEtup */
  if(strstr(str,SPARTON_MOUNTING_MSG)!=0) {
    f->Mode = FLEX_MOUNT_SETUP;
    return;
  }
  
  /* Check for Direction Command */
  if(strstr(str,SPARTON_DIRECTION_MSG)!=0) {
    f->Mode = FLEX_DIRECTION_CMD;
    return;
  }
  
  /* Check for Direction Command */
  if(strstr(str,SPARTON_TILT_MSG)!=0) {
    f->Mode = FLEX_TILT_CMD;
    return;
  }
 
  
  f->Mode = FLEX_IDLE;
            
  return;
}