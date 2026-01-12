/*
 * input_analog.h
 *
 *  Created on: Nov 11, 2025
 *      Author: nicolas
 */

#ifndef INC_INPUT_ANALOG_H_
#define INC_INPUT_ANALOG_H_

#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

extern ADC_HandleTypeDef hadc1;

void input_analog_init(void);

#endif /* INC_INPUT_ANALOG_H_ */
