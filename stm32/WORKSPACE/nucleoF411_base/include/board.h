#ifndef _BOARD_H_
#define _BOARD_H_

/*
#ifndef __ASSEMBLY__
	#define __DEBUG__
#endif
*/
#ifdef __cplusplus
extern "C" {
#endif 

#include <stdlib.h>
#include "stm32f411xe.h"
#include "stm32f4xx_ll_rcc.h"

/* This variable is updated in three ways:
 *     1) by calling CMSIS function SystemCoreClockUpdate()
 *     2) by calling HAL API function HAL_RCC_GetSysClockFreq()
 *     3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency 
 *        Note: If you use this function to configure the system clock; then there
 *              is no need to call the 2 first functions listed above, since SystemCoreClock
 *              variable is updated automatically.
 */
extern uint32_t SystemCoreClock;          /*!< System Clock Frequency (Core Clock) */
extern Clock_t sysclks;

extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);

typedef IRQn_Type					IRQn_t;

/* type redefinition for the Cortex-M4 */
typedef SCnSCB_Type					SCnSCB_t;
typedef SCB_Type					SCB_t;
typedef SysTick_Type				SysTick_t;
typedef NVIC_Type					NVIC_t;
typedef ITM_Type					ITM_t;
typedef DWT_Type					DWT_t;
typedef TPI_Type					TPI_t;
typedef CoreDebug_Type				CoreDebug_t;

#if (__MPU_PRESENT == 1U)
typedef MPU_Type					MPU_t;
#endif

#if (__FPU_PRESENT == 1U)
typedef FPU_Type					FPU_t;
#endif

/* type redefinition for the STM32 microcontroller */
typedef ADC_Alt_TypeDef				ADC_t;					/* Analog to Digital Converter  */
typedef ADC_Common_TypeDef			ADC_Common_t;
typedef CRC_TypeDef					CRC_t;					/* CRC calculation unit */
typedef DBGMCU_TypeDef				DBGMCU_t;				/* Debug MCU */
typedef DMA_Stream_TypeDef			DMA_Stream_t;			/* DMA Controller */
typedef DMA_TypeDef					DMA_t;
typedef EXTI_TypeDef				EXTI_t;					/* External Interrupt/Event Controller */
typedef FLASH_TypeDef				FLASH_t;				/* FLASH Registers */
typedef GPIO_TypeDef				GPIO_t;					/* General Purpose I/O */
typedef SYSCFG_TypeDef				SYSCFG_t;				/* System configuration controller */
typedef I2C_TypeDef					I2C_t;					/* Inter-integrated Circuit Interface */
typedef IWDG_TypeDef				IWDG_t;					/* Independent WATCHDOG */
typedef PWR_TypeDef					PWR_t;					/* Power Control */
typedef RCC_TypeDef					RCC_t;					/* Reset and Clock Control */
typedef RTC_TypeDef					RTC_t;					/* Real-Time Clock */
typedef SDIO_TypeDef				SDIO_t;					/* SD host Interface */
typedef SPI_TypeDef					SPI_t;					/* Serial Peripheral Interface */
typedef TIM_Alt_TypeDef				TIM_t;					/* Timer */
typedef	TIM_t						PWM_t;					/* PWM redefinition */
typedef USART_TypeDef				USART_t;				/* Universal Synchronous Asynchronous Receiver Transmitter */
typedef WWDG_TypeDef				WWDG_t;					/* Window WATCHDOG */

typedef USB_OTG_GlobalTypeDef		USB_OTG_Global_t;		/* USB OTG Core registers */
typedef USB_OTG_DeviceTypeDef		USB_OTG_Device_t;		/* USB OTG device registers*/
typedef USB_OTG_INEndpointTypeDef	USB_OTG_INEndpoint_t;	/* IN_Endpoint-Specific_Register */
typedef USB_OTG_OUTEndpointTypeDef	USB_OTG_OUTEndpoint_t;	/* OUT_Endpoint-Specific_Registers */
typedef USB_OTG_HostTypeDef			USB_OTG_Host_t;			/* Host_Mode_Register_Structures */
typedef USB_OTG_HostChannelTypeDef	USB_OTG_HostChannel_t;	/* Host_Channel_Specific_Registers */


/* peripheral access variables */
#ifndef __DEBUG__

#define _SCnSCB				((SCnSCB_t *)SCS_BASE)			/*!< System control Register not in SCB */
#define _SCB				((SCB_t *)SCB_BASE)				/*!< SCB configuration struct */
#define _SysTick			((SysTick_t *)SysTick_BASE)		/*!< SysTick configuration struct */
#define _NVIC				((NVIC_t *)NVIC_BASE)			/*!< NVIC configuration struct */
#define _ITM				((ITM_t *)ITM_BASE)				/*!< ITM configuration struct */
#define _DWT				((DWT_t *)DWT_BASE)				/*!< DWT configuration struct */
#define _TPI				((TPI_t *)TPI_BASE)				/*!< TPI configuration struct */
#define _CoreDebug			((CoreDebug_t *)CoreDebug_BASE)	/*!< Core Debug configuration struct */

#if (__MPU_PRESENT == 1U)
  #define _MPU				((MPU_t *)MPU_BASE)				/*!< Memory Protection Unit */
#endif

#if (__FPU_PRESENT == 1U)
  #define _FPU				((FPU_t *)FPU_BASE)				/*!< Floating Point Unit */
#endif

/*!< APB1 peripherals */
#define _TIM2				((TIM_t *)TIM2_BASE)
#define _TIM3				((TIM_t *)TIM3_BASE)
#define _TIM4				((TIM_t *)TIM4_BASE)
#define _TIM5				((TIM_t *)TIM5_BASE)
#define _RTC				((RTC_t *)RTC_BASE)
#define _WWDG				((WWDG_t *)WWDG_BASE)
#define _IWDG				((IWDG_t *)IWDG_BASE)
#define _I2S2ext			((SPI_t *)I2S2ext_BASE)
#define _SPI2				((SPI_t *)SPI2_BASE)
#define _SPI3				((SPI_t *)SPI3_BASE)
#define _I2S3ext			((SPI_t *)I2S3ext_BASE)
#define _USART2				((USART_t *)USART2_BASE)
#define _I2C1				((I2C_t *)I2C1_BASE)
#define _I2C2				((I2C_t *)I2C2_BASE)
#define _I2C3				((I2C_t *)I2C3_BASE)
#define _PWR				((PWR_t *)PWR_BASE)

/*!< APB2 peripherals */
#define _TIM1				((TIM_t *)TIM1_BASE)
#define _USART1				((USART_t *)USART1_BASE)
#define _USART6				((USART_t *)USART6_BASE)
#define _ADC1				((ADC_t *)ADC1_BASE)
#define _ADC				((ADC_Common_t *)ADC_BASE)
#define _SDIO				((SDIO_t *)SDIO_BASE)
#define _SPI1				((SPI_t *)SPI1_BASE)
#define _SPI4				((SPI_t *)SPI4_BASE)
#define _SYSCFG				((SYSCFG_t *)SYSCFG_BASE)
#define _EXTI				((EXTI_t *)EXTI_BASE)
#define _TIM9				((TIM_t *)TIM9_BASE)
#define _TIM10				((TIM_t *)TIM10_BASE)
#define _TIM11				((TIM_t *)TIM11_BASE)
#define _SPI5				((SPI_t *)SPI5_BASE)

/*!< AHB1 peripherals */
#define _GPIOA				((GPIO_t *)GPIOA_BASE)
#define _GPIOB				((GPIO_t *)GPIOB_BASE)
#define _GPIOC				((GPIO_t *)GPIOC_BASE)
#define _GPIOD				((GPIO_t *)GPIOD_BASE)
#define _GPIOE				((GPIO_t *)GPIOE_BASE)
#define _GPIOH				((GPIO_t *)GPIOH_BASE)
#define _CRC				((CRC_t *)CRC_BASE)
#define _RCC				((RCC_t *)RCC_BASE)
#define _FLASH				((FLASH_t *)FLASH_R_BASE)
#define _DMA1				((DMA_t *)DMA1_BASE)
#define _DMA1_Stream0		((DMA_Stream_t *)DMA1_Stream0_BASE)
#define _DMA1_Stream1		((DMA_Stream_t *)DMA1_Stream1_BASE)
#define _DMA1_Stream2		((DMA_Stream_t *)DMA1_Stream2_BASE)
#define _DMA1_Stream3		((DMA_Stream_t *)DMA1_Stream3_BASE)
#define _DMA1_Stream4		((DMA_Stream_t *)DMA1_Stream4_BASE)
#define _DMA1_Stream5		((DMA_Stream_t *)DMA1_Stream5_BASE)
#define _DMA1_Stream6		((DMA_Stream_t *)DMA1_Stream6_BASE)
#define _DMA1_Stream7		((DMA_Stream_t *)DMA1_Stream7_BASE)
#define _DMA2				((DMA_t *)DMA2_BASE)
#define _DMA2_Stream0		((DMA_Stream_t *)DMA2_Stream0_BASE)
#define _DMA2_Stream1		((DMA_Stream_t *)DMA2_Stream1_BASE)
#define _DMA2_Stream2		((DMA_Stream_t *)DMA2_Stream2_BASE)
#define _DMA2_Stream3		((DMA_Stream_t *)DMA2_Stream3_BASE)
#define _DMA2_Stream4		((DMA_Stream_t *)DMA2_Stream4_BASE)
#define _DMA2_Stream5		((DMA_Stream_t *)DMA2_Stream5_BASE)
#define _DMA2_Stream6		((DMA_Stream_t *)DMA2_Stream6_BASE)
#define _DMA2_Stream7		((DMA_Stream_t *)DMA2_Stream7_BASE)

/* Debug MCU registers base address */
#define _DBGMCU				((DBGMCU_t *)DBGMCU_BASE)

/*!< USB registers base address */
#define _USB_OTG_FS			((USB_OTG_Global_t *)USB_OTG_FS_PERIPH_BASE)

#else
/* Cortex-M4 ecosystem */
extern	SCnSCB_t *  const _SCnSCB;
extern	SCB_t * 	const _SCB;
extern	SysTick_t * const _SysTick;
extern	NVIC_t *  	const _NVIC;
extern	ITM_t *  	const _ITM;
extern	DWT_t *  	const _DWT;
extern	TPI_t *  	const _TPI;
extern	CoreDebug_t *  	const _CoreDebug;

#if (__MPU_PRESENT == 1U)
extern	MPU_t *		const _MPU;
#endif

#if (__FPU_PRESENT == 1U)
extern	FPU_t *		const _FPU;
#endif


/* STM32 ecosystem */
/*!< APB1 peripherals */
extern	TIM_t *  		const  _TIM2;			// 32 bit timer
extern	TIM_t *  		const  _TIM3;			// 16 bit timer
extern	TIM_t *  		const  _TIM4;			// 16 bit timer
extern	TIM_t *  		const  _TIM5;			// 32 bit timer
extern	RTC_t *  		const  _RTC;
extern	WWDG_t *  		const  _WWDG;
extern	IWDG_t *  		const  _IWDG;
extern	SPI_t *  		const  _I2S2ext;
extern	SPI_t *  		const  _SPI2;
extern	SPI_t *  		const  _SPI3;
extern	SPI_t *  		const  _I2S3ext;
extern	USART_t *  		const  _USART2;
extern	I2C_t *  		const  _I2C1;
extern	I2C_t *  		const  _I2C2;
extern	I2C_t *  		const  _I2C3;
extern	PWR_t *  		const  _PWR;

/*!< APB2 peripherals */
extern	TIM_t *  		const  _TIM1;
extern	USART_t *  		const  _USART1;
extern	USART_t *  		const  _USART6;
extern	ADC_t *  		const  _ADC1;
extern	ADC_Common_t *  const  _ADC;
extern	SDIO_t *  		const  _SDIO;
extern	SPI_t *  		const  _SPI1;
extern	SPI_t *  		const  _SPI4;
extern	SYSCFG_t *  	const  _SYSCFG;
extern	EXTI_t *  		const  _EXTI;
extern	TIM_t *  		const  _TIM9;
extern	TIM_t *  		const  _TIM10;
extern	TIM_t *  		const  _TIM11;
extern	SPI_t *  		const  _SPI5;

/*!< AHB1 peripherals */
extern	GPIO_t *  		const  _GPIOA;
extern	GPIO_t *  		const  _GPIOB;
extern	GPIO_t *  		const  _GPIOC;
extern	GPIO_t *  		const  _GPIOD;
extern	GPIO_t *  		const  _GPIOE;
extern	GPIO_t *  		const  _GPIOH;
extern	CRC_t *  		const  _CRC;
extern	RCC_t *  		const  _RCC;
extern	FLASH_t * 	 	const  _FLASH;
extern	DMA_t *  		const  _DMA1;
extern	DMA_Stream_t *  const  _DMA1_Stream0;
extern	DMA_Stream_t *  const  _DMA1_Stream1;
extern	DMA_Stream_t *  const  _DMA1_Stream2;
extern	DMA_Stream_t *  const  _DMA1_Stream3;
extern	DMA_Stream_t *  const  _DMA1_Stream4;
extern	DMA_Stream_t *  const  _DMA1_Stream5;
extern	DMA_Stream_t *  const  _DMA1_Stream6;
extern	DMA_Stream_t *  const  _DMA1_Stream7;
extern	DMA_t *  		const  _DMA2;
extern	DMA_Stream_t *  const  _DMA2_Stream0;
extern	DMA_Stream_t *  const  _DMA2_Stream1;
extern	DMA_Stream_t *  const  _DMA2_Stream2;
extern	DMA_Stream_t *  const  _DMA2_Stream3;
extern	DMA_Stream_t *  const  _DMA2_Stream4;
extern	DMA_Stream_t *  const  _DMA2_Stream5;
extern	DMA_Stream_t *  const  _DMA2_Stream6;
extern	DMA_Stream_t *  const  _DMA2_Stream7;

/* Debug MCU registers base address */
extern	DBGMCU_t *  	const  _DBGMCU;

/*!< USB registers base address */
extern	USB_OTG_Global_t *  const  _USB_OTG_FS;
#endif

#include "config.h"

#ifdef __cplusplus
}
#endif
#endif
