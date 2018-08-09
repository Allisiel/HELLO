/**
  *******************************************************************************************************
  * File Name: main.c
  * Author: JC.Yang
  * Version: V1.0.0
  * Date: 2018-7-31
  * Brief: ���ļ�����ʵ��TFTLCD���Ĵ������Ʋ���
  *******************************************************************************************************
  * History:
  *			Date: 2018-3-3
  *			Mod: �����ļ�
  *         description:
              TFT_LCD �ӿ���ʾ����(stm32ϵ��)
              ˵��: 
              -�˲���Ϊ���������ӿڣ�BLKΪ���⿪�أ�Ĭ�Ͽ��Բ���---------------------------------------------------------------
              GND    ��Դ��
              VCC    3.3v��Դ
              CLK    ��PA5
              MOSI   ��PA7
              RES    ��PA2
              DC     ��PA4
              CS1    ��PA0               
              -����Ϊ�������ƽӿڣ�������ó�û���ܿ��Բ���---------------------------------------------------------------
              MISO   ��PA6
              CS2    ��PA1
              PEN    ��PA3
  
  *******************************************************************************************************
  */	
#ifndef __OLED_H
#define __OLED_H			  	 
#include "stdlib.h"	
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_gpio.h"

#define LCD_W 240
#define LCD_H 320
#define	u8 unsigned char
#define	u16 unsigned int
#define	u32 unsigned long

//OLEDģʽ����
//0:4�ߴ���ģʽ
//1:����8080ģʽ
#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED�˿ڶ���----------------  					   
#define OLED_CS_Clr()    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET) //CS
#define OLED_CS_Set()    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET)

#define OLED_RST_Clr() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET)   //RES
#define OLED_RST_Set() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET)

#define OLED_DC_Clr() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)    //DC
#define OLED_DC_Set() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)


#define OLED_SCLK_Clr() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET)  //CLK
#define OLED_SCLK_Set() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET)

#define OLED_SDIN_Clr() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET)  //DIN MOSI
#define OLED_SDIN_Set() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET)

 		     
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

extern  u16 BACK_COLOR, POINT_COLOR;   //����ɫ������ɫ

void LCD_Init(void); 
void LCD_Clear(u16 Color);
void Address_Set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void LCD_WR_DATA8(char da); //��������-8λ����
void LCD_WR_DATA(int da);
void LCD_WR_REG(char da);

void LCD_DrawPoint(u16 x,u16 y);//����
void LCD_DrawPoint_Big(u16 x,u16 y);//��һ�����
u16  LCD_ReadPoint(u16 x,u16 y); //����
void Draw_Circle(u16 x0,u16 y0,u8 r);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode);//��ʾһ���ַ�
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len);//��ʾ����
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len);//��ʾ2������
void LCD_ShowString(u16 x,u16 y,const u8 *p);//��ʾһ���ַ���,16����
 
void LCD_ShowSinogram(unsigned int m,unsigned int n,unsigned int x,unsigned int y,unsigned char index);
void LCD_ShowRETURN(unsigned int x,unsigned int y,unsigned char index);

void Fast_DrawFont_GBK16(u16 x, u16 y, u16 fc, u16 bc, u8 *s);


//������ɫ
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
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)


					  		 
#endif  
	 

