#include "unity.h"
#include "console.h"
#include "../inc/includes.h"


void setUp(void)
{
	console_t console;          // Console State
	SystemValues_t ActiveSys;   // Normal running system values
	SystemValues_t FRAMSys;     // structures saved to FRAM

    console.state = Calibration;
    console.previousState = Main;
    console.SerialNumber = 7600;

    // Set System States
    FRAMSys.slope = 4.2;
    FRAMSys.intercept = 38.3;

    ActiveSys = FRAMSys;
}

void tearDown(void)
{
}

void test_module_generator_needs_to_be_implemented(void)
{
	TEST_IGNORE_MESSAGE("Implement me!");
}


void test_module_intial(void)
{
	console_t console;          // Console State
		SystemValues_t ActiveSys;   // Normal running system values
		SystemValues_t FRAMSys;     // structures saved to FRAM

	    console.state = Calibration;
	    console.previousState = Main;
	    console.SerialNumber = 7600;

	    // Set System States
	    FRAMSys.slope = 4.2;
	    FRAMSys.intercept = 38.3;

}
