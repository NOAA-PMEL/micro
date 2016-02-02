#include "unity.h"
#include "buffer_d.h"
#include "../inc/msp430fr5969.h"


void setUp(void)
{

}

void tearDown(void)
{
}


/************************ BufferD Testing ************************/
void test_Buffer_Put_and_Get_should_WorkTogetherToInsertAndExtractAValue(void)
{
	CircularBufferD_s tBuffer;
	BufferD_Clear(&tBuffer);

	double data[] = {0.0, 1.1, 256.1, -191.2};
	double fetched;
	int i;

	for(i=0;i<DIMENSION_OF(data);i++)
	{
		TEST_ASSERT_EQUAL(0,BufferD_Put(&tBuffer, data[i]));
		TEST_ASSERT_EQUAL(0,BufferD_Get(&tBuffer, &fetched));
		TEST_ASSERT_EQUAL_HEX16( data[i], fetched);
	}

}


void test_Buffer_Put_and_Get_should_WorkTogetherToInsertAndExtractManyValues(void)
{
	CircularBufferD_s tBuffer;
	BufferD_Clear(&tBuffer);

    const double data[] = {0,1,4818189,-111,3.223235,-999999,0.001,-0.0001};
    double fetched;
    int i;

    for(i=0;i<DIMENSION_OF(data);i++)
    {
        TEST_ASSERT_EQUAL(0,BufferD_Put(&tBuffer,data[i]));
    }

    for(i=0;i<DIMENSION_OF(data);i++)
    {
        TEST_ASSERT_EQUAL(0,BufferD_Get(&tBuffer, &fetched));
        TEST_ASSERT_EQUAL_HEX16( data[i] , fetched);
    }
}


void test_Buffer_Put_and_Get_should_WorkTogetherToInsertAndExtractManyValuesRepeatedly(void)
{
	CircularBufferD_s tBuffer;
	BufferD_Clear(&tBuffer);

	const double data[] = {0,1,14818189,-1112411,13.223235,-99999999.9,0.00001,-0.0235001};
    double fetched;
    int i;

    for(i=0;i<(DIMENSION_OF(data)/2);i++)
    {
        TEST_ASSERT_EQUAL(0,BufferD_Put(&tBuffer, data[i]));
    }

    for(i=0;i<(DIMENSION_OF(data)/2);i++)
    {
        TEST_ASSERT_EQUAL(0,BufferD_Get(&tBuffer, &fetched));
        TEST_ASSERT_EQUAL_HEX16( data[i] , fetched);
    }

    for(i=(DIMENSION_OF(data)/2);i<(DIMENSION_OF(data));i++)
    {
        TEST_ASSERT_EQUAL(0,BufferD_Put(&tBuffer, data[i]));
    }

    for(i=(DIMENSION_OF(data)/2);i<(DIMENSION_OF(data));i++)
    {
        TEST_ASSERT_EQUAL(0,BufferD_Get(&tBuffer, &fetched));
        TEST_ASSERT_EQUAL_HEX16( data[i] , fetched);
    }
}

void test_Buffer_Get_should_ReturnErrorIfCalledWhenEmpty(void)
{
	CircularBufferD_s tBuffer;
	BufferD_Clear(&tBuffer);

    double fetched;

    TEST_ASSERT_EQUAL(BUFFER_ERROR_EMPTY,BufferD_Get(&tBuffer, &fetched));
}


void test_Buffer_Put_should_ReturnErrorIfCalledWhenFull(void)
{
	CircularBufferD_s tBuffer;
	BufferD_Clear(&tBuffer);

    int i;
    int Expected[BUFFER_D_SIZE];
    int Actual[BUFFER_D_SIZE];

    for(i=0;i<BUFFER_D_SIZE;i++)
    {
        Expected[i] = 0;
        Actual[i] = BufferD_Put(&tBuffer, i);
    }

    TEST_ASSERT_EQUAL_INT_ARRAY(Expected,Actual,BUFFER_D_SIZE);
    TEST_ASSERT_EQUAL(BUFFER_ERROR_FULL,BufferD_Put(&tBuffer, 0x10));
}

//
void test_Buffer_Clear_should_ResetReadAndWriteToZero(void)
{
	CircularBufferD_s tBuffer;
	BufferD_Clear(&tBuffer);

    TEST_ASSERT_EQUAL(0,BufferD_Clear(&tBuffer));
}


void test_Buffer_IsFull_should_ReturnZeroWhenNotFull(void)
{
	CircularBufferD_s tBuffer;
    BufferD_Clear(&tBuffer);
    TEST_ASSERT_EQUAL(BUFFER_NOT_FULL,BufferD_IsFull(&tBuffer));
}

void test_Buffer_IsFull_should_ReturnValueWhenIsFull(void)
{
	CircularBufferD_s tBuffer;
	BufferD_Clear(&tBuffer);
    uint16_t i = 0;

    for(i=0;i<(BUFFER_D_SIZE+20);i++)
    {
        BufferD_Put(&tBuffer, i);
    }
    TEST_ASSERT_EQUAL(BUFFER_IS_FULL,BufferD_IsFull(&tBuffer));
}


void test_Buffer_IsEmpty_should_ReturnValueWhenEmpty(void)
{
	CircularBufferD_s tBuffer;
	const double data[] = {0,1,14818189,-1112411,13.223235,-99999999.9,0.00001,-0.0235001};
	BufferD_Clear(&tBuffer);
	BufferD_Put(&tBuffer,data[0]);
	BufferD_Put(&tBuffer,data[1]);
    TEST_ASSERT_EQUAL(BUFFER_NOT_EMPTY,BufferD_IsEmpty(&tBuffer));
}



void test_Buffer_IsEmpty_should_returnZeroWhenNotEmpty(void)
{
	CircularBufferD_s tBuffer;
    BufferD_Clear(&tBuffer);
    TEST_ASSERT_EQUAL(BUFFER_IS_EMPTY,BufferD_IsEmpty(&tBuffer));
}
