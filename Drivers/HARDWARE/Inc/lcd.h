/**
  *******************************************************************************************************
  * File Name: main.c
  * Author: JC.Yang
  * Version: V1.0.0
  * Date: 2018-7-31
  * Brief: 本文件用于实现TFTLCD屏的触摸控制操作
  *******************************************************************************************************
  * History:
  *			Date: 2018-3-3
  *			Mod: 建立文件
  *         description:
              TFT_LCD 接口演示例程(stm32系列)
              说明: 
              -此部分为不带触摸接口，BLK为背光开关，默认可以不接---------------------------------------------------------------
              GND    电源地
              VCC    3.3v电源
              CLK    接PA5
              MOSI   接PA7
              RES    接PA2
              DC     接PA4
              CS1    接PA0               
              -以下为触摸控制接口；如果不用出没功能可以不接---------------------------------------------------------------
              MISO   接PA6
              CS2    接PA1
              PEN    接PA3
  
  *******************************************************************************************************
  */	
#ifndef __LCD_H
#define __LCD_H			  	 
#include "stdlib.h"	
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_gpio.h"


#define LCD_W 240
#define LCD_H 320
#define	u8 unsigned char
#define	u16 unsigned int
#define	u32 unsigned long

//LCD模式设置
//0:4线串行模式
//1:并行8080模式
#define LCD_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------LCD端口定义----------------  					   
#define LCD_CS_Clr()    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET) //CS
#define LCD_CS_Set()    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET)

#define LCD_RST_Clr() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET)   //RES
#define LCD_RST_Set() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET)

#define LCD_DC_Clr() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)    //DC
#define LCD_DC_Set() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)


#define LCD_SCLK_Clr() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET)  //CLK
#define LCD_SCLK_Set() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET)

#define LCD_SDIN_Clr() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET)  //DIN MOSI
#define LCD_SDIN_Set() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET)

 		     
#define LCD_CMD  0	//写命令
#define LCD_DATA 1	//写数据

extern  u16 BACK_COLOR, POINT_COLOR;   //背景色，画笔色


void Main_Menu(void);//显示主菜单信息
void Menu_One(void);   //显示菜单一信息

void LCD_ShowImage(int m,int n,int x,int y,const unsigned char *p);        
void DrawPicture(void);
unsigned char Detect(void); 


void LCD_Init(void); 
void LCD_Clear(u16 Color);
void Address_Set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void LCD_WR_DATA8(char da); //发送数据-8位参数
void LCD_WR_DATA(int da);
void LCD_WR_REG(char da);

void LCD_DrawPoint(u16 x,u16 y);//画点
void LCD_DrawPoint_Big(u16 x,u16 y);//画一个大点
u16  LCD_ReadPoint(u16 x,u16 y); //读点
void Draw_Circle(u16 x0,u16 y0,u8 r);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode);//显示一个字符
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len);//显示数字
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len);//显示2个数字
void LCD_ShowString(u16 x,u16 y,const u8 *p);//显示一个字符串,16字体
 
void LCD_ShowSinogram_32(unsigned int m,unsigned int n,unsigned int x,unsigned int y,unsigned char index);
void LCD_ShowSinogram_16(unsigned int x,unsigned int y,unsigned char index);

void Fast_DrawFont_GBK16(u16 x, u16 y, u16 fc, u16 bc, u8 *s);


//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

/*    菜单枚举变量    */
typedef enum
{
    Menu_0 = 0x00,
	Menu_1,
	Menu_2,
	Menu_3,
	
}Menu_EnumTypeDef;



					  		 
#endif  
	 


