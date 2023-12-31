/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
RC_Ctl_t RC_Ctl;   							//声明遥控器数据结构体
uint8_t sbus_rx_buffer[25]; 		        //声明遥控器缓存数组

int TongDao0 = 0;               //声明一个变量
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM8_Init();
  MX_TIM1_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_UART_Receive_DMA(&huart3,sbus_rx_buffer,25);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		TongDao0 = RC_Ctl.rc.ch0;
		HAL_Delay(10);
    /* USER CODE END WHILE */

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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 6;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle) 
{  
			if(sbus_rx_buffer[0]==0x0F)
		{   
			HAL_NVIC_DisableIRQ(USART3_IRQn);
			
			RC_Ctl.rc.ch0 = ((sbus_rx_buffer[1] | (sbus_rx_buffer[2] << 8))                                 & 0x07FF);
            RC_Ctl.rc.ch1 = ((sbus_rx_buffer[2] >> 3 | (sbus_rx_buffer[3] << 5 ))                            & 0x07FF);
            RC_Ctl.rc.ch2 = ((sbus_rx_buffer[3] >> 6 | (sbus_rx_buffer[4] << 2 ) | sbus_rx_buffer[5] << 10 ) & 0x07FF);
            RC_Ctl.rc.ch3 = ((sbus_rx_buffer[5] >> 1 | (sbus_rx_buffer[6] << 7 ))                            & 0x07FF);		
			RC_Ctl.rc.ch4 =((sbus_rx_buffer[6]>>4 |sbus_rx_buffer[7]<<4)                                     & 0x07FF);
			RC_Ctl.rc.ch5 =((sbus_rx_buffer[7]>>7 |sbus_rx_buffer[8]<<1 |sbus_rx_buffer[9]<<9)               & 0x07FF);
//			RC_Ctl.rc.ch6 =((sbus_rx_buffer[9]>>2 |sbus_rx_buffer[10]<<6)                                    & 0x07FF);
//			RC_Ctl.rc.ch7 =((sbus_rx_buffer[10]>>5|sbus_rx_buffer[11]<<3)                                    & 0x07FF);
//			RC_Ctl.rc.ch8 =((sbus_rx_buffer[12] |sbus_rx_buffer[13]<<8)                                      & 0x07FF);
//			RC_Ctl.rc.ch9 =((sbus_rx_buffer[13]>>3|sbus_rx_buffer[14]<<5)                                    & 0x07FF);
//			RC_Ctl.rc.ch10=((sbus_rx_buffer[14]>>6|sbus_rx_buffer[15]<<2|sbus_rx_buffer[16]<<10)             & 0x07FF);
//			RC_Ctl.rc.ch11=((sbus_rx_buffer[16]>>1|sbus_rx_buffer[17]<<7)                                    & 0x07FF);
//			RC_Ctl.rc.ch12=((sbus_rx_buffer[17]>>4|sbus_rx_buffer[18]<<4)                                    & 0x07FF);
//			RC_Ctl.rc.ch13=((sbus_rx_buffer[18]>>7|sbus_rx_buffer[19]<<1|sbus_rx_buffer[20]<<9)              & 0x07FF);
//			RC_Ctl.rc.ch14=((sbus_rx_buffer[20]>>2|sbus_rx_buffer[21]<<6)		                             & 0x07FF);
//			RC_Ctl.rc.ch15=((sbus_rx_buffer[21]>>5|sbus_rx_buffer[22]<<3)                                    & 0x07FF);
			while(!(sbus_rx_buffer[24]==0x00));
			HAL_NVIC_EnableIRQ(USART3_IRQn);
			
			
			
			
			
//             RC_Ctl.rc.s1 = (sbus_rx_buffer[6] >> 4 | (sbus_rx_buffer[7] << 4 )) & 0x07FF;
//             RC_Ctl.rc.s2 = (sbus_rx_buffer[7] >> 7 | (sbus_rx_buffer[8] << 1 ) | sbus_rx_buffer[9] << 9 ) & 0x07FF;
//            RC_Ctl.mouse.x = (sbus_rx_buffer[9] >> 2 | (sbus_rx_buffer[10] << 6 )) & 0x07FF;
//             RC_Ctl.mouse.y= (sbus_rx_buffer[10] >> 5 | (sbus_rx_buffer[11] << 3 )) & 0x07FF;
//            RC_Ctl.mouse.z = (sbus_rx_buffer[12] << 0 | (sbus_rx_buffer[13] << 8 )) & 0x07FF;
//              RC_Ctl.key.v = (sbus_rx_buffer[13] >> 3 | (sbus_rx_buffer[14] << 5 )) & 0x07FF;




			
//			    RC_Ctl.rc.ch0 = (sbus_rx_buffer[1]| (sbus_rx_buffer[2] << 8)) & 0x07ff;          
//				RC_Ctl.rc.ch1 = ((sbus_rx_buffer[2] >> 6) | (sbus_rx_buffer[3] << 5)) & 0x07ff;       
//				RC_Ctl.rc.ch2 = ((sbus_rx_buffer[3] >> 1) | (sbus_rx_buffer[4] << 2) | (sbus_rx_buffer[5] << 10)) & 0x07ff;          
//				RC_Ctl.rc.ch3 = ((sbus_rx_buffer[5] >> 4) | (sbus_rx_buffer[6] << 7)) & 0x07ff;           
//				RC_Ctl.rc.s1  = ((sbus_rx_buffer[6] >> 2)& 0x000C) >> 2;                           
//				RC_Ctl.rc.s2  = ((sbus_rx_buffer[6] >> 2)& 0x0003);    

//				RC_Ctl.mouse.x = sbus_rx_buffer[7] | (sbus_rx_buffer[8] << 8);                    //!< Mouse X axis        
//				RC_Ctl.mouse.y = sbus_rx_buffer[9] | (sbus_rx_buffer[10] << 8);                    //!< Mouse Y axis      
//				RC_Ctl.mouse.z = sbus_rx_buffer[11] | (sbus_rx_buffer[12] << 8);                  //!< Mouse Z axis         
//				RC_Ctl.mouse.press_l = sbus_rx_buffer[13];                                        //!< Mouse Left Is Press      
//				RC_Ctl.mouse.press_r = sbus_rx_buffer[14];                                        //!< Mouse Right Is Press 
//				RC_Ctl.key.v = sbus_rx_buffer[15] | (sbus_rx_buffer[16] << 8);   			
        }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
