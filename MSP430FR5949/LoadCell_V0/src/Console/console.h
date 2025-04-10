#ifndef console_H
#define console_H

#include "../../inc/includes.h"
#include "../buffers/buffers.h"

#define MENU_TIMEOUT				(10)
#define ANYKEY_TIMEOUT			    (10)
#define CALIBRATION_TIMEOUT         (10)
#define CALIBRATION_SAMPLE_COUNT 	(10)

/** @brief Main Console State
 *
 *	Main state for console display
 *
 *  @return void
 */
void CONSOLE_State_Main(void);

#endif // console_H
