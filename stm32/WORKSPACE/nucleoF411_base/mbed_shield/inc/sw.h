#ifndef MBED_SHIELD_SW_H_
#define MBED_SHIELD_SW_H_
#include <stdint.h>

uint32_t sw_right_debounce(void);
uint32_t sw_left_debounce(void);
uint32_t sw_up_debounce(void);
uint32_t sw_down_debounce(void);
uint32_t sw_center_debounce(void);

uint32_t sw_right_raw(void);
uint32_t sw_left_raw(void);
uint32_t sw_center_raw(void);
uint32_t sw_up_raw(void);
uint32_t sw_down_raw(void);

uint32_t sw_input_raw(void);

#endif /* MBED_SHIELD_SW_H_ */
