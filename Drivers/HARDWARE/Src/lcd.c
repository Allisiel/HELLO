  /*
  *******************************************************************************************************
  * File Name: LCD.c
  * Author:    JC.Yang
  * Version: 1.0.0
  * Date: 2018.7.31
  * Brief: ���ļ��ṩ��TFTLCD���ĳ�ʼ����ʾ�Ȳ���
  *******************************************************************************************************
  * History
  *		  1.Author: JC.Yang 
  *			Date:2018.8.1 
  *			Mod: ���STM32L432��TFTLCD���������������ã�������ɳ�ʼ��
  *
  *		  2.Author: JC.Yang 
  *			Date: 2018.8.2
  *			Mod: �����Ļ��ʼ������Ļ��ʼ�����䰵������ʾ���˻�������֣���������������Ż�������ʾ�֣�����ˢ����������
  *
  *		  3.Author: JC.Yang 
  *			Date: 2018.8.3
  *			Mod: ����ľ�����²μ�ReadMe.txt�к�GitHub����Description
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
* Description: ͼƬ�༭���������ݸú�����������ʾ������ʽ�ʹ�СͼƬ
*             
* Arguments  : 1> m:ͼƬ���
*			   2> n:ͼƬ�߶�
*  			   3> x:ͼƬ����
*  			   3> y:ͼƬ����
*  			   3>*p:����ͼƬ���飨bmp.h�ж��壩
*
* Reutrn     : none.
*
* Note(s)    : None.
*********************************************************************************************************
*/
unsigned char ref=0;//ˢ����ʾ��־λ
void LCD_ShowImage(int m,int n,int x,int y,const unsigned char *p)        
{
  	int i,j,k; 
	unsigned char picH,picL;
//	LCD_Clear(WHITE); //����  
//	Main_Menu(); //��ʾ��Ϣ
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);
	
	for(k=0;k<x;k++)									//��
	{
	   	for(j=0;j<y;j++)								//��
		{	
			Address_Set(m*j,n*j,m*j+m-1,n*j+n-1);
		    for(i=0;i<m*n;i++)
			 { 	
				 if(Detect()) //��ⴥ���Ͱ���
				 {	
				 	ref=1;
				  	return;	
				 }		
				picL=*(p+i*2);	//���ݵ�λ��ǰ
				picH=*(p+i*2+1);				
				LCD_WR_DATA8(picH);  						
				LCD_WR_DATA8(picL);			
			 }	
		 }
	}
	ref=0;
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);	
}

/*
*********************************************************************************************************
*                      Main_Menu                    
*
* Description: ���˵���ʾ����
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void Main_Menu()
{   
	u16 lx,ly;
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);

	BACK_COLOR=WHITE;
	POINT_COLOR=RED;	
	LCD_ShowSinogram_32(32,32,70,0,0); 
	LCD_ShowSinogram_32(32,32,102,0,1);	
	LCD_ShowSinogram_32(32,32,134,0,2);  
	
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
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);
}  

/*
*********************************************************************************************************
*                      Menu_First                    
*
* Description: ����һ
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void Menu_First(void)
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);
	
	BACK_COLOR=WHITE;
	POINT_COLOR=RED;	
	LCD_DrawPoint_Big(120,160);
	Draw_Circle(120,160,10);
	Draw_Circle(120,160,30);	
	Draw_Circle(120,160,50);	
	Draw_Circle(120,160,70);	
	Draw_Circle(120,160,90);	
	Draw_Circle(120,160,110);	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);
}
/*
*********************************************************************************************************
*                      DrawPicture                    
*
* Description: ��ͼ�������
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void DrawPicture()
{
	LCD_Clear(WHITE); //����
	BACK_COLOR=WHITE;
	POINT_COLOR=RED;
	Point();
}  
/*
*********************************************************************************************************
*                      Detect                    
*
* Description: ��ⴥ���Ͱ���
*             
* Arguments  : None.
*
* Reutrn     : 1��˵����������
*              0��˵���޴���
*
* Note(s)    : None.
*********************************************************************************************************
*/
unsigned char Detect() 
{
	if (tpstate()==0) //����������£�������ͼ����
	 {
		DrawPicture();//��ͼ
		return 1;
	 }	
	   return 0;
}
/*
*********************************************************************************************************
*                      LCD_Writ_Bus                    
*
* Description: ��������д��
*              BACK_COLOR������ɫ
*			   POINT_COLOR������ɫ
*             
* Arguments  : dat��д�������
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
u16 BACK_COLOR, POINT_COLOR; 
void LCD_Writ_Bus(char dat)   
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
/*
*********************************************************************************************************
*                      LCD_WR_DATA8                    
*
* Description: ��������8λ����
*             
* Arguments  : da�����͵�8λ����
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void LCD_WR_DATA8(char da) 
{	
	LCD_CS_Clr();
    LCD_DC_Set();
	LCD_Writ_Bus(da);  
	LCD_CS_Set();
} 
/*
*********************************************************************************************************
*                      LCD_WR_DATA                    
*
* Description: ��������16λ����
*             
* Arguments  : da�����͵�16λ����
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
 void LCD_WR_DATA(int da)
{	
	LCD_CS_Clr();
    LCD_DC_Set();
	LCD_Writ_Bus(da>>8);
    LCD_Writ_Bus(da);
	LCD_CS_Set();
}
/*
*********************************************************************************************************555555
*                      LCD_WR_REG                    
*
* Description: ��Ļ��ʼ���������õ��ĺ���
*             
* Arguments  : da��������д�������
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void LCD_WR_REG(char da)	 
{		
	LCD_CS_Clr();
    LCD_DC_Clr();
	LCD_Writ_Bus(da);
	LCD_CS_Set();
}
/*
*********************************************************************************************************55555555
*                      LCD_WR_REG_DATA                    
*
* Description: ��Ļ��ʼ�����õ��ĺ�����ǰ�ߵ���չ�棩 ����������л���û�õ���
*             
* Arguments  : reg��д��ĵ�ַ
*			   da��д�������
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
 void LCD_WR_REG_DATA(int reg,int da)
{	
	LCD_CS_Clr();
    LCD_WR_REG(reg);
	LCD_WR_DATA(da);
	LCD_CS_Set();
}
/*
*********************************************************************************************************
*                      Address_Set                    
*
* Description: ���ý�Ҫ���������ص�����
*             
* Arguments  : x1�����Ͻǵĺ�����
*              y1�����Ͻǵ�������
*              x2�����½ǵĺ�����
*              y2�����½ǵ�������
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
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

/*
*********************************************************************************************************
*                      LCD_Init                    
*
* Description: LCD��Ļ�ĳ�ʼ�����루����ģ��SPI���ŵĳ�ʼ����
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void LCD_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* GPIO Ports Clock Enable */
	__HAL_RCC_SPI1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStructure.Pin = GPIO_PIN_0|GPIO_PIN_5|GPIO_PIN_2|GPIO_PIN_7|GPIO_PIN_4;	 //PD3,PD6�������  
 	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; 		 //�������
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;     //�ٶ�50MHz   80
 	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);	  //��ʼ��GPIOA0,5
	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_5|GPIO_PIN_2|GPIO_PIN_7|GPIO_PIN_4, GPIO_PIN_SET);// ����� A0 A5
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pin = GPIO_PIN_3;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);

//����һ����Щ��������ñ����ʱ����ʾ����
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
	LCD_CS_Clr();  //��Ƭѡʹ��
	LCD_RST_Clr();
	HAL_Delay(20);
	LCD_RST_Set();
	HAL_Delay(20);
	
//************* Start Initial Sequence **********//
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
/*
*********************************************************************************************************
*                      LCD_Clear                    
*
* Description: ��������
*             
* Arguments  : Color��Ҫ���������ɫ
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void LCD_Clear(u16 Color)
{
	u16 i,j;  
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);	
	Address_Set(0,0,LCD_W-1,LCD_H-1);
    for(i=0;i<LCD_W;i++)
	 {
	  for (j=0;j<LCD_H;j++)
	   	{
        	LCD_WR_DATA(Color);	 			 
	    }
	  }
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);

}

/*
*********************************************************************************************************
*                      LCD_ShutDown                    
*
* Description: ��������
*             
* Arguments  : sec������δ�����೤ʱ�����������λΪ�룩
*
* Reutrn     : 0������
*			   1������
*
* Note(s)    : None.
*********************************************************************************************************
*/
u8 LCD_ShutDown(u8 sec)
{
	u32 Tim1,Tim2;
	for(Tim1=0;Tim1<(sec*1000);Tim1++)
	{
		for(Tim2=0;Tim2<5000;Tim2++)
		{
			if(!PEN)
			{
				return 1;
			}
		}
	}
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);

	return 0;
}
/*
*********************************************************************************************************
*                      LCD_Show                    
*
* Description: ���������д�������������Ļ
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void LCD_Show(void)
{
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3) == 0)
	{
		if(!PEN)	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);	
	}
	
}	

/*
*********************************************************************************************************
*                            LCD_ShowSinogram_32                  
*
* Description: ������ʾ������������ʾ���ִ�С����ĺ���
* 			   POINT_COLORΪ������ɫ��BACK_COLORΪ������ɫ
* Arguments  : 1> m:���ֿ��
*              2> n:���ָ߶�
*              3> x:���������ϽǺ����꣨���240��
*              4> y:���������Ͻ������꣨���320��
*              5> index:���ֿ⣨�������ֿ�16*16��С��32*32��С��
*			     �����庺�ֱ�Ųο�LCDfont.h�ļ���Font_Library_16��Font_Library_32���飩
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void LCD_ShowSinogram_32(unsigned int m,unsigned int n,unsigned int x,unsigned int y,unsigned char index)	
{  
	unsigned char i,j,k;
	k=m*n/8;
	u8 *temp=Font_Library_32;    
    Address_Set(x,y,x+m-1,y+n-1); //��������      
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
/*
*********************************************************************************************************
*                      LCD_ShowSinogram_16                    
*
* Description: ������ʾ������������ʾ���ִ�С����ĺ���(���Ż� ����һ��������ͬ)
*             
* Arguments  : x�����ϽǺ�����
*              y�����Ͻ�������
*              index������ɸѡ
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void LCD_ShowSinogram_16(unsigned int x,unsigned int y,unsigned char index)	
{  
	unsigned char i,j;
	unsigned char *temp=Font_Library_16;    
    Address_Set(x,y,x+15,y+15); //��������      
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
/*
*********************************************************************************************************
*                      LCD_SetPixel                    
*
* Description: ���㺯��(POINT_COLOR:�˵����ɫ)
*             
* Arguments  : x���������
*              y����������
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void LCD_SetPixel(u16 x,u16 y)
{
	Address_Set(x,y,x,y);//���ù��λ�� 
	LCD_WR_DATA(POINT_COLOR); 	    
} 	 
/*
*********************************************************************************************************
*                      LCD_DrawPoint_Big                    
*
* Description: ��һ�����(POINT_COLOR:�˵����ɫ)
*             
* Arguments  : x���������
*              y����������
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void LCD_DrawPoint_Big(u16 x,u16 y)
{
	LCD_Fill(x-1,y-1,x+1,y+1,POINT_COLOR);
} 

u16 LCD_ReadPoint(u16 x,u16 y)
{
	return 0;
}
/*
*********************************************************************************************************
*                      LCD_Fill                    
*
* Description: ��ָ�����������ָ����ɫ
*              �����С��xend-xsta)*(yend-ysta)    
*
* Arguments  : xsta�����ϽǺ�����
*              xend�����Ͻ�������
*              ysta�����½Ǻ�����
*              yend�����½�������
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 i,j; 
	Address_Set(xsta,ysta,xend,yend);      //���ù��λ�� 
	for(i=ysta;i<=yend;i++)
	{													   	 	
		for(j=xsta;j<=xend;j++)LCD_WR_DATA(color);//���ù��λ�� 	    
	} 					  	    
}  
/*
*********************************************************************************************************
*                      LCD_DrawLine                    
*
* Description: ���ߺ���
*             
* Arguments  : x1����������
*			   y1�����������
*              x2���յ������
*			   y2���յ�������
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_SetPixel(uRow,uCol);//���� 
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
/*
*********************************************************************************************************
*                      LCD_DrawRectangle                    
*
* Description: �����κ���������䣩
*             
* Arguments  : x1����������
*			   y1�����������
*              x2���յ������
*			   y2���յ�������
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
/*
*********************************************************************************************************
*                      Draw_Circle                    
*
* Description: ��ָ��λ�û�һ��ָ����С��Բ
*             
* Arguments  : x�����ĵ������
*			   y�����ĵ�������
*			   r���뾶
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
	while(a<=b)
	{
		LCD_SetPixel(x0-b,y0-a);             //3           
		LCD_SetPixel(x0+b,y0-a);             //0           
		LCD_SetPixel(x0-a,y0+b);             //1       
		LCD_SetPixel(x0-b,y0-a);             //7           
		LCD_SetPixel(x0-a,y0-b);             //2             
		LCD_SetPixel(x0+b,y0+a);             //4               
		LCD_SetPixel(x0+a,y0-b);             //5
		LCD_SetPixel(x0+a,y0+b);             //6 
		LCD_SetPixel(x0-b,y0+a);             
		a++;
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_SetPixel(x0+a,y0+b);
	}
} 
/*
*********************************************************************************************************
*                      LCD_ShowChar                    
*
* Description: ��ָ��λ����ʾһ���ַ���8*16��С��
*             
* Arguments  : x���ַ�������
*			   y���ַ�������
*              num��Ҫ��ʾ���ַ�:" "--->"~"
*			   mode�����ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)   
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode)
{
    u8 temp;
    u8 pos,t;
	u16 x0=x;
	u16 colortemp=POINT_COLOR;      
    if(x>LCD_W-16||y>LCD_H-16)return;	    
	//���ô���		   
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	Address_Set(x,y,x+8-1,y+16-1);      //���ù��λ�� 
	if(!mode) //�ǵ��ӷ�ʽ
	{
		for(pos=0;pos<16;pos++)
		{ 
			temp=asc2_1608[(u16)num*16+pos];		 //����1608����
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
	}else//���ӷ�ʽ
	{
		for(pos=0;pos<16;pos++)
		{
		    temp=asc2_1608[(u16)num*16+pos];		 //����1608����
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)LCD_SetPixel(x+t,y+pos);//��һ����     
		        temp>>=1; 
		    }
		}
	}
	POINT_COLOR=colortemp;	    	   	 	  
}
/*
*********************************************************************************************************
*                      mypow                    
*
* Description: m^n����
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}	
/*
*********************************************************************************************************
*                      LCD_ShowNum                    
*
* Description: ��ʾ���ֺ���
*             
* Arguments  : x,y���������	 
*   		   len�����ֵ�λ��
*			   num:��ֵ(0~4294967295);
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
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
/*
*********************************************************************************************************
*                      LCD_Show2Num                    
*
* Description: ��ʾ2������
*             
* Arguments  : x,y���������
*			   num����ֵ(0~99)
*			   len�����ֳ���
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len)
{         	
	u8 t,temp;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
	 	LCD_ShowChar(x+8*t,y,temp+'0',0); 
	}
} 
/*
*********************************************************************************************************
*                      LCD_ShowString                    
*
* Description: ��ʾ�ַ�����������16���壩
*             
* Arguments  : x,y:�������  
*		   	   *p:�ַ�����ʼ��ַ
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
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







