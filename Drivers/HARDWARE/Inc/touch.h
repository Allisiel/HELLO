//////////////////////////////////////////////////////////////////////////////////	 
//
//  �� �� ��   : touch.c
//  �� �� ��   : v1.0
//  ��    ��   : JC.Yang
//  ��������   : 2018.7.31
//  ����޸�   : 
//  ��������   : TFT_LCD �ӿ���ʾ����(stm32l432ϵ��)
//              ˵��: 
//              -�˲���Ϊ���������ӿڣ�BLKΪ���⿪�أ�Ĭ�Ͽ��Բ���---------------------------------------------------------------
//              GND    ��Դ��
//              VCC    3.3v��Դ
//              CLK    ��PA5
//              MOSI   ��PA7
//              RES    ��PA2
//              DC     ��PA4
//              CS1    ��PA0               
//              -����Ϊ�������ƽӿڣ�������ó�û���ܿ��Բ���---------------------------------------------------------------
//              MISO   ��PA6
//              CS2    ��PA1
//              PEN    ��PA3
// �޸���ʷ   : ���STM32L432�޸Ķ�Ӧ�����������Ŷ���
// ��    ��   : 2018.8.2


//
// �޸���ʷ   : ��Դ��������⿪�ص����Ŷ���  
//             PB3����������������
//             PA8����������Ч������
// ��    ��   : 2018.8.20
//******************************************************************************/

#ifndef __TOUCH_H
#define __TOUCH_H	

/* touch panel interface define */
#define  CMD_RDX  0xD0	 //����IC�����������
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


/*    �¶ȵ��ڽ��津������    */
#define RETURN    tp_pixad.x < 1000 && tp_pixad.y < 800
#define CONFIRM   tp_pixad.x > 2500 && tp_pixad.y < 700
#define	SET_UP    tp_pixad.x > 1300 && tp_pixad.x < 2400 && tp_pixad.y > 700 && tp_pixad.y <1400
#define SET_DOWN  tp_pixad.x > 1300 && tp_pixad.x < 2400 && tp_pixad.y > 2300 && tp_pixad.y <2900
#define TEMP      tp_pixad.x > 400 && tp_pixad.x < 1500 && tp_pixad.y > 3000 && tp_pixad.y <3500
#define MODE      tp_pixad.x > 2500 && tp_pixad.x < 3500 && tp_pixad.y > 3000 && tp_pixad.y <3500
	 
/*    ģʽ���ڽ��津������    */
//#define 
//#define 





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
extern struct tp_pix_  tp_pixad,tp_pixlcd;	 //��ǰ���������ADֵ,ǰ�������������ֵ   
extern u16 vx,vy;  //�������ӣ���ֵ����1000֮���ʾ���ٸ�ADֵ����һ�����ص�
extern u16 chx,chy;//Ĭ�����ص�����Ϊ0ʱ��AD��ʼֵ
u8 tpstate(void);
void Touch_Init(void);  
void Touch_Adjust(void);
void Point(void); //��ͼ����
u16 ReadFromCharFrom7843(void);         //SPI ������
u8 TP_Init(void);
void SET_MODE(void);

#endif  
	 
	 



