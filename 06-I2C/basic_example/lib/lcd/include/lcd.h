/*
  Longan Nano LCD driver
*/

#ifndef __LCD_H
#define __LCD_H		

#include "stdlib.h"	
#include "gd32vf103_gpio.h"

#define LCD_NORMAL    0
#define LCD_INVERTED  1

#define LCD_W 160
#define LCD_H 80

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;    			

#define LED_ON 
#define LED_OFF 


//-----------------OLED Port definition ---------------- 

#define OLED_CS_Clr() gpio_bit_reset(GPIOC,GPIO_PIN_13)     //CS PB2
#define OLED_CS_Set() gpio_bit_set(GPIOC,GPIO_PIN_13)

#define OLED_DC_Clr() gpio_bit_reset(GPIOC,GPIO_PIN_15)      //DC PB0
#define OLED_DC_Set() gpio_bit_set(GPIOC,GPIO_PIN_15)

#define OLED_CMD  0	// Write command
#define OLED_DATA 1	// Write data

#define TRANSPARENT 1
#define OPAQUE      0

extern  u16 BACK_COLOR;   // Background color

void LCD_WR_Queue();
void LCD_Wait_On_Queue();
void LCD_Write_Bus(int dat);
void LCD_Writ_Bus(u8 dat);
void LCD_WR_DATA8(u8 dat);
void LCD_WR_DATA(u16 dat);
void LCD_WR_REG(u8 dat);
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);
void Lcd_SetType(int type);
void Lcd_Init(void);
void LCD_Clear(u16 Color);
void LCD_ShowChinese(u16 x,u16 y,u8 index,u8 size,u16 color);
void LCD_DrawPoint(u16 x,u16 y,u16 color);
void LCD_DrawPoint_big(u16 x,u16 y,u16 color);
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);
void LCD_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);
void Draw_Circle(u16 x0,u16 y0,u8 r,u16 color);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode,u16 color);
void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 color);
void LCD_ShowStr(u16 x,u16 y,const u8 *p,u16 color, u8 mode);
void LCD_ShowNum(u16 x,u16 y,u16 num,u8 len,u16 color);
void LCD_ShowNum1(u16 x,u16 y,float num,u8 len,u16 color);
void LCD_ShowPicture(u16 x1, u16 y1, u16 x2, u16 y2, u8 *image);
void LCD_ShowLogo(u16 y);
u32 mypow(u8 m,u8 n);

// Color predefines
#define WHITE            0xFFFF
#define BLACK            0x0000	  
#define BLUE             0x001F  
#define BRED             0XF81F
#define GRED             0XFFE0
#define GBLUE            0X07FF
#define RED              0xF800
#define MAGENTA          0xF81F
#define GREEN            0x07E0
#define CYAN             0x7FFF
#define YELLOW           0xFFE0
#define BROWN            0XBC40
#define BRRED            0XFC07 
#define GRAY             0X8430
#define DARKBLUE         0X01CF	
#define LIGHTBLUE        0X7D7C
#define GRAYBLUE         0X5458
#define LIGHTGREEN       0X841F
#define LGRAY            0XC618 
#define DGRAY            0X31A6 
#define LGRAYBLUE        0XA651 
#define LBBLUE           0X2B12 


					  		 
#endif  
	 
	 



