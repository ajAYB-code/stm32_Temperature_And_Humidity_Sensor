#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_ll_rcc.h"

//=======================================================================
void HAL_TIM_IRQHandler(TIM_HandleTypeDef *htim)
{
	htim->Instance->SR &= ~0x1F; // Acquittement de l'interruption
	HAL_TIM_PeriodElapsedCallback(htim); // Appel de la fonction de callback ( définie dans le main.c )
}
//=======================================================================
int HAL_TIM_wait_ms(TIM_HandleTypeDef *htim, uint32_t ms)
{
    return 0;
}
//=======================================================================
int HAL_TIM_wait_us(TIM_HandleTypeDef *htim, uint32_t us)
{
    return 0;
}
//=======================================================================
int HAL_TIM_Base_Init(TIM_HandleTypeDef *htim)
{
	htim->Instance->SR &= ~0x1F; // clear pending interrupts
	htim->Instance->CR1 = (htim->Instance->CR1 & 0xFC00); // set mode
	htim->Instance->PSC = htim->Init.Prescaler; // set prescaler
	htim->Instance->ARR =htim->Init.Period; 	// set period
	
    return 0;
}
//=======================================================================
void HAL_TIM_Base_Start(TIM_HandleTypeDef *htim)
{
	// A COMPLETER
}
//=======================================================================
void HAL_TIM_Base_Start_IT(TIM_HandleTypeDef *htim)
{
	htim->Instance->EGR |= 0x1;
	htim->Instance->SR =0; // clear pending interrupts
	htim->Instance->DIER |= 0x1;
	htim->Instance->CR1 |= 0x1;
}
//=======================================================================
void HAL_TIM_Base_Stop(TIM_HandleTypeDef *htim)
{
	// A COMPLETER 
}
//=======================================================================
void HAL_TIM_Base_Stop_IT(TIM_HandleTypeDef *htim)
{
	htim->Instance->CR1 &= ~0x1;
	htim->Instance->DIER &= ~0x1;
}
//=======================================================================
int HAL_TIM_PWM_Init(TIM_HandleTypeDef *htim )
{
	uint32_t clk=0;

	clk = sysclks.apb1_timer_freq;
	htim->Instance->SR &= ~0x1F; 	// clear pending interrupts
	htim->Instance->BDTR = 0x8000;
	htim->Instance->CR1 = (1<<7); 	// set mode
	htim->Instance->PSC = htim->Init.Prescaler;	// set prescaler
	htim->Instance->ARR = htim->Init.Period; 	// set period
	
    return 0;
}
//=======================================================================
int HAL_TIM_PWM_ConfigChannel(TIM_HandleTypeDef *htim, uint32_t channel, uint32_t dutycycle, uint32_t oe )
{
	if (channel>4 || dutycycle>100) return 0;
	
	switch(channel)
	{
	case 1 : htim->Instance->CCR1 = (htim->Instance->ARR+1)*dutycycle/100; break;
	case 2 : htim->Instance->CCR2 = (htim->Instance->ARR+1)*dutycycle/100; break;
	case 3 : htim->Instance->CCR3 = (htim->Instance->ARR+1)*dutycycle/100; break;
	case 4 : htim->Instance->CCR4 = (htim->Instance->ARR+1)*dutycycle/100; break;
	default : break;
	}

	channel = channel - 1;
	switch(channel)
	{
	case 0 : htim->Instance->CCMR1 = (htim->Instance->CCMR1 & ~(0xFF << ((channel & 1)<<3))) | (((6<<4) | (1<<3)) << ((channel & 1)<<3)); break;
	case 1 : htim->Instance->CCMR1 = (htim->Instance->CCMR1 & ~(0xFF << ((channel & 1)<<3))) | (((6<<4) | (1<<3)) << ((channel & 1)<<3)); break;
	case 2 : htim->Instance->CCMR2 = (htim->Instance->CCMR2 & ~(0xFF << ((channel & 1)<<3))) | (((6<<4) | (1<<3)) << ((channel & 1)<<3)); break;
	case 3 : htim->Instance->CCMR2 = (htim->Instance->CCMR2 & ~(0xFF << ((channel & 1)<<3))) | (((6<<4) | (1<<3)) << ((channel & 1)<<3)); break;
	default : break;
	}

	if (oe)
	{
		htim->Instance->CCER = (htim->Instance->CCER & ~(0xF<<(channel<<2))) | (1 << (channel<<2));
	}
	else
	{
		htim->Instance->CCER = (htim->Instance->CCER & ~(0xF<<(channel<<2)));
	}
	
	return 0;
}
//=======================================================================
int HAL_TIM_PWM_Channel_Set(TIM_HandleTypeDef *htim,  uint32_t channel, uint32_t dutycycle )
{
	if (channel>4 || dutycycle>100) return 0;
	
	switch(channel)
	{
		case 1 : htim->Instance->CCR1 = (htim->Instance->ARR+1)*dutycycle/100; break;
		case 2 : htim->Instance->CCR2 = (htim->Instance->ARR+1)*dutycycle/100; break;
		case 3 : htim->Instance->CCR3 = (htim->Instance->ARR+1)*dutycycle/100; break;
		case 4 : htim->Instance->CCR4 = (htim->Instance->ARR+1)*dutycycle/100; break;
		default : break;
	}

	return 0;
}
//=======================================================================
