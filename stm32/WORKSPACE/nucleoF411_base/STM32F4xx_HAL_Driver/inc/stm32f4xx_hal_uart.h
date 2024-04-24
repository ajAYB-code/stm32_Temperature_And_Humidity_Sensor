#ifndef _UART_H_
#define _UART_H_

#ifdef __cplusplus
extern "C" {
#endif 

#include "include/board.h"

typedef struct
{
  uint32_t BaudRate;
  uint32_t WordLength;
  uint32_t StopBits;
  uint32_t Parity;
  uint32_t Mode;
  uint32_t HwFlowCtl;
  uint32_t OverSampling;
} UART_InitTypeDef;

typedef struct __UART_HandleTypeDef
{
  USART_TypeDef                 *Instance;        /*!< UART registers base address        */
  UART_InitTypeDef              Init;             /*!< UART communication parameters      */
  uint8_t                       *pTxBuffPtr;      /*!< Pointer to UART Tx transfer Buffer */
  uint16_t                      TxXferSize;       /*!< UART Tx Transfer size              */
  __IO uint16_t                 TxXferCount;      /*!< UART Tx Transfer Counter           */
  uint8_t                       *pRxBuffPtr;      /*!< Pointer to UART Rx transfer Buffer */
  uint16_t                      RxXferSize;       /*!< UART Rx Transfer size              */
  __IO uint16_t                 RxXferCount;      /*!< UART Rx Transfer Counter           */
} UART_HandleTypeDef;


#define UART_WORDLENGTH_8B        0x00000000U
#define UART_STOPBITS_1           0x00000000U
#define UART_PARITY_NONE          0x00000000U
#define UART_HWCONTROL_NONE       0x00000000U
#define UART_OVERSAMPLING_16      0x00000000U


#define UART_MODE_TX_RX           ((uint32_t)(USART_CR1_TE | USART_CR1_RE))


int HAL_UART_Init(UART_HandleTypeDef *huart);
int HAL_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
int HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
int HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);

void uart_putc(UART_HandleTypeDef *huart, uint8_t c);
void uart_puts(UART_HandleTypeDef *huart, uint8_t *s);
void uart_printf(UART_HandleTypeDef *huart, const uint8_t* fmt, ...);

void HAL_USART_IRQHandler(UART_HandleTypeDef *huart);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);



#ifdef __cplusplus
}
#endif
#endif
