/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "i2c.h"
#include "gpio.h"
#include "lcd.h"
#include "lcd_gpio.h"
#include "lcd_i2c.h"
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
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  int a = 0;


  	  	  	  	  	  	  	  	  /* CREATING LCD HANDLE THROUGH GPIO */

	LCD_GPIO_t lcd_gpio;

  	GPIO_TypeDef * data_ports[] = { LCD_D0_GPIO_Port, LCD_D1_GPIO_Port, LCD_D2_GPIO_Port, LCD_D3_GPIO_Port,
   								    LCD_D4_GPIO_Port, LCD_D5_GPIO_Port, LCD_D6_GPIO_Port, LCD_D7_GPIO_Port };

//  GPIO_TypeDef * data_ports[] = {LCD_D4_GPIO_Port, LCD_D5_GPIO_Port, LCD_D6_GPIO_Port, LCD_D7_GPIO_Port}; /* For 4-bits operations */

  	LCD_GPIO_Ports_t ports = { LCD_RS_GPIO_Port, LCD_RW_GPIO_Port, LCD_EN_GPIO_Port}; // Filling the port structure handle

    memcpy(ports.data_ports, data_ports, sizeof data_ports); // Concatenating data ports with RS, RW and EN ports

  	uint16_t data_pins[] = { LCD_D0_Pin, LCD_D1_Pin, LCD_D2_Pin, LCD_D3_Pin,
  						     LCD_D4_Pin, LCD_D5_Pin, LCD_D6_Pin, LCD_D7_Pin };

//  uint16_t data_pins[] = {LCD_D4_Pin, LCD_D5_Pin, LCD_D6_Pin, LCD_D7_Pin}; /* For 4-bits operations */

  	LCD_GPIO_Pins_t pins = { LCD_RS_Pin, LCD_RW_Pin, LCD_EN_Pin}; // Filling the pin structure handle

    memcpy(pins.data_pins, data_pins, sizeof data_pins); // Concatenating data pins with RS, RW and EN pins

	LCD_GPIO_Constructor(&lcd_gpio, ports, pins);

	LCD_PrintString(&lcd_gpio.super,"HELLO WORLD");
	LCD_GoTo(&lcd_gpio.super, 1, 10);
	LCD_PrintNumber(&lcd_gpio.super, 1234, 10);
	LCD_GoTo(&lcd_gpio.super, 0, 8);
	LCD_PrintString(&lcd_gpio.super, "KAKAb");
	LCD_GoTo(&lcd_gpio.super, 1, 1);
	LCD_PrintString(&lcd_gpio.super, "Yilmazza");



    							/* CREATING LCD HANDLE THROUGH I2C BUS */


	LCD_I2C_t lcd_i2c;

	LCD_I2C_Constructor(&lcd_i2c, &hi2c1, SLAVE_ADDR);

	LCD_PrintString(&lcd_i2c.super, "HELLO WORLD");
	LCD_GoTo(&lcd_i2c.super, 1, 5);
	LCD_PrintNumber(&lcd_i2c.super, 44453356, 10);

	HAL_Delay(1000);
	LCD_Clear(&lcd_gpio.super);
	HAL_Delay(1000);
	LCD_Clear(&lcd_i2c.super);
	HAL_Delay(1000);

	LCD_ReturnHome(&lcd_gpio.super);
	LCD_ReturnHome(&lcd_i2c.super);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	LCD_PrintNumber(&lcd_gpio.super, a, 10);
	LCD_PrintNumber(&lcd_i2c.super, a, 10);

	HAL_Delay(1000);

	a++;
	if( a > 100) a = 0;
	LCD_ReturnHome(&lcd_gpio.super);
	LCD_ReturnHome(&lcd_i2c.super);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
