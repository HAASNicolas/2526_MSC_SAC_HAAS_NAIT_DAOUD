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
#define DUTY_CYCLE_INITIAL  0.60      // utilisé avec CCR
#define DUTY_CYCLE_STOP     0.50      // utilisé avec CCR
#define ARR					8500
//#define CCR                 ARR * DUTY_CYCLE_TEST // = 5100 pour 60%
#define MAX_SPEED			1000



void motor_set_duty_cycle(float duty_cycle) {
	int ccr = ARR * duty_cycle;
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1, ccr);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2, ccr);
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
	float duty_cycle = speed / MAX_SPEED * DUTY_CYCLE_STOP + DUTY_CYCLE_STOP;
	motor_set_duty_cycle(duty_cycle);
}
