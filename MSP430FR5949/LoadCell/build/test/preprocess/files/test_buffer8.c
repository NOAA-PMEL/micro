#include "../test/msp430fr5969.h"
#include "buffer8.h"
#include "unity.h"




void setUp(void)

{



}



void tearDown(void)

{

}







void test_Buffer_Put_and_Get_should_WorkTogetherToInsertAndExtractAValue(void)

{

 CircularBuffer8_s tBuffer;

 Buffer8_Clear(&tBuffer);



 const uint8_t data[] = {0,1,0x5A,0xF0,0xFF};

 uint8_t fetched;

 int i;



 for(i=0;i<(sizeof(data)/sizeof(data[0]));i++)

 {

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((Buffer8_Put(&tBuffer, data[i]))), (((void *)0)), (_U_UINT)28, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((Buffer8_Get(&tBuffer, &fetched))), (((void *)0)), (_U_UINT)29, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)(_US16)((data[i])), (_U_SINT)(_US16)((fetched)), (((void *)0)), (_U_UINT)30, UNITY_DISPLAY_STYLE_HEX16);

 }



}





void test_Buffer_Put_and_Get_should_WorkTogetherToInsertAndExtractManyValues(void)

{

 CircularBuffer8_s tBuffer;

 Buffer8_Clear(&tBuffer);



    const uint8_t data[] = {0,1,0x5A,0x11,0x0A,0xFF,0xA0,0xFF};

    uint8_t fetched;

    int i;



    for(i=0;i<(sizeof(data)/sizeof(data[0]));i++)

    {

        UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((Buffer8_Put(&tBuffer,data[i]))), (((void *)0)), (_U_UINT)47, UNITY_DISPLAY_STYLE_INT);

    }



    for(i=0;i<(sizeof(data)/sizeof(data[0]));i++)

    {

        UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((Buffer8_Get(&tBuffer, &fetched))), (((void *)0)), (_U_UINT)52, UNITY_DISPLAY_STYLE_INT);

        UnityAssertEqualNumber((_U_SINT)(_US16)((data[i])), (_U_SINT)(_US16)((fetched)), (((void *)0)), (_U_UINT)53, UNITY_DISPLAY_STYLE_HEX16);

    }

}





void test_Buffer_Put_and_Get_should_WorkTogetherToInsertAndExtractManyValuesRepeatedly(void)

{

 CircularBuffer8_s tBuffer;

 Buffer8_Clear(&tBuffer);



    const uint8_t data[] = {0,1,0x5A,0x11,0x0A,0x7F,0xA0,0xFF,0x8C,0xDC};

    uint8_t fetched;

    int i;



    for(i=0;i<((sizeof(data)/sizeof(data[0]))/2);i++)

    {

        UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((Buffer8_Put(&tBuffer, data[i]))), (((void *)0)), (_U_UINT)69, UNITY_DISPLAY_STYLE_INT);

    }



    for(i=0;i<((sizeof(data)/sizeof(data[0]))/2);i++)

    {

        UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((Buffer8_Get(&tBuffer, &fetched))), (((void *)0)), (_U_UINT)74, UNITY_DISPLAY_STYLE_INT);

        UnityAssertEqualNumber((_U_SINT)(_US16)((data[i])), (_U_SINT)(_US16)((fetched)), (((void *)0)), (_U_UINT)75, UNITY_DISPLAY_STYLE_HEX16);

    }



    for(i=((sizeof(data)/sizeof(data[0]))/2);i<((sizeof(data)/sizeof(data[0])));i++)

    {

        UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((Buffer8_Put(&tBuffer, data[i]))), (((void *)0)), (_U_UINT)80, UNITY_DISPLAY_STYLE_INT);

    }



    for(i=((sizeof(data)/sizeof(data[0]))/2);i<((sizeof(data)/sizeof(data[0])));i++)

    {

        UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((Buffer8_Get(&tBuffer, &fetched))), (((void *)0)), (_U_UINT)85, UNITY_DISPLAY_STYLE_INT);

        UnityAssertEqualNumber((_U_SINT)(_US16)((data[i])), (_U_SINT)(_US16)((fetched)), (((void *)0)), (_U_UINT)86, UNITY_DISPLAY_STYLE_HEX16);

    }

}



void test_Buffer_Get_should_ReturnErrorIfCalledWhenEmpty(void)

{

 CircularBuffer8_s tBuffer;

 Buffer8_Clear(&tBuffer);



    uint8_t fetched;



    UnityAssertEqualNumber((_U_SINT)(((1))), (_U_SINT)((Buffer8_Get(&tBuffer, &fetched))), (((void *)0)), (_U_UINT)97, UNITY_DISPLAY_STYLE_INT);

}





void test_Buffer_Put_should_ReturnErrorIfCalledWhenFull(void)

{

 CircularBuffer8_s tBuffer;

 Buffer8_Clear(&tBuffer);



    int i;

    int Expected[(255)];

    int Actual[(255)];



    for(i=0;i<(255);i++)

    {

        Expected[i] = 0;

        Actual[i] = Buffer8_Put(&tBuffer, i);

    }



    UnityAssertEqualIntArray(( const void*)(Expected), ( const void*)(Actual), (_UU32)((255)), (((void *)0)), (_U_UINT)116, UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)(((2))), (_U_SINT)((Buffer8_Put(&tBuffer, 0x10))), (((void *)0)), (_U_UINT)117, UNITY_DISPLAY_STYLE_INT);

}





void test_Buffer_Clear_should_ResetReadAndWriteToZero(void)

{

 CircularBuffer8_s tBuffer;

 Buffer8_Clear(&tBuffer);



    UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((Buffer8_Clear(&tBuffer))), (((void *)0)), (_U_UINT)126, UNITY_DISPLAY_STYLE_INT);

}





void test_Buffer_IsFull_should_ReturnZeroWhenNotFull(void)

{

 CircularBuffer8_s tBuffer;

    Buffer8_Clear(&tBuffer);

    UnityAssertEqualNumber((_U_SINT)(((0))), (_U_SINT)((Buffer8_IsFull(&tBuffer))), (((void *)0)), (_U_UINT)134, UNITY_DISPLAY_STYLE_INT);

}



void test_Buffer_IsFull_should_ReturnValueWhenIsFull(void)

{

 CircularBuffer8_s tBuffer;

 Buffer8_Clear(&tBuffer);

    uint16_t i = 0;



    for(i=0;i<((255)+20);i++)

    {

        Buffer8_Put(&tBuffer, i);

    }

    UnityAssertEqualNumber((_U_SINT)(((2))), (_U_SINT)((Buffer8_IsFull(&tBuffer))), (((void *)0)), (_U_UINT)147, UNITY_DISPLAY_STYLE_INT);

}





void test_Buffer_IsEmpty_should_ReturnValueWhenEmpty(void)

{

 CircularBuffer8_s tBuffer;

 const uint8_t data[] = {0,1,0x5A,0x11,0x0A,0x7F,0xA0,0xFF,0x8C,0xDC};

 Buffer8_Clear(&tBuffer);

 Buffer8_Put(&tBuffer,data[0]);

 Buffer8_Put(&tBuffer,data[1]);

    UnityAssertEqualNumber((_U_SINT)((((0)))), (_U_SINT)((Buffer8_IsEmpty(&tBuffer))), (((void *)0)), (_U_UINT)158, UNITY_DISPLAY_STYLE_INT);

}







void test_Buffer_IsEmpty_should_returnZeroWhenNotEmpty(void)

{

 CircularBuffer8_s tBuffer;

    Buffer8_Clear(&tBuffer);

    UnityAssertEqualNumber((_U_SINT)(((1))), (_U_SINT)((Buffer8_IsEmpty(&tBuffer))), (((void *)0)), (_U_UINT)167, UNITY_DISPLAY_STYLE_INT);

}
