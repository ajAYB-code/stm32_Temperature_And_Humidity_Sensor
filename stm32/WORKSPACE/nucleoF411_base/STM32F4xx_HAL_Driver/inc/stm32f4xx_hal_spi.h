#ifndef _SPI_H_
#define _SPI_H_

#ifdef __cplusplus
extern "C" {
#endif 

#include "include/board.h"

typedef struct
{
  uint32_t Mode;
  uint32_t Direction;
  uint32_t DataSize;
  uint32_t CLKPolarity;
  uint32_t CLKPhase;
  uint32_t NSS;
  uint32_t BaudRatePrescaler;
  uint32_t FirstBit;
  uint32_t TIMode;
  uint32_t CRCCalculation;
  uint32_t CRCPolynomial;
  uint32_t max_rate;
  uint32_t cfg;
} SPI_InitTypeDef;

typedef struct __SPI_HandleTypeDef
{
  SPI_TypeDef                *Instance;      /*!< SPI registers base address               */
  SPI_InitTypeDef            Init;           /*!< SPI communication parameters             */
  uint8_t                    *pTxBuffPtr;    /*!< Pointer to SPI Tx transfer Buffer        */
  uint8_t                    *pRxBuffPtr;    /*!< Pointer to SPI Rx transfer Buffer        */
  uint32_t 					n_to_write;
  uint32_t 					n_wr;
  uint32_t 					n_to_read;
  uint32_t 					n_rd;
  volatile int				status;				// driver status
} SPI_HandleTypeDef;

#define SPI_MODE0				0
#define SPI_MODE1				1
#define SPI_MODE2				2
#define SPI_MODE3				3

#define SPI_LSB_FIRST			(1<<7)
#define SPI_DATA_16				(1<<11)

#define SPI_BUSY				1  		// transfer busy
#define SPI_OK					0		// transfer ended No Errors
#define SPI_ERROR				-1  	// err: general error

int HAL_SPI_Init(SPI_HandleTypeDef *hspi);
int  HAL_SPI_Transmit_IT(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size);
int  HAL_SPI_Write_Byte(SPI_HandleTypeDef *hspi, uint8_t data);

void HAL_SPI_IRQHandler(SPI_HandleTypeDef* hspi);

#ifdef __cplusplus
}
#endif
#endif
