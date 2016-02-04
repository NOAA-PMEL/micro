#include "../test/msp430fr5969.h"
#include "gpio.h"
#include "unity.h"




void setUp(void)

{

 GPIO_Init();

}



void tearDown(void)

{

}



void test_GPIO_Init_should_ClearPortRegisters(void)

{

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_Init())), (((void *)0)), (_U_UINT)17, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0)), (_U_SINT)(_US16)((P1OUT)), (((void *)0)), (_U_UINT)18, UNITY_DISPLAY_STYLE_HEX16);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0)), (_U_SINT)(_US16)((P2SEL0)), (((void *)0)), (_U_UINT)19, UNITY_DISPLAY_STYLE_HEX16);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0)), (_U_SINT)(_US16)((P3SEL1)), (((void *)0)), (_U_UINT)20, UNITY_DISPLAY_STYLE_HEX16);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0)), (_U_SINT)(_US16)((P4REN)), (((void *)0)), (_U_UINT)21, UNITY_DISPLAY_STYLE_HEX16);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0)), (_U_SINT)(_US16)((P3OUT)), (((void *)0)), (_U_UINT)22, UNITY_DISPLAY_STYLE_HEX16);

}



void test_GPIO_SetPinAsOutput_should_AcceptValidPins(void)

{

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetPinAsOutput(1,1))), (((void *)0)), (_U_UINT)27, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)(((1u << (1)))), (_U_SINT)(_US16)((P1DIR)), (((void *)0)), (_U_UINT)28, UNITY_DISPLAY_STYLE_HEX16);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetPinAsOutput(2,5))), (((void *)0)), (_U_UINT)29, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)(((1u << (5)))), (_U_SINT)(_US16)((P2DIR)), (((void *)0)), (_U_UINT)30, UNITY_DISPLAY_STYLE_HEX16);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetPinAsOutput(3,11))), (((void *)0)), (_U_UINT)31, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)(((1u << (11)))), (_U_SINT)(_US16)((P3DIR)), (((void *)0)), (_U_UINT)32, UNITY_DISPLAY_STYLE_HEX16);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetPinAsOutput(4,15))), (((void *)0)), (_U_UINT)33, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)(((1u << (15)))), (_U_SINT)(_US16)((P4DIR)), (((void *)0)), (_U_UINT)34, UNITY_DISPLAY_STYLE_HEX16);





 GPIO_Init();



 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetPinAsOutput(2,5))), (((void *)0)), (_U_UINT)39, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetPinAsOutput(2,6))), (((void *)0)), (_U_UINT)40, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetPinAsOutput(2,7))), (((void *)0)), (_U_UINT)41, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0x0E0)), (_U_SINT)(_US16)((P2DIR)), (((void *)0)), (_U_UINT)42, UNITY_DISPLAY_STYLE_HEX16);



}



void test_GPIO_SetPinAsOutput_should_RejectInvalidPins(void)

{

 UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((GPIO_SetPinAsOutput(1,16))), (((void *)0)), (_U_UINT)48, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((GPIO_SetPinAsOutput(2,32))), (((void *)0)), (_U_UINT)49, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((GPIO_SetPinAsOutput(3,127))), (((void *)0)), (_U_UINT)50, UNITY_DISPLAY_STYLE_INT);

}



void test_GPIO_SetPinAsOutput_should_RejectInvalidPorts(void)

{

 UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((GPIO_SetPinAsOutput(0,5))), (((void *)0)), (_U_UINT)55, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((GPIO_SetPinAsOutput(5,5))), (((void *)0)), (_U_UINT)56, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((GPIO_SetPinAsOutput(11,5))), (((void *)0)), (_U_UINT)57, UNITY_DISPLAY_STYLE_INT);

}



void test_GPIO_SetPin_should_AcceptValidInputs(void)

{

 GPIO_SetPinAsOutput(1,0);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetPin(1,0))), (((void *)0)), (_U_UINT)63, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)(((1u << (0)))), (_U_SINT)(_US16)((P1OUT)), (((void *)0)), (_U_UINT)64, UNITY_DISPLAY_STYLE_HEX16);





 GPIO_SetPinAsOutput(2,5);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetPin(2,5))), (((void *)0)), (_U_UINT)68, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)(((1u << (5)))), (_U_SINT)(_US16)((P2OUT)), (((void *)0)), (_U_UINT)69, UNITY_DISPLAY_STYLE_HEX16);



 GPIO_SetPinAsOutput(3,13);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetPin(3,13))), (((void *)0)), (_U_UINT)72, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)(((1u << (13)))), (_U_SINT)(_US16)((P3OUT)), (((void *)0)), (_U_UINT)73, UNITY_DISPLAY_STYLE_HEX16);



 GPIO_SetPinAsOutput(4,3);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetPin(4,3))), (((void *)0)), (_U_UINT)76, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)(((1u << (3)))), (_U_SINT)(_US16)((P4OUT)), (((void *)0)), (_U_UINT)77, UNITY_DISPLAY_STYLE_HEX16);

}





void test_GPIO_ClearPin_should_AcceptValidInputs(void)

{

 P1OUT = 0xFFFF;

 GPIO_SetPinAsOutput(1,0);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_ClearPin(1,0))), (((void *)0)), (_U_UINT)85, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((0xFFFE)), (_U_SINT)((P1OUT)), (((void *)0)), (_U_UINT)86, UNITY_DISPLAY_STYLE_INT);



 P2OUT = 0xFFFF;

 GPIO_SetPinAsOutput(2,2);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_ClearPin(2,2))), (((void *)0)), (_U_UINT)90, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((0xFFFB)), (_U_SINT)((P2OUT)), (((void *)0)), (_U_UINT)91, UNITY_DISPLAY_STYLE_INT);



 P3OUT = 0xFFFF;

 GPIO_SetPinAsOutput(3,10);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_ClearPin(3,10))), (((void *)0)), (_U_UINT)95, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((0xFBFF)), (_U_SINT)((P3OUT)), (((void *)0)), (_U_UINT)96, UNITY_DISPLAY_STYLE_INT);



 P4OUT = 0xFFFF;

 GPIO_SetPinAsOutput(4,15);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_ClearPin(4,15))), (((void *)0)), (_U_UINT)100, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((0x7FFF)), (_U_SINT)((P4OUT)), (((void *)0)), (_U_UINT)101, UNITY_DISPLAY_STYLE_INT);

}



void test_GPIO_SetAllPortOutputPinsLow_should_RejectInvalidPorts(void)

{

 UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((GPIO_SetAllPortOutputPinsLow(0))), (((void *)0)), (_U_UINT)106, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((GPIO_SetAllPortOutputPinsLow(5))), (((void *)0)), (_U_UINT)107, UNITY_DISPLAY_STYLE_INT);

}



void test_GPIO_SetAllPortOutputPinsLow_should_AcceptValidPort(void)

{

 P1DIR = 0xFFFF;

 P1OUT = 0xFFFF;

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetAllPortOutputPinsLow(1))), (((void *)0)), (_U_UINT)114, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0x0000)), (_U_SINT)(_US16)((P1OUT)), (((void *)0)), (_U_UINT)115, UNITY_DISPLAY_STYLE_HEX16);



 P2DIR = 0xFFFF;

 P2OUT = 0xFFFF;

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetAllPortOutputPinsLow(2))), (((void *)0)), (_U_UINT)119, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0x0000)), (_U_SINT)(_US16)((P2OUT)), (((void *)0)), (_U_UINT)120, UNITY_DISPLAY_STYLE_HEX16);



 P3DIR = 0xFFFF;

 P3OUT = 0xFFFF;

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetAllPortOutputPinsLow(3))), (((void *)0)), (_U_UINT)124, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0x0000)), (_U_SINT)(_US16)((P3OUT)), (((void *)0)), (_U_UINT)125, UNITY_DISPLAY_STYLE_HEX16);



 P4DIR = 0xFFFF;

 P4OUT = 0xFFFF;

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetAllPortOutputPinsLow(4))), (((void *)0)), (_U_UINT)129, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0x0000)), (_U_SINT)(_US16)((P4OUT)), (((void *)0)), (_U_UINT)130, UNITY_DISPLAY_STYLE_HEX16);



}



void test_GPIO_SetAllPortOutputPinsHigh_should_RejectInvalidPorts(void)

{

 UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((GPIO_SetAllPortOutputPinsHigh(0))), (((void *)0)), (_U_UINT)136, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((GPIO_SetAllPortOutputPinsHigh(5))), (((void *)0)), (_U_UINT)137, UNITY_DISPLAY_STYLE_INT);

}



void test_GPIO_SetAllPortOutputPinsHigh_should_AcceptValidPort(void)

{

 P1DIR = 0xFFFF;

 P1OUT = 0x0000;

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetAllPortOutputPinsHigh(1))), (((void *)0)), (_U_UINT)144, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0xFFFF)), (_U_SINT)(_US16)((P1OUT)), (((void *)0)), (_U_UINT)145, UNITY_DISPLAY_STYLE_HEX16);



 P2DIR = 0xFFFF;

 P2OUT = 0x0000;

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetAllPortOutputPinsHigh(2))), (((void *)0)), (_U_UINT)149, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0xFFFF)), (_U_SINT)(_US16)((P2OUT)), (((void *)0)), (_U_UINT)150, UNITY_DISPLAY_STYLE_HEX16);



 P3DIR = 0xFFFF;

 P3OUT = 0x0000;

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetAllPortOutputPinsHigh(3))), (((void *)0)), (_U_UINT)154, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0xFFFF)), (_U_SINT)(_US16)((P3OUT)), (((void *)0)), (_U_UINT)155, UNITY_DISPLAY_STYLE_HEX16);



 P4DIR = 0xFFFF;

 P4OUT = 0x0000;

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetAllPortOutputPinsHigh(4))), (((void *)0)), (_U_UINT)159, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0xFFFF)), (_U_SINT)(_US16)((P4OUT)), (((void *)0)), (_U_UINT)160, UNITY_DISPLAY_STYLE_HEX16);



}





void test_GPIO_SetPinAsInput_should_AcceptValidValues(void)

{

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetPinAsInput(1,0))), (((void *)0)), (_U_UINT)167, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetPinAsInput(2,5))), (((void *)0)), (_U_UINT)168, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetPinAsInput(3,12))), (((void *)0)), (_U_UINT)169, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetPinAsInput(4,15))), (((void *)0)), (_U_UINT)170, UNITY_DISPLAY_STYLE_INT);



 P1DIR = 0xFFFF;

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetPinAsInput(1,0))), (((void *)0)), (_U_UINT)173, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0xFFFE)), (_U_SINT)(_US16)((P1DIR)), (((void *)0)), (_U_UINT)174, UNITY_DISPLAY_STYLE_HEX16);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetPinAsInput(1,1))), (((void *)0)), (_U_UINT)175, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0xFFFC)), (_U_SINT)(_US16)((P1DIR)), (((void *)0)), (_U_UINT)176, UNITY_DISPLAY_STYLE_HEX16);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetPinAsInput(1,15))), (((void *)0)), (_U_UINT)177, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0x7FFC)), (_U_SINT)(_US16)((P1DIR)), (((void *)0)), (_U_UINT)178, UNITY_DISPLAY_STYLE_HEX16);



 P2DIR = 0xFFFF;

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetPinAsInput(2,2))), (((void *)0)), (_U_UINT)181, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0xFFFB)), (_U_SINT)(_US16)((P2DIR)), (((void *)0)), (_U_UINT)182, UNITY_DISPLAY_STYLE_HEX16);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetPinAsInput(2,6))), (((void *)0)), (_U_UINT)183, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0xFFBB)), (_U_SINT)(_US16)((P2DIR)), (((void *)0)), (_U_UINT)184, UNITY_DISPLAY_STYLE_HEX16);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetPinAsInput(2,10))), (((void *)0)), (_U_UINT)185, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0xFBBB)), (_U_SINT)(_US16)((P2DIR)), (((void *)0)), (_U_UINT)186, UNITY_DISPLAY_STYLE_HEX16);



 P3DIR = 0xFFFF;

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetPinAsInput(3,3))), (((void *)0)), (_U_UINT)189, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0xFFF7)), (_U_SINT)(_US16)((P3DIR)), (((void *)0)), (_U_UINT)190, UNITY_DISPLAY_STYLE_HEX16);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetPinAsInput(3,7))), (((void *)0)), (_U_UINT)191, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0xFF77)), (_U_SINT)(_US16)((P3DIR)), (((void *)0)), (_U_UINT)192, UNITY_DISPLAY_STYLE_HEX16);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetPinAsInput(3,11))), (((void *)0)), (_U_UINT)193, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0xF777)), (_U_SINT)(_US16)((P3DIR)), (((void *)0)), (_U_UINT)194, UNITY_DISPLAY_STYLE_HEX16);



 P4DIR = 0xFFFF;

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetPinAsInput(4,1))), (((void *)0)), (_U_UINT)197, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0xFFFD)), (_U_SINT)(_US16)((P4DIR)), (((void *)0)), (_U_UINT)198, UNITY_DISPLAY_STYLE_HEX16);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetPinAsInput(4,5))), (((void *)0)), (_U_UINT)199, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0xFFDD)), (_U_SINT)(_US16)((P4DIR)), (((void *)0)), (_U_UINT)200, UNITY_DISPLAY_STYLE_HEX16);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetPinAsInput(4,9))), (((void *)0)), (_U_UINT)201, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0xFDDD)), (_U_SINT)(_US16)((P4DIR)), (((void *)0)), (_U_UINT)202, UNITY_DISPLAY_STYLE_HEX16);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_SetPinAsInput(4,13))), (((void *)0)), (_U_UINT)203, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0xDDDD)), (_U_SINT)(_US16)((P4DIR)), (((void *)0)), (_U_UINT)204, UNITY_DISPLAY_STYLE_HEX16);

}





void test_GPIO_SetPinAsInput_should_RejectInvalidValues(void)

{

 UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((GPIO_SetPinAsInput(0,0))), (((void *)0)), (_U_UINT)210, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((GPIO_SetPinAsInput(5,1))), (((void *)0)), (_U_UINT)211, UNITY_DISPLAY_STYLE_INT);

}



void test_GPIO_ReadInputPin_should_RejectInvalidValues(void)

{

 UnityAssertEqualNumber((_U_SINT)((-1)), (_U_SINT)((GPIO_ReadInputPin(0,0))), (((void *)0)), (_U_UINT)216, UNITY_DISPLAY_STYLE_INT);

}



void test_GPIO_ReadInputPin_should_AcceptValidValues(void)

{



 P1IN = 0x0001;

 UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((GPIO_ReadInputPin(1,0))), (((void *)0)), (_U_UINT)223, UNITY_DISPLAY_STYLE_INT);

 P2IN = 0x0010;

 UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((GPIO_ReadInputPin(2,4))), (((void *)0)), (_U_UINT)225, UNITY_DISPLAY_STYLE_INT);

 P3IN = 0x1400;

 UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((GPIO_ReadInputPin(3,12))), (((void *)0)), (_U_UINT)227, UNITY_DISPLAY_STYLE_INT);

 P4IN = 0x0000;

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_ReadInputPin(4,15))), (((void *)0)), (_U_UINT)229, UNITY_DISPLAY_STYLE_INT);

}



void test_GPIO_ReadInputPort_should_RejectInvalidPorts(void)

{

 UnityAssertEqualNumber((_U_SINT)((-1)), (_U_SINT)((GPIO_ReadInputPort(0))), (((void *)0)), (_U_UINT)234, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((-1)), (_U_SINT)((GPIO_ReadInputPort(5))), (((void *)0)), (_U_UINT)235, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((-1)), (_U_SINT)((GPIO_ReadInputPort(255))), (((void *)0)), (_U_UINT)236, UNITY_DISPLAY_STYLE_INT);

}



void test_GPIO_ReadInputPort_should_AcceptValidPorts(void)

{



 P1IN = 0x0010;

 UnityAssertEqualNumber((_U_SINT)((16)), (_U_SINT)((GPIO_ReadInputPort(1))), (((void *)0)), (_U_UINT)243, UNITY_DISPLAY_STYLE_INT);

 P2IN = 0x00F0;

 UnityAssertEqualNumber((_U_SINT)((240)), (_U_SINT)((GPIO_ReadInputPort(2))), (((void *)0)), (_U_UINT)245, UNITY_DISPLAY_STYLE_INT);

 P3IN = 0xFFFF;

 UnityAssertEqualNumber((_U_SINT)((65535)), (_U_SINT)((GPIO_ReadInputPort(3))), (((void *)0)), (_U_UINT)247, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_ReadInputPort(4))), (((void *)0)), (_U_UINT)248, UNITY_DISPLAY_STYLE_INT);

}





void test_GPIO_TogglePin_should_AcceptValidValues(void)

{

 P1DIR = 0xFFFF;

 P1OUT = 0x0050;

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_TogglePin(1,6))), (((void *)0)), (_U_UINT)256, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0x10)), (_U_SINT)(_US16)((P1OUT)), (((void *)0)), (_U_UINT)257, UNITY_DISPLAY_STYLE_HEX16);



 P2DIR = 0xFFFF;

 P2OUT = 0xC006;

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_TogglePin(2,0))), (((void *)0)), (_U_UINT)261, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0xC007)), (_U_SINT)(_US16)((P2OUT)), (((void *)0)), (_U_UINT)262, UNITY_DISPLAY_STYLE_HEX16);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_TogglePin(2,0))), (((void *)0)), (_U_UINT)263, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0xC006)), (_U_SINT)(_US16)((P2OUT)), (((void *)0)), (_U_UINT)264, UNITY_DISPLAY_STYLE_HEX16);



 P3DIR = 0x0100;

 P3OUT = 0x0100;

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_TogglePin(3,8))), (((void *)0)), (_U_UINT)268, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0x0000)), (_U_SINT)(_US16)((P3OUT)), (((void *)0)), (_U_UINT)269, UNITY_DISPLAY_STYLE_HEX16);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_TogglePin(3,7))), (((void *)0)), (_U_UINT)270, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0x0000)), (_U_SINT)(_US16)((P3OUT)), (((void *)0)), (_U_UINT)271, UNITY_DISPLAY_STYLE_HEX16);



 P4DIR = 0xF000;

 P4OUT = 0xF000;

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_TogglePin(4,12))), (((void *)0)), (_U_UINT)275, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0xE000)), (_U_SINT)(_US16)((P4OUT)), (((void *)0)), (_U_UINT)276, UNITY_DISPLAY_STYLE_HEX16);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_TogglePin(4,13))), (((void *)0)), (_U_UINT)277, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0xC000)), (_U_SINT)(_US16)((P4OUT)), (((void *)0)), (_U_UINT)278, UNITY_DISPLAY_STYLE_HEX16);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_TogglePin(4,14))), (((void *)0)), (_U_UINT)279, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0x8000)), (_U_SINT)(_US16)((P4OUT)), (((void *)0)), (_U_UINT)280, UNITY_DISPLAY_STYLE_HEX16);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_TogglePin(4,15))), (((void *)0)), (_U_UINT)281, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0x0000)), (_U_SINT)(_US16)((P4OUT)), (((void *)0)), (_U_UINT)282, UNITY_DISPLAY_STYLE_HEX16);



 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((GPIO_TogglePin(3,10))), (((void *)0)), (_U_UINT)284, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0x0000)), (_U_SINT)(_US16)((P4OUT)), (((void *)0)), (_U_UINT)285, UNITY_DISPLAY_STYLE_HEX16);

}



void test_GPIO_TogglePin_should_RejectInvalidValues(void)

{

 UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((GPIO_SetPinAsOutput(1,33))), (((void *)0)), (_U_UINT)290, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((GPIO_SetPinAsOutput(2,64))), (((void *)0)), (_U_UINT)291, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((GPIO_SetPinAsOutput(3,24))), (((void *)0)), (_U_UINT)292, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((GPIO_SetPinAsOutput(4,17))), (((void *)0)), (_U_UINT)293, UNITY_DISPLAY_STYLE_INT);

}







void test_GPIO_AttachInputInterrupt_should_AcceptValidPorts_and_Pins(void)

{



 UnityAssertEqualNumber((_U_SINT)(((0))), (_U_SINT)((GPIO_AttachInputInterrupt(1,5,1))), (((void *)0)), (_U_UINT)301, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)(((1u << (5)))), (_U_SINT)(_US16)((P1IES)), (((void *)0)), (_U_UINT)302, UNITY_DISPLAY_STYLE_HEX16);

 UnityAssertEqualNumber((_U_SINT)(_US16)(((1u << (5)))), (_U_SINT)(_US16)((P1IE)), (((void *)0)), (_U_UINT)303, UNITY_DISPLAY_STYLE_HEX16);



 UnityAssertEqualNumber((_U_SINT)(((0))), (_U_SINT)((GPIO_AttachInputInterrupt(2,10,1))), (((void *)0)), (_U_UINT)305, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)(((1u << (10)))), (_U_SINT)(_US16)((P2IES)), (((void *)0)), (_U_UINT)306, UNITY_DISPLAY_STYLE_HEX16);

 UnityAssertEqualNumber((_U_SINT)(_US16)(((1u << (10)))), (_U_SINT)(_US16)((P2IE)), (((void *)0)), (_U_UINT)307, UNITY_DISPLAY_STYLE_HEX16);



 UnityAssertEqualNumber((_U_SINT)(((0))), (_U_SINT)((GPIO_AttachInputInterrupt(3,0,1))), (((void *)0)), (_U_UINT)309, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)(((1u << (0)))), (_U_SINT)(_US16)((P3IES)), (((void *)0)), (_U_UINT)310, UNITY_DISPLAY_STYLE_HEX16);

 UnityAssertEqualNumber((_U_SINT)(_US16)(((1u << (0)))), (_U_SINT)(_US16)((P3IE)), (((void *)0)), (_U_UINT)311, UNITY_DISPLAY_STYLE_HEX16);



 UnityAssertEqualNumber((_U_SINT)(((0))), (_U_SINT)((GPIO_AttachInputInterrupt(4,15,1))), (((void *)0)), (_U_UINT)313, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)(((1u << (15)))), (_U_SINT)(_US16)((P4IES)), (((void *)0)), (_U_UINT)314, UNITY_DISPLAY_STYLE_HEX16);

 UnityAssertEqualNumber((_U_SINT)(_US16)(((1u << (15)))), (_U_SINT)(_US16)((P4IE)), (((void *)0)), (_U_UINT)315, UNITY_DISPLAY_STYLE_HEX16);



}



void test_GPIO_AttachInputInterrupt_should_OnlyAttachInputPins(void)

{

 P1DIR = 0xFFFF;

 P1IE = 0x0000;

 UnityAssertEqualNumber((_U_SINT)(((0))), (_U_SINT)((GPIO_AttachInputInterrupt(1,5,1))), (((void *)0)), (_U_UINT)323, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0x0000)), (_U_SINT)(_US16)((P1IE)), (((void *)0)), (_U_UINT)324, UNITY_DISPLAY_STYLE_HEX16);



 P2DIR = 0xFF00;

 P2IE = 0x0000;

 UnityAssertEqualNumber((_U_SINT)(((0))), (_U_SINT)((GPIO_AttachInputInterrupt(2,5,1))), (((void *)0)), (_U_UINT)328, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)(((1u << (5)))), (_U_SINT)(_US16)((P2IE)), (((void *)0)), (_U_UINT)329, UNITY_DISPLAY_STYLE_HEX16);



}





void test_GPIO_AttachInputInterrupt_should_RejectInvalidPorts_and_Pins(void)

{

 UnityAssertEqualNumber((_U_SINT)(((1))), (_U_SINT)((GPIO_AttachInputInterrupt(0,5,1))), (((void *)0)), (_U_UINT)336, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(((1))), (_U_SINT)((GPIO_AttachInputInterrupt(5,5,1))), (((void *)0)), (_U_UINT)337, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(((1))), (_U_SINT)((GPIO_AttachInputInterrupt(0XFF,5,1))), (((void *)0)), (_U_UINT)338, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(((1))), (_U_SINT)((GPIO_AttachInputInterrupt(1,16,1))), (((void *)0)), (_U_UINT)339, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(((1))), (_U_SINT)((GPIO_AttachInputInterrupt(1,0xFFFF,1))), (((void *)0)), (_U_UINT)340, UNITY_DISPLAY_STYLE_INT);

}







void test_GPIO_DetachInputInterrupt_should_RejectInvalidPorts_and_Pins(void)

{

 UnityAssertEqualNumber((_U_SINT)(((1))), (_U_SINT)((GPIO_DetachInputInterrupt(0,5))), (((void *)0)), (_U_UINT)347, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(((1))), (_U_SINT)((GPIO_DetachInputInterrupt(5,5))), (((void *)0)), (_U_UINT)348, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(((1))), (_U_SINT)((GPIO_DetachInputInterrupt(0XFF,5))), (((void *)0)), (_U_UINT)349, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(((1))), (_U_SINT)((GPIO_DetachInputInterrupt(1,16))), (((void *)0)), (_U_UINT)350, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(((1))), (_U_SINT)((GPIO_DetachInputInterrupt(1,0xFFFF))), (((void *)0)), (_U_UINT)351, UNITY_DISPLAY_STYLE_INT);

}





void test_GPIO_DetachInputInterrupt_should_AcceptValidPorts_and_Pins(void)

{

 P1IE = (1u << (5));

 UnityAssertEqualNumber((_U_SINT)(((0))), (_U_SINT)((GPIO_DetachInputInterrupt(1,5))), (((void *)0)), (_U_UINT)358, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0)), (_U_SINT)(_US16)(((P1IE & (1u << (5))))), (((void *)0)), (_U_UINT)359, UNITY_DISPLAY_STYLE_HEX16);



 UnityAssertEqualNumber((_U_SINT)(((0))), (_U_SINT)((GPIO_DetachInputInterrupt(2,10))), (((void *)0)), (_U_UINT)361, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0)), (_U_SINT)(_US16)(((P2IE & (1u << (10))))), (((void *)0)), (_U_UINT)362, UNITY_DISPLAY_STYLE_HEX16);



 UnityAssertEqualNumber((_U_SINT)(((0))), (_U_SINT)((GPIO_DetachInputInterrupt(3,0))), (((void *)0)), (_U_UINT)364, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0)), (_U_SINT)(_US16)(((P3IE & (1u << (0))))), (((void *)0)), (_U_UINT)365, UNITY_DISPLAY_STYLE_HEX16);



 UnityAssertEqualNumber((_U_SINT)(((0))), (_U_SINT)((GPIO_DetachInputInterrupt(4,15))), (((void *)0)), (_U_UINT)367, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)(_US16)((0)), (_U_SINT)(_US16)(((P4IE & (1u << (15))))), (((void *)0)), (_U_UINT)368, UNITY_DISPLAY_STYLE_HEX16);



}
