//#include "RS485.h"
//#include "stm32l4xx_hal_gpio.h"


//#ifdef EN_USART2_RX       //���ʹ���˽���


////���ջ�����
//u8 RS485_RX_BUF[64];
////���յ������ݳ���
//u8 RS485_RX_CNT=0;

//void USART2_IRQHandler(void)
//{
//	u8 res;
//	
////	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET) //��������
////	{
////		res = USART_ReceiveData(USART2);       //��ȡ���յ�������
////		if(RS485_RX_CNT<64)
////		{
////			RS485_RX_BUF[RS485_RX_CNT]=res;    //��¼���յ���ֵ 
////			RS485_RX_CNT++;					   //������������1
////		}
////	}
//}

//#endif

////��ʼ��IO ����2
////pclk1��PCLK1ʱ��Ƶ�ʣ�MHz��
////bound��������
//void RS485_Init(u32 bound)
//{
//	GPIO_InitTypeDef GPIO_InitStruct;
//	__HAL_RCC_GPIOA_CLK_ENABLE();     //ʹ��GPIOAʱ��
//	__HAL_RCC_USART1_CLK_ENABLE();    //ʹ��UASRT1ʱ��
//	
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  //�������
//	GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//	HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);
//	
//	GPIO_InitStruct.Pin = GPIO_PIN_2;	//PA2
//	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;	//��������
//	HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);
//	
////	GPIO_InitStruct.Pin = GPIO_PIN_3;//PA3
////	GPIO_InitStruct.Alternate = 
////	GPIO_InitStruct.Mode = GPIO_Mode_IN_FLOATING; //��������
////	HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);

//	

//}


//#ifdef EN_USART2_RX		  	//���ʹ���˽���
//	USART_InitStruct.USART_BaudRate = bound;//����������
//	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//8λ���ݳ���
//	USART_InitStruct.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
//	USART_InitStruct.USART_Parity = USART_Parity_No;///��żУ��λ
//	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
//	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ

//	USART_Init(USART1, &USART_InitStructure); ; //��ʼ������

//	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn; //ʹ�ܴ���2�ж�
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�2��
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3; //�����ȼ�2��
//	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
//	NVIC_Init(&NVIC_InitStruct); //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�

//	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 

//#endif

//  RS485_TX_EN=0;			//Ĭ��Ϊ����ģʽ
// 
//}

////RS485����len���ֽ�.
////buf:�������׵�ַ
////len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
//void RS485_Send_Data(u8 *buf,u8 len)
//{
//	u8 t;
//	RS485_TX_EN=1;			//����Ϊ����ģʽ
//  	for(t=0;t<len;t++)		//ѭ����������
//	{		   
//		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);	  
//		USART_SendData(USART2,buf[t]);
//	}	 
// 
//	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);		
//	RS485_RX_CNT=0;	  
//	RS485_TX_EN=0;				//����Ϊ����ģʽ	
//}
////RS485��ѯ���յ�������
////buf:���ջ����׵�ַ
////len:���������ݳ���
//void RS485_Receive_Data(u8 *buf,u8 *len)
//{
//	u8 rxlen=RS485_RX_CNT;
//	u8 i=0;
//	*len=0;				//Ĭ��Ϊ0
//	delay_ms(10);		//�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս���
//	if(rxlen==RS485_RX_CNT&&rxlen)//���յ�������,�ҽ��������
//	{
//		for(i=0;i<rxlen;i++)
//		{
//			buf[i]=RS485_RX_BUF[i];	
//		}		
//		*len=RS485_RX_CNT;	//��¼�������ݳ���
//		RS485_RX_CNT=0;		//����
//	}
//}








