#include "stm32f4xx_hal.h"

void Set_Pin_Output (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void Set_Pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void DHT11_Init(void);
uint8_t DHT_Response(void);
uint8_t DHT_Read(void);
void Display_Temp(float temp);
void Display_Hum(float hum);

extern uint8_t Presence;
extern float Temperature;
extern float Humidity;
extern uint8_t Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2;
extern uint16_t RH, TEMP;
extern uint32_t SUM;
