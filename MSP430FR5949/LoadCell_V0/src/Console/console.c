#include "console.h"

static void CONSOLE_DisplayState_Main(void);
static void CONSOLE_DisplayState_ManualCalibrationInput(uint8_t line, float value);
static void CONSOLE_DisplayMetadata(void);
static void CONSOLE_DisplayCalData(void);
static void CONSOLE_Calibration(void);
static void CONSOLE_SampleSensor(float *AverageValue);
static void RetreiveData(char *value, uint8_t *dataCount);
static uint8_t RetreiveData2(char *value, uint8_t *dataCount);
static uint8_t StringFloatCheck(char *value,uint8_t length);
/*********************************************************************************
*									GLOBAL FUNCTIONS
*********************************************************************************/
void CONSOLE_State_Main(void)
{
	uint8_t OutText[] = "\n\rRestarting Sampling\r\n";
	uint8_t ExitConsoleFlag = false;
	char UserInputVal[BUFFER_C_SIZE] = {0};
	uint8_t NewlineFlag = 0;
	uint8_t dataCount = 0;

	// Reset the time and display the menu options
	MenuTimeoutA = 0;
	CONSOLE_DisplayState_Main();
	ConsoleState = Hold;


	while(ExitConsoleFlag == false)
	{
		// Check for menu timeout, return to sampling if overflowed
		if(MenuTimeoutA > 10)
		{
			ExitConsoleFlag = true;

		}

		// Check the Console buffer for new inputs.  If there is
		// then transfer the data, clear the buffer and whittle
		// the new data down to one value
		if( BufferC_CheckForNewline(&ConsoleData) == BUFFER_C_NEWLINE_DETECTED)
		{
    	RetreiveData(&UserInputVal[0],&dataCount);
        NewlineFlag = true;
		}

        // Using user input, set the state commanded
		if(NewlineFlag == 1)
		{
			switch(UserInputVal[0])
			{
				case '1':
					UART_WriteChar('1',UART_A1);
          ConsoleState = Calibration;
					break;
				case '2':
					UART_WriteChar('2',UART_A1);
          ConsoleState = ManualCal;
					break;
				case '5':
					UART_WriteChar('5',UART_A1);
          ConsoleState = DisplayCal;
					break;
				case '6':
					UART_WriteChar('6',UART_A1);
          ConsoleState = DisplayMetadata;
					break;
				case '9':
					UART_WriteChar('9',UART_A1);
          ConsoleState = UpdateSN;
					break;
				case 'Q':
				case 'q':
					UART_WriteChar('Q',UART_A1);
          ConsoleState = Hold;
          ExitConsoleFlag = true;
					break;
				case '?':
					UART_WriteChar('\n', UART_A1);
					ConsoleState = Main;
					break;
				default:
          ConsoleState = Hold;
					UART_WriteChar('I',UART_A1);
					break;
			}

            for(uint8_t i =0;i<BUFFER_C_SIZE;i++)
            {
              UserInputVal[i] = 0;
            }
            NewlineFlag = false;
		}

        // Display the state commanded
		switch(ConsoleState)
		{
			case Hold:
				break;
			case Main:
				CONSOLE_DisplayState_Main();
        MenuTimeoutA = 0;
        ConsoleState = Hold;
				break;
			case Calibration:
				CONSOLE_Calibration();
                ConsoleState = Main;
				MenuTimeoutA = 0;
				break;
			case ManualCal:
				MenuTimeoutA = 0;
				break;
			case DisplayCal:
				CONSOLE_DisplayCalData();
				MenuTimeoutA = 0;
				break;
			case DisplayMetadata:
				CONSOLE_DisplayMetadata();
				MenuTimeoutA = 0;
				break;
			case UpdateSN:
				MenuTimeoutA = 0;
				break;
			case AutoSample:
				MenuTimeoutA = 0;
				break;
			default:
				break;


		}
        ConsoleState = Hold;
	}

	UART_Write(&OutText[0],LENGTH_OF(OutText),UART_A1);
	SystemState = Sample;
  return;
}



void CONSOLE_State_ManualCalibration(void)
{
	CONSOLE_DisplayState_ManualCalibrationInput(0,0);
	CONSOLE_DisplayState_ManualCalibrationInput(1,1.1);
	CONSOLE_DisplayState_ManualCalibrationInput(2,0);
	CONSOLE_DisplayState_ManualCalibrationInput(3,86.7);


}
/*********************************************************************************
*									STATIC FUNCTIONS
*********************************************************************************/
static void CONSOLE_DisplayState_Main(void)
{

	uint8_t splash[] = "\n\r*********** MAIN *************\n\r\n";
	uint8_t line0[] = "SERIAL #001\n\r";
	uint8_t line1[] = "1 - Calibration Mode\n\r";
	uint8_t line2[] = "2 - Manual Calibration Input\n\r";
	uint8_t line3[] = "5 - Display Calibration Points\n\r";
	uint8_t line4[] = "6 - Display Slope & Intercept\n\r";
	uint8_t line5[] = "9 - Update Serial #\n\r";
	uint8_t line6[] = "Q - Exit (to Log Mode)\n\n\r";

	//UART_ClearChar();
	UART_Write(&splash[0],LENGTH_OF(splash),UART_A1);
	UART_Write(&line0[0],LENGTH_OF(line0),UART_A1);
	UART_Write(&line1[0],LENGTH_OF(line1),UART_A1);
	UART_Write(&line2[0],LENGTH_OF(line2),UART_A1);
	UART_Write(&line3[0],LENGTH_OF(line3),UART_A1);
	UART_Write(&line4[0],LENGTH_OF(line4),UART_A1);
	UART_Write(&line5[0],LENGTH_OF(line5),UART_A1);
	UART_Write(&line6[0],LENGTH_OF(line6),UART_A1);
	UART_WriteChar('>',UART_A1);
}



static void CONSOLE_DisplayState_ManualCalibrationInput(uint8_t line, float value)
{
	uint8_t line0[] = "\n\rEnter SLOPE (xxxx.y): ";
	uint8_t line1[] = "\n\rSlope = ";
	uint8_t line2[] = "\n\rEnter INTERCEPT (xxx.y): ";
	uint8_t line3[] = "\n\rIntercept = ";
	uint8_t temp[16] = "";

	// check valid input conditions
	if(line > 3)
	{
		return;
	}

	if(line == 1 || line == 3)
	{
		sprintf(temp,"%3.1f\n\r",value);
	}

	switch(line)
	{
		case 0:
			UART_Write(&line0[0],LENGTH_OF(line0),UART_A1);
			break;
		case 1:
            UART_Write(&line1[0],LENGTH_OF(line1),UART_A1);
            UART_Write(&temp[0],LENGTH_OF(temp),UART_A1);
			break;
		case 2:
            UART_Write(&line2[0],LENGTH_OF(line2),UART_A1);
			break;
		case 3:
            UART_Write(&line3[0],LENGTH_OF(line3),UART_A1);
            UART_Write(&temp[0],LENGTH_OF(temp),UART_A1);

			break;
		default:
			return;
			break;
	}

}


static void CONSOLE_DisplayMetadata(void)
{
	uint8_t line0[] = "\r\n\nSlope = ";
	uint8_t line1[] = "Intercept = ";
	char values[32];
	uint8_t vals[32];

	sprintf(values, "%3.3f\r\n",Metadata.Slope);
	for(uint8_t i=0;i<32;i++)
	{
		vals[i] = (uint8_t) values[i];
	}
	UART_Write(&line0[0],LENGTH_OF(line0),UART_A1);
	UART_Write(&vals[0],LENGTH_OF(values),UART_A1);

	sprintf(values, "3.3%f\r\n",Metadata.Intercept);
	for(uint8_t i=0;i<32;i++)
	{
		vals[i] = (uint8_t) values[i];
	}
	UART_Write(&line1[0],LENGTH_OF(line1),UART_A1);
	UART_Write(&vals[0],LENGTH_OF(values),UART_A1);
	UART_WriteChar('>',UART_A1);


	return;
}


static void CONSOLE_DisplayCalData(void)
{
	uint8_t line0[] = "Load[";
	uint8_t line1[] = "Pressure[";
	uint8_t load[32] = {0};
	uint8_t pressure[32] = {0};


	UART_WriteChar('\r',UART_A1);
	__delay_cycles(5000);
	UART_WriteChar('\n',UART_A1);

	for(uint8_t i=0;i<METADATA_ARRAY_SIZE;i++)
	{

		sprintf(load,"Load[%d] = %6.2f(lbs)\r\n",i,Metadata.InputLoad[i]);
		sprintf(pressure,"Pres[%d] = %6.2f(bar)\r\n",i,Metadata.RecordedData[i]);

		//UART_Write(&line0[0],LENGTH_OF(line0),UART_A1);
		UART_Write(&load[0],LENGTH_OF(load),UART_A1);
		//UART_Write(&line1[0],LENGTH_OF(line1),UART_A1);
		UART_Write(&pressure[0],LENGTH_OF(pressure),UART_A1);
			__delay_cycles(5000);
		UART_WriteChar('\r',UART_A1);
			__delay_cycles(5000);
		UART_WriteChar('\n',UART_A1);

	}


}

static void CONSOLE_DisplayCalibration(void)
{



}

static void CONSOLE_Calibration(void)
{
	uint8_t line0[] = "\r\n\r\nEnter load in LBS: ";
	uint8_t line1[] = "Sampling\r\n";
	uint8_t rxValues[BUFFER_C_SIZE] = {0};
	uint8_t dataCount = 0;
	uint8_t NewlineFlag = false;
	uint8_t ExitFlag = false;
    uint8_t ValidFlag = false;
	float tempLoad = NAN;




	// Display user command
	UART_Write(&line0[0],LENGTH_OF(line0),UART_A1);

	// Wait for User to input data or timeout
	MenuTimeoutA = 0;
	while(ExitFlag == false)
	{
		if(MenuTimeoutA > CALIBRATION_TIMEOUT)
		{
			ExitFlag = true;
		}

		// Check for newline in the buffer.  If it exists, retreive the data and process it.
		if(BufferC_IsEmpty(&ConsoleData) == BUFFER_NOT_EMPTY)
		{
          ExitFlag = RetreiveData2(&rxValues[dataCount++],&dataCount);

        }


	}

    // Check string for valid ascii values
    if(dataCount > 0)
    {
      ValidFlag = StringFloatCheck(&rxValues[0],--dataCount);
    }
    else
    {
      ValidFlag = false;
    }
    // If
	if(ValidFlag == true)
    {
      	tempLoad = atof(rxValues);
        // Initialize float sensor
        float SensorAverageValue = 0.0;
        CONSOLE_SampleSensor(&SensorAverageValue);
    }
	else
    {
      uint8_t returnline[] = "\r\nInvalid Entry\r\n";
      UART_Write(&returnline[0],LENGTH_OF(returnline),UART_A1);
      return;
    }

}


static void RetreiveData(char *value, uint8_t *dataCount)
{
	uint8_t NewlineFlag = false;
    uint8_t count = 0;
	char rxValues[BUFFER_C_SIZE] = {0};

	count = 0;
	while(BufferC_IsEmpty(&ConsoleData) == BUFFER_NOT_EMPTY)
	{
		BufferC_Get(&ConsoleData, &rxValues[count++]);
	}

	BufferC_Clear(&ConsoleData);
	NewlineFlag = false;
	for(uint8_t i =0;i<BUFFER_C_SIZE;i++)
	{
		// Check for newline character
		if(rxValues[i] == '\n'  || rxValues[i] == '\r')
		{
		NewlineFlag = i;
		}

		// If the newline flag is write a zero to values, otherwise use retreived value
		if(NewlineFlag == true)
		{
		value[i] =0;
		}
		else
		{
			value[i] = rxValues[i];
		}


	}

	*dataCount = count;
	return;
}


static uint8_t RetreiveData2(char *value, uint8_t *dataCount)
{
  uint8_t NewlineFlag = false;

  // Check the buffer for data.  If there is, retreive it and display it.  If it is a newline, report the end of the entry
  if(BufferC_IsEmpty(&ConsoleData) == BUFFER_NOT_EMPTY)
  {
    BufferC_Get(&ConsoleData,value);
    if(*value == '\r' || *value == '\n')
    {
        value = 0;
        NewlineFlag = true;
    }
    else
    {
        // Echo the value and increase the
        UART_WriteChar(*value,UART_A1);
    }
  }

  return NewlineFlag;
}

static uint8_t StringFloatCheck(char *value,uint8_t length)
{
	uint8_t ValidFlag = true;

	for(uint8_t i=0;i<length;i++)
	{
      if( !((value[i] >= '0') && (value[i] <= '9') || (value[i] == '.'))  )
      {
        ValidFlag = false;
      }
      if(value[i] == 0)
      {
   		i = length;
      }
	}

	return ValidFlag;
}



static void CONSOLE_SampleSensor(float *AverageValue)
{
  uint8_t line0[] = "\r\nSampling";


  // Display text
	UART_Write(&line0[0],LENGTH_OF(line0),UART_A1);
//
//   // Request data from sensor
//    PAxLDRequestDataOnInterrupt(&pxSensor);
//
//    // Read the sensors
//    sensorRead(&pxSensor);
//
//    // Process sensor data
//    sensorProcessData(&pxSensor);
//
//    //Pressures[sampleCount] = pxSensor.pressure;
//    BufferF_Put_Circular(&PressureDataBuffer,pxSensor.pressure);
//    BufferF_Put_Circular(&TemperatureDataBuffer,pxSensor.temperature);



}