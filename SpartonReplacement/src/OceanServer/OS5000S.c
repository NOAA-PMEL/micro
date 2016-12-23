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
#include "OS5000S.h"

/************************************************************************
*					VARIABLES
************************************************************************/
__persistent OS5000S_t OS5000S;

/************************************************************************
*					STATIC FUNCTION PROTOTYPES
************************************************************************/
static OSSTAT_t OS5000S_SplitSubString(char *str, char *substr);
static OSSTAT_t OS5000S_ParseSubString(char *str, OS5000S_t *s);
/************************************************************************
*					GLOBAL FUNCTION PROTOTYPES
************************************************************************/
void OS5000S_Init(void) {
  OS5000S.Calibrate = false;
  OS5000S.heading = 0.0;
  OS5000S.pitch = 0.0;
  OS5000S.roll = 0.0;
  OS5000S.UART = pOS_UART;
  
  
  return;
}

void OS5000S_ParseBuffer(void) {
  
  /* Check for newline in buffer */
  if(BufferC_HasNewline(&OS5000S.UART->Buffer) == BUFFER_C_HAS_NEWLINE) {
    
  
    /* retreive the string from the circular buffer */
    uint16_t idx = 0;
    uint8_t status = BUFFER_C_NOT_FULL;
    char str[BUFFER_C_SIZE];
    memset(&str[0],0,BUFFER_C_SIZE);
    char substr[BUFFER_C_SIZE];
    memset(&substr,0,BUFFER_C_SIZE);

    while((idx < BUFFER_C_SIZE) && (status != BUFFER_C_IS_EMPTY)) {
      status = BufferC_Get(&OS5000S.UART->Buffer,&str[idx++]);
    }
    
    
    /* Split the string and parse the substring */
    if(OS5000S_SplitSubString(&str[0],&substr[0]) == OS_VALID) {
      OS5000S_ParseSubString(&substr[0],&OS5000S);
      
    } 
  }
  
  return;
}


void OS5000S_CurrentHeading(float *heading) {
  *heading = OS5000S.heading;
  return;
}

void OS5000S_CurrentPandR(float *pitch,float *roll){
  *pitch = OS5000S.pitch;
  *roll = OS5000S.roll;
  return;
}

void OS5000S_CurrentHeading_Int(uint16_t *heading){
  *heading = (uint16_t) ((OS5000S.heading *4096)/ 360.0);
//  fHeading *= 4096;
  
 
  
  return;
}

void OS5000S_CurrentPandR_Int(uint16_t *pitch, uint16_t *roll) {
  *pitch = (int16_t) ((OS5000S.pitch * 4096) / 90.0) ;
  *roll = (int16_t) ((OS5000S.roll * 4096) / 180.0);
  
  
  return;
}

static OSSTAT_t OS5000S_SplitSubString(char *str, char *substr) {
  
  /* Find the last string from the sensor and parse it */
  uint8_t eIdx = 0;
  uint8_t sIdx = 0;
  
  for(uint16_t i=BUFFER_C_SIZE-1;i>0;i--) {
    if(str[i] == '\n'){
      eIdx = i;
    }
    
    if(str[i] == '$') {
      sIdx = i;
      break;
    }
  }
  
  uint8_t length = eIdx - sIdx;
  
  /* Check for Valid Length of string */
  if(length < VALID_STRING_LENGTH) {
    return OS_INVALID_STRING;
  }
 
  /* Copy the substring */
  memcpy(substr,&str[sIdx],length);
  
  /* Return */
  return OS_VALID;
}
           
           
static OSSTAT_t OS5000S_ParseSubString(char *str, OS5000S_t *s){
  
  /* Find the Heading, Pitch and Roll values */

  
  
  /* Validate Checksum */
  uint8_t len = strlen(str);
  uint8_t idx = 0;
//  uint8_t starIdx = 0;
  unsigned char chksum = 0;
  do{
      if(str[idx] == '$') {
          chksum = 0;
      }
      else if(str[idx] == '*'){
//          starIdx = idx;
          break;
      } else {
          chksum ^= str[idx];
      }
  }while(++idx<len);

  
  char chkVal[8];
  sprintf(chkVal,"%x",chksum);

//  char *tempIdx = 0;
//  uint8_t tempLen = 0;
  
  char *token;
  char tmp_chksum[8];
  token = strtok(str,"*");
  token = strtok(NULL,"\r\n");
  

  
  strcpy(tmp_chksum,token);
//  tempIdx = strchr(&str[0],'\r');   /* Find the CR that terminatest the line */
//  tempLen = tempIdx - &str[starIdx+1];
  
  
//  memcpy(tmp_chksum,&str[starIdx],tempLen-1);
//  
//  printf("tmp_chksum = %s\n",tmp_chksum);
//  printf("chkVal = %s\n",chkVal);

  for(uint8_t i=0;i<8;i++) {
    tmp_chksum[i] = toupper(tmp_chksum[i]);
    chkVal[i] = toupper(chkVal[i]);
  }
  
  if(strcmp(&chkVal[0],&tmp_chksum[0])!=0) {
    return OS_INVALID_STRING;
  } 
  
  /* Find Heading, Pitch and Roll */
  
  token = strtok(str,"C");
  /* Find heading */
  char heading[16];
  token = strtok(NULL,"P");
  strcpy(heading,token);
  s->heading = atof(heading);
  
  /* Find pitch */
  char pitch[16];
  token = strtok(NULL,"R");
  strcpy(pitch,token);
  s->pitch = atof(pitch);
  
  /* Find roll */
  char roll[16];
  token = strtok(NULL,"*");
  strcpy(roll,token);
  s->roll = atof(roll);
  
  return OS_VALID;
}

