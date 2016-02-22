/** @file UART.c
 *  @brief
 *
 *  @author Matt Casari, matthew.casari@noaa.org
 *  @date Dec 4, 2015
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
#include "uart.h"
/************************************************************************
*					STATIC FUNCTION PROTOTYPES
************************************************************************/
static uint8_t UCBRSLookup( uint32_t clkFreq, uint32_t baudRate, uint16_t *UCBRx, uint16_t *MCTLWReg);
static uint8_t UART_BaudrateIsValid(uint32_t Baudrate);
static uint8_t UART_ClockFrequencyIsValid(uint32_t ClkFreq);

/************************************************************************
*					STATIC VARIABLES
************************************************************************/
/************************************************************************
*					GLOBAL FUNCTIONS
************************************************************************/
uint8_t UART_Init(uint8_t UartPort,uint32_t Baudrate,uint32_t ClkFreq, uint8_t Clk)
{

	uint16_t UCBRx;
	uint16_t MCTLWReg;

	if(UartPort > 1)
	{
		return UART_FAIL;
	}
	if(UART_BaudrateIsValid(Baudrate) == BAUDRATE_FAIL)
	{
		return UART_FAIL;
	}
	if(UART_ClockFrequencyIsValid(ClkFreq) == CLOCK_FAIL)
	{
		return UART_FAIL;
	}
	if(Clk > UART_CLK_SMCLK)
	{
		return UART_FAIL;
	}

	// Configure Pins
	switch(UartPort)
	{
		case UART_A0:
			P2SEL1 |= (UART_A0_TX | UART_A0_RX);
			P2SEL0 &= ~(UART_A0_TX | UART_A0_RX);

			break;
		case UART_A1:
			P2SEL1 |= (UART_A1_TX | UART_A1_RX);
			P2SEL0 &= ~(UART_A1_TX | UART_A1_RX);
			break;
		default:
			// add logging
			break;
	}

	// Configure Port for UART Mode
	switch(UartPort)
	{
		case UART_A0:
			UCA0CTLW0 = UCSWRST;
			UCA0CTLW0 |= UCSSEL__ACLK;
			break;
		case UART_A1:
			UCA1CTLW0 = UCSWRST;
			UCA1CTLW0 |= UCSSEL__ACLK;
			break;
		default:
			break;
	}

	if( (UCBRSLookup( ClkFreq, Baudrate, &UCBRx, &MCTLWReg)) == BAUDRATE_FAIL)
	{
		return UART_FAIL;
	}
	else
	{
		switch(UartPort)
		{
			case UART_A0:
				UCA0BRW = UCBRx;
				UCA0MCTLW = MCTLWReg;
				break;
			case UART_A1:
				UCA1BRW = UCBRx;
				UCA1MCTLW = MCTLWReg;
				break;

			default:
				break;
		}
	}

    UCA1CTLW0 &= ~UCSWRST;                    // Take UART out of reset
    UCA1IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

	return UART_OK;
}


uint8_t UART_Read(uint8_t *value, uint8_t Port)
{
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

uint8_t UART_Write(uint8_t *value, uint8_t length, uint8_t Port)
{
	uint8_t i=0;

	if(Port != UART_A0 && Port != UART_A1)
	{
		return UART_FAIL;
	}
	for(i=0;i<length;i++)
	{
		if(value[i] != 0x00)
		{
			UART_WriteChar(value[i],Port);
            __delay_cycles(5000);
		}
		else
		{
			i=length;
		}
	}

	UART_WriteChar('\r',Port);
    __delay_cycles(5000);
	UART_WriteChar('\n',Port);
    __delay_cycles(5000);
	return UART_OK;
}
/************************************************************************
*					STATIC FUNCTIONS
************************************************************************/
/** @brief Looks up UART Register settings
 *
 *	Look-up function for appropriate UART baudrate register settings
 *
 *  @param clkFreq Peripheral Clock Frequency
 *  @param baudRate UART Baudrate
 *  @param *UCBRx UCBRx variable pointer
 *  @param *MCTLWReg variable pointer
 *
 *
 *  @return status
 */
static uint8_t UCBRSLookup( uint32_t clkFreq, uint32_t baudRate, uint16_t *UCBRx, uint16_t *MCTLWReg){
	uint16_t osval = 0x00u;
	uint16_t ucbrf = 0u;
	uint16_t ucbrs = 0u;
	uint8_t response = BAUDRATE_FAIL;
	switch( clkFreq)
	{
		case CLK_32768:
			switch( baudRate){
				case UART_BAUD_1200:
					osval = 0x01;
					*UCBRx = 1;
					ucbrf = UCBRF_11;
					ucbrs = 0x25;
					break;
				case UART_BAUD_2400:
					osval = 0x00;
					*UCBRx = 13;
					ucbrf = UCBRF_0;
					ucbrs = 0xB6;
					break;
				case UART_BAUD_4800:
					osval = 0x00;
					*UCBRx = 6;
					ucbrf = UCBRF_0;
					ucbrs = 0xEE;
					break;
				case UART_BAUD_9600:
					osval = 0x00u;
					*UCBRx = 3;
					ucbrf = UCBRF_0;
					ucbrs = 0x92;
					break;
				default:
//					// Log error
					response = BAUDRATE_FAIL;
					break;
			}
			break;
		case CLK_1000000:
			switch( baudRate){
				case UART_BAUD_9600:
					osval = 1;
					*UCBRx = 6;
					ucbrf = 8;
					ucbrs = 0x20;
					break;
				case UART_BAUD_19200:
					osval = 1;
					*UCBRx = 3;
					ucbrf = 4;
					ucbrs = 0x2;
					break;
				case UART_BAUD_38400:
					osval = 1;
					*UCBRx = 1;
					ucbrf = 10;
					ucbrs = 0x00;
					break;
				case UART_BAUD_57600:
					osval = 0;
					*UCBRx = 17;
					ucbrf = 0;
					ucbrs = 0x4A;
					break;
				case UART_BAUD_115200:
					osval = 0;
					*UCBRx = 8;
					ucbrf = 0;
					ucbrs = 0xD6;
					break;
				default:
					// Log error
					response = BAUDRATE_FAIL;
					break;
			}
			break;
		case CLK_1048576:
			switch( baudRate){
				case UART_BAUD_9600:
					osval = 1;
					*UCBRx = 6;
					ucbrf = 13;
					ucbrs = 0x22;
					break;
				case UART_BAUD_19200:
					osval = 1;
					*UCBRx = 3;
					ucbrf = 6;
					ucbrs = 0xAD;
					break;
				case UART_BAUD_38400:
					osval = 1;
					*UCBRx = 1;
					ucbrf = 11;
					ucbrs = 0x25;
					break;
				case UART_BAUD_57600:
					osval = 0;
					*UCBRx = 18;
					ucbrf = 0;
					ucbrs = 0x11;
					break;
				case UART_BAUD_115200:
					osval = 0;
					*UCBRx = 9;
					ucbrf = 0;
					ucbrs = 0x08;
					break;
				default:
					// Log error
					response = BAUDRATE_FAIL;
					break;
			}
			break;
		case CLK_4000000:
			switch( baudRate){
				case UART_BAUD_9600:
					osval = 1;
					*UCBRx = 26;
					ucbrf = 0;
					ucbrs = 0xB6;
					break;
				case UART_BAUD_19200:
					osval = 1;
					*UCBRx = 13;
					ucbrf = 0;
					ucbrs = 0x84;
					break;
				case UART_BAUD_38400:
					osval = 1;
					*UCBRx = 6;
					ucbrf = 8;
					ucbrs = 0x20;
					break;
				case UART_BAUD_57600:
					osval = 1;
					*UCBRx = 4;
					ucbrf = 5;
					ucbrs = 0x55;
					break;
				case UART_BAUD_115200:
					osval = 1;
					*UCBRx = 2;
					ucbrf = 2;
					ucbrs = 0xBB;
					break;
				case UART_BAUD_230400:
					osval = 0;
					*UCBRx = 17;
					ucbrf = 0;
					ucbrs = 0x4A;
					break;

				default:
					// Log error
					response = BAUDRATE_FAIL;
					break;
			}
			break;
		case CLK_4194304:
			switch( baudRate){
				case UART_BAUD_9600:
					osval = 1;
					*UCBRx = 27;
					ucbrf = 4;
					ucbrs = 0xFB;
					break;
				case UART_BAUD_19200:
					osval = 1;
					*UCBRx = 13;
					ucbrf = 10;
					ucbrs = 0x55;
					break;
				case UART_BAUD_38400:
					osval = 1;
					*UCBRx = 6;
					ucbrf = 13;
					ucbrs = 0x22;
					break;
				case UART_BAUD_57600:
					osval = 1;
					*UCBRx = 4;
					ucbrf = 8;
					ucbrs = 0xEE;
					break;
				case UART_BAUD_115200:
					osval = 1;
					*UCBRx = 2;
					ucbrf = 4;
					ucbrs = 0x92;
					break;
				case UART_BAUD_230400:
					osval = 0;
					*UCBRx = 18;
					ucbrf = 0;
					ucbrs = 0x11;
					break;

				default:
					// Log error
					response = BAUDRATE_FAIL;
					break;
			}
			break;
		case CLK_8000000:
			switch( baudRate){
				case UART_BAUD_9600:
					osval = UCOS16;
					*UCBRx = 52;
					ucbrf = UCBRF_1;
					ucbrs = 0x49;
					//ucbrs = 0;
                    break;
				case UART_BAUD_19200:
					osval = 1;
					*UCBRx = 26;
					ucbrf = 0;
					ucbrs = 0xB6;
					break;
				case UART_BAUD_38400:
					osval = 1;
					*UCBRx = 6;
					ucbrf = 0;
					ucbrs = 0x84;
					break;
				case UART_BAUD_57600:
					osval = 1;
					*UCBRx = 4;
					ucbrf = 10;
					ucbrs = 0xF7;
					break;
				case UART_BAUD_115200:
					osval = 1;
					*UCBRx = 2;
					ucbrf = 5;
					ucbrs = 0x55;
					break;
				case UART_BAUD_230400:
					osval = 0;
					*UCBRx = 18;
					ucbrf = 2;
					ucbrs = 0xBB;
					break;
				case UART_BAUD_460800:
					osval = 0;
					*UCBRx = 18;
					ucbrf = 0;
					ucbrs = 0x4A;
					break;
				default:
					// Log error
					response = BAUDRATE_FAIL;
					break;
			}
			break;
		default:
			// Log error
			break;

	}

    // Shift the UCBR second byte to left by 8 bits for register
	ucbrs = (ucbrs << 8);

    // Clear and populate the Modulation Control Register
	*MCTLWReg = 0x0000;
	*MCTLWReg = ucbrf | ucbrs | osval;
	if(response == BAUDRATE_FAIL)
	{
		if( MCTLWReg == 0 && UCBRx == 0)
		{
			return BAUDRATE_FAIL;
		}
	}
    

	return BAUDRATE_OK;

}


/** @brief Valid Baudrate test
 *
 *	Test to see if baudrate passed is a valid baudrate
 *
 *  @param Baudrate UART baudrate
 *
 *  @return status
 */
static uint8_t UART_BaudrateIsValid(uint32_t Baudrate)
{
	uint8_t response = BAUDRATE_FAIL;
	switch(Baudrate)
	{
		case UART_BAUD_1200:
			response = BAUDRATE_OK;
			break;
		case UART_BAUD_2400:
			response = BAUDRATE_OK;
			break;
		case UART_BAUD_4800:
			response = BAUDRATE_OK;
			break;

		case UART_BAUD_9600:
			response = BAUDRATE_OK;
			break;
		case UART_BAUD_14400:
			response = BAUDRATE_OK;
			break;
		case UART_BAUD_19200:
			response = BAUDRATE_OK;
			break;
		case UART_BAUD_28800:
			response = BAUDRATE_OK;
			break;
		case UART_BAUD_38400:
			response = BAUDRATE_OK;
			break;
		case UART_BAUD_57600:
			response = BAUDRATE_OK;
			break;
		case UART_BAUD_115200:
			response = BAUDRATE_OK;
			break;
		case UART_BAUD_230400:
			response = BAUDRATE_OK;
			break;
		case UART_BAUD_460800:
			response = BAUDRATE_OK;
			break;
		default:
			response = BAUDRATE_FAIL;
			break;
	}

	return response;
}

/** @brief Valid Clock Frequency test
 *
 *	Test to see if clock frequency passed is valid
 *
 *  @param ClkFreq
 *
 *  @return status
 */
static uint8_t UART_ClockFrequencyIsValid(uint32_t ClkFreq)
{
	uint8_t response = CLOCK_FAIL;
	switch(ClkFreq)
	{
		case CLK_32768:
			response = CLOCK_OK;
			break;
		case CLK_1000000:
			response = CLOCK_OK;
			break;
		case CLK_1048576:
			response = CLOCK_OK;
			break;
		case CLK_4000000:
			response = CLOCK_OK;
			break;
		case CLK_4194304:
			response = CLOCK_OK;
			break;
		case CLK_8000000:
			response = CLOCK_OK;
			break;
		default:
			response = CLOCK_FAIL;
			break;
	}

	return response;
}

/************************************************************************
*					INTERRUPT VECTOR
************************************************************************/
#ifdef PMEL
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
	switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG) )
	{
		case USCI_NONE:
			break;
		case USCI_UART_UCRXIFG:
			//SystemState = BUFFER_RECEIVE;
			//__bis_SR_register(GIE);
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

#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
	switch(__even_in_range(UCA1IV, USCI_UART_UCTXCPTIFG))
	{
		case USCI_NONE:
			break;
		case USCI_UART_UCRXIFG:
          if( UCA1RXBUF =='D' || UCA1RXBUF == 'd')
					{
						//UCA1TXBUF = 'D';
						SystemState = Compute;
					}
					
          //FET_TOGGLE();
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
#endif
