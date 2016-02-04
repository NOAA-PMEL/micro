#ifndef console_H
#define console_H
/************************************************************************
*							HEADER FILES
************************************************************************/
#include <stdint.h>
#include "../../inc/includes.h"

#ifndef PMEL
#include "../../test/MSP430FR5969.h"
#else
#include <msp430fr5969.h>
#endif
/************************************************************************
*						STANDARD LIBRARIES
************************************************************************/


/************************************************************************
*							CONSTANTS
************************************************************************/
typedef struct SystemValues {
    float slope;
    float intercept;
}SystemValues_t;

typedef enum mode {
    Display,
    Read,
    Continue,
    Exit
} modes_t;

typedef enum state {
    Main,
    Calibration,
    ManualCal,
    DisplayCal,
    DisplayMetadata,
    UpdateSN,
    Sample
} state_t;

typedef struct console {
    modes_t mode;
    state_t state;
    state_t previousState;
    uint8_t inputChar;
    uint16_t SerialNumber;
}console_t;


/************************************************************************
*					GLOBAL FUNCTION PROTOTYPES
************************************************************************/
/** @brief Console Main State
 *
 *  Main Console State for choosing options
 *
 *  @param none
 *
 *  @return none
 */
void CONSOLE_State_Main(void);

/** @brief Console Manual Calibration State
 *
 *	Manual Calibration State, inputing user defined slope and intercept values
 *
 *  @param none
 *
 *  @return none
 */
void CONSOLE_State_ManualCalibration(void);

/** @brief Console Calibration Mode State
 *
 *	Calibration Mode State, awaiting user choice for calibration.
 *
 *  @param none
 *
 *  @return none
 */
void CONSOLE_State_CalibrationMode(void);

/** @brief Console Calibration Input State
 *
 *	Calibration input state, prompting/recording user load values
 *
 *  @param none
 *
 *  @return none
 */
void CONSOLE_State_CalibrationInput(void);

/** @brief Console Main Display State
 *
 * 	Display of Main State options.
 *
 *  @param none
 *
 *  @return none
 */
void CONSOLE_DisplayState_Main(void);

/** @brief Console Calibration Mode Display State
 *
 *	Display of Calibration Mode options
 *
 *  @param none
 *
 *  @return none
 */
void CONSOLE_DisplayState_CalibrationMode(void);

/** @brief Console Calibration Input Display State
 *
 *	Display of Calibration Input options
 *
 *  @param none
 *
 *  @return none
 */
void CONSOLE_DisplayState_CalibrationInput(void);

/** @brief Console Manual Calibration Input Display State
 *
 *	Display of manual calibration input values
 *
 *  @param line Line to display (0 - 3)
 *  @param value Value to display (valid for lines 1 & 3)
 *
 *  @return none
 */
void CONSOLE_DisplayState_ManualCalibrationInput(uint8_t line, float value);

/** @brief Console Calibration Points Display
 *
 *	Display of all existing calibration points (pressure & load)
 *
 *  @param none
 *
 *  @return none
 */
void CONSOLE_DisplayState_CalibrationPoints(void);

/** @brief Console Read Single Char
 *
 *	Monitors serial port for single character
 *
 *  @param none
 *
 *  @return none
 */
uint8_t CONSOLE_ReadChar(void);

/** @brief Console Read Float Value
 *
 * 	Read serial port and convert value to float string
 *
 *  @param *rxChars Array pointer for incoming float string
 *  @param length Length of rxChars
 *
 *  @return none
 */
void CONSOLE_ReadInputFloat(uint8_t *rxChars,uint8_t length);

/** @brief Console Read String
 *
 *  @param *str Array pointer to save input string
 *  @param length Length of array
 *
 *  @return none
 */
void CONSOLE_ReadString(uint8_t *str, uint8_t length);



#endif // console_H
