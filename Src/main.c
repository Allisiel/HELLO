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
#include "stm32l4xx_hal.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */
#include "LCD.h"
#include "bmp.h"
#include "touch.h"
#include "RS485.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;        //硬件SPI结构体初始化
DMA_HandleTypeDef hdma_spi1_tx;
DMA_HandleTypeDef hdma_spi1_rx;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;

osThreadId defaultTaskHandle;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
//static u8  LED_State = 0;

/* USER CODE END PV */

/*/ Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_SPI1_Init(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(int bound);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_NVIC_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

void StartDefaultTask(void const * argument);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
//static u8 Set_Temp;
//extern u8 ref;

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
	/* USER CODE BEGIN 1 */

//	extern u8 LED_State;
	/* USER CODE END 1 */

	/* MCU Configuration----------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();
	/* USER CODE BEGIN SysInit */
	
	HAL_NVIC_SetPriority(SPI1_IRQn,2,2);
	HAL_SPI_MspInit(&hspi1);
	TP_Init();
	Touch_Init()  ;//模拟SPI初始化
	MX_SPI1_Init();//硬件SPI初始化
	LCD_Init();
	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_SPI1_Init();
	MX_USART1_UART_Init(9600);
    MX_TIM1_Init();
    MX_TIM2_Init();
	
	/* Initialize interrupts */
	MX_NVIC_Init();
	/* USER CODE BEGIN 2 */
	BACK_COLOR=GRAYBLUE; POINT_COLOR=WHITE;

//	LCD_Clear(PALE_DENIM);
//	Detect();
//	Main_Menu();

	HAL_TIM_Base_Start_IT(&htim2);     //使能定时器2更新中断：TIM_IT_UPDATE
	HAL_TIM_Base_Start_IT(&htim1);
    LCD_Clear(GRAYBLUE);
	
	LCD_ShowSinogram_32( 65,145,0);
	LCD_ShowSinogram_32(105,145,1);
	LCD_ShowSinogram_32(145,145,2);
	Detect();
	/* USER CODE END 2 */

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* Create the thread(s) */
	/* definition and creation of defaultTask */
	osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
	defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	/* USER CODE END RTOS_THREADS */

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */


	/* Start scheduler */
	osKernelStart();

	/* We should never get here as control is now taken by the scheduler */


	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		
//		LCD_ShutDown(10);
//		LCD_Show();
		if(PEN == 0)
		{
			LCD_ShowImage(240,320,1,1,gImage_Mode_SetTemp);
			Point();
		}

//		BACK_COLOR=SNOW; POINT_COLOR=SILVER;    //充电动画PALE_DENIM
//		LCD_ShowIcon(0,0,1);
//		HAL_Delay(800);
//		LCD_ShowIcon(0,0,2);
//		HAL_Delay(800);
//		LCD_ShowIcon(0,0,3);
//		HAL_Delay(800);
//		LCD_ShowIcon(0,0,4);
//		HAL_Delay(800);
//		LCD_ShowIcon(0,0,5);
//		HAL_Delay(800);
		

		/* USER CODE END WHILE */


		/* USER CODE BEGIN 3 */

	}
		/* USER CODE END 3 */

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	/* Prevent unused argument(s) compilation warning */
//	UNUSED(htim);

	/* NOTE : This function should not be modified, when the callback is needed,
			the HAL_TIM_PeriodElapsedCallback could be implemented in the user file
	*/
//	static u32 LED_Add = 0;
	
	if(htim == (&htim2))              //定时器1 更新中断 5ms一次
	{
		
		
//		LCD_ShutDown(10);
//		LCD_Show();
//		LED_Add++;
//		if(LED_Add == 200)     //1秒钟
//		{
////			LED_Add =0;
////			LED_State = 1;
////			LCD_Blink();         //呼吸灯	
////			HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_3);
////			LCD_ShowImage(240,320,1,1,gImage_Mode_SetTemp);			
//		}
	}
	
	if(htim == (&htim1))
	{
			

	}

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

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* DMA1_Channel2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
  /* DMA1_Channel3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
  /* TIM2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM2_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

/* SPI1 init function */
static void MX_SPI1_Init(void)
{

	/* SPI1 parameter configuration*/
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
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

/* TIM1 init function */   //80000000/400 = 200000   5微秒一次
static void MX_TIM1_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 399;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_ENABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter = 0;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim1);

}

/* TIM2 init function */  //  5ms一次中断
static void MX_TIM2_Init(void)    
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART1 init function */
static void MX_USART1_UART_Init(int bound)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = bound;
  huart1.Init.WordLength = UART_WORDLENGTH_7B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
  /* DMA1_Channel3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);

}

/** Pinout Configuration
*/
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

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

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */ 
}

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
