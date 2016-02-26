#ifndef console_H
#define console_H

#include "../../inc/includes.h"
#include "../buffers/buffers.h"

#define CALIBRATION_TIMEOUT (10)
#define CALIBRATION_SAMPLE_COUNT 	(3)


void CONSOLE_State_Main(void);
void CONSOLE_State_ManualCalibration(void);
void CONSOLE_CalibrationInputState(void);
#endif // console_H
