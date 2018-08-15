//#include "RS485.h"
//#include "stm32l4xx_hal_gpio.h"


//#ifdef EN_USART2_RX       //如果使能了接收


////接收缓冲区
//u8 RS485_RX_BUF[64];
////接收到的数据长度
//u8 RS485_RX_CNT=0;

//void USART2_IRQHandler(void)
//{
//	u8 res;
//	
////	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET) //接收数据
////	{
////		res = USART_ReceiveData(USART2);       //读取接收到的数据
////		if(RS485_RX_CNT<64)
////		{
////			RS485_RX_BUF[RS485_RX_CNT]=res;    //记录接收到的值 
////			RS485_RX_CNT++;					   //接收数据增加1
////		}
////	}
//}

//#endif

////初始化IO 串口2
////pclk1：PCLK1时钟频率（MHz）
////bound：波特率
//void RS485_Init(u32 bound)
//{
//	GPIO_InitTypeDef GPIO_InitStruct;
//	__HAL_RCC_GPIOA_CLK_ENABLE();     //使能GPIOA时钟
//	__HAL_RCC_USART1_CLK_ENABLE();    //使能UASRT1时钟
//	
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  //推挽输出
//	GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//	HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);
//	
//	GPIO_InitStruct.Pin = GPIO_PIN_2;	//PA2
//	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;	//复用推挽
//	HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);
//	
////	GPIO_InitStruct.Pin = GPIO_PIN_3;//PA3
////	GPIO_InitStruct.Alternate = 
////	GPIO_InitStruct.Mode = GPIO_Mode_IN_FLOATING; //浮空输入
////	HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);

//	

//}


//#ifdef EN_USART2_RX		  	//如果使能了接收
//	USART_InitStruct.USART_BaudRate = bound;//波特率设置
//	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//8位数据长度
//	USART_InitStruct.USART_StopBits = USART_StopBits_1;//一个停止位
//	USART_InitStruct.USART_Parity = USART_Parity_No;///奇偶校验位
//	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
//	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式

//	USART_Init(USART1, &USART_InitStructure); ; //初始化串口

//	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn; //使能串口2中断
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3; //先占优先级2级
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3; //从优先级2级
//	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
//	NVIC_Init(&NVIC_InitStruct); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断

//	USART_Cmd(USART2, ENABLE);                    //使能串口 

//#endif

//  RS485_TX_EN=0;			//默认为接收模式
// 
//}

////RS485发送len个字节.
////buf:发送区首地址
////len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
//void RS485_Send_Data(u8 *buf,u8 len)
//{
//	u8 t;
//	RS485_TX_EN=1;			//设置为发送模式
//  	for(t=0;t<len;t++)		//循环发送数据
//	{		   
//		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);	  
//		USART_SendData(USART2,buf[t]);
//	}	 
// 
//	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);		
//	RS485_RX_CNT=0;	  
//	RS485_TX_EN=0;				//设置为接收模式	
//}
////RS485查询接收到的数据
////buf:接收缓存首地址
////len:读到的数据长度
//void RS485_Receive_Data(u8 *buf,u8 *len)
//{
//	u8 rxlen=RS485_RX_CNT;
//	u8 i=0;
//	*len=0;				//默认为0
//	delay_ms(10);		//等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束
//	if(rxlen==RS485_RX_CNT&&rxlen)//接收到了数据,且接收完成了
//	{
//		for(i=0;i<rxlen;i++)
//		{
//			buf[i]=RS485_RX_BUF[i];	
//		}		
//		*len=RS485_RX_CNT;	//记录本次数据长度
//		RS485_RX_CNT=0;		//清零
//	}
//}








