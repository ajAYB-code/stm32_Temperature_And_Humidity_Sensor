#ifndef _I2C_H_
#define _I2C_H_

#ifdef __cplusplus
extern "C" {
#endif 

#include "include/board.h"


/* ADC modes */
#define ADC_MODE_12BITS					(0<<24)
#define ADC_MODE_10BITS					(1<<24)
#define ADC_MODE_8BITS					(2<<24)
#define ADC_MODE_6BITS					(3<<24)

#define ADC_MODE_SINGLE					(0)
#define ADC_MODE_CONTINUOUS				(1)
#define ADC_MODE_TRIGGERED				(2)

#define ADC_MODE_POLL					(0)
#define ADC_MODE_IRQ					(1<<1)
#define ADC_MODE_DMA					(0)


/* ADC channel 0 to 15 for regular input channels + specific channels */
#define ADC_CHANNEL_TEMP				16
#define ADC_CHANNEL_VREFINT				17
#define ADC_CHANNEL_VBAT				18

/* ADC sample timing 
 * for ADC_MODE_12BIT mode and ADC_SMPL_28_CYCLES, conversion time is
 * 12 + 28 = 40 cycles long
 */
#define	ADC_SMPL_3CYCLES				0
#define	ADC_SMPL_15CYCLES				1
#define	ADC_SMPL_28CYCLES				2
#define	ADC_SMPL_56CYCLES				3
#define	ADC_SMPL_84CYCLES				4
#define	ADC_SMPL_112CYCLES				5
#define	ADC_SMPL_144CYCLES				6
#define	ADC_SMPL_480CYCLES				7


/* ADC triggering events */
#define ADC_TRIG_ON_TIM1_CH1			(0x10<<24)
#define ADC_TRIG_ON_TIM1_CH2			(0x11<<24)
#define ADC_TRIG_ON_TIM1_CH3			(0x12<<24)
#define ADC_TRIG_ON_TIM2_CH2			(0x13<<24)
#define ADC_TRIG_ON_TIM2_CH3			(0x14<<24)
#define ADC_TRIG_ON_TIM2_CH4			(0x15<<24)
#define ADC_TRIG_ON_TIM2_TRGO			(0x16<<24)
#define ADC_TRIG_ON_TIM3_CH1			(0x17<<24)
#define ADC_TRIG_ON_TIM3_TRGO			(0x18<<24)
#define ADC_TRIG_ON_TIM4_CH4			(0x19<<24)
#define ADC_TRIG_ON_TIM5_CH1			(0x1A<<24)
#define ADC_TRIG_ON_TIM5_CH2			(0x1B<<24)
#define ADC_TRIG_ON_TIM5_CH3			(0x1C<<24)
#define ADC_TRIG_ON_EXTI11_RISING		(0x1F<<24)
#define ADC_TRIG_ON_EXTI11_FALLING		(0x2F<<24)
#define ADC_TRIG_ON_EXTI11_BOTH			(0x3F<<24)


typedef void (*OnSample)(uint16_t channel, uint16_t val);

/* adc_init
 *   intialize ADC
 */
int adc_init(ADC_t *adc, uint32_t mode, OnSample cb);

/* adc_channel_enable
 *   set up adc channel
 */
int adc_channel_enable(ADC_t *adc, uint32_t channel, uint32_t in_sample_time);

/* adc_channel_sample
 *   sample the specified channel and generate an interrupt when it is done
 */
int adc_channel_sample(ADC_t *adc, uint32_t channel);

uint32_t adc_get_sampling_freq(ADC_t *adc);

/* adc_set_trigger_evt
 *   set the timer trigger event to trigger a sampling scan loop
 */
int adc_set_trigger_evt(ADC_t *adc, uint32_t evt);

/* adc_start
 *   start conversion
 */
void adc_start(ADC_t *adc);

/* adc_stop
 *   stop conversion
 */
void adc_stop(ADC_t *adc);

uint32_t adc_read(ADC_t *adc);

#ifdef __cplusplus
}
#endif
#endif
