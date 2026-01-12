/*
 * input_encoder.c
 *
 *  Created on: Nov 11, 2025
 *      Author: nicolas
 */

#include "acquisition/input_encoder.h"

void input_encoder_init() {
	// Start Encoder
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_3);
}
