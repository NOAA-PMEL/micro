/** @file GPIO.c
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

/************************************************************************
*					STATIC FUNCTION PROTOTYPES
************************************************************************/
#include "gpio.h"
/************************************************************************
*					STATIC VARIABLES
************************************************************************/

/************************************************************************
*					GLOBAL FUNCTIONS
************************************************************************/
uint8_t GPIO_Init(void)
{
	P1OUT = 0;
	P2OUT = 0;
	P3OUT = 0;
	P4OUT = 0;

	P1DIR = 0;
	P2DIR = 0;
	P3DIR = 0;
	P4DIR = 0;

	P1SEL0 = 0;
	P2SEL0 = 0;
	P3SEL0 = 0;
	P4SEL0 = 0;

	P1SEL1 = 0;
	P2SEL1 = 0;
	P3SEL1 = 0;
	P4SEL1 = 0;

	P1REN = 0;
	P2REN = 0;
	P3REN = 0;
	P4REN = 0;

	P1IN = 0;
	P2IN = 0;
	P3IN = 0;
	P4IN = 0;

	return (GPIO_OK);
}

uint8_t GPIO_SetPinAsOutput(uint8_t Port, uint8_t Pin)
{
	if(Port < 1 || Port > 4)
	{
		return (GPIO_FAIL);
	}

	if(Pin > 15)
	{
		return (GPIO_FAIL);
	}

	switch(Port)
	{
		case GPIO_PORTP1:
			P1DIR |= BIT_TO_MASK(Pin);
			break;
		case GPIO_PORTP2:
			P2DIR |= BIT_TO_MASK(Pin);
			break;
		case GPIO_PORTP3:
			P3DIR |= BIT_TO_MASK(Pin);
			break;
		case GPIO_PORTP4:
			P4DIR |= BIT_TO_MASK(Pin);
			break;
		default:
			break;

	}


	return (GPIO_OK);
}


uint8_t GPIO_SetPinAsInput( uint8_t Port, uint16_t Pin )
{
	if(Port < 1 || Port > 4)
	{
		return (GPIO_FAIL);
	}

	switch(Port)
	{
		case GPIO_PORTP1:
			P1DIR &= ~BIT_TO_MASK(Pin);
			break;
		case GPIO_PORTP2:
			P2DIR &= ~BIT_TO_MASK(Pin);
			break;
		case GPIO_PORTP3:
			P3DIR &= ~BIT_TO_MASK(Pin);
			break;
		case GPIO_PORTP4:
			P4DIR &= ~BIT_TO_MASK(Pin);
			break;
		default:
			break;

	}

	return (GPIO_OK);
}

uint8_t GPIO_SetPin(uint8_t Port, uint8_t Pin)
{

	if(Port < 1 || Port > 4)
	{
		return (GPIO_FAIL);
	}

	if( Pin > 16)
	{
		return (GPIO_FAIL);
	}


	switch(Port)
	{
		case GPIO_PORTP1:
			P1OUT |= ( BIT_TO_MASK(Pin) & P1DIR );
			break;
		case GPIO_PORTP2:
			P2OUT |= ( BIT_TO_MASK(Pin) & P2DIR );
			break;
		case GPIO_PORTP3:
			P3OUT |= ( BIT_TO_MASK(Pin) & P3DIR );
			break;
		case GPIO_PORTP4:
			P4OUT |= ( BIT_TO_MASK(Pin) & P4DIR );
			break;
		default:
			break;
	}

	return (GPIO_OK);
}


uint8_t GPIO_ClearPin(uint8_t Port, uint8_t Pin)
{
	if(Port < 1 || Port > 4)
	{
		return (GPIO_FAIL);
	}

	if( Pin > 16)
	{
		return (GPIO_FAIL);
	}

	switch(Port)
	{
		case GPIO_PORTP1:
			if(P1DIR & BIT_TO_MASK(Pin))
			{
				P1OUT &= ~BIT_TO_MASK(Pin);
			}
			break;

		case GPIO_PORTP2:
			if(P2DIR & BIT_TO_MASK(Pin))
			{
				P2OUT &= ~BIT_TO_MASK(Pin);
			}
			break;
		case GPIO_PORTP3:
			if(P3DIR & BIT_TO_MASK(Pin))
			{
				P3OUT &= ~BIT_TO_MASK(Pin);
			}
			break;

		case GPIO_PORTP4:
			if(P4DIR & BIT_TO_MASK(Pin))
			{
				P4OUT &= ~BIT_TO_MASK(Pin);
			}
		break;

		default:
			break;

	}
	return (GPIO_OK);
}

uint8_t GPIO_SetAllPortOutputPinsLow(uint8_t Port)
{
	if(Port < 1 || Port > 4)
	{
		return (GPIO_FAIL);
	}
	switch(Port)
	{
		case GPIO_PORTP1:
			P1OUT &= ~P1DIR;
			break;
		case GPIO_PORTP2:
			P2OUT &= ~P2DIR;
			break;
		case GPIO_PORTP3:
			P3OUT &= ~P3DIR;
			break;
		case GPIO_PORTP4:
			P4OUT &= ~P4DIR;
			break;
	}
	return (GPIO_OK);
}

uint8_t GPIO_SetAllPortOutputPinsHigh(uint8_t Port)
{
	if(Port < 1 || Port > 4)
	{
		return (GPIO_FAIL);
	}

	switch(Port)
	{
		case GPIO_PORTP1:
			P1OUT |= P1DIR;
			break;
		case GPIO_PORTP2:
			P2OUT |= P2DIR;
			break;
		case GPIO_PORTP3:
			P3OUT |= P3DIR;
			break;
		case GPIO_PORTP4:
			P4OUT |= P4DIR;
			break;
		default:
			break;
	}
	return (GPIO_OK);
}

int8_t GPIO_ReadInputPin( uint8_t Port, uint8_t Pin)
{
	int response;
	if( Port < 1 || Port > 4 )
	{
		return (GPIO_PORT_FAIL);
	}

	switch(Port)
	{
		case GPIO_PORTP1:
			response = (P1IN & BIT_TO_MASK(Pin));
			break;
		case GPIO_PORTP2:
			response = (P2IN & BIT_TO_MASK(Pin));
			break;
		case GPIO_PORTP3:
			response = (P3IN & BIT_TO_MASK(Pin));
			break;
		case GPIO_PORTP4:
			response = (P4IN & BIT_TO_MASK(Pin));
			break;
		default:
			break;
	}
	return (response && BIT_TO_MASK(Pin));
}

int32_t GPIO_ReadInputPort(uint8_t Port)
{
	int32_t response = 0;

	if( Port < 1 || Port > 4)
	{
		return (GPIO_PORT_FAIL);
	}

	switch(Port)
	{
		case GPIO_PORTP1:
			response = (int32_t) (P1IN & ~P1DIR);
			break;
		case GPIO_PORTP2:
			response = (int32_t) (P2IN & ~P2DIR);
			break;
		case GPIO_PORTP3:
			response = (int32_t) (P3IN & ~P3DIR);
			break;
		case GPIO_PORTP4:
			response = (int32_t) (P4IN & ~P4DIR);
			break;
		default:
			break;
	}

	return response;
}


uint8_t GPIO_TogglePin(uint8_t Port, uint8_t Pin)
{
	if( Port < 1 || Port > 4)
	{
		return (GPIO_FAIL);
	}

	if(Pin > 15)
	{
		return (GPIO_FAIL);
	}

	switch(Port)
	{
		case GPIO_PORTP1:
			P1OUT = P1DIR & (P1OUT ^ BIT_TO_MASK(Pin));
			break;
		case GPIO_PORTP2:
			P2OUT = P2DIR & (P2OUT ^ BIT_TO_MASK(Pin));
			break;
		case GPIO_PORTP3:
			P3OUT = P3DIR & (P3OUT ^ BIT_TO_MASK(Pin));
			break;
		case GPIO_PORTP4:
			P4OUT = P4DIR & (P4OUT ^ BIT_TO_MASK(Pin));
			break;
		default:
			break;
	}

	return GPIO_OK;
}


uint8_t GPIO_AttachInputInterrupt(uint8_t gpioPort, uint16_t gpioPin, uint8_t edge)
{
	uint8_t response = GPIO_OK;
	if(gpioPort < 1 || gpioPort > 4)
	{
		return GPIO_FAIL;
	}

	if(gpioPin > 15)
	{
		return GPIO_FAIL;
	}

	switch(gpioPort)
	{
		case GPIO_PORTP1:
			// Clear Interrupt Flag
			P1IFG = 0;
			// Set edge for interrupt
			if(edge == 0)
			{
				P1IES &= ~BIT_TO_MASK(gpioPin);
			}
			else
			{
				P1IES |= BIT_TO_MASK(gpioPin);
			}
			// Set Interrupt
			P1IE |= (BIT_TO_MASK(gpioPin) & ~P1DIR);
			break;
		case GPIO_PORTP2:
			// Clear Interrupt Flag
			P2IFG = 0;
			// Set edge for interrupt
			if(edge == 0)
			{
				P2IES &= ~BIT_TO_MASK(gpioPin);
			}
			else
			{
				P2IES |= BIT_TO_MASK(gpioPin);
			}
			// Set Interrupt
			P2IE |= (BIT_TO_MASK(gpioPin) & ~P2DIR);
			break;
		case GPIO_PORTP3:
			// Clear Interrupt Flag
			P3IFG = 0;
			// Set edge for interrupt
			if(edge == 0)
			{
				P3IES &= ~BIT_TO_MASK(gpioPin);
			}
			else
			{
				P3IES |= BIT_TO_MASK(gpioPin);
			}
			// Set Interrupt
			P3IE |= (BIT_TO_MASK(gpioPin) & ~P3DIR);
			break;
		case GPIO_PORTP4:
			// Clear Interrupt Flag
			P4IFG = 0;
			// Set edge for interrupt
			if(edge == 0)
			{
				P4IES &= ~BIT_TO_MASK(gpioPin);
			}
			else
			{
				P4IES |= BIT_TO_MASK(gpioPin);
			}
			// Set Interrupt
			P4IE |= (BIT_TO_MASK(gpioPin) & ~P4DIR);
			break;
		default:
			response = GPIO_FAIL;
			// Log error
			break;
	}

	return response;
}

uint8_t GPIO_DetachInputInterrupt(uint8_t gpioPort, uint16_t gpioPin)
{
	if(gpioPort < 1 || gpioPort > 4)
	{
		return GPIO_FAIL;
	}

	if(gpioPin > 15)
	{
		return GPIO_FAIL;
	}

	switch(gpioPort)
	{
		case GPIO_PORTP1:
			P1IE &= ~BIT_TO_MASK(gpioPin);
			break;
		case GPIO_PORTP2:
			P2IE &= ~BIT_TO_MASK(gpioPin);
			break;
		case GPIO_PORTP3:
			P3IE &= ~BIT_TO_MASK(gpioPin);
			break;
		case GPIO_PORTP4:
			P4IE &= ~BIT_TO_MASK(gpioPin);
			break;
		default:
			return GPIO_FAIL;

	}
	return GPIO_OK;
}
/************************************************************************
*						MAIN
************************************************************************/

/************************************************************************
*					STATIC FUNCTIONS
************************************************************************/
/** @brief
 *
 *	Add full description here
 *
 *  @param none
 *
 *  @return none
 */




/************************************************************************
*					INTERRUPT VECTOR
************************************************************************/

#ifdef PMEL
#pragma vector = PORT1_VECTOR
__interrupt void Port_1 (void)
{
	switch (__even_in_range(P1IV,P1IV_P1IFG7))
	{
		case P1IV_NONE:
			break;			//Vector 0 - No Interrupt
		case P1IV_P1IFG0:
			break;		// Vector 2 - Interrupt on Pin 1
		case P1IV_P1IFG1:
			__bis_SR_register(GIE);
			SystemState = BUFFER_TRANSMIT;
			break;		// Vector 4 - Interrupt on Pin 1
		case P1IV_P1IFG2:
			break;		// Vector 6 - Interrupt on Pin 1
		case P1IV_P1IFG3:
			break;		// Vector 8 - Interrupt on Pin 1
		case P1IV_P1IFG4:
			break;		// Vector 10 - Interrupt on Pin 1
		case P1IV_P1IFG5:
			break;		// Vector 12 - Interrupt on Pin 1
		case P1IV_P1IFG6:
			break;		// Vector 14 - Interrupt on Pin 1
		case P1IV_P1IFG7:
			break;		// Vector 16 - Interrupt on Pin 1
		default:
			break;		// Default case
	}
}

/**************************** PORT2_VECTOR  *******************************/
#pragma vector = PORT2_VECTOR
__interrupt void Port_2 (void)
{
	switch (__even_in_range(P2IV,P2IV_P2IFG7))
	{
		case P2IV_NONE:
			break;			//Vector 0 - No Interrupt
		case P2IV_P2IFG0:
			break;		// Vector 2 - Interrupt on Pin 1
		case P2IV_P2IFG1:
			break;		// Vector 4 - Interrupt on Pin 1
		case P2IV_P2IFG2:
			break;		// Vector 6 - Interrupt on Pin 1
		case P2IV_P2IFG3:
			break;		// Vector 8 - Interrupt on Pin 1
		case P2IV_P2IFG4:
			break;		// Vector 10 - Interrupt on Pin 1
		case P2IV_P2IFG5:
			break;		// Vector 12 - Interrupt on Pin 1
		case P2IV_P2IFG6:
			break;		// Vector 14 - Interrupt on Pin 1
		case P2IV_P2IFG7:
			break;		// Vector 16 - Interrupt on Pin 1
		default:
			break;		// Default case
	}
}

/**************************** PORT3_VECTOR  *******************************/
#pragma vector = PORT3_VECTOR
__interrupt void Port_3 (void)
{
	switch (__even_in_range(P3IV,P3IV_P3IFG7))
	{
		case P3IV_NONE:
			break;			//Vector 0 - No Interrupt
		case P3IV_P3IFG0:
			break;		// Vector 2 - Interrupt on Pin 1
		case P3IV_P3IFG1:
			break;		// Vector 4 - Interrupt on Pin 1
		case P3IV_P3IFG2:
			break;		// Vector 6 - Interrupt on Pin 1
		case P3IV_P3IFG3:
			break;		// Vector 8 - Interrupt on Pin 1
		case P3IV_P3IFG4:
			break;		// Vector 10 - Interrupt on Pin 1
		case P3IV_P3IFG5:
			break;		// Vector 12 - Interrupt on Pin 1
		case P3IV_P3IFG6:
			break;		// Vector 14 - Interrupt on Pin 1
		case P3IV_P3IFG7:
			break;		// Vector 16 - Interrupt on Pin 1
		default:
			break;		// Default case
	}
}

/**************************** PORT4_VECTOR  *******************************/
#pragma vector = PORT4_VECTOR
__interrupt void Port_4 (void)
{
	switch (__even_in_range(P4IV,P4IV_P4IFG7))
	{
		case P4IV_NONE:
			break;			//Vector 0 - No Interrupt
		case P4IV_P4IFG0:
			break;			// Vector 2 - Interrupt on Pin 1
		case P4IV_P4IFG1:
			break;			// Vector 4 - Interrupt on Pin 1
		case P4IV_P4IFG2:
			break;			// Vector 6 - Interrupt on Pin 1
		case P4IV_P4IFG3:
			break;			// Vector 8 - Interrupt on Pin 1
		case P4IV_P4IFG4:
			break;			// Vector 10 - Interrupt on Pin 1
		case P4IV_P4IFG5:
			break;			// Vector 12 - Interrupt on Pin 1
		case P4IV_P4IFG6:
			break;			// Vector 14 - Interrupt on Pin 1
		case P4IV_P4IFG7:
			break;			// Vector 16 - Interrupt on Pin 1
		default:
			break;		// Default case
	}
}

#endif
