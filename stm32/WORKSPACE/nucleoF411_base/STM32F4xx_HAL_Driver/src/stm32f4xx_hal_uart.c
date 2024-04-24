#include "stm32f4xx_hal_uart.h"

#include <stdlib.h>
#include <stdarg.h>

#include "stm32f4xx_hal_gpio.h"
#include "util.h"

#include "stm32f4xx_hal_uart.h"

void HAL_USART_IRQHandler(UART_HandleTypeDef *huart)
{
	uint32_t sr = huart->Instance->SR;

	if (sr & (1<<5)) // Read data register not empty interrupt
		{ HAL_UART_RxCpltCallback(huart);}
	else if (sr & (1<<4)) // idle interrupt
		{ huart->Instance->DR;}
	else if (sr & (1<<3)) // overrun interrupt
		{ huart->Instance->DR;}
	else if (sr & (1<<0)) // parity error interrupt
		{ huart->Instance->DR;}
}
//=======================================================================================
int HAL_UART_Init(UART_HandleTypeDef *huart)
{
	uint32_t baud;
	baud = huart->Init.BaudRate;
	

	 if ((huart->Instance == USART1) || (huart->Instance == USART6))
	 {
		 huart->Instance->BRR = sysclks.apb2_freq / baud;
	 }
	 else
	 {
		 	 huart->Instance->BRR = sysclks.apb1_freq / baud;
	 }
	huart->Instance->GTPR = 0;
	huart->Instance->CR3 = (huart->Instance->CR3 & 0xFFFFF000);
	huart->Instance->CR2 = (huart->Instance->CR2 & 0xFFFF8080) | ((huart->Init.StopBits));

	huart->Instance->CR1 = (huart->Instance->CR1 & 0xFFFF4000) |
			(huart->Init.WordLength)|(huart->Init.Mode)| (1<<3) | (1<<2) |(1<<13);
			 
    return 0;
}
//=======================================================================================
int HAL_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
	huart->Instance->DR = pData[0];
	while ((huart->Instance->SR & (1<<7)) == 0) {}
	return 0;
}
//=======================================================================================
int HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
	while(Size > 0)
	{
		while ((huart->Instance->SR & (1<<5)) == 0) {} ;
		*pData++ = (uint8_t)(huart->Instance->DR & (uint8_t)0x00FF);
		Size--;
	}
	return 0;
}
//=======================================================================================
int HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
	huart->Instance->CR1 = (huart->Instance->CR1) | (1<<5); // Receiver Not Empty Interrupt Enable

	huart->pRxBuffPtr = pData;
	huart->RxXferSize = Size;
	huart->RxXferCount = 0;

	return 0;
}
//=======================================================================================
void uart_putc(UART_HandleTypeDef *huart, uint8_t c)
{
	uint8_t data[5];
	data[0] = c;

	HAL_UART_Transmit(huart, data, 1);
}
//=====================================================================================
void uart_puts(UART_HandleTypeDef *huart, uint8_t *s)
{
	int i = 0;
	uint8_t data[5];

	while(s[i] != '\0'){
		data[0] = s[i];
		HAL_UART_Transmit(huart, data, 1);
		i++;
	}
}
//=====================================================================================
void uart_printf(UART_HandleTypeDef *huart, const uint8_t* fmt, ...)
{
	__gnuc_va_list        ap;
	uint8_t          *p;
	uint8_t           ch;
	unsigned long  ul;
	unsigned long long tmp;
	unsigned long long ull;
	uint8_t           s[34];
	
	va_start(ap, fmt);
	while (*fmt != '\0') {
		if (*fmt =='%') {
			switch (*++fmt) {
				case '%':
					uart_putc(huart,'%');
					break;
				case 'c':
					ch = va_arg(ap, int);
					uart_putc(huart, ch);
					break;
				case 's':
					p = va_arg(ap, uint8_t *);
					uart_puts(huart, p);
					break;
				case 'd':
					ul = va_arg(ap, long);
					if ((long)ul < 0) {
						uart_putc(huart, '-');
						ul = -(long)ul;
					}
					num2str(s, ul, 10);
					uart_puts(huart, s);
					break;
				case 'u':
					ul = va_arg(ap, unsigned int);
					num2str(s, ul, 10);
					uart_puts(huart, s);
					break;
				case 'x':
					ul = va_arg(ap, unsigned int);
					num2str(s, ul, 16);
					uart_puts(huart, s);
					break;

				case 'f':
						ull = va_arg(ap, long long unsigned int);
						tmp = ull >> 63;
						int sign = tmp & 0x1 ;
						tmp = (ull & 0x000FFFFFFFFFFFFF) ;
						double mf = (double)tmp ;
						mf = mf / pow(2.0,52.0);
						mf = mf + 1.0;
						tmp = ( ull & 0x7FF0000000000000 ) >> 52;
						tmp = tmp - 1023;
						float f = mf*myPow(2.0,tmp);
						if(sign==1){ uart_putc(huart,'-'); }
						float2str((char*)s, f, 5);
						uart_puts(huart,(char*)s);
						break;

				default:
				    uart_putc(huart, *fmt);
			}
		} else uart_putc(huart, *fmt);
		fmt++;
	}
	va_end(ap);
}
