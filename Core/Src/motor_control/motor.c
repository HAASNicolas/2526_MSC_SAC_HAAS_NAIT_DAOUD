/*
 * motor.c
 *
 *  Created on: Nov 11, 2025
 *      Author: nicolas
 */

#include "motor_control/motor.h"

#include "motor.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



#define DEAD_TIME_NS        39
#define DUTY_CYCLE_INITIAL  0.50      // utilisé avec CCR
#define DUTY_CYCLE_STOP     0.50
#define DUTY_CYCLE_RANGE	(1 - DUTY_CYCLE_STOP)
#define ARR					8500
//#define CCR                 ARR * DUTY_CYCLE_TEST // = 5100 pour 60%
#define MAX_SPEED			100.0



void motor_set_duty_cycle(float duty_cycle) {
	int ccr1 = ARR * duty_cycle;
	int ccr2 = ARR * (1 - duty_cycle);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1, ccr1);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2, ccr2);
}

void motor_init(void) {
	HAL_TIM_PWM_Init(&htim1);
	HAL_TIMEx_ConfigDeadTime(&htim1, DEAD_TIME_NS);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
	motor_set_duty_cycle(DUTY_CYCLE_INITIAL);
}

void set_speed(int speed) {
	if (speed > MAX_SPEED) {
		speed = MAX_SPEED;
	}
	float duty_cycle = (float) speed;
	duty_cycle = DUTY_CYCLE_STOP + ((duty_cycle / MAX_SPEED) * DUTY_CYCLE_RANGE);
	motor_set_duty_cycle(duty_cycle);
}
