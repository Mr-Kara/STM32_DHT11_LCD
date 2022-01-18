/**
  ******************************************************************************
  * @file    lcd.c
  * @brief   This file provides code for the configuration
  *          of 16x2 LCD with 4 bits of parallel communication
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 Emre KARA
  * All rights reserved.</center></h2>
  *
  *
  ******************************************************************************
	*/
	
#include "lcd.h"
#include "main.h"

// 16x2 LCD Functions 

void LCD_Data(unsigned char Data)
{
		// For 4 bits communication checking every bit of data. 
  	// If bit is 1 SET the pin, if it isn't RESET the pin.
    if(Data & 1)
    	HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, GPIO_PIN_SET); 
    else
    	HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, GPIO_PIN_RESET);
    if(Data & 2)
    	HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, GPIO_PIN_SET);
    else
    	HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, GPIO_PIN_RESET);
    if(Data & 4)
    	HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, GPIO_PIN_SET);
    else
    	HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, GPIO_PIN_RESET);
    if(Data & 8)
    	HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, GPIO_PIN_SET);
    else
    	HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, GPIO_PIN_RESET);
}
void LCD_Cmd(unsigned char a_Cmd)
{
    // Select command register
    HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET);
    // send the command data to lcd
    LCD_Data(a_Cmd);
    // send enable clock signal
    HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_SET);
  	HAL_Delay(2); 
    HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_RESET);
    HAL_Delay(2);
}
void LCD_Clear()
{
	//Clear LCD
    LCD_Cmd(0);
    LCD_Cmd(1);
    HAL_Delay(2);
}
void LCD_Set_Cursor(unsigned char r, unsigned char c)
{
    unsigned char Temp,Low4,High4; //Using High4 and Low4 for 4 bit mode of LCD
    if(r == 1)
    {
      Temp  = 0x80 + c - 1; //0x80 is used to move the cursor
      High4 = Temp >> 4;
      Low4  = Temp & 0x0F;
      LCD_Cmd(High4);
      LCD_Cmd(Low4);
    }
    if(r == 2)
    {
      Temp  = 0xC0 + c - 1; 
      High4 = Temp >> 4;
      Low4  = Temp & 0x0F;
      LCD_Cmd(High4);
      LCD_Cmd(Low4);
    }
    HAL_Delay(2);
}
void LCD_Init()
{
	// Initialization of LCD described in datasheet
	
  GPIO_InitTypeDef GPIO_InitStruct = {0};
	HAL_Delay(2);
    // IO Configurations
	__HAL_RCC_GPIOA_CLK_ENABLE(); // All pins that we used from GPIOA 

 	 HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, GPIO_PIN_RESET);
	 HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, GPIO_PIN_RESET);
	 HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, GPIO_PIN_RESET);
	 HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, GPIO_PIN_RESET);
	 HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET);
	 HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_RESET);
	 GPIO_InitStruct.Pin = LCD_D4_Pin | LCD_D5_Pin |
			LCD_D6_Pin |LCD_D7_Pin | LCD_RS_Pin |
			LCD_E_Pin;
	 GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	 GPIO_InitStruct.Pull = GPIO_NOPULL;
	 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	 HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_RESET);
    LCD_Data(0x00); //All Data pins reset
    HAL_Delay(2);
    LCD_Cmd(0x30);	
    HAL_Delay(2);
    LCD_Cmd(0x30);
    HAL_Delay(2);
    LCD_Cmd(0x30);
    LCD_Cmd(0x02);
    LCD_Cmd(0x02);
    LCD_Cmd(0x08);
    LCD_Cmd(0x00);	//Display ON/OFF first 4 bit command
    LCD_Cmd(0x0C);  //DIsplay ON/OFF second 4 bit command
    LCD_Cmd(0x00);
    LCD_Cmd(0x06);
    LCD_Cmd(0x00);
    LCD_Cmd(0x01);
}
void LCD_Write_Char(char Data)
{
   char Low4,High4;
   Low4  = Data & 0x0F;
   High4 = Data & 0xF0;
	
   HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET);
	 HAL_Delay(2);
   LCD_Data(High4>>4); 
   HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_SET);
   HAL_Delay(2);
   HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_RESET);
   HAL_Delay(2);

   LCD_Data(Low4);
   HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_SET);
   HAL_Delay(2);
   HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_RESET);
   HAL_Delay(2);
}
void LCD_Write_String(char *str)
{
      for(int i=0;str[i]!='\0';i++)	{	//Write all string array until it ends
       LCD_Write_Char(str[i]);
			  }		

}
void LCD_Shift_L()
{
    LCD_Cmd(0x01); // Cursor/Display shift first 4 bit command
    LCD_Cmd(0x08); // second 4 bit command
    HAL_Delay(35);
}
void LCD_Shift_R()
{
    LCD_Cmd(0x01); // Cursor/Display shift first 4 bit command
    LCD_Cmd(0x0C); // second 4 bit command
    HAL_Delay(35);
}

