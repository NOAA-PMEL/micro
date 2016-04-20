/** @file MAX1247.c
 *  @brief
 *
 *  @author Matt Casari, matthew.casari@noaa.org
 *  @date 
 *  @version 
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *  @note
 *
 *  @bug  No known bugs
 */
#include "MAX1247.h"


const uint8_t MAX1247_CHAN0_START = 0x9E;
const uint8_t MAX1247_CHAN1_START = 0xDE;
const uint8_t MAX1247_CHAN2_START = 0xAE;
const uint8_t MAX1247_CHAN3_START = 0xEE;
const uint8_t MAX1247_FAST_SHUTDOWN = 0x81;
const uint8_t MAX1247_FULL_SHUTDOWN = 0x80;
const uint8_t MAX1247_DUMMY = 0x00;

uint8_t MAX1247_Init(void) 
{
  
  /*  Set \SHDN pin to ouput and set LOW */
  MAX1247_SHDN_ON();
  MAX1247_SHDN_INIT();

  /*  Set GPIO Pins to inputs fro \CS and STRB */
  MAX1247_CS_INIT();
  MAX1247_STRB_INIT();
  
  /*  Initialize the SPI  */ 
  SPI_Init();
  
  return MAX1247_VALID;
}

uint8_t MAX1247_Read(uint16_t *value)
{
  uint8_t byte_h = 0;
  uint8_t byte_l = 0;
  uint16_t data = 0;
  
  /* Retreive data for high & low byte */
  for(uint8_t i=0;i<2;i++)
  {
    /* Clear Timeout Timer */
    MAX1247_TimeoutTimer = 0;
    
    /* Write 0's to SPI bus to read value */
    SPI_Write(MAX1247_DUMMY);
    
    /* Wait for data */
    while( MAX1247_STRB_READ() != 1)
    {
      /* If timeout reach, exit with fail status */
      if(MAX1247_TimeoutTimer >= MAX1247_TIMEOUT_VALUE )
      {
        return MAX1247_INVALID;
      }
    }
    
    /* Retreive data from buffer */
    if(i==0)
    {
      byte_h = UCB0RXBUF;
    } else {
      byte_l = UCB0RXBUF;
    }
  }
  
  /* Convert data, combine high & low byte, then right shift by 4 for 12-bit data*/
  data = (uint16_t) byte_h;
  data = data << 8;
  data += (uint16_t) byte_l;
  data = data >> 4;
  
  /* transfer data */
  *value = data;
  
  return MAX1247_VALID;
}

uint8_t MAX1247_Start(uint8_t channel)
{
  /*  Check for valid channel number */
  if( channel > (MAX1247_NUM_CHANNELS - 1) )
  {
    return MAX1247_INVALID;
  }
  
  /*  Write Control Word to SPI  */
  switch(channel)
  {
    case 0: 
      SPI_Write(MAX1247_CHAN0_START);
      break;
    case 1:
      SPI_Write(MAX1247_CHAN1_START);
      break;
    case 2:
      SPI_Write(MAX1247_CHAN2_START);
      break;
    case 3:
      SPI_Write(MAX1247_CHAN3_START);
      break;
    default:
      break;
  }
  
  return MAX1247_VALID;
}