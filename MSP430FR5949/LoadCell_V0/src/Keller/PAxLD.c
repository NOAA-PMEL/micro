/** @file PAxLD.c
 *  @brief 	Brief description of file
 *  
 *  @author Matt Casari, matthew.casari@noaa.org
 *  @date March 28, 2016
 *  @version 1.0.0
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab 
 *  @copyright Environmental Development Division
 *  
 *	@note 
 *
 *  @bug  No known bugs
 */


#include "PAxLD.h"

/************************************************************************
*					STATIC FUNCTION PROTOTYPES
************************************************************************/
void PAxLDNullStructure(PAXLD_t *sensor);
static uint8_t	PopulateSensorStruct( PAXLD_t *sensor);
static float PAxLDConvertPressureData( uint16_t counts, float pressureMin, float pressureMax);
static float PAxLDConvertTemperatureData( uint16_t counts);
static void PAxLDMemoryRead(PAXLD_t *sensor, uint16_t *cmd, uint8_t dataLength);
static void PAxLDNullStructureDataOnly(PAXLD_t *sensor);
/************************************************************************
*					STATIC VARIABLES
************************************************************************/
const uint16_t PAXLD_WRITE_COMMAND = 0xAC;

/************************************************************************
*					GLOBAL FUNCTIONS
************************************************************************/
void PAxLDInit(PAXLD_t *sensor, uint8_t address, uint8_t port, uint16_t pin)
{
	/* Clear the structure */
	PAxLDNullStructure(sensor);

	/* Set address */
	sensor->address = address;

	/* Set EOC Port & Pin */
	sensor->eocPort = port;
	sensor->eocPin = pin;

	/* Attach Interrupt to EOC Pin */
	GPIO_AttachInputInterrupt(sensor->eocPort, sensor->eocPin,GPIO_EDGE_LOW_TO_HIGH);
	
	/* Retreive data from memory */
	sensor->dataIndex = 0;
	PAxLDRequestScaleValues(sensor);
	
	return;
}

/***********************************************************************/
void PAxLDClose(PAXLD_t *sensor)
{
	/* Close I2C */
	I2CClose();

	/* Turn power to PAXLD sensor OFF */
	PAXLD_POWER_OFF();

	/* Clear the structure */
	for(uint8_t i=0;i<NUM_SENSORS;i++)
	{
		PAxLDNullStructure(&sensor[i]);
	}

	return;
}

/***********************************************************************/
void PAxLDOpen(void)
{
	/* Turn power to PAXLD sensor ON */
	PAXLD_POWER_ON();

	/* Open the I2C */
	I2CInit();

	return;
}

/***********************************************************************/
void PAxLDRestart(PAXLD_t *sensor)
{
	/* Stop PAXLD */
	PAxLDClose(&sensor[0]);

	/* Start PAXLD */
	PAxLDOpen();

	return;
}

/***********************************************************************/
uint8_t	PAxLDRequestDataDirect(PAXLD_t *sensor)
{
	uint8_t response;
		uint8_t dataLength = PAXLD_DATA_LENGTH;
		uint16_t command = PAXLD_REQUEST_DATA;

		/* Write to sensor */
		I2CWrite(sensor->address, &command, 1 );

		/* Delay and wait for sensor to converge */
		__delay_cycles(200);

		/* clear the data array */
		sensor->pressure = 0.0;
		sensor->temperature = 0.0;
		sensor->pressureCount = 0;
		sensor->temperatureCount = 0;
		for(uint8_t i=0;i<PAXLD_DATA_LENGTH;i++)
		{
			sensor->data[i] = 0;
		}

		/* Look for EOC pin high or timeout */
		msTimeoutCounter = 0;
		while( (msTimeoutCounter < PAXLD_TIMEOUT_MS));


		/*if((GpioReadInputPin(sensor->eocPort,sensor->eocPin)) > 0) */
		if(sensor->dataAvailableFlag == true)
		{
			/* Start reading the data from the sensor */
			sensor->dataIndex = 0;
			I2CRead(sensor->address,dataLength);
			msTimeoutCounter = 0;
			while((sensor->dataIndex< dataLength) && (msTimeoutCounter < PAXLD_TIMEOUT_MS));
		}
		else
		{
			sensor->dataIndex = dataLength;
			sensor->pressure = 0.0;
			sensor->temperature = 0.0;
			sensor->pressureCount = 0;
			sensor->temperatureCount = 0;
			for(uint8_t i=0;i<PAXLD_DATA_LENGTH;i++)
			{
				sensor->data[i] = 0;
			}
		}
		/* Check for valid data */
		if(PopulateSensorStruct(sensor) == VALID_STRING)
		{
			response = true;
		}
		else
		{
			response = false;
		}

		return response;
}

/***********************************************************************/
void PAxLDRequestDataOnInterrupt(PAXLD_t *sensor)
{
	uint16_t command = PAXLD_REQUEST_DATA;

	/* Clear the data flag */
	sensor->dataAvailableFlag = false;

	/* Null the structure values */
	PAxLDNullStructureDataOnly(sensor);

	/* Write to I2C */
	I2CWrite(sensor->address,&command,1);

	return;
}

/***********************************************************************/
void PAxLDRequestDataRead(PAXLD_t *sensor,uint8_t dataLength)
{
	/* Start reading the data from the sensor */
	sensor->dataIndex = 0;
	I2CRead(sensor->address,dataLength);
	msTimeoutCounter = 0;
	while(sensor->dataIndex < 5 &&  msTimeoutCounter < 10);

	return;
}

/***********************************************************************/
uint8_t PAxLDProcessReceivedData(PAXLD_t *sensor)
{
    uint8_t response;
    /* Check for valid data */
    if(PopulateSensorStruct(sensor) == VALID_STRING)
    {
        response = true;
    }
    else
    {
        response = false;
    }

    return response;
}


/***********************************************************************/
void PAxLDRequestScaleValues(PAXLD_t *sensor)
{
	uint8_t dataLength = 3;
	uint16_t cmdVal = 0;
	uint16_t temp0 = 0;
	uint16_t temp1 = 0;
	uint32_t temp2 = 0;
	intToFloat_t tempX;


	/* Retreive Scaling 1 Data */
	cmdVal = PAXLD_SCALING1_CMD;
	PAxLDMemoryRead(sensor, &cmdVal, dataLength);
	temp0 = (uint16_t) sensor->data[1];
    temp0 = temp0 << 8;
	temp0 = temp0 + sensor->data[2];


	/* Retreive Scaling 2 Data */
	cmdVal = PAXLD_SCALING2_CMD;
	PAxLDMemoryRead(sensor, &cmdVal, dataLength);
	temp1 = (uint16_t) sensor->data[1];
    temp1 = temp1 << 8;
	temp1 = temp1 + sensor->data[2];
 
	/* Combine 1/2 Data into float */
	temp2 = (uint32_t) temp0 << 16;
	temp2 += (uint32_t) temp1;
	tempX.i = temp2;
	sensor->pressureMin = (float) tempX.f;

	/* Retreive Scaling 3 Data */
	cmdVal = PAXLD_SCALING3_CMD;
	PAxLDMemoryRead(sensor, &cmdVal, dataLength);
	temp0 = (uint16_t) sensor->data[1];
	temp0 = temp0 << 8;
	temp0 = temp0 + sensor->data[2];


	/* Retreive Scaling 4 Data */
	cmdVal = PAXLD_SCALING4_CMD;
	PAxLDMemoryRead(sensor, &cmdVal, dataLength);
	temp1 = (uint16_t) sensor->data[1];
    temp1 = temp1 << 8;
	temp1 = temp1 + sensor->data[2];

	/* Combine 3/4 Data into float then store in structure */
	temp2 = (uint32_t) temp0 << 16;
	temp2 += (uint32_t) temp1;
	tempX.i = temp2;
	sensor->pressureMax = (float) tempX.f;

	return;
}


/***********************************************************************/
void PAxLDWriteNewAddress(PAXLD_t *sensor, uint16_t address)
{
	uint16_t command[3]= {0xA9,0x00,address};
	uint8_t dataLength = 3;

	/* Enter command mode */
	I2CWrite(sensor->address, &command[0], 1);
	__delay_cycles(200);
	sensor->dataIndex = 0;
	__delay_cycles(20000);

	/* Optional - Read Slave Address */
	I2CRead(sensor->address,dataLength); 

	/* Set new slave address */
	command[0] = 0x42;
	I2CWrite(address, &command[0], dataLength);

	/* Verify new address */
	__delay_cycles(2000);
	I2CRead(sensor->address,dataLength);

	/* Shutdown indicator */
	if((sensor->data[0] == 0x00) && (sensor->data[1] == address))
	{

	}
	else
	{

	}

}

/************************************************************************
*					STATIC FUNCTIONS
************************************************************************/
/** @brief Clears the PAXLD structure
 *
 *	Nulls values in PAXLD structure.  Does not replace address, Port & Pin
 *
 *  @param none
 *
 *  @return none
 */
static void PAxLDNullStructure(PAXLD_t *sensor)
{
	/* Initialize the structure */
	sensor->badDataCount = 0;
	sensor->status = 0;
	sensor->dataReadyFlag = 0;
	sensor->dataIndex = 0;
	sensor->pressureCount = 0;
	sensor->temperatureCount = 0;
	sensor->pressure = 0.0;
	sensor->temperature = 0.0;
	sensor->pressureMin = 0.0;
	sensor->pressureMax = 0.0;


	/* Clear the data */
	for(uint8_t i=0;i<PAXLD_DATA_LENGTH;i++)
	{
		sensor->data[i] = 0;
	}

	return;
}


/** @brief Clears the PAXLD structure data ONLY
 *
 *  Only nulls data values in PAXLD structure.  Does not replace address, Port & Pin
 *
 *  @param none
 *
 *  @return none
 */
static void PAxLDNullStructureDataOnly(PAXLD_t *sensor)
{
    /* Initialize the structure */
    sensor->status = 0;
    sensor->dataReadyFlag = 0;
    sensor->dataIndex = 0;
    sensor->pressureCount = 0;
    sensor->temperatureCount = 0;
    sensor->pressure = 0.0;
    sensor->temperature = 0.0;

    /* Clear the data */
    for(uint8_t i=0;i<PAXLD_DATA_LENGTH;i++)
    {
        sensor->data[i] = 0;
    }

    return;
}


/** @brief Update Sensor Structure Data values
 *
 *	Uses the currently received data to update the sensor
 *	structure values.
 *
 *
 *  @param *sensor Sensor structure to be updated
 *
 *  @return valid/invalid response flag
 */
static uint8_t	PopulateSensorStruct( PAXLD_t *sensor)
{
	uint16_t temp0 = 0;
	uint16_t temp1 = 0;
	uint16_t tempVal = 0;
	uint8_t response = INVALID_STRING;

	/* update status */
	sensor->status = sensor->data[0];

	if((sensor->status & 0x40)> 0 )
	{
		/* Setup pressure counts in structure */
		temp0 = (uint16_t) sensor->data[1];
		temp0 = temp0 << 8;
		temp1 = (uint16_t) sensor->data[2];
		tempVal = (temp0 + temp1);

		sensor->pressureCount = tempVal;


		/* Setup temperature counts in structure */
		temp0 = 0;
		temp1 = 0;

		temp0 = (uint16_t) sensor->data[3];
		temp0 = temp0 << 8;
		temp1 = (uint16_t) sensor->data[4];
		sensor->temperatureCount = (temp0 + temp1);

		/* Check for no response */
		if(sensor->pressureCount == 0)
		{
            sensor->pressureCount = NAN;
			sensor->pressure = NAN;
			sensor->badDataCount++;
		}
		else
		{
			/* Convert pressure and temperature */
			if(sensor->pressureMax > 0)
			{
				sensor->pressure = PAxLDConvertPressureData(sensor->pressureCount,sensor->pressureMin,sensor->pressureMax);
			}
			else
			{
				sensor->pressure = NAN;
			}
			sensor->temperature = PAxLDConvertTemperatureData(sensor->temperatureCount);
			sensor->badDataCount = 0;
		}
		response = VALID_STRING;
	}
	else
	{
		sensor->pressureCount = NAN;
		sensor->pressure = NAN;
		sensor->badDataCount++;
		response = INVALID_STRING;
	}




	return(response);
}

/** @brief Convert int pressure data to float
 *
 *	Takes in counts from PAxLD pressure data and converts to a float using the formula:
 *
 *	Ptotal = (Pcounts - Pcounts_min) * (MaxPressure - MinPressure)/(Maxcounts - Mincounts) + MinPressure
 *
 *
 *  @param counts Pressure counts from PAxLD
 *  @param pressureMin Minimum calibrated pressure of the sensor
 *  @param pressureMax Maximum calibrated pressure of the sensor
 *
 *  @return pressure value
 */
static float PAxLDConvertPressureData( uint16_t counts, float pressureMin, float pressureMax)
{
	float temp = 0.0;
	float pressureTemp1 = 0.0;
	float pressureTemp2 = 0.0;
	float pressureTotal = 0.0;

	if(counts < PAXLD_PRESSURE_CONVERSION_MIN)
	{
		counts = PAXLD_PRESSURE_CONVERSION_MIN;
	}

	if(counts > PAXLD_PRESSURE_CONVERSION_MAX)
	{
		counts = PAXLD_PRESSURE_CONVERSION_MAX;
	}
	/* Formula is  Ptotal = (Pcounts - Pcounts_min) * (MaxPressure - MinPressure)/(Maxcounts - Mincounts) + MinPressure */
	/* Need to figure out how to incorporate pressureMax and pressureMin to be variables */

	temp = (float) (PAXLD_PRESSURE_CONVERSION_MAX - PAXLD_PRESSURE_CONVERSION_MIN);
	pressureTemp1 = (pressureMax - pressureMin);
	pressureTemp1 /= temp;


	pressureTemp2 = (float) (counts - PAXLD_PRESSURE_CONVERSION_MIN);
	pressureTemp2 *= pressureTemp1;
	pressureTotal = pressureTemp2 + pressureMin;


	return pressureTotal;

}

/** @brief Convert int temperature data to float
 *
 *	Takes in counts from PAxLD pressure data and converts to a float using the formula:
 *
 *  Ptotal = (Pcounts - Pcounts_min) * (MaxPressure - MinPressure)/(Maxcounts - Mincounts) + MinPressure
 *
 *
 *  @param counts Pressure counts from PAxLD
 *  @param pressureMin Minimum calibrated pressure of the sensor
 *  @param pressureMax Maximum calibrated pressure of the sensor
 *
 *  @return temperature value
 */
static float PAxLDConvertTemperatureData( uint16_t counts)
{
	float temperatureTemp = 0.0;


	/* Formula is T = (( value >> 4) - 24) * 0.5 - 50 */
	/* 4 bits are in noise floor, hence 4 bit right shift */
	counts = (counts>>4);
	counts -= 4;

	temperatureTemp = (float) counts;
	temperatureTemp *= 0.05;
	temperatureTemp -= 50;

	return temperatureTemp;


}

/** @brief Read sensor memory value
 *
 *	Reads the value of the PAxLD memory
 *
 *	@param *sensor Sensor structure
 *	@param *cmd Command used to select which memory location
 *	@param *dataLength Length of data to be received
 *
 *  @return none
 */
static void PAxLDMemoryRead(PAXLD_t *sensor, uint16_t *cmd, uint8_t dataLength)
{

	I2CWrite(sensor->address, cmd, 1 );
	sensor->dataIndex = 0;
    msTimeoutCounter = 0;
    while(msTimeoutCounter < 2);
	I2CRead(sensor->address,dataLength);

	msTimeoutCounter = 0;
	while((sensor->dataIndex< dataLength) && (msTimeoutCounter < PAXLD_TIMEOUT_MS));
	return;
}


