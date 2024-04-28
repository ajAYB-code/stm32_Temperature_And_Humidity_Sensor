/****************************************************************************
 * IRQ Priority
 ****************************************************************************/
#define EXTI0_IRQ_PRIORITY		8
#define EXTI1_IRQ_PRIORITY		8
#define EXTI2_IRQ_PRIORITY		8
#define EXTI3_IRQ_PRIORITY		8
#define EXTI4_IRQ_PRIORITY		8
#define EXTI9_5_IRQ_PRIORITY	8
#define EXTI15_10_IRQ_PRIORITY	8

#define TIM2_IRQ_PRIORITY		4
#define TIM3_IRQ_PRIORITY		4
#define TIM4_IRQ_PRIORITY		4
#define TIM5_IRQ_PRIORITY		4

#define	USART1_IRQ_PRIORITY		3
#define	USART2_IRQ_PRIORITY		3
#define	USART6_IRQ_PRIORITY		3

#define	I2C1_IRQ_PRIORITY		2
#define	I2C1_IRQERR_PRIORITY	1
#define	I2C2_IRQ_PRIORITY		2
#define	I2C2_IRQERR_PRIORITY	1
#define	I2C3_IRQ_PRIORITY		2
#define	I2C3_IRQERR_PRIORITY	1

#define SPI1_IRQ_PRIORITY		4
#define SPI2_IRQ_PRIORITY		4
#define SPI3_IRQ_PRIORITY		4
#define SPI4_IRQ_PRIORITY		4
#define SPI5_IRQ_PRIORITY		4

#define ADC1_IRQ_PRIORITY		5

/****************************************************************************
 * USART Pin and use configuration 
 ****************************************************************************/

//#define USE_USART1
//#define USART1_GPIO_PORT
//#define USART1_GPIO_PINS

// USART2: Tx --> PA2, Rx --> PA3
#define USE_USART2
#define USART2_GPIO_PORT	_GPIOA
#define USART2_GPIO_PINS	PIN_2|PIN_3

//#define USE_USART6
//#define USART6_GPIO_PORT
//#define USART6_GPIO_PINS

/****************************************************************************
 *  I2C Pin and use configuration 
 ****************************************************************************/

// I2C1 : SCL --> PB8, SDA --> PB9
#define USE_I2C1
#define I2C1_GPIO_PORT		_GPIOB
#define I2C1_GPIO_PINS		PIN_8|PIN_9

//#define USE_I2C2
//#define I2C2_GPIO_PORT
//#define I2C2_GPIO_PINS

//#define USE_I2C3
//#define I2C3_GPIO_PORT
//#define I2C3_GPIO_PINS

/****************************************************************************
 *  SPI pin and use configuration 
 ****************************************************************************/

// SPI1 pins : LCD_SCK   --> D13 --> PA5 : SPI1_SCK
//             LCD_MOSI  --> D11 --> PA7 : SPI1_MOSI
#define USE_SPI1
#define SPI1_GPIO_PORT		_GPIOA
#define SPI1_GPIO_PINS		PIN_5|PIN_7

//#define USE_SPI2
//#define SPI2_GPIO_PORT
//#define SPI2_GPIO_PINS			

//#define USE_SPI3
//#define SPI3_GPIO_PORT
//#define SPI3_GPIO_PINS			

//#define USE_SPI4
//#define SPI4_GPIO_PORT
//#define SPI4_GPIO_PINS			

//#define USE_SPI5
//#define SPI5_GPIO_PORT
//#define SPI5_GPIO_PINS			

/****************************************************************************
 *  ADC pin and use configuration 
 ****************************************************************************/

// ADC1: ADC1_0 --> PA0, ADC1_1 -->PA1
#define USE_ADC1
#define ADC1_GPIO_PORT		_GPIOA
#define ADC1_GPIO_PINS		PIN_0|PIN_1

