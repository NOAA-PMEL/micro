#include "unity.h"
#include "buffer_f.h"
#include "../test/msp430fr5969.h"


void setUp(void)
{

}

void tearDown(void)
{
}


/************************ BufferF Testing ************************/
void test_Buffer_Put_and_Get_should_WorkTogetherToInsertAndExtractAValue(void)
{
	CircularBufferF_s tBuffer;
	BufferF_Clear(&tBuffer);

	float data[] = {0.0, 1.46, 2256.1, 53.2};
	float fetched;
	int i;

	for(i=0;i<DIMENSION_OF(data);i++)
	{
		TEST_ASSERT_EQUAL(0,BufferF_Put(&tBuffer, data[i]));
		TEST_ASSERT_EQUAL(0,BufferF_Get(&tBuffer, &fetched));
		TEST_ASSERT_EQUAL_HEX16( data[i], fetched);
	}

}


void test_Buffer_Put_and_Get_should_WorkTogetherToInsertAndExtractManyValues(void)
{
	CircularBufferF_s tBuffer;
	BufferF_Clear(&tBuffer);

	float data[] = {0.000, 1.1346, 12256.12, 53.2002};
    float fetched;
    int i;

    for(i=0;i<DIMENSION_OF(data);i++)
    {
        TEST_ASSERT_EQUAL(0,BufferF_Put(&tBuffer,data[i]));
    }

    for(i=0;i<DIMENSION_OF(data);i++)
    {
        TEST_ASSERT_EQUAL(0,BufferF_Get(&tBuffer, &fetched));
        TEST_ASSERT_EQUAL_HEX16( data[i] , fetched);
    }
}


void test_Buffer_Put_and_Get_should_WorkTogetherToInsertAndExtractManyValuesRepeatedly(void)
{
	CircularBufferF_s tBuffer;
	BufferF_Clear(&tBuffer);

	float data[] = {0.000, 1.1346, 12256.12, 53.2002,-3298.2};
    float fetched;
    int i;

    for(i=0;i<(DIMENSION_OF(data)/2);i++)
    {
        TEST_ASSERT_EQUAL(0,BufferF_Put(&tBuffer, data[i]));
    }

    for(i=0;i<(DIMENSION_OF(data)/2);i++)
    {
        TEST_ASSERT_EQUAL(0,BufferF_Get(&tBuffer, &fetched));
        TEST_ASSERT_EQUAL_HEX16( data[i] , fetched);
    }

    for(i=(DIMENSION_OF(data)/2);i<(DIMENSION_OF(data));i++)
    {
        TEST_ASSERT_EQUAL(0,BufferF_Put(&tBuffer, data[i]));
    }

    for(i=(DIMENSION_OF(data)/2);i<(DIMENSION_OF(data));i++)
    {
        TEST_ASSERT_EQUAL(0,BufferF_Get(&tBuffer, &fetched));
        TEST_ASSERT_EQUAL_HEX16( data[i] , fetched);
    }
}

void test_Buffer_Get_should_ReturnErrorIfCalledWhenEmpty(void)
{
	CircularBufferF_s tBuffer;
	BufferF_Clear(&tBuffer);

    float fetched;

    TEST_ASSERT_EQUAL(BUFFER_ERROR_EMPTY,BufferF_Get(&tBuffer, &fetched));
}


void test_Buffer_Put_should_ReturnErrorIfCalledWhenFull(void)
{
	CircularBufferF_s tBuffer;
	BufferF_Clear(&tBuffer);

    int i;
    int Expected[BUFFER_F_SIZE];
    int Actual[BUFFER_F_SIZE];

    for(i=0;i<BUFFER_F_SIZE;i++)
    {
        Expected[i] = 0;
        Actual[i] = BufferF_Put(&tBuffer, i);
    }

    TEST_ASSERT_EQUAL_INT_ARRAY(Expected,Actual,BUFFER_F_SIZE);
    TEST_ASSERT_EQUAL(BUFFER_ERROR_FULL,BufferF_Put(&tBuffer, 0x10));
}

//
void test_Buffer_Clear_should_ResetReadAndWriteToZero(void)
{
	CircularBufferF_s tBuffer;
	BufferF_Clear(&tBuffer);

    TEST_ASSERT_EQUAL(0,BufferF_Clear(&tBuffer));
}


void test_Buffer_IsFull_should_ReturnZeroWhenNotFull(void)
{
	CircularBufferF_s tBuffer;
    BufferF_Clear(&tBuffer);
    TEST_ASSERT_EQUAL(BUFFER_NOT_FULL,BufferF_IsFull(&tBuffer));
}

void test_Buffer_IsFull_should_ReturnValueWhenIsFull(void)
{
	CircularBufferF_s tBuffer;
	BufferF_Clear(&tBuffer);
    uint16_t i = 0;

    for(i=0;i<(BUFFER_F_SIZE+20);i++)
    {
        BufferF_Put(&tBuffer, i);
    }
    TEST_ASSERT_EQUAL(BUFFER_IS_FULL,BufferF_IsFull(&tBuffer));
}


void test_Buffer_IsEmpty_should_ReturnValueWhenEmpty(void)
{
	CircularBufferF_s tBuffer;
	float data[] = {0.000, 1.1346, 12256.12, 53.2002, -1839.8};
	BufferF_Clear(&tBuffer);
	BufferF_Put(&tBuffer,data[0]);
	BufferF_Put(&tBuffer,data[1]);
    TEST_ASSERT_EQUAL(BUFFER_NOT_EMPTY,BufferF_IsEmpty(&tBuffer));
}



void test_Buffer_IsEmpty_should_returnZeroWhenNotEmpty(void)
{
	CircularBufferF_s tBuffer;
    BufferF_Clear(&tBuffer);
    TEST_ASSERT_EQUAL(BUFFER_IS_EMPTY,BufferF_IsEmpty(&tBuffer));
}
