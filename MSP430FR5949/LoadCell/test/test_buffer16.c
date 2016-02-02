#include "unity.h"
#include "buffer16.h"
#include "../inc/msp430fr5969.h"


void setUp(void)
{

}

void tearDown(void)
{
}


/************************ Buffer16 Testing ************************/
void test_Buffer_Put_and_Get_should_WorkTogetherToInsertAndExtractAValue(void)
{
	CircularBuffer16_s tBuffer;
	Buffer16_Clear(&tBuffer);

	const uint16_t data[] = {0,1,0xA45A,0x1CF0,0xFFFF};
	uint16_t fetched;
	int i;

	for(i=0;i<DIMENSION_OF(data);i++)
	{
		TEST_ASSERT_EQUAL(0,Buffer16_Put(&tBuffer, data[i]));
		TEST_ASSERT_EQUAL(0,Buffer16_Get(&tBuffer, &fetched));
		TEST_ASSERT_EQUAL_HEX16( data[i], fetched);
	}

}


void test_Buffer_Put_and_Get_should_WorkTogetherToInsertAndExtractManyValues(void)
{
	CircularBuffer16_s tBuffer;
	Buffer16_Clear(&tBuffer);

    const uint16_t data[] = {0,1,0x5A,0x11,0x0A,0xFF,0xA0,0xFF};
    uint16_t fetched;
    int i;

    for(i=0;i<DIMENSION_OF(data);i++)
    {
        TEST_ASSERT_EQUAL(0,Buffer16_Put(&tBuffer,data[i]));
    }

    for(i=0;i<DIMENSION_OF(data);i++)
    {
        TEST_ASSERT_EQUAL(0,Buffer16_Get(&tBuffer, &fetched));
        TEST_ASSERT_EQUAL_HEX16( data[i] , fetched);
    }
}


void test_Buffer_Put_and_Get_should_WorkTogetherToInsertAndExtractManyValuesRepeatedly(void)
{
	CircularBuffer16_s tBuffer;
	Buffer16_Clear(&tBuffer);

    const uint16_t data[] = {0,1,0x5A,0x11,0x0A,0x7FFF,0xA0A0,0xFFFF,0x8C8C,0x11DC};
    uint16_t fetched;
    int i;

    for(i=0;i<(DIMENSION_OF(data)/2);i++)
    {
        TEST_ASSERT_EQUAL(0,Buffer16_Put(&tBuffer, data[i]));
    }

    for(i=0;i<(DIMENSION_OF(data)/2);i++)
    {
        TEST_ASSERT_EQUAL(0,Buffer16_Get(&tBuffer, &fetched));
        TEST_ASSERT_EQUAL_HEX16( data[i] , fetched);
    }

    for(i=(DIMENSION_OF(data)/2);i<(DIMENSION_OF(data));i++)
    {
        TEST_ASSERT_EQUAL(0,Buffer16_Put(&tBuffer, data[i]));
    }

    for(i=(DIMENSION_OF(data)/2);i<(DIMENSION_OF(data));i++)
    {
        TEST_ASSERT_EQUAL(0,Buffer16_Get(&tBuffer, &fetched));
        TEST_ASSERT_EQUAL_HEX16( data[i] , fetched);
    }
}

void test_Buffer_Get_should_ReturnErrorIfCalledWhenEmpty(void)
{
	CircularBuffer16_s tBuffer;
	Buffer16_Clear(&tBuffer);

    uint16_t fetched;

    TEST_ASSERT_EQUAL(BUFFER_ERROR_EMPTY,Buffer16_Get(&tBuffer, &fetched));
}


void test_Buffer_Put_should_ReturnErrorIfCalledWhenFull(void)
{
	CircularBuffer16_s tBuffer;
	Buffer16_Clear(&tBuffer);

    int i;
    int Expected[BUFFER16_SIZE];
    int Actual[BUFFER16_SIZE];

    for(i=0;i<BUFFER16_SIZE;i++)
    {
        Expected[i] = 0;
        Actual[i] = Buffer16_Put(&tBuffer, i);
    }

    TEST_ASSERT_EQUAL_INT_ARRAY(Expected,Actual,BUFFER16_SIZE);
    TEST_ASSERT_EQUAL(BUFFER_ERROR_FULL,Buffer16_Put(&tBuffer, 0x10));
}

//
void test_Buffer_Clear_should_ResetReadAndWriteToZero(void)
{
	CircularBuffer16_s tBuffer;
	Buffer16_Clear(&tBuffer);

    TEST_ASSERT_EQUAL(0,Buffer16_Clear(&tBuffer));
}


void test_Buffer_IsFull_should_ReturnZeroWhenNotFull(void)
{
	CircularBuffer16_s tBuffer;
    Buffer16_Clear(&tBuffer);
    TEST_ASSERT_EQUAL(BUFFER_NOT_FULL,Buffer16_IsFull(&tBuffer));
}

void test_Buffer_IsFull_should_ReturnValueWhenIsFull(void)
{
	CircularBuffer16_s tBuffer;
	Buffer16_Clear(&tBuffer);
    uint16_t i = 0;

    for(i=0;i<(BUFFER16_SIZE+20);i++)
    {
        Buffer16_Put(&tBuffer, i);
    }
    TEST_ASSERT_EQUAL(BUFFER_IS_FULL,Buffer16_IsFull(&tBuffer));
}


void test_Buffer_IsEmpty_should_ReturnValueWhenEmpty(void)
{
	CircularBuffer16_s tBuffer;
	const uint16_t data[] = {0,1,0x5A,0x11,0x0A,0x137F,0xA0,0xFFFF,0x6E8C,0xD3EC};
	Buffer16_Clear(&tBuffer);
	Buffer16_Put(&tBuffer,data[0]);
	Buffer16_Put(&tBuffer,data[1]);
    TEST_ASSERT_EQUAL(BUFFER_NOT_EMPTY,Buffer16_IsEmpty(&tBuffer));
}



void test_Buffer_IsEmpty_should_returnZeroWhenNotEmpty(void)
{
	CircularBuffer16_s tBuffer;
    Buffer16_Clear(&tBuffer);
    TEST_ASSERT_EQUAL(BUFFER_IS_EMPTY,Buffer16_IsEmpty(&tBuffer));
}
