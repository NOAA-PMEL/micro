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
	
	// Set the Clock division to Clock/4
	UCB0BRW = 0x04;				
	    
	// Set the stop condition to auto generate on byte counter
	UCB0CTLW1 |= UCASTP_2;	
	
	// Set Byte Counter for Tx of 1 byte
	UCB0TBCNT = 1;					

	// Release from Reset
	UCB0CTLW0 &= ~UCSWRST;					

	// Set interrupt on STOP and RECEIVE
	UCB0IE = UCSTPIE | UCRXIE;
	return;
}

/***********************************************************************/
void I2CClose(void)
{
	// Hold I2C in reset
	UCB0CTLW0 |= UCSWRST;			
	
	// Make P1.6 & P1.7 GPIO
	P1SEL1 &= ~(BIT6 | BIT7);		
	
	// Set P1.6 & P1.7 as output LOW
	GPIO_SetPinAsOutput(1,6);
	GPIO_SetPinAsOutput(1,7);
	GPIO_ClearPin(1, 6);
	GPIO_ClearPin(1, 7);
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
	
	UCB0I2CSA = slaveAddress;			// Set slave address
	UCB0TBCNT = length;					// Set byte count before Stop bit
	UCB0CTLW0 |= UCTR + UCTXSTT;		// Set Transmit + Start bit

	for(uint8_t i=0;i<length;i++)
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

/*****************************  INTERRUPTS  ********************************/
#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
{

    switch(__even_in_range(UCB0IV, USCI_I2C_UCBIT9IFG))
      {
        case USCI_NONE:
            break;         // Vector 0: No interrupts
        case USCI_I2C_UCALIFG:
            break;         // Vector 2: ALIFG
        case USCI_I2C_UCNACKIFG:                // Vector 4: NACKIFG
            break;
        case USCI_I2C_UCSTTIFG:
            break;         // Vector 6: STTIFG
        case USCI_I2C_UCSTPIFG:
            break;         // Vector 8: STPIFG
        case USCI_I2C_UCRXIFG3:
            break;         // Vector 10: RXIFG3
        case USCI_I2C_UCTXIFG3:
            break;         // Vector 12: TXIFG3
        case USCI_I2C_UCRXIFG2:
            break;         // Vector 14: RXIFG2
        case USCI_I2C_UCTXIFG2:
            break;         // Vector 16: TXIFG2
        case USCI_I2C_UCRXIFG1:
            break;         // Vector 18: RXIFG1
        case USCI_I2C_UCTXIFG1:
            break;         // Vector 20: TXIFG1
        case USCI_I2C_UCRXIFG0:
            pxSensor.data[pxSensor.dataIndex] = UCB0RXBUF;
            pxSensor.dataIndex++;
            break;         // Vector 22: RXIFG0
        case USCI_I2C_UCTXIFG0:                 // Vector 24: TXIFG0
          break;
        default:
            break;
      }

}

