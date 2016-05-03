/** @file DMA.c
 *  @brief
 *
 *  @author Matt Casari, matthew.casari@noaa.org
 *  @date MAY 2, 2016
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

/************************************************************************
*					STATIC FUNCTION PROTOTYPES
************************************************************************/
#include "dma.h"

/************************************************************************
*					GLOBAL FUNCTIONS
************************************************************************/

uint8_t DMA_Init(uint16_t src_addr, uint16_t dest_addr, uint16_t size){
  
  /* Set DMA Source Address */
  DMA0SA = src_addr;
  
  /* Set DMA Destination Address */
  DMA0DA = dest_addr;
  
  /* Set number of bytes to send */
  DMA0SZ = size;
  
  /* Set for Single Transfer, SRC increment, DEST unchanged, send Byte */
  DMA0CTL = DMADT_0 | DMADSTINCR_0 | DMASRCINCR_3 | DMASRCBYTE | DMAIE;
  
  /* Enable DMA */
  DMA0CTL |= DMAEN;

  return 0;
}


uint8_t DMA_SetSourceAddress(uint16_t src_addr)
{
  DMA0SA = src_addr;
  return 0;
}


uint8_t DMA_SetDestinationAddress( uint16_t dest_addr )
{
  DMA0DA = dest_addr;
  
  return 0;
}


uint8_t DMA_Restart(void)
{
  /* Set for Single Transfer, SRC increment, DEST unchanged, send Byte */
  DMA0CTL = DMADT_0 | DMADSTINCR_0 | DMASRCINCR_3 | DMASRCBYTE | DMAIE;
  
  /* Enable DMA */
  DMA0CTL |= DMAEN;
  
  return 0;
}


uint8_t DMA_TriggerBlock(void)
{
  /* Trigger block/byte transfer */
  DMA0CTL |= DMAREQ;                      
  
  return 0;
}

uint8_t DMA_CompleteTransmit(uint8_t channel)
{
  uint8_t response;
  switch(channel)
  {
    case 0:
      response = ((DMAIV & 0x0002) == 0);
      DMAIV &= ~0x0002;
      break;
    case 1:
      response = ((DMAIV & 0x0004) == 0);
      break;
    case 2:
      response = ((DMAIV & 0x0006) == 0);
      break;
    case 3:
      response = ((DMAIV & 0x0008) == 0);
      break;
    case 4:
      response = ((DMAIV & 0x000A) == 0);
      break;
    case 5:
      response = ((DMAIV & 0x00C) == 0);
      break;
    case 6:
      response = ((DMAIV & 0x00E) == 0);
      break;
    case 7:
      response = ((DMAIV & 0x0010) == 0);
      break;
    default:
      break;
  }
  
  return response;
}



/************************************************************************
*					INTERRUPT VECTORS
************************************************************************/
#pragma vector=DMA_VECTOR
__interrupt void DMA_ISR(void)
{
	switch(__even_in_range(DMAIV, DMAIFG) )
	{
		case DMAIV_NONE:
          break;
		case DMAIV_DMA0IFG:
          DMATransmitComplete = true;
          break;
		case DMAIV_DMA1IFG:
          break;
		case DMAIV_DMA2IFG:
          break;
		default:
          break;
	}
}