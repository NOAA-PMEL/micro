/** @file i2c.h
 *  @brief Drivers for MSP430FR5969 uart
 *  
 *  @author Matt Casari, matthew.casari@noaa.org
 *  @date September 10, 2015
 *  @version 0.0.1
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab 
 *  @copyright Environmental Development Division
 *  
 *	@note Tested on MSP430FR5969
 *	@note
 *
 *	@todo Need to add a baud rate generator function
 *	@todo Need to add a I2CClose Function
 *  @bug  No known bugs
 */
 
/************************************************************************
*							HEADER FILES
************************************************************************/

#ifndef _I2C_H
#define _I2C_H

/************************************************************************
*							CONSTANTS
************************************************************************/
#ifndef TRUE
#define TRUE	(1)
#define	FALSE	(0)
#endif


#define I2C_DATA_LENGTH	(5)

// UART Pin Definitions
#define I2C_SCL		(BIT7)
#define I2C_SDA		(BIT6)

// I2C Data Transmit Length
#define I2C_TX_BUFFER_LENGTH	(2)
#define I2C_RX_BUFFER_LENGTH	(6)
// 
#define BAUDRATE_PASS	(1)
#define BAUDRATE_FAIL	(0)


// BAUD RATES
#define I2C_BAUD_25000		(25000)
#define I2C_BAUD_50000		(50000)
#define I2C_BAUD_100000		(100000)
#define I2C_BAUD_400000		(400000)
#define	I2C_BAUD_1000000	(1000000)
#define I2C_BAUD_3400000	(3400000)


/************************************************************************
*						STANDARD LIBRARIES		
************************************************************************/
#include <stdint.h>
#include <msp430fr5969.h>
#include "../includes.h"


extern volatile uint8_t sensorCount;
/************************************************************************
*					GLOBAL FUNCTION PROTOTYPES
************************************************************************/
/** @brief Initializes the MSP430FR5969 I2C
 *
 *	Configure the designated I2C Port 
 *
 *
 *  @param i2cBaudRate The I2C Baud rate to set
 *  @param clkFreq The clock frequency that the I2C is running on
 *  @param msglength The length of the I2C message responses
 *
 *  @return Void
 */
void I2CInit(void);

/** @brief Closes the MSP430FR5969 I2C
 *
 *	Close the I2C Port and release pins
 *
 *
 *  @param i2cBaudRate The I2C Baud rate to set
 *  @param clkFreq The clock frequency that the I2C is running on
 *  @param msglength The length of the I2C message responses
 *
 *  @return Void
 */
void I2CClose(void);

/** @brief Write the I2C bus
 *
 *	Configure the designated I2C Port 
 *
 *
 *  @param slaveAddress	Address of I2C Slave Device
 *  @param *value Pointer to Values being sent
 *  @param length The length of the values @pointer
 *
 *  @return Void
 */
void I2CWrite( uint16_t slaveAddress, uint16_t *value, uint8_t length );

/** @brief Read The I2C bus
 *
 *	Write the MSP430FR5969 I2C port with value of length
 *
 *  @param length Number of messages to Receive
 *  
 *  @return value read (uint_8)
 */
uint8_t I2CRead(  uint16_t slaveAddress,uint8_t length );

/** @brief Set I2C Interrupts
 *
 *	Sets the I2C Stop & Rx interrupts
 *
 *  @param slaveAddress address of Slave I2C
 *  @param length number of messages to receive
 *
 *  @return Void
 */
void I2CSetInterrupts(void);

/************************************************************************
*						GLOBAL VARIABLES
************************************************************************/

#endif
