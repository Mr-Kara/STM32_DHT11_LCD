
#ifndef lcd_h_
#define lcd_h_

// Prototypes for LCD functions

void LCD_Init(void);              
void LCD_Clear(void);             
void LCD_Cmd(unsigned char);      
void LCD_Data(unsigned char);     
void LCD_Set_Cursor(unsigned char, unsigned char); 
void LCD_Write_Char(char);        
void LCD_Write_String(char*);     
void LCD_Shift_L(void);                   
void LCD_Shift_R(void);                   

#endif 
