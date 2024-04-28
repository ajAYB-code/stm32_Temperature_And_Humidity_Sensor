#ifndef __STM32F4xx_HAL_I2C_H
#define __STM32F4xx_HAL_I2C_H

#ifdef __cplusplus
extern "C" {
#endif 

#include "include/board.h"


typedef struct
{
  uint32_t ClockSpeed;
  uint32_t DutyCycle;
  uint32_t OwnAddress1;
  uint32_t AddressingMode;
  uint32_t DualAddressMode;
  uint32_t OwnAddress2;
  uint32_t GeneralCallMode;
  uint32_t NoStretchMode;
} I2C_InitTypeDef;

typedef struct __I2C_HandleTypeDef
{
  I2C_TypeDef     *Instance;      /*!< I2C registers base address               */
  I2C_InitTypeDef  Init;           /*!< I2C communication parameters             */
  volatile int	status;				// driver status
  uint32_t      Devaddress;     /*!< I2C Target device address*/
  uint8_t       *pBuffPtr;      /*!< Pointer to I2C transfer buffer           */
  uint8_t		op;					// read/write operation
  uint32_t		n_to_read;			// how many bytes to be read?
  uint32_t		n_to_write;			// how many bytes to write?
  uint32_t		n_wr;				// how many data actually written?
  uint32_t		n_rd;				// how many data actually read?
} I2C_HandleTypeDef;

/**** Status Errors ****/
#define I2C_BUSY				1  		// transfer busy
#define I2C_OK					0		// transfer ended No Errors
#define I2C_ERROR				-1  	// err: general error
#define I2C_DEVICE_NOT_PRESENT	-5  	// err: Device not present
#define I2C_ARBITRATION_LOST	-6  	// err: Arbitration lost
#define I2C_TIME_OUT			-7  	// err: Time out occurred
#define I2C_SLAVE_ERROR			-8  	// err: Slave mode error

int  HAL_I2C_Init(I2C_HandleTypeDef *hi2c);

int HAL_I2C_Master_Transmit_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);
int HAL_I2C_Master_Receive_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);
int HAL_I2C_Master_Transmit_Receive_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint32_t nwr, uint32_t nrd, uint32_t Timeout);

void HAL_I2C_ER_IRQHandler(I2C_HandleTypeDef *hi2c);
void HAL_I2C_EV_IRQHandler(I2C_HandleTypeDef *hi2c);

#ifdef __cplusplus
}
#endif
#endif
