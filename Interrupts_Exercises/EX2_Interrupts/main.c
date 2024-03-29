/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum
{
  BUTTON_PULL_DOWN = 0,
  BUTTON_PULL_UP
}BUTTON_PUPD;
typedef enum
{
  SOLTO = 0,
  AO_PRESSIONAR,
  PRESSIONADO,
  AO_SOLTAR
}ESTADO_BOTAO;
typedef enum
{
	DESLIGADO=0,
	VERMELHO,
	AZUL,
	VERDE,
	VERMELHO_VERDE,
	VERDE_AZUL,
	AZUL_VERMELHO,
	COLORIDO
}ESTADO_RGB;
typedef struct {
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
	BUTTON_PUPD pull;
	uint8_t atual;
	uint8_t anterior;
	ESTADO_BOTAO estado;
} BUTTON_CONTROLLER;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
ESTADO_RGB leds = DESLIGADO;
BUTTON_CONTROLLER controller1;

#define TCMD 1
uint8_t comando;
char comando_letra = '\0';
uint8_t comando_numero = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
BUTTON_CONTROLLER start_controller(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BUTTON_PUPD pull);
void gerenciador_botao_struct(BUTTON_CONTROLLER * controller);
void zeraLeds();
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
	controller1 = start_controller(Button1_GPIO_Port, Button1_Pin, BUTTON_PULL_UP);
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
  MX_USART2_UART_Init();
  MX_TIM10_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim10);
  HAL_UART_Receive_IT(&huart2, &comando, TCMD);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

BUTTON_CONTROLLER start_controller(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BUTTON_PUPD pull){
	BUTTON_CONTROLLER controller;
	controller.GPIO_Pin = GPIO_Pin;
	controller.GPIOx = GPIOx;
	controller.pull = pull;
	controller.atual = 0;
	controller.anterior = 0;
	controller.estado = SOLTO;
	return controller;
}

void gerenciador_botao_struct(BUTTON_CONTROLLER * controller) {
	void gerenciador_botao(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BUTTON_PUPD pull, uint8_t * atual, uint8_t * anterior, ESTADO_BOTAO * estado);
	gerenciador_botao(controller->GPIOx, controller->GPIO_Pin, controller->pull, &(controller->atual), &(controller->anterior), &(controller->estado));
}

void gerenciador_botao(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BUTTON_PUPD pull, uint8_t * atual, uint8_t * anterior, ESTADO_BOTAO * estado) {
	ESTADO_BOTAO estado_botao(uint8_t atual, uint8_t * anterior);
	if ((*atual == 0) && (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) != pull)){
		for (int i = 0; i < 80000; i++);
		if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) != pull){
			*atual = 1;
		}
	}
	if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == pull) {
		*atual = 0;
	}
	*estado = estado_botao(*atual, anterior);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	gerenciador_botao_struct(&controller1);
	if (controller1.estado == AO_PRESSIONAR){
		zeraLeds();
		if (++leds > COLORIDO) {
			leds = DESLIGADO;
		}
	}
}


ESTADO_BOTAO estado_botao(uint8_t atual, uint8_t * anterior) {
	if ((*anterior == 0) && (atual == 1)) {
		*anterior = atual;
		return AO_PRESSIONAR;
	} else if ((*anterior == 1) && (atual == 1)) {
		*anterior = atual;
		return PRESSIONADO;
	} else if ((*anterior == 1) && (atual == 0)) {
		*anterior = atual;
		return AO_SOLTAR;
	}
	*anterior = atual;
	return SOLTO;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	void estadoPiscaLEDs();
	__HAL_TIM_CLEAR_IT(&htim10,TIM_IT_UPDATE);
	estadoPiscaLEDs();
}

void estadoPiscaLEDs() {
	void zeraLeds();
	switch(leds) {
	case VERMELHO:
		HAL_GPIO_TogglePin(LED_VERMELHO_GPIO_Port, LED_VERMELHO_Pin);
		HAL_GPIO_WritePin(LED_AZUL_GPIO_Port, LED_AZUL_Pin, 0);
		HAL_GPIO_WritePin(LED_VERDE_GPIO_Port, LED_VERDE_Pin, 0);
		break;
	case AZUL:
		HAL_GPIO_WritePin(LED_VERMELHO_GPIO_Port, LED_VERMELHO_Pin, 0);
		HAL_GPIO_TogglePin(LED_AZUL_GPIO_Port, LED_AZUL_Pin);
		HAL_GPIO_WritePin(LED_VERDE_GPIO_Port, LED_VERDE_Pin, 0);
		break;
	case VERDE:
		HAL_GPIO_WritePin(LED_VERMELHO_GPIO_Port, LED_VERMELHO_Pin, 0);
		HAL_GPIO_WritePin(LED_AZUL_GPIO_Port, LED_AZUL_Pin, 0);
		HAL_GPIO_TogglePin(LED_VERDE_GPIO_Port, LED_VERDE_Pin);
		break;
	case VERMELHO_VERDE:
		HAL_GPIO_TogglePin(LED_VERMELHO_GPIO_Port, LED_VERMELHO_Pin);
		HAL_GPIO_WritePin(LED_AZUL_GPIO_Port, LED_AZUL_Pin, 0);
		HAL_GPIO_TogglePin(LED_VERDE_GPIO_Port, LED_VERDE_Pin);
		break;
	case VERDE_AZUL:
		HAL_GPIO_WritePin(LED_VERMELHO_GPIO_Port, LED_VERMELHO_Pin, 0);
		HAL_GPIO_TogglePin(LED_AZUL_GPIO_Port, LED_AZUL_Pin);
		HAL_GPIO_TogglePin(LED_VERDE_GPIO_Port, LED_VERDE_Pin);
		break;
	case AZUL_VERMELHO:
		HAL_GPIO_TogglePin(LED_VERMELHO_GPIO_Port, LED_VERMELHO_Pin);
		HAL_GPIO_TogglePin(LED_AZUL_GPIO_Port, LED_AZUL_Pin);
		HAL_GPIO_WritePin(LED_VERDE_GPIO_Port, LED_VERDE_Pin, 0);
		break;
	case COLORIDO:
		HAL_GPIO_TogglePin(LED_VERMELHO_GPIO_Port, LED_VERMELHO_Pin);
		HAL_GPIO_TogglePin(LED_AZUL_GPIO_Port, LED_AZUL_Pin);
		HAL_GPIO_TogglePin(LED_VERDE_GPIO_Port, LED_VERDE_Pin);
		break;
	case DESLIGADO:
		zeraLeds();
	default:
		break;
	}
}

void zeraLeds() {
	HAL_GPIO_WritePin(LED_VERMELHO_GPIO_Port, LED_VERMELHO_Pin, 0);
	HAL_GPIO_WritePin(LED_AZUL_GPIO_Port, LED_AZUL_Pin, 0);
	HAL_GPIO_WritePin(LED_VERDE_GPIO_Port, LED_VERDE_Pin, 0);
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
