/** @file spi.h
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
#include "spi.h"

/************************************************************************
*					GLOBAL FUNCTIONS
************************************************************************/
uint8_t SPI_Init(void) 
{ 
  /* Configure GPIO selection bits for SPI on B0 */
  P1SEL1 |= BIT6 | BIT7;    /* MISO & MOSI Setup */
  P1SEL0 ~= (BIT6 | BIT7);  /* MISO & MOSI Setup */
  P2SEL1 |= BIT2;           /* CLK Setup */
  P2SEL0 ~= BIT2;           /* CLK Setup */
  
  /*  Set the Shutdown Line Low to keep 
  /* Initialize the SPI for Master mode, Clock High Polarity, 250kHz, */
  /* Most Significant Bit First, ACLK source, 3-Wire mode */
  UCB0CTLW0 = UCSWRST;  /*  Put SPI in reset to set register */
  UCB0CTLW0 |= UCMST | UCSYNC | UCCKPL | UCMSB; 
  UCB0CTLW0 |= UCSSEL__ACLK;    
  
  /* Baud Rate Generator */
  /* ACLK is 1MHz, Divide by 4 for 250kHz */
  UCBOBR0 = 0x04;
  UCB0BR1 = 0x00;
  
  /* No modulation */
  UCB0MCTLW = 0;
  
  /* 
  
}