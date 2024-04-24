/*
 * led.h
 *
 *  Created on: Nov 21, 2022
 *      Author: kerhoas
 */

#ifndef MBED_SHIELD_LEDS_H_
#define MBED_SHIELD_LEDS_H_

#include <stdint.h>

void leds(uint16_t val);

void red_led(uint32_t on);
void green_led(uint32_t on);
void blue_led(uint32_t on);


#endif /* MBED_SHIELD_LEDS_H_ */
