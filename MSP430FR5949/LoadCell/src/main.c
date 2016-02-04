/** @file main.c
 *  @brief 	
 *
 *  @author Matt Casari, matthew.casari@noaa.org
 *  @date Feb 3, 2016
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

#define PMEL
#include "../inc/includes.h"
/************************************************************************
*					FUNCTION PROTOTYPES
************************************************************************/

/************************************************************************
*					GLOBAL VARIABLES
************************************************************************/
// Initialize structures
console_t console;          // Console State
SystemValues_t ActiveSys;   // Normal running system values
SystemValues_t FRAMSys;     // structures saved to FRAM


/************************************************************************
*						MAIN
************************************************************************/

int main()
{
    // Set Console States
    console.state = Calibration;
    console.previousState = Main;
    console.SerialNumber = 7600;

    // Set System States
    FRAMSys.slope = 4.2;
    FRAMSys.intercept = 38.3;

    ActiveSys = FRAMSys;

    uint8_t rxChar = 0;

    printf("Hello, World!\n");



    // Main
    CONSOLE_State_Main();

    // Calibration Mode
    //CONSOLE_State_CalibrationMode();



    printf("\n\n");

    // Manual Cal
    //CONSOLE_State_ManualCalibration();



    return 0;
}





/************************************************************************
*						FUNCTIONS
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
