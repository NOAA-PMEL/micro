#include "unity.h"
#include "buffer8.h"
#include "../inc/msp430fr5969.h"


void setUp(void)
{

}

void tearDown(void)
{
}


/************************ Buffer8_Put Testing ************************/
void test_Buffer_Put_and_Get_should_WorkTogetherToInsertAndExtractAValue(void)
{
	CircularBuffer8_s tBuffer;
	Buffer8_Clear(&tBuffer);

	const uint8_t data[] = {0,1,0x5A,0xF0,0xFF};
	uint8_t fetched;
	int i;

	for(i=0;i<DIMENSION_OF(data);i++)
	{
		TEST_ASSERT_EQUAL(0,Buffer8_Put(&tBuffer, data[i]));
		TEST_ASSERT_EQUAL(0,Buffer8_Get(&tBuffer, &fetched));
		TEST_ASSERT_EQUAL_HEX16( data[i], fetched);
	}

}


void test_Buffer_Put_and_Get_should_WorkTogetherToInsertAndExtractManyValues(void)
{
	CircularBuffer8_s tBuffer;
	Buffer8_Clear(&tBuffer);

    const uint8_t data[] = {0,1,0x5A,0x11,0x0A,0xFF,0xA0,0xFF};
    uint8_t fetched;
    int i;

    for(i=0;i<DIMENSION_OF(data);i++)
    {
        TEST_ASSERT_EQUAL(0,Buffer8_Put(&tBuffer,data[i]));
    }

    for(i=0;i<DIMENSION_OF(data);i++)
    {
        TEST_ASSERT_EQUAL(0,Buffer8_Get(&tBuffer, &fetched));
        TEST_ASSERT_EQUAL_HEX16( data[i] , fetched);
    }
}


void test_Buffer_Put_and_Get_should_WorkTogetherToInsertAndExtractManyValuesRepeatedly(void)
{
	CircularBuffer8_s tBuffer;
	Buffer8_Clear(&tBuffer);

    const uint8_t data[] = {0,1,0x5A,0x11,0x0A,0x7F,0xA0,0xFF,0x8C,0xDC};
    uint8_t fetched;
    int i;

    for(i=0;i<(DIMENSION_OF(data)/2);i++)
    {
        TEST_ASSERT_EQUAL(0,Buffer8_Put(&tBuffer, data[i]));
    }

    for(i=0;i<(DIMENSION_OF(data)/2);i++)
    {
        TEST_ASSERT_EQUAL(0,Buffer8_Get(&tBuffer, &fetched));
        TEST_ASSERT_EQUAL_HEX16( data[i] , fetched);
    }

    for(i=(DIMENSION_OF(data)/2);i<(DIMENSION_OF(data));i++)
    {
        TEST_ASSERT_EQUAL(0,Buffer8_Put(&tBuffer, data[i]));
    }

    for(i=(DIMENSION_OF(data)/2);i<(DIMENSION_OF(data));i++)
    {
        TEST_ASSERT_EQUAL(0,Buffer8_Get(&tBuffer, &fetched));
        TEST_ASSERT_EQUAL_HEX16( data[i] , fetched);
    }
}

void test_Buffer_Get_should_ReturnErrorIfCalledWhenEmpty(void)
{
	CircularBuffer8_s tBuffer;
	Buffer8_Clear(&tBuffer);

    uint8_t fetched;

    TEST_ASSERT_EQUAL(BUFFER_ERROR_EMPTY,Buffer8_Get(&tBuffer, &fetched));
}


void test_Buffer_Put_should_ReturnErrorIfCalledWhenFull(void)
{
	CircularBuffer8_s tBuffer;
	Buffer8_Clear(&tBuffer);

    int i;
    int Expected[BUFFER8_SIZE];
    int Actual[BUFFER8_SIZE];

    for(i=0;i<BUFFER8_SIZE;i++)
    {
        Expected[i] = 0;
        Actual[i] = Buffer8_Put(&tBuffer, i);
    }

    TEST_ASSERT_EQUAL_INT_ARRAY(Expected,Actual,BUFFER8_SIZE);
    TEST_ASSERT_EQUAL(BUFFER_ERROR_FULL,Buffer8_Put(&tBuffer, 0x10));
}

//
void test_Buffer_Clear_should_ResetReadAndWriteToZero(void)
{
	CircularBuffer8_s tBuffer;
	Buffer8_Clear(&tBuffer);

    TEST_ASSERT_EQUAL(0,Buffer8_Clear(&tBuffer));
}


void test_Buffer_IsFull_should_ReturnZeroWhenNotFull(void)
{
	CircularBuffer8_s tBuffer;
    Buffer8_Clear(&tBuffer);
    TEST_ASSERT_EQUAL(BUFFER_NOT_FULL,Buffer8_IsFull(&tBuffer));
}

void test_Buffer_IsFull_should_ReturnValueWhenIsFull(void)
{
	CircularBuffer8_s tBuffer;
	Buffer8_Clear(&tBuffer);
    uint16_t i = 0;

    for(i=0;i<(BUFFER8_SIZE+20);i++)
    {
        Buffer8_Put(&tBuffer, i);
    }
    TEST_ASSERT_EQUAL(BUFFER_IS_FULL,Buffer8_IsFull(&tBuffer));
}


void test_Buffer_IsEmpty_should_ReturnValueWhenEmpty(void)
{
	CircularBuffer8_s tBuffer;
	const uint8_t data[] = {0,1,0x5A,0x11,0x0A,0x7F,0xA0,0xFF,0x8C,0xDC};
	Buffer8_Clear(&tBuffer);
	Buffer8_Put(&tBuffer,data[0]);
	Buffer8_Put(&tBuffer,data[1]);
    TEST_ASSERT_EQUAL(BUFFER_NOT_EMPTY,Buffer8_IsEmpty(&tBuffer));
}



void test_Buffer_IsEmpty_should_returnZeroWhenNotEmpty(void)
{
	CircularBuffer8_s tBuffer;
    Buffer8_Clear(&tBuffer);
    TEST_ASSERT_EQUAL(BUFFER_IS_EMPTY,Buffer8_IsEmpty(&tBuffer));
}
