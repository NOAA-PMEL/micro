#include "../test/msp430fr5969.h"
#include "buffer_d.h"
#include "unity.h"




void setUp(void)

{



}



void tearDown(void)

{

}







void test_Buffer_Put_and_Get_should_WorkTogetherToInsertAndExtractAValue(void)

{

 CircularBufferD_s tBuffer;

 BufferD_Clear(&tBuffer);



 double data[] = {0.0, 1.1, 256.1, -191.2};

 double fetched;

 int i;



 for(i=0;i<(sizeof(data)/sizeof(data[0]));i++)

 {

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((BufferD_Put(&tBuffer, data[i]))), (((void *)0)), (_U_UINT)28, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((BufferD_Get(&tBuffer, &fetched))), (((void *)0)), (_U_UINT)29, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)(_US16)((data[i])), (_U_SINT)(_US16)((fetched)), (((void *)0)), (_U_UINT)30, UNITY_DISPLAY_STYLE_HEX16);

 }



}





void test_Buffer_Put_and_Get_should_WorkTogetherToInsertAndExtractManyValues(void)

{

 CircularBufferD_s tBuffer;

 BufferD_Clear(&tBuffer);



    const double data[] = {0,1,4818189,-111,3.223235,-999999,0.001,-0.0001};

    double fetched;

    int i;



    for(i=0;i<(sizeof(data)/sizeof(data[0]));i++)

    {

        UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((BufferD_Put(&tBuffer,data[i]))), (((void *)0)), (_U_UINT)47, UNITY_DISPLAY_STYLE_INT);

    }



    for(i=0;i<(sizeof(data)/sizeof(data[0]));i++)

    {

        UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((BufferD_Get(&tBuffer, &fetched))), (((void *)0)), (_U_UINT)52, UNITY_DISPLAY_STYLE_INT);

        UnityAssertEqualNumber((_U_SINT)(_US16)((data[i])), (_U_SINT)(_US16)((fetched)), (((void *)0)), (_U_UINT)53, UNITY_DISPLAY_STYLE_HEX16);

    }

}





void test_Buffer_Put_and_Get_should_WorkTogetherToInsertAndExtractManyValuesRepeatedly(void)

{

 CircularBufferD_s tBuffer;

 BufferD_Clear(&tBuffer);



 const double data[] = {0,1,14818189,-1112411,13.223235,-99999999.9,0.00001,-0.0235001};

    double fetched;

    int i;



    for(i=0;i<((sizeof(data)/sizeof(data[0]))/2);i++)

    {

        UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((BufferD_Put(&tBuffer, data[i]))), (((void *)0)), (_U_UINT)69, UNITY_DISPLAY_STYLE_INT);

    }



    for(i=0;i<((sizeof(data)/sizeof(data[0]))/2);i++)

    {

        UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((BufferD_Get(&tBuffer, &fetched))), (((void *)0)), (_U_UINT)74, UNITY_DISPLAY_STYLE_INT);

        UnityAssertEqualNumber((_U_SINT)(_US16)((data[i])), (_U_SINT)(_US16)((fetched)), (((void *)0)), (_U_UINT)75, UNITY_DISPLAY_STYLE_HEX16);

    }



    for(i=((sizeof(data)/sizeof(data[0]))/2);i<((sizeof(data)/sizeof(data[0])));i++)

    {

        UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((BufferD_Put(&tBuffer, data[i]))), (((void *)0)), (_U_UINT)80, UNITY_DISPLAY_STYLE_INT);

    }



    for(i=((sizeof(data)/sizeof(data[0]))/2);i<((sizeof(data)/sizeof(data[0])));i++)

    {

        UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((BufferD_Get(&tBuffer, &fetched))), (((void *)0)), (_U_UINT)85, UNITY_DISPLAY_STYLE_INT);

        UnityAssertEqualNumber((_U_SINT)(_US16)((data[i])), (_U_SINT)(_US16)((fetched)), (((void *)0)), (_U_UINT)86, UNITY_DISPLAY_STYLE_HEX16);

    }

}



void test_Buffer_Get_should_ReturnErrorIfCalledWhenEmpty(void)

{

 CircularBufferD_s tBuffer;

 BufferD_Clear(&tBuffer);



    double fetched;



    UnityAssertEqualNumber((_U_SINT)(((1))), (_U_SINT)((BufferD_Get(&tBuffer, &fetched))), (((void *)0)), (_U_UINT)97, UNITY_DISPLAY_STYLE_INT);

}





void test_Buffer_Put_should_ReturnErrorIfCalledWhenFull(void)

{

 CircularBufferD_s tBuffer;

 BufferD_Clear(&tBuffer);



    int i;

    int Expected[(255)];

    int Actual[(255)];



    for(i=0;i<(255);i++)

    {

        Expected[i] = 0;

        Actual[i] = BufferD_Put(&tBuffer, i);

    }



    UnityAssertEqualIntArray(( const void*)(Expected), ( const void*)(Actual), (_UU32)((255)), (((void *)0)), (_U_UINT)116, UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)(((2))), (_U_SINT)((BufferD_Put(&tBuffer, 0x10))), (((void *)0)), (_U_UINT)117, UNITY_DISPLAY_STYLE_INT);

}





void test_Buffer_Clear_should_ResetReadAndWriteToZero(void)

{

 CircularBufferD_s tBuffer;

 BufferD_Clear(&tBuffer);



    UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((BufferD_Clear(&tBuffer))), (((void *)0)), (_U_UINT)126, UNITY_DISPLAY_STYLE_INT);

}





void test_Buffer_IsFull_should_ReturnZeroWhenNotFull(void)

{

 CircularBufferD_s tBuffer;

    BufferD_Clear(&tBuffer);

    UnityAssertEqualNumber((_U_SINT)(((0))), (_U_SINT)((BufferD_IsFull(&tBuffer))), (((void *)0)), (_U_UINT)134, UNITY_DISPLAY_STYLE_INT);

}



void test_Buffer_IsFull_should_ReturnValueWhenIsFull(void)

{

 CircularBufferD_s tBuffer;

 BufferD_Clear(&tBuffer);

    uint16_t i = 0;



    for(i=0;i<((255)+20);i++)

    {

        BufferD_Put(&tBuffer, i);

    }

    UnityAssertEqualNumber((_U_SINT)(((2))), (_U_SINT)((BufferD_IsFull(&tBuffer))), (((void *)0)), (_U_UINT)147, UNITY_DISPLAY_STYLE_INT);

}





void test_Buffer_IsEmpty_should_ReturnValueWhenEmpty(void)

{

 CircularBufferD_s tBuffer;

 const double data[] = {0,1,14818189,-1112411,13.223235,-99999999.9,0.00001,-0.0235001};

 BufferD_Clear(&tBuffer);

 BufferD_Put(&tBuffer,data[0]);

 BufferD_Put(&tBuffer,data[1]);

    UnityAssertEqualNumber((_U_SINT)((((0)))), (_U_SINT)((BufferD_IsEmpty(&tBuffer))), (((void *)0)), (_U_UINT)158, UNITY_DISPLAY_STYLE_INT);

}







void test_Buffer_IsEmpty_should_returnZeroWhenNotEmpty(void)

{

 CircularBufferD_s tBuffer;

    BufferD_Clear(&tBuffer);

    UnityAssertEqualNumber((_U_SINT)(((1))), (_U_SINT)((BufferD_IsEmpty(&tBuffer))), (((void *)0)), (_U_UINT)167, UNITY_DISPLAY_STYLE_INT);

}
