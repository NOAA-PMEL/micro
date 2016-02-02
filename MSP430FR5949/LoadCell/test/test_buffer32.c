#include "unity.h"
#include "buffer32.h"
#include "../inc/msp430fr5969.h"


void setUp(void)
{

}

void tearDown(void)
{
}


/************************ Buffer32 Testing ************************/
void test_Buffer_Put_and_Get_should_WorkTogetherToInsertAndExtractAValue(void)
{
	CircularBuffer32_s tBuffer;
	Buffer32_Clear(&tBuffer);

	const uint32_t data[] = {0,1,0xAAACA45A,0x10FD1CF0,0xFFFFFFFF};
	uint32_t fetched;
	int i;

	for(i=0;i<DIMENSION_OF(data);i++)
	{
		TEST_ASSERT_EQUAL(0,Buffer32_Put(&tBuffer, data[i]));
		TEST_ASSERT_EQUAL(0,Buffer32_Get(&tBuffer, &fetched));
		TEST_ASSERT_EQUAL_HEX32( data[i], fetched);
	}

}


void test_Buffer_Put_and_Get_should_WorkTogetherToInsertAndExtractManyValues(void)
{
	CircularBuffer32_s tBuffer;
	Buffer32_Clear(&tBuffer);

    const uint32_t data[] = {0,1,0x5AF0CD1C,0x11111111,0x0A0A0A0A,0xF000000F,0xA0A0A0A0,0xFFFFFFFF};
    uint32_t fetched;
    int i;

    for(i=0;i<DIMENSION_OF(data);i++)
    {
        TEST_ASSERT_EQUAL(0,Buffer32_Put(&tBuffer,data[i]));
    }

    for(i=0;i<DIMENSION_OF(data);i++)
    {
        TEST_ASSERT_EQUAL(0,Buffer32_Get(&tBuffer, &fetched));
        TEST_ASSERT_EQUAL_HEX32( data[i] , fetched);
    }
}


void test_Buffer_Put_and_Get_should_WorkTogetherToInsertAndExtractManyValuesRepeatedly(void)
{
	CircularBuffer32_s tBuffer;
	Buffer32_Clear(&tBuffer);

    const uint32_t data[] = {0,1,0x5A,0x11,0x0A,0x7FFF,0xA0A0,0xFFFF,0x8C8C,0x11DC};
    uint32_t fetched;
    int i;

    for(i=0;i<(DIMENSION_OF(data)/2);i++)
    {
        TEST_ASSERT_EQUAL(0,Buffer32_Put(&tBuffer, data[i]));
    }

    for(i=0;i<(DIMENSION_OF(data)/2);i++)
    {
        TEST_ASSERT_EQUAL(0,Buffer32_Get(&tBuffer, &fetched));
        TEST_ASSERT_EQUAL_HEX32( data[i] , fetched);
    }

    for(i=(DIMENSION_OF(data)/2);i<(DIMENSION_OF(data));i++)
    {
        TEST_ASSERT_EQUAL(0,Buffer32_Put(&tBuffer, data[i]));
    }

    for(i=(DIMENSION_OF(data)/2);i<(DIMENSION_OF(data));i++)
    {
        TEST_ASSERT_EQUAL(0,Buffer32_Get(&tBuffer, &fetched));
        TEST_ASSERT_EQUAL_HEX32( data[i] , fetched);
    }
}

void test_Buffer_Get_should_ReturnErrorIfCalledWhenEmpty(void)
{
	CircularBuffer32_s tBuffer;
	Buffer32_Clear(&tBuffer);

    uint32_t fetched;

    TEST_ASSERT_EQUAL(BUFFER_ERROR_EMPTY,Buffer32_Get(&tBuffer, &fetched));
}


void test_Buffer_Put_should_ReturnErrorIfCalledWhenFull(void)
{
	CircularBuffer32_s tBuffer;
	Buffer32_Clear(&tBuffer);

    int i;
    int Expected[BUFFER32_SIZE];
    int Actual[BUFFER32_SIZE];

    for(i=0;i<BUFFER32_SIZE;i++)
    {
        Expected[i] = 0;
        Actual[i] = Buffer32_Put(&tBuffer, i);
    }

    TEST_ASSERT_EQUAL_INT_ARRAY(Expected,Actual,BUFFER32_SIZE);
    TEST_ASSERT_EQUAL(BUFFER_ERROR_FULL,Buffer32_Put(&tBuffer, 0x10));
}

//
void test_Buffer_Clear_should_ResetReadAndWriteToZero(void)
{
	CircularBuffer32_s tBuffer;
	Buffer32_Clear(&tBuffer);

    TEST_ASSERT_EQUAL(0,Buffer32_Clear(&tBuffer));
}


void test_Buffer_IsFull_should_ReturnZeroWhenNotFull(void)
{
	CircularBuffer32_s tBuffer;
    Buffer32_Clear(&tBuffer);
    TEST_ASSERT_EQUAL(BUFFER_NOT_FULL,Buffer32_IsFull(&tBuffer));
}

void test_Buffer_IsFull_should_ReturnValueWhenIsFull(void)
{
	CircularBuffer32_s tBuffer;
	Buffer32_Clear(&tBuffer);
    uint32_t i = 0;

    for(i=0;i<(BUFFER32_SIZE+20);i++)
    {
        Buffer32_Put(&tBuffer, i);
    }
    TEST_ASSERT_EQUAL(BUFFER_IS_FULL,Buffer32_IsFull(&tBuffer));
}


void test_Buffer_IsEmpty_should_ReturnValueWhenEmpty(void)
{
	CircularBuffer32_s tBuffer;
	const uint32_t data[] = {0,1,0x5A,0x11,0x0A,0x137F,0xA0,0xFFFF,0x6E8C,0xD3EC};
	Buffer32_Clear(&tBuffer);
	Buffer32_Put(&tBuffer,data[0]);
	Buffer32_Put(&tBuffer,data[1]);
    TEST_ASSERT_EQUAL(BUFFER_NOT_EMPTY,Buffer32_IsEmpty(&tBuffer));
}



void test_Buffer_IsEmpty_should_returnZeroWhenNotEmpty(void)
{
	CircularBuffer32_s tBuffer;
    Buffer32_Clear(&tBuffer);
    TEST_ASSERT_EQUAL(BUFFER_IS_EMPTY,Buffer32_IsEmpty(&tBuffer));
}
