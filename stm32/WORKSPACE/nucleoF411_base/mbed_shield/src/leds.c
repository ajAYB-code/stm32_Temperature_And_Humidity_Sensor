#include "leds.h"
#include "stm32f4xx_hal_gpio.h"

// LED_RED   --> PB4
// LED_GREEN --> PC7
// LED_BLUE  --> PA9

// NUCLEO LED --> PA5
//=======================================================
void nucleo_led(uint32_t on)
{
	// A COMPLETER
}
//=======================================================
void red_led(uint32_t on)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, on);
}
//=======================================================
void green_led(uint32_t on)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, on);
}
//=======================================================
void blue_led(uint32_t on)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, on);
}
//=======================================================
