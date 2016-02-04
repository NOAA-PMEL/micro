#include "../test/msp430fr5969.h"
#include "buffer_c.h"
#include "unity.h"




void setUp(void)

{



}



void tearDown(void)

{

}







void test_Buffer_Put_and_Get_should_WorkTogetherToInsertAndExtractAValue(void)

{

 CircularBufferC_s tBuffer;

 BufferC_Clear(&tBuffer);



 const char data[] = {0,1,0x5A,0xF0,0xFF};

 char fetched;

 int i;



 for(i=0;i<(sizeof(data)/sizeof(data[0]));i++)

 {

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((BufferC_Put(&tBuffer, data[i]))), (((void *)0)), (_U_UINT)28, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((BufferC_Get(&tBuffer, &fetched))), (((void *)0)), (_U_UINT)29, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)(_US16)((data[i])), (_U_SINT)(_US16)((fetched)), (((void *)0)), (_U_UINT)30, UNITY_DISPLAY_STYLE_HEX16);

 }



}





void test_Buffer_Put_and_Get_should_WorkTogetherToInsertAndExtractManyValues(void)

{

 CircularBufferC_s tBuffer;

 BufferC_Clear(&tBuffer);



    const char data[] = {0,1,0x5A,0x11,0x0A,0xFF,0xA0,0xFF};

    char fetched;

    int i;



    for(i=0;i<(sizeof(data)/sizeof(data[0]));i++)

    {

        UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((BufferC_Put(&tBuffer,data[i]))), (((void *)0)), (_U_UINT)47, UNITY_DISPLAY_STYLE_INT);

    }



    for(i=0;i<(sizeof(data)/sizeof(data[0]));i++)

    {

        UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((BufferC_Get(&tBuffer, &fetched))), (((void *)0)), (_U_UINT)52, UNITY_DISPLAY_STYLE_INT);

        UnityAssertEqualNumber((_U_SINT)(_US16)((data[i])), (_U_SINT)(_US16)((fetched)), (((void *)0)), (_U_UINT)53, UNITY_DISPLAY_STYLE_HEX16);

    }

}





void test_Buffer_Put_and_Get_should_WorkTogetherToInsertAndExtractManyValuesRepeatedly(void)

{

 CircularBufferC_s tBuffer;

 BufferC_Clear(&tBuffer);



    const char data[] = {0,1,0x5A,0x11,0x0A,0x7F,0xA0,0xFF,0x8C,0xDC};

    char fetched;

    int i;



    for(i=0;i<((sizeof(data)/sizeof(data[0]))/2);i++)

    {

        UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((BufferC_Put(&tBuffer, data[i]))), (((void *)0)), (_U_UINT)69, UNITY_DISPLAY_STYLE_INT);

    }



    for(i=0;i<((sizeof(data)/sizeof(data[0]))/2);i++)

    {

        UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((BufferC_Get(&tBuffer, &fetched))), (((void *)0)), (_U_UINT)74, UNITY_DISPLAY_STYLE_INT);

        UnityAssertEqualNumber((_U_SINT)(_US16)((data[i])), (_U_SINT)(_US16)((fetched)), (((void *)0)), (_U_UINT)75, UNITY_DISPLAY_STYLE_HEX16);

    }



    for(i=((sizeof(data)/sizeof(data[0]))/2);i<((sizeof(data)/sizeof(data[0])));i++)

    {

        UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((BufferC_Put(&tBuffer, data[i]))), (((void *)0)), (_U_UINT)80, UNITY_DISPLAY_STYLE_INT);

    }



    for(i=((sizeof(data)/sizeof(data[0]))/2);i<((sizeof(data)/sizeof(data[0])));i++)

    {

        UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((BufferC_Get(&tBuffer, &fetched))), (((void *)0)), (_U_UINT)85, UNITY_DISPLAY_STYLE_INT);

        UnityAssertEqualNumber((_U_SINT)(_US16)((data[i])), (_U_SINT)(_US16)((fetched)), (((void *)0)), (_U_UINT)86, UNITY_DISPLAY_STYLE_HEX16);

    }

}



void test_Buffer_Get_should_ReturnErrorIfCalledWhenEmpty(void)

{

 CircularBufferC_s tBuffer;

 BufferC_Clear(&tBuffer);



    char fetched;



    UnityAssertEqualNumber((_U_SINT)(((1))), (_U_SINT)((BufferC_Get(&tBuffer, &fetched))), (((void *)0)), (_U_UINT)97, UNITY_DISPLAY_STYLE_INT);

}





void test_Buffer_Put_should_ReturnErrorIfCalledWhenFull(void)

{

 CircularBufferC_s tBuffer;

 BufferC_Clear(&tBuffer);



    int i;

    char Expected[(255)];

    char Actual[(255)];



    for(i=0;i<(255);i++)

    {

        Expected[i] = 'a';



        UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((BufferC_Put(&tBuffer,'a'))), (((void *)0)), (_U_UINT)114, UNITY_DISPLAY_STYLE_INT);





    }



    UnityAssertEqualIntArray(( const void*)(tBuffer.buffer), ( const void*)(Expected), (_UU32)((255)), (((void *)0)), (_U_UINT)119, UNITY_DISPLAY_STYLE_HEX8);







}





void test_Buffer_Clear_should_ResetReadAndWriteToZero(void)

{

 CircularBufferC_s tBuffer;

 BufferC_Clear(&tBuffer);



    UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((BufferC_Clear(&tBuffer))), (((void *)0)), (_U_UINT)131, UNITY_DISPLAY_STYLE_INT);

}





void test_Buffer_IsFull_should_ReturnZeroWhenNotFull(void)

{

 CircularBufferC_s tBuffer;

    BufferC_Clear(&tBuffer);

    UnityAssertEqualNumber((_U_SINT)(((0))), (_U_SINT)((BufferC_IsFull(&tBuffer))), (((void *)0)), (_U_UINT)139, UNITY_DISPLAY_STYLE_INT);

}



void test_Buffer_IsFull_should_ReturnValueWhenIsFull(void)

{

 CircularBufferC_s tBuffer;

 BufferC_Clear(&tBuffer);

    uint16_t i = 0;



    for(i=0;i<((255)+20);i++)

    {

        BufferC_Put(&tBuffer, i);

    }

    UnityAssertEqualNumber((_U_SINT)(((2))), (_U_SINT)((BufferC_IsFull(&tBuffer))), (((void *)0)), (_U_UINT)152, UNITY_DISPLAY_STYLE_INT);

}





void test_Buffer_IsEmpty_should_ReturnValueWhenEmpty(void)

{

 CircularBufferC_s tBuffer;

 const char data[] = {0,1,0x5A,0x11,0x0A,0x7F,0xA0,0xFF,0x8C,0xDC};

 BufferC_Clear(&tBuffer);

 BufferC_Put(&tBuffer,data[0]);

 BufferC_Put(&tBuffer,data[1]);

    UnityAssertEqualNumber((_U_SINT)((((0)))), (_U_SINT)((BufferC_IsEmpty(&tBuffer))), (((void *)0)), (_U_UINT)163, UNITY_DISPLAY_STYLE_INT);

}







void test_Buffer_IsEmpty_should_returnZeroWhenNotEmpty(void)

{

 CircularBufferC_s tBuffer;

    BufferC_Clear(&tBuffer);

    UnityAssertEqualNumber((_U_SINT)(((1))), (_U_SINT)((BufferC_IsEmpty(&tBuffer))), (((void *)0)), (_U_UINT)172, UNITY_DISPLAY_STYLE_INT);

}
