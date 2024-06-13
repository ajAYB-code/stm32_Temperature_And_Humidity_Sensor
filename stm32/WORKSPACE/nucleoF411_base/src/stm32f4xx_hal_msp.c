// MCU Support Package

#include "stm32f4xx_hal_msp.h"

#include "stm32f4xx_hal_gpio_ex.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_hal_gpio.h"
#include "board.h"

#define I2C1_ER_IRQ_PRIO    2
#define I2C1_EV_IRQ_PRIO    3
#define TIM5_IRQ_PRIO       8
#define EXTI15_10_IRQ_PRIO  5
#define EXTI4_IRQ_PRIO      6
#define USART2_IRQ_PRIO     7
#define SPI1_IRQ_PRIO		4
#define USART1_IRQ_PRIO	8

void HAL_GPIO_LEDS_MspInit(void);
void HAL_TIM2_MspInit(void);
void HAL_TIM5_MspInit(void);
void HAL_GPIO_SWITCHS_MspInit(void);
void HAL_UART2_MspInit(void);
void HAL_I2C1_MspInit(void);
void HAL_EXTI_MspInit(void);
void HAL_GPIO_SWITCH_UP_EXTI_MspInit(void);
void HAL_LCD_MspInit(void);

//===========================================================
void HAL_MspInit(void)
{
	  __GPIOA_CLK_ENABLE();
	  __GPIOB_CLK_ENABLE();
	  __GPIOC_CLK_ENABLE();

	  HAL_GPIO_LEDS_MspInit();
	  //HAL_GPIO_SWITCHS_MspInit();
	  HAL_GPIO_SWITCH_RIGHT_EXTI_MspInit();
	  HAL_GPIO_SWITCH_LEFT_EXTI_MspInit();
	  HAL_TIM2_MspInit();
	  HAL_TIM5_MspInit();
	  HAL_UART2_MspInit();
	  HAL_I2C1_MspInit();

}
//===========================================================
// LED_RED   --> PB4
// LED_GREEN --> PC7
// LED_BLUE  --> PA9
void HAL_GPIO_LEDS_MspInit(void)
{
	  GPIO_InitTypeDef  GPIO_InitStruct;


	  GPIO_InitStruct.Pin = GPIO_PIN_4;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	  GPIO_InitStruct.Pin = GPIO_PIN_9;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	  GPIO_InitStruct.Pin = GPIO_PIN_7;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
	  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}
//===========================================================
// SW_UP     --> PA4
// SW_DOWN   --> PB0
// SW_CENTER --> PB5
// SW_RIGHT  --> PC0
// SW_LEFT   --> PC1
void HAL_GPIO_SWITCHS_MspInit()
{
	 GPIO_InitTypeDef  GPIO_InitStruct;

	 GPIO_InitStruct.Pin = GPIO_PIN_0;
	 GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	 GPIO_InitStruct.Pull = GPIO_NOPULL;
	 HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	 GPIO_InitStruct.Pin = GPIO_PIN_1;
	 GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	 GPIO_InitStruct.Pull = GPIO_NOPULL;
	 HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	 GPIO_InitStruct.Pin = GPIO_PIN_0;
	 GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	 GPIO_InitStruct.Pull = GPIO_NOPULL;
	 HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	 GPIO_InitStruct.Pin = GPIO_PIN_4;
	 GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	 GPIO_InitStruct.Pull = GPIO_NOPULL;
	 HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	 GPIO_InitStruct.Pin = GPIO_PIN_5;
	 GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	 GPIO_InitStruct.Pull = GPIO_NOPULL;
	 HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

//===========================================================
void  HAL_TIM2_MspInit(void)
{
	__TIM2_CLK_ENABLE();
}

void  HAL_TIM5_MspInit(void)
{
	   GPIO_InitTypeDef  GPIO_InitStruct;

	    __TIM5_CLK_ENABLE();

	     GPIO_InitStruct.Pin = GPIO_PIN_7;
	     GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	     GPIO_InitStruct.Pull = GPIO_NOPULL;
	     GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
	     GPIO_InitStruct.Alternate =  GPIO_AF2_TIM3;
	     HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	     NVIC_SetPriority(TIM5_IRQn, TIM5_IRQ_PRIO);
	     NVIC_EnableIRQ(TIM5_IRQn);
}

//===========================================================
//			USART2 : ST-LINK virtual Port
// TX --> PA2
// RX --> PA3
//===========================================================


void HAL_I2C1_MspInit(void)
{
      GPIO_InitTypeDef  GPIO_InitStruct;

      __I2C1_CLK_ENABLE();

      GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
      GPIO_InitStruct.Alternate =   GPIO_AF4_I2C1 ; // hal_gpio_ex.h

      HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

      NVIC_SetPriority(I2C1_ER_IRQn, I2C1_ER_IRQ_PRIO);
      NVIC_EnableIRQ(I2C1_ER_IRQn);

      NVIC_SetPriority(I2C1_EV_IRQn, I2C1_EV_IRQ_PRIO);
      NVIC_EnableIRQ(I2C1_EV_IRQn);
}

void HAL_UART2_MspInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  __GPIOA_CLK_ENABLE();
  __USART2_CLK_ENABLE();

  GPIO_InitStruct.Pin       = GPIO_PIN_2 | GPIO_PIN_3;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_MEDIUM;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void HAL_GPIO_SWITCH_RIGHT_EXTI_MspInit()
{
      GPIO_InitTypeDef  GPIO_InitStruct;

      GPIO_InitStruct.Pin = GPIO_PIN_0;
      GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
      GPIO_InitStruct.Pull = GPIO_NOPULL;

      HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

      NVIC_SetPriority(EXTI0_IRQn, EXTI0_IRQ_PRIORITY);
      NVIC_EnableIRQ(EXTI0_IRQn);
}

void HAL_GPIO_SWITCH_LEFT_EXTI_MspInit()
{
      GPIO_InitTypeDef  GPIO_InitStruct;

      GPIO_InitStruct.Pin = GPIO_PIN_1;
      GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
      GPIO_InitStruct.Pull = GPIO_NOPULL;

      HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

      NVIC_SetPriority(EXTI1_IRQn , EXTI1_IRQ_PRIORITY);
      NVIC_EnableIRQ(EXTI1_IRQn);
}

void HAL_UART1_MspInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  __GPIOA_CLK_ENABLE();

  __USART1_CLK_ENABLE();

  GPIO_InitStruct.Pin       = GPIO_PIN_9;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_LOW;	//GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate =  GPIO_AF7_USART1;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = GPIO_PIN_3;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_LOW;	//GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate =  GPIO_AF7_USART1;

  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  HAL_NVIC_SetPriority(USART1_IRQn, USART1_IRQ_PRIO, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
}
//===========================================================


