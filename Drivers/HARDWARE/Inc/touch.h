//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//
//  文 件 名   : touch.c
//  版 本 号   : v1.0
//  作    者   : JC.Yang
//  生成日期   : 2018.7.31
//  最近修改   : 
//  功能描述   : TFT_LCD 接口演示例程(stm32l432系列)
//              说明: 
//              -此部分为不带触摸接口，BLK为背光开关，默认可以不接---------------------------------------------------------------
//              GND    电源地
//              VCC    3.3v电源
//              CLK    接PA5
//              MOSI   接PA7
//              RES    接PA2
//              DC     接PA4
//              CS1    接PA0               
//              -以下为触摸控制接口；如果不用出没功能可以不接---------------------------------------------------------------
//              MISO   接PA6
//              CS2    接PA1
//              PEN    接PA3
// 修改历史   :
// 日    期   : 

//******************************************************************************/

#ifndef __TOUCH_H
#define __TOUCH_H	

/* touch panel interface define */
#define  CMD_RDX  0xD0	 //触摸IC读坐标积存器
#define  CMD_RDY  0x90
#define	u8 unsigned char
#define	u16 unsigned int
#define	u32 unsigned long


//#define PEN  	PCin(10)  	//PC10 INT
//#define MISO 	PCin(2)   	//PC2  MISO
////#define DIN 	PCout(3)  	//PC3  MOSI
////#define CLK 	PCout(1)  	//PC1  SCLK
//#define CS2  	PCout(0)  	//PC0  CS

#define PEN  	HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3)
#define MISO 	HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6)

#define LCD_CS2_Clr()    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET) //CS2
#define LCD_CS2_Set()    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET)
struct tp_pix_
{
	u16 x;
	u16 y;
};
struct tp_pixu32_
{
	u32 x;
	u32 y;
};
extern struct tp_pix_  tp_pixad,tp_pixlcd;	 //当前触控坐标的AD值,前触控坐标的像素值   
extern u16 vx,vy;  //比例因子，此值除以1000之后表示多少个AD值代表一个像素点
extern u16 chx,chy;//默认像素点坐标为0时的AD起始值
u8 tpstate(void);
void SPI_Init(void);  
void Touch_Adjust(void);
void Point(void); //绘图函数
u16 ReadFromCharFrom7843(void);         //SPI 读数据
u8 TP_Init(void);
#endif  
	 
	 



