#include "unity.h"
#include "buffer_c.h"
#include "../test/msp430fr5969.h"


void setUp(void)
{

}

void tearDown(void)
{
}


/************************ BufferC_Put Testing ************************/
void test_Buffer_Put_and_Get_should_WorkTogetherToInsertAndExtractAValue(void)
{
	CircularBufferC_s tBuffer;
	BufferC_Clear(&tBuffer);

	const char data[] = {0,1,0x5A,0xF0,0xFF};
	char fetched;
	int i;

	for(i=0;i<DIMENSION_OF(data);i++)
	{
		TEST_ASSERT_EQUAL(0,BufferC_Put(&tBuffer, data[i]));
		TEST_ASSERT_EQUAL(0,BufferC_Get(&tBuffer, &fetched));
		TEST_ASSERT_EQUAL_HEX16( data[i], fetched);
	}

}


void test_Buffer_Put_and_Get_should_WorkTogetherToInsertAndExtractManyValues(void)
{
	CircularBufferC_s tBuffer;
	BufferC_Clear(&tBuffer);

    const char data[] = {0,1,0x5A,0x11,0x0A,0xFF,0xA0,0xFF};
    char fetched;
    int i;

    for(i=0;i<DIMENSION_OF(data);i++)
    {
        TEST_ASSERT_EQUAL(0,BufferC_Put(&tBuffer,data[i]));
    }

    for(i=0;i<DIMENSION_OF(data);i++)
    {
        TEST_ASSERT_EQUAL(0,BufferC_Get(&tBuffer, &fetched));
        TEST_ASSERT_EQUAL_HEX16( data[i] , fetched);
    }
}


void test_Buffer_Put_and_Get_should_WorkTogetherToInsertAndExtractManyValuesRepeatedly(void)
{
	CircularBufferC_s tBuffer;
	BufferC_Clear(&tBuffer);

    const char data[] = {0,1,0x5A,0x11,0x0A,0x7F,0xA0,0xFF,0x8C,0xDC};
    char fetched;
    int i;

    for(i=0;i<(DIMENSION_OF(data)/2);i++)
    {
        TEST_ASSERT_EQUAL(0,BufferC_Put(&tBuffer, data[i]));
    }

    for(i=0;i<(DIMENSION_OF(data)/2);i++)
    {
        TEST_ASSERT_EQUAL(0,BufferC_Get(&tBuffer, &fetched));
        TEST_ASSERT_EQUAL_HEX16( data[i] , fetched);
    }

    for(i=(DIMENSION_OF(data)/2);i<(DIMENSION_OF(data));i++)
    {
        TEST_ASSERT_EQUAL(0,BufferC_Put(&tBuffer, data[i]));
    }

    for(i=(DIMENSION_OF(data)/2);i<(DIMENSION_OF(data));i++)
    {
        TEST_ASSERT_EQUAL(0,BufferC_Get(&tBuffer, &fetched));
        TEST_ASSERT_EQUAL_HEX16( data[i] , fetched);
    }
}

void test_Buffer_Get_should_ReturnErrorIfCalledWhenEmpty(void)
{
	CircularBufferC_s tBuffer;
	BufferC_Clear(&tBuffer);

    char fetched;

    TEST_ASSERT_EQUAL(BUFFER_ERROR_EMPTY,BufferC_Get(&tBuffer, &fetched));
}


void test_Buffer_Put_should_ReturnErrorIfCalledWhenFull(void)
{
//	CircularBufferC_s tBuffer;
//	BufferC_Clear(&tBuffer);
//
//    int i;
//    char Expected[BUFFER_C_SIZE];
//    char Actual[BUFFER_C_SIZE];
//
//    for(i=0;i<BUFFER_C_SIZE;i++)
//    {
//        Expected[i] = 'a';
//        //Actual[i] = BufferC_Put(&tBuffer, 'a');
//        TEST_ASSERT_EQUAL(0,BufferC_Put(&tBuffer,'a'));
//
//        //TEST_ASSERT_EQUAL(Expected[i],Actual[i]);
//    }
//
//    TEST_ASSERT_EQUAL_HEX8_ARRAY(tBuffer.buffer,Expected,BUFFER_C_SIZE);
//
////    TEST_ASSERT_EQUAL_HEX8_ARRAY(Expected,Actual,BUFFER_C_SIZE);
//    TEST_ASSERT_EQUAL(BUFFER_ERROR_FULL,BufferC_Put(&tBuffer, 0x10));


    CircularBufferC_s tBuffer;
	BufferC_Clear(&tBuffer);

	int i;
	int Expected[BUFFER_C_SIZE];
	int Actual[BUFFER_C_SIZE];

	for(i=0;i<BUFFER_C_SIZE;i++)
	{
		Expected[i] = 0;
		Actual[i] = BufferC_Put(&tBuffer, i);
	}

	TEST_ASSERT_EQUAL_INT_ARRAY(Expected,Actual,BUFFER_C_SIZE);
	TEST_ASSERT_EQUAL(BUFFER_ERROR_FULL,BufferC_Put(&tBuffer, 0x10));
}

//
void test_Buffer_Clear_should_ResetReadAndWriteToZero(void)
{
	CircularBufferC_s tBuffer;
	BufferC_Clear(&tBuffer);

    TEST_ASSERT_EQUAL(0,BufferC_Clear(&tBuffer));
}


void test_Buffer_IsFull_should_ReturnZeroWhenNotFull(void)
{
	CircularBufferC_s tBuffer;
    BufferC_Clear(&tBuffer);
    TEST_ASSERT_EQUAL(BUFFER_NOT_FULL,BufferC_IsFull(&tBuffer));
}

void test_Buffer_IsFull_should_ReturnValueWhenIsFull(void)
{
	CircularBufferC_s tBuffer;
	BufferC_Clear(&tBuffer);
    uint16_t i = 0;

    for(i=0;i<(BUFFER_C_SIZE+20);i++)
    {
        BufferC_Put(&tBuffer, i);
    }
    TEST_ASSERT_EQUAL(BUFFER_IS_FULL,BufferC_IsFull(&tBuffer));
}


void test_Buffer_IsEmpty_should_ReturnValueWhenEmpty(void)
{
	CircularBufferC_s tBuffer;
	const char data[] = {0,1,0x5A,0x11,0x0A,0x7F,0xA0,0xFF,0x8C,0xDC};
	BufferC_Clear(&tBuffer);
	BufferC_Put(&tBuffer,data[0]);
	BufferC_Put(&tBuffer,data[1]);
    TEST_ASSERT_EQUAL(BUFFER_NOT_EMPTY,BufferC_IsEmpty(&tBuffer));
}



void test_Buffer_IsEmpty_should_returnZeroWhenNotEmpty(void)
{
	CircularBufferC_s tBuffer;
    BufferC_Clear(&tBuffer);
    TEST_ASSERT_EQUAL(BUFFER_IS_EMPTY,BufferC_IsEmpty(&tBuffer));
}


