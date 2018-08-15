#include "stm32l4xx_hal.h"
#include "stdlib.h"	 
#include "touch.h"
#include "LCD.h"
//#include "LCDfont.h"


//***���������β�ͬ��ԭ��Ĭ�ϵ�У׼����ֵ���ܻ�������ʶ��׼������У׼����ʹ�ã�������ʹ�ù̶���Ĭ��У׼����
u16 vx=15542,vy=11165;  //�������ӣ���ֵ����1000֮���ʾ���ٸ�ADֵ����һ�����ص�
u16 chx=140,chy=146;//Ĭ�����ص�����Ϊ0ʱ��AD��ʼֵ
//***���������β�ͬ��ԭ��Ĭ�ϵ�У׼����ֵ���ܻ�������ʶ��׼������У׼����ʹ�ã�������ʹ�ù̶���Ĭ��У׼����

struct tp_pix_  tp_pixad,tp_pixlcd;	 //��ǰ���������ADֵ,ǰ�������������ֵ

/*
*********************************************************************************************************
*                      TP_Init                    
*
* Description: �Դ������������Ž���ģ��SPI��ʼ��
*             
* Arguments  : None.
*
* Reutrn     : 0.
*
* Note(s)    : None.
*********************************************************************************************************
*/
u8 TP_Init(void)
{	   
	//ע��,ʱ��ʹ��֮��,��GPIO�Ĳ�������Ч
	//��������֮ǰ,����ʹ��ʱ��.����ʵ��������������� 	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	//CS  SCLK MOSI 
	GPIO_InitStructure.Pin = GPIO_PIN_1;
  	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//MISO INT ��������
	GPIO_InitStructure.Pin = GPIO_PIN_3 | GPIO_PIN_6;
  	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull  = GPIO_PULLUP;
  	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	return 0;
}
/*
*********************************************************************************************************
*                      tpstate                    
*
* Description: ���PEN���ţ����ڼ���Ƿ��д�������
*             
* Arguments  : None.
*
* Reutrn     : 0���д�������
*			   1��δ�д���
*
* Note(s)    : None.
*********************************************************************************************************
*/
u8 tpstate(void)
{
	return 	PEN;
}
/*
*********************************************************************************************************
*                      Touch_Init                    
*
* Description: �Դ������Ž���SPIʱ������(SPI��ʼ)
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void Touch_Init(void)                                  
{
	LCD_CS2_Set();
	LCD_SCLK_Set();
	LCD_SDIN_Set();
	LCD_SCLK_Set();
}
/*
*********************************************************************************************************
*                      WriteCharTo7843                    
*
* Description: SPIд����
*             
* Arguments  : num��д������
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
void WriteCharTo7843(unsigned char num)      
{
	unsigned char count=0;
	LCD_SCLK_Clr();
	for(count=0;count<8;count++)
	{
	LCD_SCLK_Clr();
			if(num&0x80)
			   LCD_SDIN_Set();
			else 
			   LCD_SDIN_Clr();
					LCD_SCLK_Set();
		num<<=1;
	}
}
/*
*********************************************************************************************************
*                      ReadFromCharFrom7843                    
*
* Description: SPI ������
*             
* Arguments  : None.
*
* Reutrn     : ����������
*
* Note(s)    : None.
*********************************************************************************************************
*/
u16 ReadFromCharFrom7843()             
{
	u8 count=0;
	u16 Num=0;
	for(count=0;count<12;count++)
	{
		Num<<=1;		
		LCD_SCLK_Set();               //�½�����Ч
		LCD_SCLK_Clr();
		if(MISO)
		{
			Num|=1;
		}
		
	}

	return(Num);
}	
/*
*********************************************************************************************************
*                      ADS_Read_AD                    
*
* Description: ��7846/7843/XPT2046/UH7843/UH7846��ȡadcֵ
*             
* Arguments  : 0x90=y   0xd0-x
*
* Reutrn     : l��������ֵ
*
* Note(s)    : None.
*********************************************************************************************************
*/
//	 
u16 ADS_Read_AD(unsigned char CMD)          
{
	u16 l;
	LCD_CS2_Clr();
	WriteCharTo7843(CMD);        //�Ϳ����ּ��ò�ַ�ʽ��X���� ��ϸ����й�����
	LCD_SCLK_Set(); 
	LCD_SCLK_Clr();
	l=ReadFromCharFrom7843();
	LCD_CS2_Set();
	return l;
}	
/*
*********************************************************************************************************
*                      ADS_Read_XY                    
*
* Description: ��ȡһ������ֵ
*              ������ȡREAD_TIMES������,����Щ������������,
*		       Ȼ��ȥ����ͺ����LOST_VAL����,ȡƽ��ֵ 
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
#define READ_TIMES 15 //��ȡ����
#define LOST_VAL 5	  //����ֵ

u16 ADS_Read_XY(u8 xy)
{
	u16 i, j;
	u16 buf[READ_TIMES];
	u16 sum=0;
	u16 temp;
	for(i=0;i<READ_TIMES;i++)
	{				 
		buf[i]=ADS_Read_AD(xy);	    
	}				    
	for(i=0;i<READ_TIMES-1; i++)//����
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//��������
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 
/*
*********************************************************************************************************
*                      Read_ADS                    
*
* Description: ���˲��������ȡ
*			   ��Сֵ��������100.
*             
* Arguments  : *x����ȡ�ĺ�����
*			   *y����ȡ��������
*
* Reutrn     : 0������ʧ��
*			   1�������ɹ�
*
* Note(s)    : None.
*********************************************************************************************************
*/
u8 Read_ADS(u16 *x,u16 *y)
{
	u16 xtemp,ytemp;			 	 		  
	xtemp=ADS_Read_XY(CMD_RDX);
	ytemp=ADS_Read_XY(CMD_RDY);	 									   
	if(xtemp<100||ytemp<100)return 0;
	*x=xtemp;
	*y=ytemp;
	return 1;
}
/*
*********************************************************************************************************
*                      Read_ADS2                    
*
* Description: 2�ζ�ȡADS7846,������ȡ2����Ч��ADֵ,�������ε�ƫ��ܳ���50,��������,����Ϊ������ȷ,�����������.	   
			   �ú����ܴ�����׼ȷ��
*             
* Arguments  : ͬ�ϸ�����
*
* Reutrn     : ͬ�ϸ�����
*
* Note(s)    : None.
*********************************************************************************************************
*/
#define ERR_RANGE 20 //��Χ 
u8 Read_ADS2(u16 *x,u16 *y) 
{
	u16 x1,y1;
 	u16 x2,y2;
 	u8 flag;    
    flag=Read_ADS(&x1,&y1);   
    if(flag==0)return(0);
    flag=Read_ADS(&x2,&y2);	
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//ǰ�����β�����+-ERR_RANGE��
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)>>1;
        *y=(y1+y2)>>1;		
        return 1;
    }else return 0;	  
} 
/*
*********************************************************************************************************
*                      Read_TP_Once                    
*
* Description: ��ȷ��ȡһ������,У׼��ʱ���õ�	
*             
* Arguments  : None.
*
* Reutrn     : re����ȡ������
*
* Note(s)    : None.
*********************************************************************************************************
*/ 
u8 Read_TP_Once(void)
{
	u8 re=0;
	u16 x1,y1;
	while(re==0)
	{
		while(!Read_ADS2(&tp_pixad.x,&tp_pixad.y));
		HAL_Delay(10);
		while(!Read_ADS2(&x1,&y1));
		if(tp_pixad.x==x1&&tp_pixad.y==y1)
		{
			re=1; 
		}
	} 
	return re;
}
/*
*********************************************************************************************************
*                      Drow_Touch_Point                    
*
* Description: ��LCD�����йصĺ���  
*			   ��һ�������㣬����У׼�õ�
*             
* Arguments  : x������ĺ�����
*			   y�������������
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/

void Drow_Touch_Point(u16 x,u16 y)
{
	LCD_DrawLine(x-12,y,x+13,y);//����
	LCD_DrawLine(x,y-12,x,y+13);//����
	LCD_SetPixel(x+1,y+1);
	LCD_SetPixel(x-1,y+1);
	LCD_SetPixel(x+1,y-1);
	LCD_SetPixel(x-1,y-1);
//	Draw_Circle(x,y,6);//������Ȧ
}	  
/*
*********************************************************************************************************
*                      Convert_Pos                    
*
* Description: ת�����
*			   ���ݴ�������У׼����������ת����Ľ��,������X0,Y0��
*             
* Arguments  : None.
*
* Reutrn     : l��ת�����
*
* Note(s)    : None.
*********************************************************************************************************
*/
u8 Convert_Pos(void)
{		 	 
	u8 l=0; 
	if(Read_ADS2(&tp_pixad.x,&tp_pixad.y))
	{
		l=1;
		tp_pixlcd.x=tp_pixad.x>chx?((u32)tp_pixad.x-(u32)chx)*1000/vx:((u32)chx-(u32)tp_pixad.x)*1000/vx;
		tp_pixlcd.y=tp_pixad.y>chy?((u32)tp_pixad.y-(u32)chy)*1000/vy:((u32)chy-(u32)tp_pixad.y)*1000/vy;
	}
	return l;
}	   
/*
*********************************************************************************************************
*                      Touch_Adjust                    
*
* Description: ������У׼���룬�õ��ĸ�У׼����
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
#define tp_pianyi 50   //У׼����ƫ����	
#define tp_xiaozhun 1000   //У׼����

void Touch_Adjust(void)
{	
	float vx1,vx2,vy1,vy2;  //�������ӣ���ֵ����1000֮���ʾ���ٸ�ADֵ����һ�����ص�
	u16 chx1,chx2,chy1,chy2;//Ĭ�����ص�����Ϊ0ʱ��AD��ʼֵ
	u16 lx,ly;				 
	struct tp_pixu32_ p[4];
	u8  cnt=0;	 
	cnt=0;				
	POINT_COLOR=BLUE;
	BACK_COLOR =WHITE;
	LCD_Clear(WHITE);//����   
	POINT_COLOR=RED;//��ɫ 
	LCD_Clear(WHITE);//���� 
	Drow_Touch_Point(tp_pianyi,tp_pianyi);//����1 
	while(1)
	{
		if(PEN==0)//����������
		{
			if(Read_TP_Once())//�õ����ΰ���ֵ
			{  								   
				p[cnt].x=tp_pixad.x;
				p[cnt].y=tp_pixad.y;
				cnt++; 
			}			 
			switch(cnt)
			{			   
				case 1:
					LCD_Clear(WHITE);//���� 
					while(!PEN)  //�ȴ�����
					{
					}
					Drow_Touch_Point(LCD_W-tp_pianyi-1,tp_pianyi);//����2
					break;
				case 2:
					LCD_Clear(WHITE);//���� 
					while(!PEN)  //�ȴ�����
					{
					}
					Drow_Touch_Point(tp_pianyi,LCD_H-tp_pianyi-1);//����3
					break;
				case 3:
					LCD_Clear(WHITE);//���� 
					while(!PEN)  //�ȴ�����
					{
					}
					Drow_Touch_Point(LCD_W-tp_pianyi-1,LCD_H-tp_pianyi-1);//����4
					break;
				case 4:	 //ȫ���ĸ����Ѿ��õ�
	    		   	LCD_Clear(WHITE);//���� 
				   	while(!PEN)  //�ȴ�����
					{
					}
			   		vx1=p[1].x>p[0].x?(p[1].x-p[0].x+1)*1000/(LCD_W-tp_pianyi-tp_pianyi):(p[0].x-p[1].x-1)*1000/(LCD_W-tp_pianyi-tp_pianyi);
				 	chx1=p[1].x>p[0].x?p[0].x-(vx1*tp_pianyi)/1000:p[0].x+(vx1*tp_pianyi)/1000;
				   	vy1=p[2].y>p[0].y?(p[2].y-p[0].y-1)*1000/(LCD_H-tp_pianyi-tp_pianyi):(p[0].y-p[2].y-1)*1000/(LCD_H-tp_pianyi-tp_pianyi);
					chy1=p[2].y>p[0].y?p[0].y-(vy1*tp_pianyi)/1000:p[0].y+(vy1*tp_pianyi)/1000; 
					
					vx2=p[3].x>p[2].x?(p[3].x-p[2].x+1)*1000/(LCD_W-tp_pianyi-tp_pianyi):(p[2].x-p[3].x-1)*1000/(LCD_W-tp_pianyi-tp_pianyi);
					chx2=p[3].x>p[2].x?p[2].x-(vx2*tp_pianyi)/1000:p[2].x+(vx2*tp_pianyi)/1000;
				   	vy2=p[3].y>p[1].y?(p[3].y-p[1].y-1)*1000/(LCD_H-tp_pianyi-tp_pianyi):(p[1].y-p[3].y-1)*1000/(LCD_H-tp_pianyi-tp_pianyi);
					chy2=p[3].y>p[1].y?p[1].y-(vy2*tp_pianyi)/1000:p[1].y+(vy2*tp_pianyi)/1000; 


					if((vx1>vx2&&vx1>vx2+tp_xiaozhun)||(vx1<vx2&&vx1<vx2-tp_xiaozhun)||(vy1>vy2&&vy1>vy2+tp_xiaozhun)||(vy1<vy2&&vy1<vy2-tp_xiaozhun))
					{
						cnt=0;
						LCD_Clear(WHITE);//���� 
						Drow_Touch_Point(tp_pianyi,tp_pianyi);//����1 
						continue;
					}
					vx=(vx1+vx2)/2;vy=(vy1+vy2)/2;
					chx=(chx1+chx2)/2;chy=(chy1+chy2)/2;	
																
					//��ʾУ׼��Ϣ
					LCD_Clear(WHITE);//���� 
					POINT_COLOR=BLACK;
					BACK_COLOR=BLUE;	
			
					lx=0;ly=50;			
					LCD_ShowString(lx,ly,"VX1:");lx+=40;LCD_ShowNum(lx,ly,vx1,5);					
					lx=0;ly+=20;
					LCD_ShowString(lx,ly,"Vy1:");lx+=40;LCD_ShowNum(lx,ly,vy1,5);					
					lx=0;ly+=20; 
					LCD_ShowString(lx,ly,"CHX1:");lx+=40;LCD_ShowNum(lx,ly,chx1,5);					
				    lx=0;ly+=20; 
					LCD_ShowString(lx,ly,"CHY1:");lx+=40;LCD_ShowNum(lx,ly,chy1,5);

					lx=100;ly=50;			
					LCD_ShowString(lx,ly,"VX2:");lx+=40;LCD_ShowNum(lx,ly,vx2,5);					
					lx=100;ly+=20;
					LCD_ShowString(lx,ly,"Vy2:");lx+=40;LCD_ShowNum(lx,ly,vy2,5);					
					lx=100;ly+=20; 
					LCD_ShowString(lx,ly,"CHX2:");lx+=40;LCD_ShowNum(lx,ly,chx2,5);					
				    lx=100;ly+=20; 
					LCD_ShowString(lx,ly,"CHY2:");lx+=40;LCD_ShowNum(lx,ly,chy2,5);
				
					lx=50;ly=150;			
					LCD_ShowString(lx,ly,"VX:");lx+=40;LCD_ShowNum(lx,ly,vx,5);					
					lx=50;ly+=20;
					LCD_ShowString(lx,ly,"Vy:");lx+=40;LCD_ShowNum(lx,ly,vy,5);					
					lx=50;ly+=20; 
					LCD_ShowString(lx,ly,"CHX:");lx+=40;LCD_ShowNum(lx,ly,chx,5);					
				    lx=50;ly+=20; 
					LCD_ShowString(lx,ly,"CHY:");lx+=40;LCD_ShowNum(lx,ly,chy,5);

					lx=30;ly+=30;
					LCD_ShowString(lx,ly,"Adjust OK!  Touch Anywhere To Continue");										  
					Read_TP_Once(); //�ȴ�����������

					LCD_Clear(WHITE);//����
					return;//У�����				 
			}
		}
	} 
}

/*
*********************************************************************************************************
*                      Point                    
*
* Description: ��ͼ����
*             
* Arguments  : None.
*
* Reutrn     : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
#define RETURN tp_pixad.x < 1600 && tp_pixad.y > 3300
#define	CONFIRM tp_pixad.x > 3000 && tp_pixad.y > 3300

void Point(void) 
{
	double static s=0,t=0;
	extern u8 ref;
	extern const unsigned char *gImage_Knife;
    while(1)
	{  	
		if(PEN==0)
		{
			s=0;t=0;
			if(Convert_Pos())	//�õ�����ֵ
			{
			//	LCD_ShowString(10,250,"X:");LCD_ShowNum(30,250,(u32)tp_pixad.x,6);	
				//LCD_ShowString(180,250,"Y:");LCD_ShowNum(200,250,(u32)tp_pixad.y,6);	
				LCD_ShowString(10,250,"X:");LCD_ShowNum(30,250,tp_pixad.x,4);
				LCD_ShowString(180,250,"Y:");LCD_ShowNum(200,250,tp_pixad.y,4);       //�������
				LCD_ShowSinogram_16(0,288,31);
				LCD_ShowSinogram_16(32,288,32);		      //ȡ��		
				LCD_ShowSinogram_16(176,288,29);
				LCD_ShowSinogram_16(208,288,30);          //ȷ��
				
				LCD_DrawPoint_Big(tp_pixlcd.x,tp_pixlcd.y);   
			 }
			if(RETURN)
			{
				LCD_Clear(WHITE); 
				Main_Menu();
			}
		    if(CONFIRM)
			{				
				LCD_Clear(WHITE); 
				Menu_First();
			}
			 
		}
		else
		{	
			for(s=0;s<65000;s++)
			{
				for(t=0;t<10;t++)
				{
					if(Detect()) //��ⴥ���Ͱ���
					{	
						ref=1;
						LCD_ShowImage(240,320,1,1,gImage_Knife); 
						return;
					}		

				}
			}
			return;
//			t++;		
//			if(t>65000)
//			{
//				s++;
//				return;
//			}
		}		

		}
}	    	
