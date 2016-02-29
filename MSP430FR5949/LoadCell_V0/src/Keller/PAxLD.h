/** @file paxld.h
 *  @brief Drivers for Keller-Druck PAxLD sensors
 *
 *  @author Matt Casari, matthew.casari@noaa.org
 *  @date Oct 2, 2015
 *  @version 0.1.1
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *	@note Tested on Microncontroller- MSP430FR5969
 *	@note Tested on Keller sensor PA-4LD
 *
 *	@note To use this module, define a PAXLD structure in your global variables.
 *	Use PAXLDInit() with a pointer to the structure, the I2C Address (which is
 *	typically 0x40 for a Keller sensor) and the Port/Pin numbers that the EOC signal
 *	from the sensor is attached to.
 *  @bug  No known bugs
 *
 *
 */

#ifndef _PAXLD_H
#define _PAXLD_H


/************************************************************************
*						STANDARD LIBRARIES		
************************************************************************/
#include <stdint.h>
#include "../../inc/includes.h"

#ifdef PMEL
#include <msp430fr5949.h>
#else
#include "../../test/msp430fr5969.h"
#endif



/************************************************************************
*						GLOBAL VARIABLES
************************************************************************/

/************************************************************************
*							CONSTANTS
************************************************************************/
// Number remapping
#define NUM_SENSORS				NUM_KELLER_SENSORS
// function re-mapping
#define	PAxLDDelayMs(i)	delayMs(i)		//Replace delay_ms(i) with implemented millisecond delay

#define PAXLD_POWER_ON()	FET_ON()
#define PAXLD_POWER_OFF()	FET_OFF()


#define PAXLD_BAUD_RATE (100000)

// Define the number of pressure sensors on the bus
#define NUMBER_OF_PAXLD_SENSORS (5)

// PAXLD Sensor Data Retreive Max Timeout
#define PAXLD_DELAY_MAX (25)			// Milliseconds
#define PAXLD_TIMEOUT_MS (15)			// Millisecond timeout

// PA7LD Sensor Error Conditions
#define INVALID_INIT 	(0x00)			// I2C did not initialize correctly
#define VALID_INIT		(0x01)			// I2C & Sensor correctly initialized
#define INVALID_SENSOR	(0x02)			// I2C initialized correctly, but Sensor is registering error

// PA7LD Sensor Comm Codes
#define	PAXLD_REQUEST_DATA	(0xAC)

// PA7LD Sensor Address
#define PAXLD_ADDR 		(0x40)
#define PAXLD_DATA_LENGTH (5)
#define PAXLD_DATA_LENGTH_FULL	(5)
#define PAXLD_DATA_LENGTH_P_ONLY	(3)		// Pressure only reading, no temperature

// PA7LD Sensor Conversion Values
#define PAXLD_PRESSURE_CONVERSION_MIN	(16384u)		// Lowest int value returned (min of range)
#define PAXLD_PRESSURE_CONVERSION_MAX	(49152u)		// Greatest int value returned (max of range)

// pa7ldstring.c
#define INVALID_STRING	(-1)
#define VALID_STRING	(0)
#define READ_REQUEST	(1)
#define WRITE_REQUEST	(2)


// Scaling commands
#define PAXLD_SCALING1_CMD (0x13)
#define PAXLD_SCALING2_CMD (0x14)
#define PAXLD_SCALING3_CMD (0x15)
#define PAXLD_SCALING4_CMD (0x16)


/************************************************************************
*					Structures
************************************************************************/
/** @struct PAXLD
 *
 *	@brief This structure contains the sensor info and data for the PAxLD sensor
 *
 */
typedef struct PAXLD {
	uint16_t address;
	uint8_t eocPort;
	uint8_t eocPin;
	uint8_t badDataCount;
	uint8_t status;
	uint8_t dataReadyFlag;
	uint8_t dataAvailableFlag;
	uint8_t dataIndex;
	uint8_t data[PAXLD_DATA_LENGTH];
	int32_t pressureCount;
	uint16_t temperatureCount;
	float pressure;
	float temperature;
	float pressureMin;
	float pressureMax;
}PAXLD_t;


/** @union intToFloat_t
 *
 *	@brief This takes the bitmap from a unsigned int and directly maps to bit-map of a float
 *
 */
typedef union {
  uint32_t i;
  float f;
 } intToFloat_t;
/************************************************************************
*					GLOBAL FUNCTION PROTOTYPES
************************************************************************/

/** @brief Initialize PAXLD Structure and starts sensor comms
  *
  *  @param *sensor Pointer to PAxLD sensor structure
  *  @param address Address of sensor being used
  *  @param port Port of EOC pin input
  *  @param pin Pin of EOC pin input
  *
  *  @return none
  */
void PAxLDInit(PAXLD_t *sensor, uint8_t address, uint8_t port, uint16_t pin);

/** @brief Close out the sensor
 *
 *	Close the sensor & associated structure
 *
 *  @param none
 *
 *  @return none
 *
 *  @note NOT CURRENTLY IMPLEMENTED
 *  @todo add code to this call
 */
void PAxLDOpen(void);

/** @brief Close out the sensor
 *
 *	Close the sensor & associated structure
 *
 *  @param none
 *
 *  @return none
 *
 *  @note NOT CURRENTLY IMPLEMENTED
 *  @todo add code to this call
 */
void PAxLDClose(PAXLD_t *sensor);

/** @brief Restart the sensor
 *
 *	Close the sensor & associated structure
 *
 *  @param none
 *
 *  @return none
 *
 *  @note NOT CURRENTLY IMPLEMENTED
 *  @todo add code to this call
 */
void PAxLDRestart(PAXLD_t *sensor);

/** @brief Request data from PAxLD sensor on I2C Interrupts
 *
 *	writes to PAxLD sensor with a request for the sensor
 *	to return the data.  Uses I2C STOP and RX interrupts
 *
 *  @param *sensor Sensor data structure
 *
 *  @return Void
 */
void	PAxLDRequestDataOnInterrupt(PAXLD_t *sensor);


/** @brief Request data from PAxLD sensor
 *
 *	writes to PAxLD sensor with a request for the sensor
 *	to return the datas
 *
 *  @param *sensor Sensor data structure
 *
 *  @return Valid string
 */
uint8_t	PAxLDRequestDataDirect(PAXLD_t *sensor);


/** @brief Set up I2C Data Read
 *
 *	Sets up the I2C Read of  *
 *  @param *sensor Sensor data structure
 *  @param dataLength length of return message
 *
 *  @return Void
 */
void PAxLDRequestDataRead(PAXLD_t *sensor,uint8_t dataLength);

/** @brief Set up I2C Data Read
 *
 *  Sets up the I2C Read of  *
 *  @param *sensor Sensor data structure
 *
 *  @return response
 */
uint8_t PAxLDProcessReceivedData(PAXLD_t *sensor);

/** @brief Request calibrated scale values from PAxLD sensor
 *
 *	Requests the preset calibrated scale values from the PAxLD memory
 *
 *  @param *sensor Sensor data structure
 *
 *  @return Void
 *
 *
 */
void PAxLDRequestScaleValues(PAXLD_t *sensor);

/** @brief Change sensor I2C address
 *
 *	Writes new I2C address to PAxLD memory
 *
 *  @param *sensor Sensor data structure
 *  @param address New address value
 *
 *  @return Void
 *
 *  @note THIS DOES NOT WORK YET
 *  @todo Fix this code
 */
void PAxLDWriteNewAddress(PAXLD_t *sensor, uint16_t address);


#endif
