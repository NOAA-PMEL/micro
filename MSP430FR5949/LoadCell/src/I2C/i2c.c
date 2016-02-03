/** @file i2c.c
 *  @brief I2C driver for MSP430FR5969
 *
 *  @author Matt Casari, matthew.casari@noaa.org
 *  @date Oct 2, 2015
 *  @version A.0.1
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *	@note Tested on MSP430FR5969
 *
 *  @bug  No known bugs
 */

#include "i2c.h"

/************************************************************************
*					STATIC FUNCTION PROTOTYPES
************************************************************************/

/************************************************************************
*					GLOBAL VARIABLES
************************************************************************/

/************************************************************************
*					GLOBAL FUNCTIONS
************************************************************************/

void I2CInit(void)
{
     
	// Initialize I2C for Master Mode, Synchronous, 250kHz
	// and a 1 Byte transmit counters
	P1SEL1 |= BIT6 | BIT7;					// Set P1.6 and P1.7 to I2C (MSP430FR5969 specific)
	UCB0CTLW0 |= UCSWRST;					// Hold in Reset
	UCB0CTLW0 |= UCMODE_3 + UCSYNC + UCMST;	// I2C, Synchronous Mode, Master
	UCB0CTLW0 |= UCSSEL_2;					// SMCLK Used
	UCB0CTLW0 |= UCTR;						// Set for Transmit

	//UCB0BRW = 0x08;							// Clock/8
	//UCB0BRW = 0x02;							// Clock/8
	UCB0BRW = 0x04;
        // Set Byte Counter for Tx of 1 byte
	UCB0CTLW1 |= UCASTP_2;
	UCB0TBCNT = 1;

	UCB0CTLW0 &= ~UCSWRST;					// Release from Reset

	// Set interrupt on STOP and RECEIVE
    UCB0IE = UCSTPIE | UCRXIE0;
	return;
}

/***********************************************************************/
void I2CClose(void)
{
	UCB0CTLW0 |= UCSWRST;			// Hold I2C in reset
	P1SEL1 &= ~(BIT6 | BIT7);		// Make P1.6 & P1.7 GPIO
	// Set P1.6 & P1.7 as output LOW
	GPIO_SetPinAsOutput(GPIO_PORTP1,6);
	GPIO_SetPinAsOutput(GPIO_PORTP1,7);
	GPIO_ClearPin(GPIO_PORTP1, 6);
	GPIO_ClearPin(GPIO_PORTP1, 7);
}

/***********************************************************************/
uint8_t I2CRead(  uint16_t slaveAddress,uint8_t length )
{
	__delay_cycles(200);


	UCB0I2CSA = slaveAddress;
	UCB0CTLW0 &= ~UCTR;		// Put I2C in Rx
	UCB0TBCNT = length;     // Update the Byte Counter length
	UCB0CTLW0 |= UCTXSTT;	// Set the Stop Bit



	return(0);
}



/***********************************************************************/
void I2CWrite( uint16_t slaveAddress, uint16_t *value, uint8_t length )
{
	uint8_t i= 0;
	
	UCB0I2CSA = slaveAddress;			// Set slave address
	UCB0TBCNT = length;					// Set byte count before Stop bit
	UCB0CTLW0 |= UCTR + UCTXSTT;		// Set Transmit + Start bit

	for(i=0;i<length;i++)
	{
		UCB0TXBUF = value[i];				// Set Tx buffer
        __delay_cycles(500);
	}

	return;
}



/************************************************************************
*					STATIC FUNCTIONS
************************************************************************/


/************************************************************************
*					INTERRUPT VECTOR
************************************************************************/



