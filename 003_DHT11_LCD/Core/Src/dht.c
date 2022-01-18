#include "dht.h"
#include "stm32f4xx_hal.h"
#include "main.h"
#include "tim.h"
#include "lcd.h"
#include "stdio.h"

uint8_t Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2;
uint16_t RH, TEMP;
uint32_t SUM;

float Temperature=0;
float Humidity=0;
uint8_t Presence=0;

//DHT sensor needs delay at us level
void delay_us(uint16_t time){
		
	__HAL_TIM_SET_COUNTER(&htim10, 0);
		
	while((__HAL_TIM_GET_COUNTER(&htim10) < time));	
}

//Setting pin as output or input for read DHT 
void Set_Pin_Output (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin=GPIO_Pin;
	GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOx,&GPIO_InitStruct);
		
}
void Set_Pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
		
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin=GPIO_Pin;
	GPIO_InitStruct.Mode=GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOx,&GPIO_InitStruct);
	
}

 //These functions written according to the DHT11 datasheet
void DHT11_Init(void){
		
	Set_Pin_Output(DHT_DATA_GPIO_Port,DHT_DATA_Pin);                    // Set DHT pin as output
	HAL_GPIO_WritePin(DHT_DATA_GPIO_Port,DHT_DATA_Pin,GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(DHT_DATA_GPIO_Port,DHT_DATA_Pin,GPIO_PIN_RESET);	// Pin goes to low
	HAL_Delay(18);																										// Delay 20ms for DHT sense the MCU	
	HAL_GPIO_WritePin(DHT_DATA_GPIO_Port,DHT_DATA_Pin,GPIO_PIN_SET);    // Pin goes to high because datasheet says so
	delay_us(20);																											// we need delay 20-40us, give that 50us to be sure. 		
	Set_Pin_Input(DHT_DATA_GPIO_Port,DHT_DATA_Pin);											// Set pin as input and wait for DHT response		

}


uint8_t DHT_Response(void){
	
	uint8_t response = 0; 

	delay_us(50);  //wait for 40us so sensor data pin should be low
	
	if(!(HAL_GPIO_ReadPin(DHT_DATA_GPIO_Port,DHT_DATA_Pin))) // if sensor data pin is low
	{ 
		delay_us(80);	//wait for 80us and expected the sensor goes high in this given time
		if(HAL_GPIO_ReadPin(DHT_DATA_GPIO_Port,DHT_DATA_Pin)){ //if sensor data pin is high 
			response = 1; // sensor response is 1
		}
		else
		{
			response = 0; // is sensor data pin is low response is 0
		}		   
	}
	
	
	while(HAL_GPIO_ReadPin(DHT_DATA_GPIO_Port,DHT_DATA_Pin)); // wait for go low
	
	return response;
	
}

//When DHT is sending data to MCU, EVERY BIT OF DATA begins with the 50us low-voltage-level 
//and the length of the following high-voltage-level signal determines 
//whether data bit is "0" or "1"

uint8_t DHT_Read(void){
	
	uint8_t i,r;

	for(i=0;i<8;i++){ //DTH sensor sending 8 bits data
		
	while(!(HAL_GPIO_ReadPin(DHT_DATA_GPIO_Port,DHT_DATA_Pin))); //wait for the pin become high
	
	delay_us(50); // wait for 40us to see if pin is high or low
	
	if(!(HAL_GPIO_ReadPin(DHT_DATA_GPIO_Port,DHT_DATA_Pin)))
	{ //
		r&= ~(1<<(7-i)); //write 0
	}
	else
	{
		r|=(1<<(7-i));//write 1
	}
	
	while((HAL_GPIO_ReadPin(DHT_DATA_GPIO_Port,DHT_DATA_Pin))); // wait for the pin goes low
	
 }
	
	return r;
 
}


void Display_Temp(float temp){
	
	char tmp[20]={0};
	HAL_Delay(10);
	LCD_Set_Cursor(1,1);
	sprintf(tmp,"TEMP:%.2f",temp);
	LCD_Write_String(tmp);
}

void Display_Hum(float hum){
	
	char hm[20]={0};
	HAL_Delay(10);
	LCD_Set_Cursor(2,1);
	sprintf(hm,"HUM: %.2f",hum);
	LCD_Write_String(hm);
	LCD_Set_Cursor(2,5);
	LCD_Write_String("%");
}



