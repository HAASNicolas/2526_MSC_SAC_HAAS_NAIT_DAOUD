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
#define DUTY_CYCLE_STOP     0.50
#define DUTY_CYCLE_INITIAL  DUTY_CYCLE_STOP // 0.60
//#define DUTY_CYCLE_RANGE	(1 - DUTY_CYCLE_STOP)
#define ARR					8500
//#define CCR                 ARR * DUTY_CYCLE_TEST // = 5100 pour 60%
#define MAX_SPEED			1000.0
#define ALPHA_DUTY_CYCLE	0.01 // Augmentation max du rapport cyclique à chaque IT

float duty_cycle_goal;

void motor_set_duty_cycle(float duty_cycle) {
	duty_cycle_goal = duty_cycle;
	HAL_TIM_Base_Start_IT(&htim7);
}

void asserv_duty_cycle(void) {
	static float duty_cycle_current = DUTY_CYCLE_INITIAL;
	float delta = duty_cycle_goal - duty_cycle_current;
	// Saturation
	if (delta > ALPHA_DUTY_CYCLE) {
		delta = ALPHA_DUTY_CYCLE;
	} else if (delta < -ALPHA_DUTY_CYCLE) {
		delta = -ALPHA_DUTY_CYCLE;
	} else {
		// Stop asservissement
		HAL_TIM_Base_Stop_IT(&htim7);
	}
	duty_cycle_current += delta;
	int ccr1 = ARR * duty_cycle_current;
	int ccr2 = ARR * (1 - duty_cycle_current);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1, ccr1);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2, ccr2);
}

void motor_init(void) {
	HAL_TIM_PWM_Init(&htim1);
	HAL_TIMEx_ConfigDeadTime(&htim1, DEAD_TIME_NS);
}

void motor_start() {
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
	motor_set_duty_cycle(DUTY_CYCLE_INITIAL);
}

void motor_stop() {
	HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
}

void set_speed(int speed) {
	if (speed > MAX_SPEED) {
		speed = MAX_SPEED;
	}
	float duty_cycle = (float) speed / MAX_SPEED;
	//duty_cycle = DUTY_CYCLE_STOP + ((duty_cycle / MAX_SPEED) * DUTY_CYCLE_RANGE);
	motor_set_duty_cycle(duty_cycle);
}
