/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "TEST_TOOL.h"
#include "TEST_TOOL_gpios.h"
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

I2C_HandleTypeDef hi2c2;

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_I2C2_Init(void);
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
  MX_USART3_UART_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */

  uint8_t dataR[BUFFER_SIZE]; //Buffer para almacenar los datos recibidos
  uint8_t charRecibido; //Variable para almacenar el carácter recibido
  int indice = 0; // �?ndice para rastrear la posición actual en el buffer
  uint8_t cmd_str[3];

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  //const char *comandoUsuario = ":testtool;";
  uint8_t comandoValido[] = "El formato del comando es válido \r\n";
  uint8_t comandoIncorrecto[] = "El formato del comando no es válido \r\n";
  uint8_t comandoTestTool[] = "Comando para TestTool \r\n";
  uint8_t comandoDevice[] = "Comando para device \r\n";
  char *error = "Error al enviar el valor al DAC \r\n";
  char *valido = "Valor enviado al DAC con éxito \r\n";

  uint8_t e[] = "Error \r\n";
  uint8_t comandoRelay[] = "Comando para relay \r\n";
  uint8_t comandoDac[] = "Comando para dac \r\n";
  uint8_t comandoCurrent[] = "Comando para current sensor \r\n";

  uint8_t str_end = 0;

  char msj[100];
  char msj1[100];
  char msj2[100];
  char msj3[100];
  char msj4[100];

  while (1)
  {
	  if (HAL_UART_Receive(&huart3, &charRecibido, 1, HAL_MAX_DELAY) == HAL_OK) {
	      // Verifica si el carácter recibido es un Enter (código ASCII 13)
		 // if (charRecibido == '\r' || charRecibido == '\n') {
	      if (charRecibido == ';') {
	          // Si se presionó Enter, transmite los datos recibidos de vuelta a través de UART3
	          HAL_UART_Transmit(&huart3, dataR, indice, HAL_MAX_DELAY);
	          //memset(dataR, 0, BUFFER_SIZE); // Limpia el buffer después de la transmisión
	          indice = 0; 	          // Reinicia el índice del buffer
	          str_end = 1;
	      } else {
	          // Si no es Enter, almacena el carácter en el buffer y aumenta el índice
	          dataR[indice++] = charRecibido;
	          if (indice >= BUFFER_SIZE) { // Verifica si el índice supera el tamaño del buffer
	              // Si el buffer está lleno, transmite los datos y reinicia el buffer y el índice
	              HAL_UART_Transmit(&huart3, dataR, indice, HAL_MAX_DELAY);
	              //memset(dataR, 0, BUFFER_SIZE);
	              indice = 0;
	          }
	      }
	  }

	  if (str_end == 1){
		  str_end = 0;
		  if (formatoValido ((char*)dataR)){
			  HAL_UART_Transmit(&huart3, (uint8_t*) "\r\n", 2, HAL_MAX_DELAY);
			  HAL_UART_Transmit(&huart3, comandoValido, sizeof(comandoValido), HAL_MAX_DELAY);
			  int nArgs = numArgumentos((char*)dataR);
			  snprintf(msj, sizeof(msj), "El comando tiene %i argumentos\r\n", nArgs);
			  HAL_UART_Transmit(&huart3,(uint8_t*)msj,strlen(msj), HAL_MAX_DELAY);
			  struct Comando res = saveArguments((char*)dataR);
			  switch (nArgs){
			  case 1:
				  snprintf(msj1, sizeof(msj1), "El primer argumento es %s \r\n", res.arg1);
				  HAL_UART_Transmit(&huart3,(uint8_t*)msj1,strlen(msj1), HAL_MAX_DELAY);
				  break;
			  case 2:
				  snprintf(msj1, sizeof(msj1), "El primer argumento es %s \r\n", res.arg1);
				  HAL_UART_Transmit(&huart3,(uint8_t*)msj1,strlen(msj1), HAL_MAX_DELAY);
				  snprintf(msj2, sizeof(msj2), "El segundo argumento es %s \r\n", res.arg2);
				  HAL_UART_Transmit(&huart3,(uint8_t*)msj2,strlen(msj2), HAL_MAX_DELAY);
				  break;
			  case 3:
				  snprintf(msj1, sizeof(msj1), "El primer argumento es %s \r\n", res.arg1);
				  HAL_UART_Transmit(&huart3,(uint8_t*)msj1,strlen(msj1), HAL_MAX_DELAY);
				  snprintf(msj2, sizeof(msj2), "El segundo argumento es %s \r\n", res.arg2);
				  HAL_UART_Transmit(&huart3,(uint8_t*)msj2,strlen(msj2), HAL_MAX_DELAY);
				  snprintf(msj3, sizeof(msj3), "El tercer argumento es %s \r\n", res.arg3);
				  HAL_UART_Transmit(&huart3,(uint8_t*)msj3,strlen(msj3), HAL_MAX_DELAY);
				  break;
			  case 4:
				  snprintf(msj1, sizeof(msj1), "El primer argumento es %s \r\n", res.arg1);
				  HAL_UART_Transmit(&huart3,(uint8_t*)msj1,strlen(msj1), HAL_MAX_DELAY);
				  snprintf(msj2, sizeof(msj2), "El segundo argumento es %s \r\n", res.arg2);
				  HAL_UART_Transmit(&huart3,(uint8_t*)msj2,strlen(msj2), HAL_MAX_DELAY);
				  snprintf(msj3, sizeof(msj3), "El tercer argumento es %s \r\n", res.arg3);
				  HAL_UART_Transmit(&huart3,(uint8_t*)msj3,strlen(msj3), HAL_MAX_DELAY);
				  snprintf(msj4, sizeof(msj4), "El cuarto argumento es %s \r\n", res.arg4);
				  HAL_UART_Transmit(&huart3,(uint8_t*)msj4,strlen(msj4), HAL_MAX_DELAY);
				  break;
			  }
			  if (comandoParaTestTool((char*)dataR)){
				  HAL_UART_Transmit(&huart3, comandoTestTool, sizeof(comandoTestTool), HAL_MAX_DELAY);
				  switch (segundoArgumento((char*)dataR)){
					  case 0:
						  HAL_UART_Transmit(&huart3, e, sizeof(e), HAL_MAX_DELAY);
						  break;
					  case 1:
						  HAL_UART_Transmit(&huart3, comandoRelay, sizeof(comandoRelay), HAL_MAX_DELAY);
						  break;
					  case 2:
						  HAL_UART_Transmit(&huart3, comandoDac, sizeof(comandoDac), HAL_MAX_DELAY);
						  if (tercerArgumento((char*)dataR) == 1){
							  float volt = cuartoArgumento((char*) dataR);
							  dac_set_voltage(DAC_CMD_SET_VOLTAGE_VOLATIL, cmd_str, DAC_VREF_VDD, DAC_PD_ON, DAC_GAIN_1, volt);
							  if (HAL_I2C_Master_Transmit(&hi2c2, DAC_ADDR<<1, cmd_str, 3, 1000) != HAL_OK) {
								  HAL_UART_Transmit(&huart3, (uint8_t*) error, strlen(error), 100);
								}
							   HAL_UART_Transmit(&huart3, (uint8_t*) valido, strlen(valido), 100);
						  }
						  break;
					  case 3:
						  HAL_UART_Transmit(&huart3, comandoCurrent, sizeof(comandoCurrent), HAL_MAX_DELAY);
						  break;
				  }
				  memset(dataR, 0, BUFFER_SIZE);
			  } else {
			  HAL_UART_Transmit(&huart3, comandoDevice, sizeof(comandoDevice), HAL_MAX_DELAY);
			  memset(dataR, 0, BUFFER_SIZE);
			  }
		  } else {
			  HAL_UART_Transmit(&huart3, (uint8_t*) "\r\n", 2, HAL_MAX_DELAY);
			  HAL_UART_Transmit(&huart3, comandoIncorrecto, sizeof(comandoIncorrecto), HAL_MAX_DELAY);
			  memset(dataR, 0, BUFFER_SIZE);
		  }
	  }

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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x00303D5B;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : PB14 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
