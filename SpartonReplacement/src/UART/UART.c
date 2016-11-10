/** @file UART.c
 *  @brief
 *
 *  @author Matt Casari, matthew.casari@noaa.org
 *  @date March 23, 2016
 *  @version 0.1.1
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *	@note
 *
 *  @bug  No known bugs
 */
#include "uart.h"

__persistent UART_t OS_UART;                /** OS5000S UART Struct */
__persistent UART_t *pOS_UART = &OS_UART;   /** OS5000S UART Struct Pointer */
__persistent UART_t FL_UART;                /** FLEX UART Struct */
__persistent UART_t *pFL_UART = &FL_UART;              /** FLEX UART Struct Pointer */

/************************************************************************
*					STATIC FUNCTION PROTOTYPES
************************************************************************/
static void UART_ClearStruct_FLEX(void);
static void UART_ClearStruct_OS(void);

/************************************************************************
*					GLOBAL FUNCTIONS
************************************************************************/
void TFLEX_UART_Init(void) {
  
  /* Clear the struct */
  UART_ClearStruct_FLEX();
  
  /* Put USCI_A1 in Reset */
  UCA1CTLW0 = UCSWRST;

    
  /* Set up the UART Pins */
  P2SEL1 |= (UART_A1_TX | UART_A1_RX);
  P2SEL0 &= ~(UART_A1_TX | UART_A1_RX);
  
  
  /* Set UART for 9600 Baud, using ACLK */
  UCA1CTLW0 |= UCSSEL__ACLK;    
  UCA1BRW = 3;
  UCA1MCTLW = (uint16_t)(0x92 << 8);
  
  /* Set up interrupts */
  UCA1CTLW0 &= ~UCSWRST;            /* Take UART out of reset */
  UCA1IE |=  UCRXIE;                /* Enable USCI_A1 RX interrupt */
  
  return;
}

void OS5000S_UART_Init(void) {
  
  /* Clear the Struct */
  UART_ClearStruct_OS();
  /* Put USCI_A1 in Reset */
  UCA0CTLW0 = UCSWRST;

    
  /* Set up the UART Pins */
  P2SEL1 |= (UART_A0_TX | UART_A0_RX);
  P2SEL0 &= ~(UART_A0_TX | UART_A0_RX);
  
  
  /* Set UART for 9600 Baud, using ACLK */
  UCA0CTLW0 |= UCSSEL__ACLK;    
  UCA0BRW = 3;
  UCA0MCTLW = (uint16_t)(0x92 << 8);
  
  /* Set up interrupts */
  UCA0CTLW0 &= ~UCSWRST;            /* Take UART out of reset */
  UCA0IE |=  UCRXIE |UCTXIE;                /* Enable USCI_A1 RX interrupt */
//  UCA0IE |= UCTXIE;
  return;
}

void OS5000S_UART_Halt(void) {
  UCA0IE &= ~(UCRXIE);
  
}

void OS5000S_Attach_Rx_Interrupt(void) {
  
  UCA0IE |= UCRXIE;
}

uint8_t UART_Read(uint8_t *value, uint8_t Port)
{
    /* Check for valid Port # */
	if(Port > 1)
	{
		return UART_FAIL;
	}
    
    /* Read selected port */
	switch(Port)
	{
		case UART_A0:
			*value = UCA0RXBUF & 0x00FF;
			break;
		case UART_A1:
			*value = UCA1RXBUF & 0x00FF;
			break;
		default:
			return UART_FAIL;
			break;
	}

	return UART_OK;
}

uint8_t UART_WriteChar(uint8_t value, uint8_t Port)
{
    /* Check for valid Port # */
	if(Port > 1)
	{
		return UART_FAIL;
	}
    
    /* Write value to selected port */
	switch(Port)
	{
		case UART_A0:
			UCA0TXBUF = (uint16_t) value;
			break;
		case UART_A1:
			UCA1TXBUF = value;
			break;
		default:
			return UART_FAIL;
			break;
	}
	return UART_OK;
}

uint8_t UART_Write(uint8_t *value, uint16_t length, uint8_t Port)
{
	uint16_t i=0;
    
    /* Check for valid Port # */
	if(Port != UART_A0 && Port != UART_A1)
	{
		return UART_FAIL;
	}
    
    /* Write the character array to the selected port until \0 reached*/
	for(i=0;i<length;i++)
	{
		if(value[i] != 0x00)
		{
			UART_WriteChar(value[i],Port);

            while(UCA1STATW & UCBUSY);
		}
		else
		{
			i=length;
		}
	}
	
	return UART_OK;
}

uint8_t UART_WriteIncludeNull(uint8_t *value, uint8_t length, uint8_t Port)
{
  	uint8_t i=0;
    /* Check for valid Port # */
	if(Port != UART_A0 && Port != UART_A1)
	{
		return UART_FAIL;
	}
    
    /* Write all values character array to the selected port */
	for(i=0;i<length;i++)
	{
      UART_WriteChar(value[i],Port);
      __delay_cycles(25000);
	}
	
	return UART_OK;
  
}

uint8_t UART_WriteACK(uint8_t Port) {
  /* Check for valid Port # */
  if(Port != UART_A0 && Port != UART_A1)
  {
      return UART_FAIL;
  }
  
  UART_WriteChar(0x06, Port);
  
  return UART_OK;
}

uint8_t UART_WriteNACK(uint8_t Port) {
  /* Check for valid Port # */
  if(Port != UART_A0 && Port != UART_A1)
  {
      return UART_FAIL;
  }
  
  UART_WriteChar(0x15,Port);
  
  return UART_OK;
}

uint8_t UART_WriteNewline(uint8_t Port) {
#ifdef CARRIAGE_RETURN
  UART_WriteChar('\r',Port);
#endif
  while(UCA1STATW & UCBUSY);
  UART_WriteChar('\n',Port);
  while(UCA1STATW & UCBUSY);
  
  return UART_OK;
}
/************************************************************************
*					STATIC FUNCTIONS
************************************************************************/
static void UART_ClearStruct_FLEX(void) {
//  FL_UART.CancelCnt = 0;
//  FL_UART.Cancel
  FL_UART.Timer = pFLTimer;
  BufferC_Clear(&FL_UART.Buffer);
  
  return;
}

static void UART_ClearStruct_OS(void) {
  OS_UART.Timer = pOSTimer;
  BufferC_Clear(&OS_UART.Buffer);
  
  return;
}

/************************************************************************
*					INTERRUPT VECTOR
************************************************************************/


/* A0 Vector is Ocean Server OS5000-S UART */
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
    uint8_t val;
	switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
	{
		case USCI_NONE:
			break;
		case USCI_UART_UCRXIFG:  
          /* Retreive buffer */
          val = UCA0RXBUF;
          BufferC_Put(&OS_UART.Buffer,val);
          break;
		case USCI_UART_UCTXIFG:
            
			break;
		case USCI_UART_UCSTTIFG:
			break;
		case USCI_UART_UCTXCPTIFG:
			break;
		default:
			break;
	}
}

/* A1 Vector is FLEX UART */
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
    uint8_t val;
	switch(__even_in_range(UCA1IV, USCI_UART_UCTXCPTIFG))
	{
		case USCI_NONE:
			break;
		case USCI_UART_UCRXIFG:  
          /* Retreive buffer */
          BufferC_Put(&FL_UART.Buffer,UCA1RXBUF);
            break;
		case USCI_UART_UCTXIFG:
            UCA1IFG &= ~(UCTXIFG);
			break;
		case USCI_UART_UCSTTIFG:
			break;
		case USCI_UART_UCTXCPTIFG:
			break;
		default:
			break;
	}
}



