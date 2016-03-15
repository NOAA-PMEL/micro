#include "SPI.h"



static uint16_t SPI_Calculate_BitRate(uint32_t Baud,uint32_t ClkFreq);


uint8_t SPI_Master_3W_Init(uint8_t Port,uint32_t Baud,uint32_t ClkFreq, uint16_t Clk)
{
    uint16_t BitRateDivisor = 0;
  
  /* Reject Bit Rate higher than Clock Frequency*/
  if(Baud > ClkFreq)
  {
    return SPI_FAIL;
  }
  
  /* Calculate BitRate */
  BitRateDivisor = SPI_Calculate_BitRate(Baud,ClkFreq);
  
  /* Validate CLK selection*/
  if( (Clk != UCSSEL__ACLK ) && ( Clk != UCSSEL__SMCLK ) && ( Clk != UCSSEL__UCLK ))
  {
    return SPI_FAIL;
  }
  
  /* Configure SPI GPI for Port */
  switch(Port)
  {
    case SPI_A0:
      /* Put UCA0 In Reset */
      UCA0CTLW0 = UCSWRST;
      
      /* Put UCA0 in 3-Pin, 8-Bit, Synchronous SPI Master Mode */
      /* No Modulation */
      UCA0CTLW0 = UCMST | UCSYNC | UCCKPL | UCMSB | UCMODE_0 | UCSTEM;
      UCA0BRW = BitRateDivisor;
      UCA0MCTLW = 0; 
      
      /* Initialize USCI State Machine */
      UCA0CTLW0 &= ~UCSWRST;
      
      break;
    case SPI_A1:
      /* Put UCA1 In Reset */
      
      
      /* Put UCA1 in 3-Pin, 8-Bit, Synchronous SPI Master Mode */
      /* No Modulation */
      UCA1CTLW0 = UCMST | UCSYNC | UCCKPL | UCMSB | UCMODE_0 | UCSTEM;
      UCA1BRW = BitRateDivisor;
      UCA1MCTLW = 0; 
      
      /* Initialize USCI State Machine */
      UCA1CTLW0 &= ~UCSWRST;
      
      break;
    case SPI_B0:
      /* Put UCB0 In Reset */
      UCB0CTLW0 = UCSWRST;
      
      /* Put UCB0 in 3-Pin, 8-Bit, Synchronous SPI Master Mode */
      /* No Modulation */
      UCB0CTLW0 |= UCMST | UCSYNC | UCCKPL | UCMSB;
      UCB0CTLW0 |= UCSSEL__SMCLK;
      UCB0BRW = BitRateDivisor;

      /* Initialize USCI State Machine */
      UCB0CTLW0 &= ~UCSWRST;
      
      UCB0IE |= UCRXIE;
      
      break;
    default:
      return SPI_FAIL;
      break;
  }
  

  

  
  return SPI_OK; 
}

uint8_t SPI_Master_4W_Init(uint8_t Port, uint32_t Baud, uint32_t ClkFreq, uint16_t Clk)
{
  return true; 
}

uint8_t SPI_Slave_3W_Init(uint8_t Port, uint32_t Baud, uint32_t ClkFreq, uint16_t Clk)
{
  return true; 
}

uint8_t SPI_Slave_4W_Init(uint8_t Port, uint32_t Baud, uint32_t ClkFreq, uint16_t Clk)
{
  return true; 
}





uint8_t SPI_Master_EnableInterrupts(uint8_t Port,uint8_t TxInt, uint8_t RxInt)
{
  uint16_t Interrupts = 0;
  /* Test Interrupt Values  */
  if( TxInt > 1 )
  {
    return SPI_FAIL;
  }
  
  /* Test Interrupt Values  */
  if( RxInt > 1)
  {
    return SPI_FAIL;
  }
  
  
  /* Ready the Interrupts to Load */
  if(TxInt == true)
  {
    Interrupts |= UCTXIE;
  }
  
  if(RxInt == true)
  {
    Interrupts |= UCRXIE;
  }
  
  /* Apply Interrupts to Register */
  switch(Port)
  {
    case SPI_A0:
      UCA0IE |= Interrupts;
      break;
    case SPI_A1:
      UCA1IE |= Interrupts;
      break;
    case SPI_B0:
      UCB0IE |= Interrupts;
      break;
    default:
      return SPI_FAIL;
      break;
  }
  
  
  return SPI_OK;
}


uint8_t SPI_putc(uint8_t value, uint8_t Port)
{
	switch(Port)
	{
		case SPI_A0:
			UCA0TXBUF = value;
			break;
		case SPI_A1:
			UCA1TXBUF = value;
			break;
		case SPI_B0:
			UCB0TXBUF = value;
			break;
		default:
			return SPI_FAIL;
			break;
	}
	
	return SPI_OK;
}


uint8_t SPI_puts(uint8_t *value,uint16_t length, uint8_t Port)
{
	uint8_t i=0;
	
	for(i=0;i<length;i++)
	{
		switch(Port)
		{
			case SPI_A0:
			case SPI_A1:
			case SPI_B0:
				SPI_putc(value[i],Port);
				break;
			default:
				return SPI_FAIL;
				break;
		}
		
	}
	
	return SPI_OK;
}





static uint16_t SPI_Calculate_BitRate(uint32_t Baud,uint32_t ClkFreq)
{
  uint32_t Divisor = 0;
  if(Baud != 0)
  {
    Divisor = ClkFreq / Baud;
  }
  return (uint16_t) Divisor;
}





#ifdef PMEL

//#pragma vector=USCI_A0_VECTOR
//__interrupt void USCI_A0_ISR(void)
//{
//  switch(__even_in_range(UCA0IV, USCI_SPI_UCTXIFG))
//  {
//    case USCI_NONE: break;
//    case USCI_SPI_UCRXIFG:
//      //RXData = UCA0RXBUF;
//      UCA0IFG &= ~UCRXIFG;
//      __bic_SR_register_on_exit(LPM0_bits); // Wake up to setup next TX
//      break;
//    case USCI_SPI_UCTXIFG:
//      //UCA0TXBUF = TXData;                   // Transmit characters
//      UCA0IE &= ~UCTXIE;
//      break;
//    default: break;
//  }
//}


#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
  switch(__even_in_range(UCA1IV, USCI_SPI_UCTXIFG))
  {
    case USCI_NONE: break;
    case USCI_SPI_UCRXIFG:
      //RXData = UCA1RXBUF;
      UCA1IFG &= ~UCRXIFG;
      __bic_SR_register_on_exit(LPM0_bits); // Wake up to setup next TX
      break;
    case USCI_SPI_UCTXIFG:
      //UCA1TXBUF = TXData;                   // Transmit characters
      UCA1IE &= ~UCTXIE;
      break;
    default: break;
  }
}


#pragma vector=USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
{
  switch(__even_in_range(UCB0IV, USCI_SPI_UCTXIFG))
  {
    case USCI_NONE: break;
    case USCI_SPI_UCRXIFG:
      Buffer8_Put(&Buf8_Input,UCB0RXBUF);
      //RXData = UCB0RXBUF;
      UCB0IFG &= ~UCRXIFG;
      break;
    case USCI_SPI_UCTXIFG:
      UCB0TXBUF = 0xAA;                   // Transmit characters
      UCB0IE &= ~UCTXIE;
      break;
    default: break;
  }
}

#endif