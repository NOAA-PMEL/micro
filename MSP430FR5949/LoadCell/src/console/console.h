#ifndef console_H
#define console_H

void CONSOLE_State_Main(void);
void CONSOLE_State_ManualCalibration(void);
void CONSOLE_State_CalibrationMode(void);
void CONSOLE_State_CalibrationInput(void);

void CONSOLE_DisplayState_Main(void);
void CONSOLE_DisplayState_CalibrationMode(void);
void CONSOLE_DisplayState_CalibrationInput(void);
void CONSOLE_DisplayState_ManualCalibrationInput(uint8_t line, float value);
void CONSOLE_DisplayState_CalibrationPoints(void);

uint8_t CONSOLE_ReadChar(void);
void CONSOLE_ReadInputFloat(uint8_t *rxChars,uint8_t length);
void CONSOLE_ReadString(uint8_t *str, uint8_t length);

uint8_t UART_ClearChar(void);
uint8_t UART_WriteChar(uint8_t value, uint8_t Port);
uint8_t UART_Write(uint8_t *value, uint8_t length, uint8_t Port);

#endif // console_H
