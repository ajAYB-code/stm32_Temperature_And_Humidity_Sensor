#include "stm32f4xx_hal_msp.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_uart.h"
#include "stm32f4xx_hal_i2c.h"
#include "leds.h"
#include "sw.h"
#include "lm75.h"
#include "stm32f4xx_hal.h"
#include "math.h"
#define buff_size 50

TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim5;
UART_HandleTypeDef huart2;
I2C_HandleTypeDef hi2c1;
float tempCylis;
int button_pressed = 0;

//===========================================================

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
	}
}
//============================================================

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart2)
	{
	}
}
//============================================================
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	if(htim == &htim5){
		if(button_pressed == 1){
		}
	}
}

//Fonction pour lecture de la valeur de la température et l'humidité

void read_tempAndHumd(float *temp, float *humd){
	uint8_t buffer[10];
	uint16_t rawTemp, rawHumd;
	buffer[0] = 0x2c;
	buffer[1] = 0x10;
	HAL_I2C_Master_Transmit_IT(&hi2c1, 0x45, buffer, 2, 0);
	HAL_Delay(1500);
	HAL_I2C_Master_Receive_IT(&hi2c1, 0x45, buffer, 4, 0);

	rawTemp = ((uint16_t) buffer[0] << 8) | buffer[1];
	rawHumd = ((uint16_t) buffer[3] << 8) | buffer[4];

	*temp = (float) (-45 + 175 * (rawTemp / (pow(2,16) - 1.0)));
	*humd = (float) (100 * (rawHumd / (pow(2,16) - 1.0)));
}


//============================================================
int main()
{
	HAL_Init();
	HAL_MspInit(); // Initialisation des Broches
	green_led(0);
	red_led(0);
	blue_led(0);

	//Init UART
	huart2.Instance          = USART2;
	huart2.Init.BaudRate     = 115200;
	huart2.Init.WordLength   = UART_WORDLENGTH_8B;
	huart2.Init.StopBits     = UART_STOPBITS_1;
	huart2.Init.Parity       = UART_PARITY_NONE;
	HAL_UART_Init(&huart2);

	//Init I2C
	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 400000;
	HAL_I2C_Init(&hi2c1);

	float temp;
	float humd;


	while(1)
	{
		read_tempAndHumd(&temp, &humd);

		//Envoyer les donné en liaison serial
		uart_printf(&huart2, "%f-%f\r\n", temp, humd);

		HAL_Delay(10000);

	}

	return 0;
}
//============================================================

