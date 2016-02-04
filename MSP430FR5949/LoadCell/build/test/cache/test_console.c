#include "../inc/includes.h"
#include "console.h"
#include "unity.h"




void setUp(void)

{

 console_t console;

 SystemValues_t ActiveSys;

 SystemValues_t FRAMSys;



    console.state = Calibration;

    console.previousState = Main;

    console.SerialNumber = 7600;





    FRAMSys.slope = 4.2;

    FRAMSys.intercept = 38.3;



    ActiveSys = FRAMSys;

}



void tearDown(void)

{

}



void test_module_generator_needs_to_be_implemented(void)

{

 UnityIgnore( ("Implement me!"), (_U_UINT)29);;

}





void test_module_intial(void)

{

 console_t console;

  SystemValues_t ActiveSys;

  SystemValues_t FRAMSys;



     console.state = Calibration;

     console.previousState = Main;

     console.SerialNumber = 7600;





     FRAMSys.slope = 4.2;

     FRAMSys.intercept = 38.3;



}
