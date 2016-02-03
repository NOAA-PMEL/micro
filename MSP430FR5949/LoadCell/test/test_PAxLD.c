#include "unity.h"
#include "PAxLD.h"
#include "i2c.h"
#include "gpio.h"


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
	TEST_IGNORE_MESSAGE("Implement me!");
	TEST_IGNORE_MESSAGE(*val);
}
