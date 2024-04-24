#include "include/board.h"

#ifdef __DEBUG__
/* Cortex-M4 ecosystem */
SCnSCB_t *  	const _SCnSCB		= (SCnSCB_t *)SCS_BASE;
SCB_t * 		const _SCB			= (SCB_t *)SCB_BASE;
SysTick_t *  	const _SysTick		= (SysTick_t *)SysTick_BASE;
NVIC_t *  		const _NVIC			= (NVIC_t *)NVIC_BASE;
ITM_t *  		const _ITM			= (ITM_t *)ITM_BASE;
DWT_t *  		const _DWT			= (DWT_t *)DWT_BASE;
TPI_t *  		const _TPI			= (TPI_t *)TPI_BASE;
CoreDebug_t *  	const _CoreDebug	= (CoreDebug_t *)CoreDebug_BASE;

#if (__MPU_PRESENT == 1U)
MPU_t *			const _MPU			= (MPU_t *)MPU_BASE;
#endif

#if (__FPU_PRESENT == 1U)
FPU_t *			const _FPU			= (FPU_t *)FPU_BASE;
#endif


/* STM32 ecosystem */
/*!< APB1 peripherals */
TIM_t *  		const  _TIM2		= ((TIM_t *)TIM2_BASE);
TIM_t *  		const  _TIM3		= ((TIM_t *)TIM3_BASE);
TIM_t *  		const  _TIM4		= ((TIM_t *)TIM4_BASE);
TIM_t *  		const  _TIM5		= ((TIM_t *)TIM5_BASE);
RTC_t *  		const  _RTC			= ((RTC_t *)RTC_BASE);
WWDG_t *  		const  _WWDG		= ((WWDG_t *)WWDG_BASE);
IWDG_t *  		const  _IWDG		= ((IWDG_t *)IWDG_BASE);
SPI_t *  		const  _I2S2ext		= ((SPI_t *)I2S2ext_BASE);
SPI_t *  		const  _SPI2		= ((SPI_t *)SPI2_BASE);
SPI_t *  		const  _SPI3		= ((SPI_t *)SPI3_BASE);
SPI_t *  		const  _I2S3ext		= ((SPI_t *)I2S3ext_BASE);
USART_t *  		const  _USART2		= ((USART_t *)USART2_BASE);
I2C_t *  		const  _I2C1		= ((I2C_t *)I2C1_BASE);
I2C_t *  		const  _I2C2		= ((I2C_t *)I2C2_BASE);
I2C_t *  		const  _I2C3		= ((I2C_t *)I2C3_BASE);
PWR_t *  		const  _PWR			= ((PWR_t *)PWR_BASE);

/*!< APB2 peripherals */
TIM_t *  		const  _TIM1		= ((TIM_t *)TIM1_BASE);
USART_t *  		const  _USART1		= ((USART_t *)USART1_BASE);
USART_t *  		const  _USART6		= ((USART_t *)USART6_BASE);
ADC_t *  		const  _ADC1		= ((ADC_t *)ADC1_BASE);
ADC_Common_t *  const  _ADC			= ((ADC_Common_t *)ADC_BASE);
SDIO_t *  		const  _SDIO		= ((SDIO_t *)SDIO_BASE);
SPI_t *  		const  _SPI1		= ((SPI_t *)SPI1_BASE);
SPI_t *  		const  _SPI4		= ((SPI_t *)SPI4_BASE);
SYSCFG_t *  	const  _SYSCFG		= ((SYSCFG_t *)SYSCFG_BASE);
EXTI_t *  		const  _EXTI		= ((EXTI_t *)EXTI_BASE);
TIM_t *  		const  _TIM9		= ((TIM_t *)TIM9_BASE);
TIM_t *  		const  _TIM10		= ((TIM_t *)TIM10_BASE);
TIM_t *  		const  _TIM11		= ((TIM_t *)TIM11_BASE);
SPI_t *  		const  _SPI5		= ((SPI_t *)SPI5_BASE);

/*!< AHB1 peripherals */
GPIO_t *  		const  _GPIOA		= ((GPIO_t *)GPIOA_BASE);
GPIO_t *  		const  _GPIOB		= ((GPIO_t *)GPIOB_BASE);
GPIO_t *  		const  _GPIOC		= ((GPIO_t *)GPIOC_BASE);
GPIO_t *  		const  _GPIOD		= ((GPIO_t *)GPIOD_BASE);
GPIO_t *  		const  _GPIOE		= ((GPIO_t *)GPIOE_BASE);
GPIO_t *  		const  _GPIOH		= ((GPIO_t *)GPIOH_BASE);
CRC_t *  		const  _CRC			= ((CRC_t *)CRC_BASE);
RCC_t *  		const  _RCC			= ((RCC_t *)RCC_BASE);
FLASH_t * 	 	const  _FLASH		= ((FLASH_t *)FLASH_R_BASE);
DMA_t *  		const  _DMA1		= ((DMA_t *)DMA1_BASE);
DMA_Stream_t *  const  _DMA1_Stream0= ((DMA_Stream_t *)DMA1_Stream0_BASE);
DMA_Stream_t *  const  _DMA1_Stream1= ((DMA_Stream_t *)DMA1_Stream1_BASE);
DMA_Stream_t *  const  _DMA1_Stream2= ((DMA_Stream_t *)DMA1_Stream2_BASE);
DMA_Stream_t *  const  _DMA1_Stream3= ((DMA_Stream_t *)DMA1_Stream3_BASE);
DMA_Stream_t *  const  _DMA1_Stream4= ((DMA_Stream_t *)DMA1_Stream4_BASE);
DMA_Stream_t *  const  _DMA1_Stream5= ((DMA_Stream_t *)DMA1_Stream5_BASE);
DMA_Stream_t *  const  _DMA1_Stream6= ((DMA_Stream_t *)DMA1_Stream6_BASE);
DMA_Stream_t *  const  _DMA1_Stream7= ((DMA_Stream_t *)DMA1_Stream7_BASE);
DMA_t *  		const  _DMA2		= ((DMA_t *)DMA2_BASE);
DMA_Stream_t *  const  _DMA2_Stream0= ((DMA_Stream_t *)DMA2_Stream0_BASE);
DMA_Stream_t *  const  _DMA2_Stream1= ((DMA_Stream_t *)DMA2_Stream1_BASE);
DMA_Stream_t *  const  _DMA2_Stream2= ((DMA_Stream_t *)DMA2_Stream2_BASE);
DMA_Stream_t *  const  _DMA2_Stream3= ((DMA_Stream_t *)DMA2_Stream3_BASE);
DMA_Stream_t *  const  _DMA2_Stream4= ((DMA_Stream_t *)DMA2_Stream4_BASE);
DMA_Stream_t *  const  _DMA2_Stream5= ((DMA_Stream_t *)DMA2_Stream5_BASE);
DMA_Stream_t *  const  _DMA2_Stream6= ((DMA_Stream_t *)DMA2_Stream6_BASE);
DMA_Stream_t *  const  _DMA2_Stream7= ((DMA_Stream_t *)DMA2_Stream7_BASE);

/* Debug MCU registers base address */
DBGMCU_t *  	const  _DBGMCU		= ((DBGMCU_t *)DBGMCU_BASE);

/*!< USB registers base address */
USB_OTG_Global_t *  const  _USB_OTG_FS= ((USB_OTG_Global_t *)USB_OTG_FS_PERIPH_BASE);


#else
#warning Peripheral pointers not defined
#endif
