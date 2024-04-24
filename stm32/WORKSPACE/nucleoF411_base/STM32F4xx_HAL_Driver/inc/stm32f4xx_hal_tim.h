#ifndef _TIMER_H_
#define _TIMER_H_

#ifdef __cplusplus
extern "C" {
#endif 

#include "include/board.h"

typedef void (*OnTick)(void);



typedef struct
{
  uint32_t Prescaler;
  uint32_t CounterMode;
  uint32_t Period;
  uint32_t AutoReloadPreload;
} TIM_Base_InitTypeDef;


typedef struct
{
  TIM_TypeDef                 *Instance;     /*!< Register base address             */
  TIM_Base_InitTypeDef        Init;          /*!< TIM Time Base required parameters */
} TIM_HandleTypeDef;


typedef enum
{
  HAL_TIM_STATE_RESET             = 0x00U,    /*!< Peripheral not yet initialized or disabled  */
  HAL_TIM_STATE_READY             = 0x01U,    /*!< Peripheral Initialized and ready for use    */
  HAL_TIM_STATE_BUSY              = 0x02U,    /*!< An internal process is ongoing              */
  HAL_TIM_STATE_TIMEOUT           = 0x03U,    /*!< Timeout state                               */
  HAL_TIM_STATE_ERROR             = 0x04U     /*!< Reception process is ongoing                */
} HAL_TIM_StateTypeDef;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_TIM_IRQHandler(TIM_HandleTypeDef *htim);

int HAL_TIM_wait_ms(TIM_HandleTypeDef *htim, uint32_t ms);
int HAL_TIM_wait_us(TIM_HandleTypeDef *htim, uint32_t us);

int HAL_TIM_Base_Init(TIM_HandleTypeDef *htim);
void HAL_TIM_Base_Start(TIM_HandleTypeDef *htim);
void HAL_TIM_Base_Start_IT(TIM_HandleTypeDef *htim);
void HAL_TIM_Base_Stop(TIM_HandleTypeDef *htim);
void HAL_TIM_Base_Stop_IT(TIM_HandleTypeDef *htim);

/***************************************************************************/
#define PWM_CHANNEL_1		0
#define PWM_CHANNEL_2		1
#define PWM_CHANNEL_3		2
#define PWM_CHANNEL_4		3

int HAL_TIM_PWM_Init(TIM_HandleTypeDef *htim );
int HAL_TIM_PWM_ConfigChannel(TIM_HandleTypeDef *htim, uint32_t channel, uint32_t dutycycle, uint32_t oe );
int HAL_TIM_PWM_Channel_Set(TIM_HandleTypeDef *htim,  uint32_t channel, uint32_t dutycycle );
#define HAL_TIM_PWM_Start(pwm)	HAL_TIM_Base_Start(pwm)
#define HAL_TIM_PWM_Stop(pwm)	HAL_TIM_Base_Stop(pwm)

#ifdef __cplusplus
}
#endif
#endif
