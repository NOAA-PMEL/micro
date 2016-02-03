/** @file console.h
 *  @brief Console functions
 *
 *  @author Matt Casari, matthew.casari@noaa.org
 *  @date 2/3/2016
 *  @version 0.0.1
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *	@note Tested on MSP430FR5969
 *	@note
 *
 *	@todo
 *	@todo
 *
 *  @bug  No known bugs
 */



#include "console.h"

uint8_t UART_ClearChar(void)
{
    printf("\033[2J");
}

uint8_t UART_WriteChar(uint8_t value, uint8_t Port)
{
    //putchar((char) value);

    printf("\033[2J");

    return 1;
}

uint8_t UART_Write(uint8_t *value, uint8_t length, uint8_t Port)
{
    char temp[64] = {0};

    printf(value);


    return 1;
}




uint8_t CONSOLE_ReadChar(void)
{
    uint8_t rxChar = 0;

    rxChar = getchar();

    return rxChar;

}

void CONSOLE_ReadString(uint8_t *str, uint8_t length)
{
    uint8_t rxChar = 0;
    uint8_t counter = 0;
    uint8_t endFlag = 0;

    while(counter < length)
    {
        if(endFlag == 0)
        {
            str[counter] = getchar();
        }
        else
        {
            str[counter] = 0;
        }

        if(str[counter++] == '\n')
        {
            endFlag = 1;
        }
    }

    return;
}


void CONSOLE_DisplayState_Main(void)
{
    uint8_t temp[16] = {0};

    sprintf(temp,"%d\n",console.SerialNumber);
    uint8_t splash[] = "*********** MAIN *************\n\n";
    uint8_t line0[] = "SERIAL #";
	uint8_t line1[] = "1 - Calibration Mode\n";
	uint8_t line2[] = "2 - Manual Calibration Input\n";
	uint8_t line3[] = "5 - Display Calibration Points\n";
	uint8_t line4[] = "6 - Display Slope & Intercept\n";
	uint8_t line5[] = "9 - Update Serial #\n";
	uint8_t line6[] = "Q - Exit (to Log Mode)\n\n>";

    //UART_ClearChar();
    UART_Write(&splash[0],LENGTH_OF(splash),UART_A1);
	UART_Write(&line0[0],LENGTH_OF(line0),UART_A1);
	UART_Write(&temp[0],LENGTH_OF(temp),UART_A1);
	UART_Write(&line1[0],LENGTH_OF(line1),UART_A1);
	UART_Write(&line2[0],LENGTH_OF(line0),UART_A1);
	UART_Write(&line3[0],LENGTH_OF(line1),UART_A1);
	UART_Write(&line4[0],LENGTH_OF(line0),UART_A1);
	UART_Write(&line5[0],LENGTH_OF(line1),UART_A1);
	UART_Write(&line6[0],LENGTH_OF(line0),UART_A1);

}

void CONSOLE_DisplayState_CalibrationMode(void)
{
    uint8_t splash[] = "******** CALIBRATION *********\n\n";
	uint8_t line0[] = "1 - Enter Calibration Point\n";
	uint8_t line1[] = "4 - Compute Calibration\n";
	uint8_t line2[] = "5 - Display Calibration Data\n";
	uint8_t line3[] = "6 - Display Slope & Intercept\n";
	uint8_t line4[] = "9 - Clear All Calibration Data\n";
	uint8_t line5[] = "Q - Return to main\n\n>";

    //UART_ClearChar();
    UART_Write(&splash[0],LENGTH_OF(splash),UART_A1);
	UART_Write(&line0[0],LENGTH_OF(line0),UART_A1);
	UART_Write(&line1[0],LENGTH_OF(line1),UART_A1);
	UART_Write(&line2[0],LENGTH_OF(line0),UART_A1);
	UART_Write(&line3[0],LENGTH_OF(line1),UART_A1);
	UART_Write(&line4[0],LENGTH_OF(line0),UART_A1);
	UART_Write(&line5[0],LENGTH_OF(line1),UART_A1);


	return;

}

void CONSOLE_DisplayState_CalibrationInput(void)
{
	uint8_t line0[] = "Enter Load (lbs) (0 - 65545): ";

	UART_Write(&line0[0],LENGTH_OF(line0),UART_A1);

	return;
}


void CONSOLE_DisplayState_ManualCalibrationInput(uint8_t line, float value)
{
	uint8_t line0[] = "Enter SLOPE (xxxx.y): ";
	uint8_t line1[] = "Slope = ";
	uint8_t line2[] = "Enter INTERCEPT (xxx.y): ";
	uint8_t line3[] = "Intercept = ";
	uint8_t temp[16] = "";

	// check valid input conditions
	if(line > 3)
	{
		return;
	}

	if(line == 1 || line == 3)
	{
		sprintf(temp,"%3.1f",value);
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


void CONSOLE_ReadInputFloat(uint8_t *rxChars,uint8_t length)
{
    uint8_t rxChar = 0;
    uint8_t counter = 0;
    uint8_t i = 0;


    // Read string < length
    // Check for valid characters (0 - 9 and .)
    // Reject all others (clear rxChars before returning)
    while(counter < length)
    {
        rxChar = CONSOLE_ReadChar();
        if((rxChar > '0' && rxChar <= '9') || rxChar == '.')
        {

            if(counter == (length-1))
            {
                printf("Bad String\n");
                for(i=0;i<length;i++)
                {
                    rxChars[i] = 0;
                }
                return;
            }

            rxChars[counter++] = rxChar;

        }
        else if(rxChar == '\n')
        {
            return;
        }
        else
        {
            for(i=0;i<length;i++)
            {
                rxChars[i] = 0;
            }
            printf("Bad Value\n");
            return;
        }
    }
}

void CONSOLE_State_ManualCalibration(void)
{
    uint8_t rxChars[8] = {0};
    uint8_t rxChar;
    float rxChar_f;
    uint8_t counter = 0;
    uint8_t i = 0;

    CONSOLE_ReadChar();

    // Read Slope
    CONSOLE_DisplayState_ManualCalibrationInput(0,0.0);
    CONSOLE_ReadInputFloat(&rxChars[0],8);
    rxChar_f = atof(&rxChars[0]);

    // Display Slope
    CONSOLE_DisplayState_ManualCalibrationInput(1,rxChar_f);
    printf("\n");

    // Read Intercept
    CONSOLE_DisplayState_ManualCalibrationInput(2,0.0);
    CONSOLE_ReadInputFloat(&rxChars[0],8);
    rxChar_f = atof(&rxChars[0]);

    // Display Intercept
    CONSOLE_DisplayState_ManualCalibrationInput(3,rxChar_f);
    printf("\n\n");

}


void CONSOLE_State_Main(void)
{
    console.mode = Display;
    while(1)
    {


        switch(console.mode)
        {

            case Display:
                CONSOLE_DisplayState_Main();
                console.mode = Read;
                break;

            case Read:
                console.inputChar = 0;
                console.inputChar = CONSOLE_ReadChar();
                switch(console.inputChar)
                {
                    case '1':
                        // Calibration Mode
                        console.state = Calibration;
                        CONSOLE_State_CalibrationMode();
                        console.mode = Display;
                        break;
                    case '2':
                        console.state = ManualCal;
                        CONSOLE_State_ManualCalibration();
                        console.mode = Display;
                        // Manual Calibration Input
                        break;
                    case '5':
                        // Display Calibration Data
                        console.state = DisplayCal;
                        break;
                    case '6':
                        // Display Metadata

                        console.state = DisplayMetadata;
                        CONSOLE_DisplayState_CalibrationPoints();
                        console.state = Main;
                        console.mode = Continue;

                        break;
                    case '9':
                        // Update Serial Number
                        console.state = UpdateSN;
                        break;
                    case 'Q':
                    case 'q':
                        // Exit to sample mode
                        console.mode = Exit;
                        console.state = Sample;
                        break;

                    case '\n':
                        break;
                    default:
                        break;

                }

                break;
            case Exit:
                return;
                break;

            case Continue:
                printf("Press Any key to continue: ");
                console.inputChar = 0;
                console.inputChar = CONSOLE_ReadChar();
                if(console.inputChar >= 10)
                {
                    UART_ClearChar();
                    console.mode = Display;
                }
                break;
            default:
                printf("In Default\n");
                break;

        }

    }
}

void CONSOLE_State_CalibrationMode(void)
{
    while(1)
    {
        switch(console.mode)
        {
            case Display:
                CONSOLE_DisplayState_CalibrationMode();
                console.mode = Read;
                break;
            case Read:
                console.inputChar = CONSOLE_ReadChar();
                switch(console.inputChar)
                {
                    case '1':
                        //Enter Calibration Point
                        printf("1");
                        break;
                    case '4':
                        // Compute Calibration
                        printf("4");
                        break;
                    case '5':
                        // Display Calibration Data
                        CONSOLE_DisplayState_CalibrationPoints();
                        break;
                    case '6':
                        // Display Slope & Intercept
                        break;
                    case '9':
                        // Clear All Calibration Data
                        break;
                    case 'Q':
                    case 'q':
                        console.mode = Exit;
                        break;
                    default:
                        console.mode = Display;
                        printf("\n\n");
                        break;
                }

                break;
            case Exit:
                printf("Exit Cal\n");
                console.inputChar = 0;
                return;
                break;
            default:
                console.inputChar = 'Q';
                return;
                break;

        }
    }
    console.inputChar = 0;
    return;
}

void CONSOLE_DisplayState_CalibrationPoints(void)
{

	uint8_t line0[] = "Slope = ";
	uint8_t line1[] = "Intercept = ";
	uint8_t temp[16] = "";


	sprintf(temp,"%3.3f\n",ActiveSys.slope);
	UART_Write(&line0[0],LENGTH_OF(line0),UART_A1);
	UART_Write(&temp[0],LENGTH_OF(temp),UART_A1);

	sprintf(temp,"%3.3f\n",ActiveSys.intercept);
	UART_Write(&line1[0],LENGTH_OF(line0),UART_A1);
	UART_Write(&temp[0],LENGTH_OF(temp),UART_A1);

	return;
}










//void CONSOLE_State_CalibrationMode_ValidInput(uint8_t value)

/*
void CONSOLE_State_ManualCalibration(void);
{
    while(1)
    {
        switch(console.mode)
        {
            case Display:
                CONSOLE_DisplayState_ManualCalibrationInput();
                console.mode = Read;
                break;
            case Read:
                console.inputChar = CONSOLE_ReadChar();
                console.mode = Display;
                break;
            default
        }
    }
}
*/
