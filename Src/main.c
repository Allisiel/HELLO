  /*
  *******************************************************************************************************
  * File Name: main.c
  * Author: JC.Yang
  * Version: 1.0.0
  * Date: 2018.7.31
  * Brief: 本文件提供了STM32L432通过ILI9341驱动使TFTLCD屏幕的显示的系列功能
  *******************************************************************************************************
  * History
  *		  1.Author: JC.Yang 
  *			Date: 2018.8.1
  *			Mod: 了解STM32L432和TFTLCD硬件资源信息，完成相互之间的硬件连接，但初始化未成功
  *
  *		  2.Author: JC.Yang
  *			Date: 2018.8.2
  *			Mod: 找出bug，手动将标准库移植为STM32L432可用的HAL库，并完成屏幕初始化，能正常显示已有字库的字，但存在刷新较慢的问题
  *
  *		  3.Author: JC.Yang 
  *			Date: 2018.8.3
  *			Mod: 规范化代码命名，实现显示QQ企鹅图案
  *   	      	 找到刷新较慢的问题所在：内部晶振频率太低，需外接晶振（目前无法解决）
  *
  *		  4.Author: JC.Yang 
  *			Date: 2018.8.6
  *			Mod: 实现自定义汉字32*32大小的显示，无法设置字体大小
  *   			 实现自定义图片40*40大小的显示，完成图片大小设置
  *
  *		  5.Author: JC.Yang 
  *			Date: 2018.8.7
  *			Mod: 加快了刷新屏幕速度，一整幅图片刷新需要将近一秒，但仍需更快。可以考虑后期加外部高速晶振
  *
  * 	  6.Author: JC.Yang 
  *			Date: 2018.8.8
  *			Mod: 开始着手屏幕触摸屏的代码
  *   			 
  *    
  *******************************************************************************************************
  */	
	
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "oled.h"
#include "bmp.h"
#include "touch.h"
#include "stm32l4xx_hal_spi.h"
#include "stm32l4xx_hal.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/*/ Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_SPI1_Init(void);


/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */

void Display()//显示信息
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
//	Display(); //显示信息
	for(k=0;k<x;k++)									//行
	{
	   	for(j=0;j<y;j++)								//列
		{	
			Address_Set(m*j,n*j,m*j+m-1,n*j+n-1);
		    for(i=0;i<m*n;i++)
			 { 	
//				 if(Detect()) //检测触摸和按键
//				 {	
//				 	ref=1;
//				  	return;	
//				 }		
				picL=*(p+i*2);	//数据低位在前
				picH=*(p+i*2+1);				
				LCD_WR_DATA8(picH);  						
				LCD_WR_DATA8(picL);			
			 }	
		 }
	}
	ref=0;				
}
 
int main(void)
{
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration----------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */
	
	HAL_NVIC_SetPriority(SPI1_IRQn,2,2);
	TP_Init();
	SPI_Init()  ;//模拟SPI初始化
	MX_SPI1_Init();
	LCD_Init();			//初始化OLED 
	/* USER CODE END Init */
	
//	LCD_Clear(WHITE); //清屏

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */
	BACK_COLOR=BLACK;;POINT_COLOR=WHITE; 
    LCD_ShowImage(240,320,1,1,gImage_Knife); 
//	Detect();

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */


	/* USER CODE BEGIN 2 */
	 Detect();

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
	Display();
	Detect();
	
	/* USER CODE END WHILE */
	if(Detect()) //检测触摸和按键
	{	
    LCD_ShowImage(240,320,1,1,gImage_Knife); 
	}
    if(ref) LCD_ShowImage(240,320,1,1,gImage_Knife); 

	/* USER CODE BEGIN 3 */

  }
    /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 2;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the main internal regulator output voltage 
    */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

static void MX_SPI1_Init(void)
{

  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
