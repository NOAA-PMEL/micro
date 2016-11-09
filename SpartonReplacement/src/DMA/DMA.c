/** @file DMA.C
*  @brief Direct Memory Access (DMA) Functions
*
*  @author Matt Casari, matthew.casari@noaa.org
*  @date October 11, 2016
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

#include <MSP430FR5949.h>
#include "DMA.h"








void DMA_Init(void){
  


  

  
  /* DMA-0 is set on UART A1 Receive */
  DMACTL0 = DMA0TSEL__UCA0RXIFG ;
  
  /* Configure the Source & Destination Addresses */
  __data16_write_addr((unsigned short) &DMA0SA,(unsigned long) &UCA0RXBUF);
  __data16_write_addr((unsigned short) &DMA0DA,(unsigned long) &UCA1TXBUF);
  
  /* DMA 0 Control Register */
  /* DMA Settings: */
  /* - Single Transfer Mode Repeat(DMADT=4) */
  /* - Destination Address is incremented (DMADSTINCR=3) */
  /* - Source Address is unchanged (DMASRCINC = 0) */
  /* - Destination is Byte sized (DMADSTBYTE=1) */
  /* - Source is Word Sized (DMASRCBYTE=0) */
  /* - DMA Level is edge (DMALEVEL=0) */
  /* - Clear DMA Interrupt Flag (DMAIFG = 0) */
  /* - Enable the DMA Interrupt (DMAIE=1) */
  DMA0CTL |=  (DMADT_4 | DMADSTINCR_3 | DMADSTBYTE | DMASRCINCR_0 | DMAIE);
    
  /* Set the Size of the transfer */
  DMA0SZ = 0x01;
  
  /* - Enable the DMA (DMAEN = 1) */
  DMA0CTL |=   DMAEN;
   
  return;
}


void DMA_Start(void) {
  DMA0CTL |= DMAEN;
  return;
}

void DMA_Stop(void) {
  DMA0CTL &= ~DMAEN;
  return;
}

uint16_t DMA_WriteLength(void) {
  return (uint16_t) (DMA_BUF_LENGTH - DMA0SZ);
}
#pragma vector=DMA_VECTOR
__interrupt void DMA0_ISR(void)
{
  
  switch(__even_in_range(DMAIV,16)) {
    case 0: break;
    case 2: 
    /* Disable the DMA */
//      DMA0CTL &= ~DMAEN;
      
      
      /* Enable the DMA */
      DMA0CTL |= DMAEN;
      break;
    case 0x04:
      break;
    default:
      break;
  }
}








