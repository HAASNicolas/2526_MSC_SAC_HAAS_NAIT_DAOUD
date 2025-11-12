/*
 * motor.c
 *
 *  Created on: Nov 11, 2025
 *      Author: nicolas
 */

#include "motor_control/motor.h"

#include "motor.h"
#include <stdio.h>
#include <math.h>



#define CCR 5100
#define DEAD_TIME_NS        39
#define DUTY_CYCLE_TEST     0.60      // utilisé avec CCR



void motor_init(void) {
	HAL_TIM_PWM_Init(&htim1);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);

	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1, CCR);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2, CCR);

}
