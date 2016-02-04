#include "../test/msp430fr5969.h"
#include "buffer32.h"
#include "unity.h"




void setUp(void)

{



}



void tearDown(void)

{

}







void test_Buffer_Put_and_Get_should_WorkTogetherToInsertAndExtractAValue(void)

{

 CircularBuffer32_s tBuffer;

 Buffer32_Clear(&tBuffer);



 const uint32_t data[] = {0,1,0xAAACA45A,0x10FD1CF0,0xFFFFFFFF};

 uint32_t fetched;

 int i;



 for(i=0;i<(sizeof(data)/sizeof(data[0]));i++)

 {

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((Buffer32_Put(&tBuffer, data[i]))), (((void *)0)), (_U_UINT)28, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((Buffer32_Get(&tBuffer, &fetched))), (((void *)0)), (_U_UINT)29, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)(_US32)((data[i])), (_U_SINT)(_US32)((fetched)), (((void *)0)), (_U_UINT)30, UNITY_DISPLAY_STYLE_HEX32);

 }



}





void test_Buffer_Put_and_Get_should_WorkTogetherToInsertAndExtractManyValues(void)

{

 CircularBuffer32_s tBuffer;

 Buffer32_Clear(&tBuffer);



    const uint32_t data[] = {0,1,0x5AF0CD1C,0x11111111,0x0A0A0A0A,0xF000000F,0xA0A0A0A0,0xFFFFFFFF};

    uint32_t fetched;

    int i;



    for(i=0;i<(sizeof(data)/sizeof(data[0]));i++)

    {

        UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((Buffer32_Put(&tBuffer,data[i]))), (((void *)0)), (_U_UINT)47, UNITY_DISPLAY_STYLE_INT);

    }



    for(i=0;i<(sizeof(data)/sizeof(data[0]));i++)

    {

        UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((Buffer32_Get(&tBuffer, &fetched))), (((void *)0)), (_U_UINT)52, UNITY_DISPLAY_STYLE_INT);

        UnityAssertEqualNumber((_U_SINT)(_US32)((data[i])), (_U_SINT)(_US32)((fetched)), (((void *)0)), (_U_UINT)53, UNITY_DISPLAY_STYLE_HEX32);

    }

}





void test_Buffer_Put_and_Get_should_WorkTogetherToInsertAndExtractManyValuesRepeatedly(void)

{

 CircularBuffer32_s tBuffer;

 Buffer32_Clear(&tBuffer);



    const uint32_t data[] = {0,1,0x5A,0x11,0x0A,0x7FFF,0xA0A0,0xFFFF,0x8C8C,0x11DC};

    uint32_t fetched;

    int i;



    for(i=0;i<((sizeof(data)/sizeof(data[0]))/2);i++)

    {

        UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((Buffer32_Put(&tBuffer, data[i]))), (((void *)0)), (_U_UINT)69, UNITY_DISPLAY_STYLE_INT);

    }



    for(i=0;i<((sizeof(data)/sizeof(data[0]))/2);i++)

    {

        UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((Buffer32_Get(&tBuffer, &fetched))), (((void *)0)), (_U_UINT)74, UNITY_DISPLAY_STYLE_INT);

        UnityAssertEqualNumber((_U_SINT)(_US32)((data[i])), (_U_SINT)(_US32)((fetched)), (((void *)0)), (_U_UINT)75, UNITY_DISPLAY_STYLE_HEX32);

    }



    for(i=((sizeof(data)/sizeof(data[0]))/2);i<((sizeof(data)/sizeof(data[0])));i++)

    {

        UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((Buffer32_Put(&tBuffer, data[i]))), (((void *)0)), (_U_UINT)80, UNITY_DISPLAY_STYLE_INT);

    }



    for(i=((sizeof(data)/sizeof(data[0]))/2);i<((sizeof(data)/sizeof(data[0])));i++)

    {

        UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((Buffer32_Get(&tBuffer, &fetched))), (((void *)0)), (_U_UINT)85, UNITY_DISPLAY_STYLE_INT);

        UnityAssertEqualNumber((_U_SINT)(_US32)((data[i])), (_U_SINT)(_US32)((fetched)), (((void *)0)), (_U_UINT)86, UNITY_DISPLAY_STYLE_HEX32);

    }

}



void test_Buffer_Get_should_ReturnErrorIfCalledWhenEmpty(void)

{

 CircularBuffer32_s tBuffer;

 Buffer32_Clear(&tBuffer);



    uint32_t fetched;



    UnityAssertEqualNumber((_U_SINT)(((1))), (_U_SINT)((Buffer32_Get(&tBuffer, &fetched))), (((void *)0)), (_U_UINT)97, UNITY_DISPLAY_STYLE_INT);

}





void test_Buffer_Put_should_ReturnErrorIfCalledWhenFull(void)

{

 CircularBuffer32_s tBuffer;

 Buffer32_Clear(&tBuffer);



    int i;

    int Expected[(255)];

    int Actual[(255)];



    for(i=0;i<(255);i++)

    {

        Expected[i] = 0;

        Actual[i] = Buffer32_Put(&tBuffer, i);

    }



    UnityAssertEqualIntArray(( const void*)(Expected), ( const void*)(Actual), (_UU32)((255)), (((void *)0)), (_U_UINT)116, UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)(((2))), (_U_SINT)((Buffer32_Put(&tBuffer, 0x10))), (((void *)0)), (_U_UINT)117, UNITY_DISPLAY_STYLE_INT);

}





void test_Buffer_Clear_should_ResetReadAndWriteToZero(void)

{

 CircularBuffer32_s tBuffer;

 Buffer32_Clear(&tBuffer);



    UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((Buffer32_Clear(&tBuffer))), (((void *)0)), (_U_UINT)126, UNITY_DISPLAY_STYLE_INT);

}





void test_Buffer_IsFull_should_ReturnZeroWhenNotFull(void)

{

 CircularBuffer32_s tBuffer;

    Buffer32_Clear(&tBuffer);

    UnityAssertEqualNumber((_U_SINT)(((0))), (_U_SINT)((Buffer32_IsFull(&tBuffer))), (((void *)0)), (_U_UINT)134, UNITY_DISPLAY_STYLE_INT);

}



void test_Buffer_IsFull_should_ReturnValueWhenIsFull(void)

{

 CircularBuffer32_s tBuffer;

 Buffer32_Clear(&tBuffer);

    uint32_t i = 0;



    for(i=0;i<((255)+20);i++)

    {

        Buffer32_Put(&tBuffer, i);

    }

    UnityAssertEqualNumber((_U_SINT)(((2))), (_U_SINT)((Buffer32_IsFull(&tBuffer))), (((void *)0)), (_U_UINT)147, UNITY_DISPLAY_STYLE_INT);

}





void test_Buffer_IsEmpty_should_ReturnValueWhenEmpty(void)

{

 CircularBuffer32_s tBuffer;

 const uint32_t data[] = {0,1,0x5A,0x11,0x0A,0x137F,0xA0,0xFFFF,0x6E8C,0xD3EC};

 Buffer32_Clear(&tBuffer);

 Buffer32_Put(&tBuffer,data[0]);

 Buffer32_Put(&tBuffer,data[1]);

    UnityAssertEqualNumber((_U_SINT)((((0)))), (_U_SINT)((Buffer32_IsEmpty(&tBuffer))), (((void *)0)), (_U_UINT)158, UNITY_DISPLAY_STYLE_INT);

}







void test_Buffer_IsEmpty_should_returnZeroWhenNotEmpty(void)

{

 CircularBuffer32_s tBuffer;

    Buffer32_Clear(&tBuffer);

    UnityAssertEqualNumber((_U_SINT)(((1))), (_U_SINT)((Buffer32_IsEmpty(&tBuffer))), (((void *)0)), (_U_UINT)167, UNITY_DISPLAY_STYLE_INT);

}
