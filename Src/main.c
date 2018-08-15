  /*
  *******************************************************************************************************
  * File Name: main.c
  * Author: JC.Yang
  * Version: 1.0.0
  * Date: 2018.7.31
  * Brief: ���ļ��ṩ��STM32L432ͨ��ILI9341����ʹTFTLCD��Ļ����ʾ��ϵ�й���
  *******************************************************************************************************
  * History
  *		  1.Author: JC.Yang 
  *			Date: 2018.8.1
  *			Mod: �˽�STM32L432��TFTLCDӲ����Դ��Ϣ������໥֮���Ӳ�����ӣ�����ʼ��δ�ɹ�
  *
  *		  2.Author: JC.Yang
  *			Date: 2018.8.2
  *			Mod: �ҳ�bug���ֶ�����׼����ֲΪSTM32L432���õ�HAL�⣬�������Ļ��ʼ������������ʾ�����ֿ���֣�������ˢ�½���������
  *
  *		  3.Author: JC.Yang 
  *			Date: 2018.8.3
  *			Mod: �淶������������ʵ����ʾQQ���ͼ��
  *   	      	 �ҵ�ˢ�½������������ڣ��ڲ�����Ƶ��̫�ͣ�����Ӿ���Ŀǰ�޷������
  *
  *		  4.Author: JC.Yang 
  *			Date: 2018.8.6
  *			Mod: ʵ���Զ��庺��32*32��С����ʾ���޷����������С
  *   			 ʵ���Զ���ͼƬ40*40��С����ʾ�����ͼƬ��С����
  *
  *		  5.Author: JC.Yang 
  *			Date: 2018.8.7
  *			Mod: �ӿ���ˢ����Ļ�ٶȣ�һ����ͼƬˢ����Ҫ����һ�룬��������졣���Կ��Ǻ��ڼ��ⲿ���پ���
  *
  * 	  6.Author: JC.Yang 
  *			Date: 2018.8.8
  *			Mod: ��ʼ������Ļ�������Ĵ���
  *   			 
  *    
  *******************************************************************************************************
  */	
	
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "LCD.h"
#include "bmp.h"
#include "touch.h"
#include "RS485.h"
#include "stm32l4xx_hal.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;        //Ӳ��SPI�ṹ���ʼ��
DMA_HandleTypeDef hdma_spi1_tx;
DMA_HandleTypeDef hdma_spi1_rx;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/*/ Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_SPI1_Init(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(int bound);

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
int main(void)
{
	/* USER CODE BEGIN 1 */
	extern u8 ref;
	/* USER CODE END 1 */

	/* MCU Configuration----------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */
		//��ʼ��LCD 
	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();
	/* USER CODE BEGIN SysInit */
	HAL_NVIC_SetPriority(SPI1_IRQn,2,2);
	HAL_SPI_MspInit(&hspi1);
	TP_Init();
	Touch_Init()  ;//ģ��SPI��ʼ��
	MX_SPI1_Init();//Ӳ��SPI��ʼ��
	LCD_Init();	
	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_SPI1_Init();
	MX_USART1_UART_Init(9600);

	/* USER CODE BEGIN 2 */
	BACK_COLOR=BLACK;;POINT_COLOR=WHITE; 
    LCD_ShowImage(240,320,1,1,gImage_Knife); 
	Detect();
	Main_Menu();
	Detect();
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
//		Detect();
		LCD_ShutDown(10);
		LCD_Show();
		/* USER CODE END WHILE */
//		if(Detect()) //��ⴥ���Ͱ���
//		{	
//		LCD_ShowImage(240,320,1,1,gImage_Knife); 
//		}
//		if(ref) LCD_ShowImage(240,320,1,1,gImage_Knife); 

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
