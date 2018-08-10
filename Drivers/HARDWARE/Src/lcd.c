  /*
  *******************************************************************************************************
  * File Name: LCD.c
  * Author:    JC.Yang
  * Version: 1.0.0
  * Date: 2018.7.31
  * Brief: 本文件提供了TFTLCD屏的初始化显示等操作
  *******************************************************************************************************
  * History
  *		  1.Author: JC.Yang 
  *			Date:2018.8.1 
  *			Mod: 完成STM32L432与TFTLCD屏的连接引脚配置，不能完成初始化
  *
  *		  2.Author: JC.Yang 
  *			Date: 2018.8.2
  *			Mod: 完成屏幕初始化，屏幕开始从亮变暗，但显示不了画面和文字，经过下午调整和优化，能显示字，存在刷新慢的问题
  *
  *******************************************************************************************************
  */	
	
#include "LCD.h"
#include "touch.h"
#include "stdlib.h"
#include "LCDfont.h"  	 
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_gpio.h"

/*
*********************************************************************************************************
*                            LCD_ShowImage                   
*
* Description: 图片编辑函数，根据该函数可任意显示各种样式和大小图片
*             
* Arguments  : 1> m:图片宽度
*			   2> n:图片高度
*  			   3> x:图片行数
*  			   3> y:图片列数
*  			   3>*p:各种图片数组（bmp.h中定义）
*
* Reutrn     : none.
*
* Note(s)    : None.
*********************************************************************************************************
*/
unsigned char ref=0;//刷新显示标志位
void LCD_ShowImage(int m,int n,int x,int y,const unsigned char *p)        
{
  	int i,j,k; 
	unsigned char picH,picL;
//	LCD_Clear(WHITE); //清屏  
//	Main_Menu(); //显示信息
	for(k=0;k<x;k++)									//行
	{
	   	for(j=0;j<y;j++)								//列
		{	
			Address_Set(m*j,n*j,m*j+m-1,n*j+n-1);
		    for(i=0;i<m*n;i++)
			 { 	
				 if(Detect()) //检测触摸和按键
				 {	
				 	ref=1;
				  	return;	
				 }		
				picL=*(p+i*2);	//数据低位在前
				picH=*(p+i*2+1);				
				LCD_WR_DATA8(picH);  						
				LCD_WR_DATA8(picL);			
			 }	
		 }
	}
	ref=0;				
}

void Main_Menu()//显示信息
{   
	u16 lx,ly;
	BACK_COLOR=WHITE;
	POINT_COLOR=RED;	
	LCD_ShowSinogram(32,32,70,0,0); 
	LCD_ShowSinogram(32,32,102,0,1);	
	LCD_ShowSinogram(32,32,134,0,2);  
	
	POINT_COLOR=BLUE;	
    LCD_ShowString( 40,35,"2.8 TFT SPI 240*320");
	LCD_ShowString( 10,55,"LCD_W:");LCD_ShowNum( 70,55,LCD_W,3);
	LCD_ShowString(130,55,"LCD_H:");LCD_ShowNum(180,55,LCD_H,3);	
	lx=10;ly=75;			
	LCD_ShowString(lx,ly,"VX:");lx+=40;LCD_ShowNum(lx,ly,vx,5);
	lx+=80;
	LCD_ShowString(lx,ly,"Vy:");lx+=40;LCD_ShowNum(lx,ly,vy,5);					
	lx=10;ly=95; 
	LCD_ShowString(lx,ly,"CHX:");lx+=40;LCD_ShowNum(lx,ly,chx,5);					
	lx+=80; 
	LCD_ShowString(lx,ly,"CHY:");lx+=40;LCD_ShowNum(lx,ly,chy,5);
	
	POINT_COLOR=GREEN;	
	LCD_DrawLine(0,120,240,120);
	LCD_DrawLine(0,121,240,121);
	
	POINT_COLOR=RED;	
	LCD_DrawPoint_Big(120,240);
	Draw_Circle(120,240,10);
	Draw_Circle(120,240,20);	
	Draw_Circle(120,240,30);	
	Draw_Circle(120,240,40);	
	Draw_Circle(120,240,50);	
	Draw_Circle(120,240,60);	
	Draw_Circle(120,240,70);	
	Draw_Circle(120,240,80);
	LCD_ShowString(50,130,"JC.Yang-----");LCD_ShowNum(150,130,66666,5);
}  

void DrawPicture()
{
	LCD_Clear(WHITE); //清屏
	BACK_COLOR=WHITE;
	POINT_COLOR=RED;
	Point();
}  

unsigned char Detect() //检测触摸和按键
{
	if (tpstate()==0) //如果触摸按下，则进入绘图程序
	 {
		DrawPicture();
		return 1;
	 }	
	   return 0;
}

u16 BACK_COLOR, POINT_COLOR;   //背景色，画笔色
void LCD_Writ_Bus(char dat)   //串行数据写入
{	
	unsigned char i;			  
  

	for(i=0;i<8;i++)
	{			  
		LCD_SCLK_Clr();
		if(dat&0x80)
		   LCD_SDIN_Set();
		else 
		   LCD_SDIN_Clr();
		LCD_SCLK_Set();
		dat<<=1;   
	}			
}

void LCD_WR_DATA8(char da) //发送数据-8位参数
{	
	LCD_CS_Clr();
    LCD_DC_Set();
	LCD_Writ_Bus(da);  
	LCD_CS_Set();
}  
 void LCD_WR_DATA(int da)
{	
	LCD_CS_Clr();
    LCD_DC_Set();
	LCD_Writ_Bus(da>>8);
    LCD_Writ_Bus(da);
	LCD_CS_Set();
}	  
void LCD_WR_REG(char da)	 
{		
	LCD_CS_Clr();
    LCD_DC_Clr();
	LCD_Writ_Bus(da);
	LCD_CS_Set();
}
 void LCD_WR_REG_DATA(int reg,int da)
{	LCD_CS_Clr();
    LCD_WR_REG(reg);
	LCD_WR_DATA(da);
	LCD_CS_Set();
}

void Address_Set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{ 
	LCD_WR_REG(0x2a);
    LCD_WR_DATA8(x1>>8);
    LCD_WR_DATA8(x1);
    LCD_WR_DATA8(x2>>8);
    LCD_WR_DATA8(x2);
  
    LCD_WR_REG(0x2b);
    LCD_WR_DATA8(y1>>8);
    LCD_WR_DATA8(y1);
    LCD_WR_DATA8(y2>>8);
    LCD_WR_DATA8(y2);

    LCD_WR_REG(0x2C);					 						 
}


void LCD_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* GPIO Ports Clock Enable */
//	__HAL_RCC_SPI1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStructure.Pin = GPIO_PIN_0|GPIO_PIN_5|GPIO_PIN_2|GPIO_PIN_7|GPIO_PIN_4;	 //PD3,PD6推挽输出  
 	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; 		 //推挽输出
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;     //速度50MHz   80
 	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);	  //初始化GPIOA0,5
	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_5|GPIO_PIN_2|GPIO_PIN_7|GPIO_PIN_4, GPIO_PIN_SET);// 输出高 A0 A5

//调用一次这些函数，免得编译的时候提示警告
  /* 	LCD_CS =1;
	if(LCD_CS==0)
	{
	   LCD_WR_REG_DATA(0,0);
	   LCD_ShowString(0,0," ");
	   LCD_ShowNum(0,0,0,0);
	   LCD_Show2Num(0,0,0,0);
	   LCD_DrawPoint_Big(0,0);
	   LCD_DrawRectangle(0,0,0,0);
	   Draw_Circle(0,0,0);
 	 }
	*/
	LCD_CS_Clr();  //打开片选使能
	LCD_RST_Clr();
	HAL_Delay(20);
	LCD_RST_Set();
	HAL_Delay(20);
	
//************* Start Initial Sequence **********//
//		LCD_WR_REG(0xCF);  
//		LCD_WR_DATA(0x00); 
//		LCD_WR_DATA(0xC1); 
//		LCD_WR_DATA(0X30); 
//		LCD_WR_REG(0xED);  
//		LCD_WR_DATA(0x64); 
//		LCD_WR_DATA(0x03); 
//		LCD_WR_DATA(0X12); 
//		LCD_WR_DATA(0X81); 
//		LCD_WR_REG(0xE8);  
//		LCD_WR_DATA(0x85); 
//		LCD_WR_DATA(0x10); 
//		LCD_WR_DATA(0x7A); 
//		LCD_WR_REG(0xCB);  
//		LCD_WR_DATA(0x39); 
//		LCD_WR_DATA(0x2C); 
//		LCD_WR_DATA(0x00); 
//		LCD_WR_DATA(0x34); 
//		LCD_WR_DATA(0x02); 
//		LCD_WR_REG(0xF7);  
//		LCD_WR_DATA(0x20); 
//		LCD_WR_REG(0xEA);  
//		LCD_WR_DATA(0x00); 
//		LCD_WR_DATA(0x00); 
//		LCD_WR_REG(0xC0);    //Power control 
//		LCD_WR_DATA(0x1B);   //VRH[5:0] 
//		LCD_WR_REG(0xC1);    //Power control 
//		LCD_WR_DATA(0x01);   //SAP[2:0];BT[3:0] 
//		LCD_WR_REG(0xC5);    //VCM control 
//		LCD_WR_DATA(0x30); 	 //3F
//		LCD_WR_DATA(0x30); 	 //3C
//		LCD_WR_REG(0xC7);    //VCM control2 
//		LCD_WR_DATA(0XB7); 
//		LCD_WR_REG(0x36);    // Memory Access Control 
//		LCD_WR_DATA(0x48); 
//		LCD_WR_REG(0x3A);   
//		LCD_WR_DATA(0x55); 
//		LCD_WR_REG(0xB1);   
//		LCD_WR_DATA(0x00);   
//		LCD_WR_DATA(0x1A); 
//		LCD_WR_REG(0xB6);    // Display Function Control 
//		LCD_WR_DATA(0x0A); 
//		LCD_WR_DATA(0xA2); 
//		LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
//		LCD_WR_DATA(0x00); 
//		LCD_WR_REG(0x26);    //Gamma curve selected 
//		LCD_WR_DATA(0x01); 
//		LCD_WR_REG(0xE0);    //Set Gamma 
//		LCD_WR_DATA(0x0F); 
//		LCD_WR_DATA(0x2A); 
//		LCD_WR_DATA(0x28); 
//		LCD_WR_DATA(0x08); 
//		LCD_WR_DATA(0x0E); 
//		LCD_WR_DATA(0x08); 
//		LCD_WR_DATA(0x54); 
//		LCD_WR_DATA(0XA9); 
//		LCD_WR_DATA(0x43); 
//		LCD_WR_DATA(0x0A); 
//		LCD_WR_DATA(0x0F); 
//		LCD_WR_DATA(0x00); 
//		LCD_WR_DATA(0x00); 
//		LCD_WR_DATA(0x00); 
//		LCD_WR_DATA(0x00); 		 
//		LCD_WR_REG(0XE1);    //Set Gamma 
//		LCD_WR_DATA(0x00); 
//		LCD_WR_DATA(0x15); 
//		LCD_WR_DATA(0x17); 
//		LCD_WR_DATA(0x07); 
//		LCD_WR_DATA(0x11); 
//		LCD_WR_DATA(0x06); 
//		LCD_WR_DATA(0x2B); 
//		LCD_WR_DATA(0x56); 
//		LCD_WR_DATA(0x3C); 
//		LCD_WR_DATA(0x05); 
//		LCD_WR_DATA(0x10); 
//		LCD_WR_DATA(0x0F); 
//		LCD_WR_DATA(0x3F); 
//		LCD_WR_DATA(0x3F); 
//		LCD_WR_DATA(0x0F); 
//		LCD_WR_REG(0x2B); 
//		LCD_WR_DATA(0x00);
//		LCD_WR_DATA(0x00);
//		LCD_WR_DATA(0x01);
//		LCD_WR_DATA(0x3f);
//		LCD_WR_REG(0x2A); 
//		LCD_WR_DATA(0x00);
//		LCD_WR_DATA(0x00);
//		LCD_WR_DATA(0x00);
//		LCD_WR_DATA(0xef);	 
//		LCD_WR_REG(0x11); //Exit Sleep
//		HAL_Delay(120);
//		LCD_WR_REG(0x29); //display on	

	LCD_WR_REG(0xCF);  
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0xD9); 
	LCD_WR_DATA8(0X30); 
	 
	LCD_WR_REG(0xED);  
	LCD_WR_DATA8(0x64); 
	LCD_WR_DATA8(0x03); 
	LCD_WR_DATA8(0X12); 
	LCD_WR_DATA8(0X81); 
	 
	LCD_WR_REG(0xE8);  
	LCD_WR_DATA8(0x85); 
	LCD_WR_DATA8(0x10); 
	LCD_WR_DATA8(0x78); 
	 
	LCD_WR_REG(0xCB);  
	LCD_WR_DATA8(0x39); 
	LCD_WR_DATA8(0x2C); 
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0x34); 
	LCD_WR_DATA8(0x02); 
	 
	LCD_WR_REG(0xF7);  
	LCD_WR_DATA8(0x20); 
	 
	LCD_WR_REG(0xEA);  
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0x00); 
	 
	LCD_WR_REG(0xC0);    //Power control 
	LCD_WR_DATA8(0x21);   //VRH[5:0] 
	 
	LCD_WR_REG(0xC1);    //Power control 
	LCD_WR_DATA8(0x12);   //SAP[2:0];BT[3:0] 
	 
	LCD_WR_REG(0xC5);    //VCM control 
	LCD_WR_DATA8(0x32); 
	LCD_WR_DATA8(0x3C); 
	 
	LCD_WR_REG(0xC7);    //VCM control2 
	LCD_WR_DATA8(0XC1); 
	 
	LCD_WR_REG(0x36);    // Memory Access Control 
	LCD_WR_DATA8(0x08); 
	 
	LCD_WR_REG(0x3A);   
	LCD_WR_DATA8(0x55); 

	LCD_WR_REG(0xB1);   
	LCD_WR_DATA8(0x00);   
	LCD_WR_DATA8(0x18); 
	 
	LCD_WR_REG(0xB6);    // Display Function Control 
	LCD_WR_DATA8(0x0A); 
	LCD_WR_DATA8(0xA2); 

	 
	 
	LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
	LCD_WR_DATA8(0x00); 
	 
	LCD_WR_REG(0x26);    //Gamma curve selected 
	LCD_WR_DATA8(0x01); 
	 
	LCD_WR_REG(0xE0);    //Set Gamma 
	LCD_WR_DATA8(0x0F); 
	LCD_WR_DATA8(0x20); 
	LCD_WR_DATA8(0x1E); 
	LCD_WR_DATA8(0x09); 
	LCD_WR_DATA8(0x12); 
	LCD_WR_DATA8(0x0B); 
	LCD_WR_DATA8(0x50); 
	LCD_WR_DATA8(0XBA); 
	LCD_WR_DATA8(0x44); 
	LCD_WR_DATA8(0x09); 
	LCD_WR_DATA8(0x14); 
	LCD_WR_DATA8(0x05); 
	LCD_WR_DATA8(0x23); 
	LCD_WR_DATA8(0x21); 
	LCD_WR_DATA8(0x00); 
	 
	LCD_WR_REG(0XE1);    //Set Gamma 
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0x19); 
	LCD_WR_DATA8(0x19); 
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0x12); 
	LCD_WR_DATA8(0x07); 
	LCD_WR_DATA8(0x2D); 
	LCD_WR_DATA8(0x28); 
	LCD_WR_DATA8(0x3F); 
	LCD_WR_DATA8(0x02); 
	LCD_WR_DATA8(0x0A); 
	LCD_WR_DATA8(0x08); 
	LCD_WR_DATA8(0x25); 
	LCD_WR_DATA8(0x2D); 
	LCD_WR_DATA8(0x0F); 
	 
	LCD_WR_REG(0x11);    //Exit Sleep 
	HAL_Delay(120); 
	LCD_WR_REG(0x29);    //Display on 
	 
} 

//清屏函数
//Color:要清屏的填充色
void LCD_Clear(u16 Color)
{
	u16 i,j;  	
	Address_Set(0,0,LCD_W-1,LCD_H-1);
    for(i=0;i<LCD_W;i++)
	 {
	  for (j=0;j<LCD_H;j++)
	   	{
        	LCD_WR_DATA(Color);	 			 
	    }
	  }
}

/*
*********************************************************************************************************
*                            LCD_ShowSinogram                  
*
* Description: 汉字显示函数，用于显示各种大小字体的函数
* 			   POINT_COLOR为内容颜色，BACK_COLOR为背景颜色
* Arguments  : 1> m:
*              2> n:
*              2> x:
*              2> y:
*              2> index:
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
//在指定位置显示一个汉字(32*32大小)
void LCD_ShowSinogram(unsigned int m,unsigned int n,unsigned int x,unsigned int y,unsigned char index)	
{  
	unsigned char i,j,k;
	k=m*n/8;
	u8 *temp=name;    
    Address_Set(x,y,x+m-1,y+n-1); //设置区域      
	temp+=index*k;
	for(j=0;j<k;j++)
	{
		for(i=0;i<8;i++)
		{ 		     
		 	if((*temp&(1<<i))!=0)
			{
				LCD_WR_DATA(POINT_COLOR);
			} 
			else
			{
				LCD_WR_DATA(BACK_COLOR);
			}   
		}
		temp++;
	 }
}

void LCD_ShowRETURN(unsigned int x,unsigned int y,unsigned char index)	
{  
	unsigned char i,j;
	unsigned char *temp=Return;    
    Address_Set(x,y,x+15,y+15); //设置区域      
	temp+=index*32;
	for(j=0;j<32;j++)
	{
		for(i=0;i<8;i++)
		{ 		     
		 	if((*temp&(1<<i))!=0)
			{
				LCD_WR_DATA(POINT_COLOR);
			} 
			else
			{
				LCD_WR_DATA(BACK_COLOR);
			}   
		}
		temp++;
	 }
}

//画点
//POINT_COLOR:此点的颜色
void LCD_DrawPoint(u16 x,u16 y)
{
	Address_Set(x,y,x,y);//设置光标位置 
	LCD_WR_DATA(POINT_COLOR); 	    
} 	 
//画一个大点
//POINT_COLOR:此点的颜色
void LCD_DrawPoint_Big(u16 x,u16 y)
{
	LCD_Fill(x-1,y-1,x+1,y+1,POINT_COLOR);
} 
//在指定区域内填充指定颜色
//区域大小:
//  (xend-xsta)*(yend-ysta)
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 i,j; 
	Address_Set(xsta,ysta,xend,yend);      //设置光标位置 
	for(i=ysta;i<=yend;i++)
	{													   	 	
		for(j=xsta;j<=xend;j++)LCD_WR_DATA(color);//设置光标位置 	    
	} 					  	    
}  
//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    
//画矩形
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a);             //3           
		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0-a,y0+b);             //1       
		LCD_DrawPoint(x0-b,y0-a);             //7           
		LCD_DrawPoint(x0-a,y0-b);             //2             
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0-b);             //5
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-b,y0+a);             
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_DrawPoint(x0+a,y0+b);
	}
} 
//在指定位置显示一个字符

//num:要显示的字符:" "--->"~"
//mode:叠加方式(1)还是非叠加方式(0)
//在指定位置显示一个字符

//num:要显示的字符:" "--->"~"

//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode)
{
    u8 temp;
    u8 pos,t;
	u16 x0=x;
	u16 colortemp=POINT_COLOR;      
    if(x>LCD_W-16||y>LCD_H-16)return;	    
	//设置窗口		   
	num=num-' ';//得到偏移后的值
	Address_Set(x,y,x+8-1,y+16-1);      //设置光标位置 
	if(!mode) //非叠加方式
	{
		for(pos=0;pos<16;pos++)
		{ 
			temp=asc2_1608[(u16)num*16+pos];		 //调用1608字体
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				LCD_WR_DATA(POINT_COLOR);	
				temp>>=1; 
				x++;
		    }
			x=x0;
			y++;
		}	
	}else//叠加方式
	{
		for(pos=0;pos<16;pos++)
		{
		    temp=asc2_1608[(u16)num*16+pos];		 //调用1608字体
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos);//画一个点     
		        temp>>=1; 
		    }
		}
	}
	POINT_COLOR=colortemp;	    	   	 	  
}   
//m^n函数
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//color:颜色
//num:数值(0~4294967295);	
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len)
{         	
	u8 t,temp;
	u8 enshow=0;
	num=(u16)num;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+8*t,y,' ',0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+8*t,y,temp+48,0); 
	}
} 
//显示2个数字
//x,y:起点坐标
//num:数值(0~99);	 
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len)
{         	
	u8 t,temp;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
	 	LCD_ShowChar(x+8*t,y,temp+'0',0); 
	}
} 
//显示字符串
//x,y:起点坐标  
//*p:字符串起始地址
//用16字体
void LCD_ShowString(u16 x,u16 y,const u8 *p)
{         
    while(*p!='\0')
    {       
        if(x>LCD_W-16){x=0;y+=16;}
        if(y>LCD_H-16){y=x=0;LCD_Clear(RED);}
        LCD_ShowChar(x,y,*p,0);
        x+=8;
        p++;
    }  
}







