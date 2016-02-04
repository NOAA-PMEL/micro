#include "gpio.h"
#include "i2c.h"
#include "PAxLD.h"
#include "unity.h"




void setUp(void)

{

}



void tearDown(void)

{

}



void test_module_generator_needs_to_be_implemented(void)

{

 char* val[8] = {0};

 sprintf(*val,"%d",52);

 UnityIgnore( ("Implement me!"), (_U_UINT)19);;

 UnityIgnore( (*val), (_U_UINT)20);;

}
