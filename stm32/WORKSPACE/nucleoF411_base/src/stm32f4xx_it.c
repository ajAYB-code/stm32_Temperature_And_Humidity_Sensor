#include "stm32f4xx_it.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_uart.h"
#include "stm32f4xx_hal_i2c.h"
#include "stm32f4xx_hal_spi.h"

extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim3;
extern UART_HandleTypeDef huart2;
extern I2C_HandleTypeDef hi2c1;
extern SPI_HandleTypeDef hspi1;
extern UART_HandleTypeDef 	Uart1Handle;

//===========================================================
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
#ifdef USE_RTOS_SYSTICK
	osSystickHandler();
#endif
}

//===========================================================
void TIM5_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim5);
}
void TIM3_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim3);
}
//===========================================================
void USART2_IRQHandler(void)
{
  HAL_USART_IRQHandler(&huart2);
}
//===========================================================
void I2C1_EV_IRQHandler()
{
  HAL_I2C_EV_IRQHandler(&hi2c1);
}
//===========================================================
void I2C1_ER_IRQHandler()
{
  HAL_I2C_ER_IRQHandler(&hi2c1);
}
//=======================================================
void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
}
//===========================================================
void EXTI4_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}
//===========================================================
void EXTI0_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}
//===========================================================
void EXTI1_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}
//===========================================================
void SPI1_IRQHandler(void)
{
	HAL_SPI_IRQHandler(&hspi1);
}

//===========================================================


