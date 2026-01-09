/*
 * asserv.c
 *
 *  Created on: Nov 11, 2025
 *      Author: nicolas
 */

#include "motor_control/asserv.h"
#include "asserv.h"
#include "motor_control/motor.h"


#define ARR					8500
#define ALPHA_DUTY_CYCLE	0.001 // Augmentation max du rapport cyclique à chaque IT


float duty_cycle_goal;
float duty_cycle_current = DUTY_CYCLE_STOP;

void asserv_init() {
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
}

/*
void correcteur(float k, float ki, float input, float* oldValue) {
	float b0 = ;
	float b1 = ;
	float a0 = ;
	float a1 = ;

	oldValue[1] = * oldValue[1] + * input + * oldValue[0];

	oldValue[0] = ;

	return ;
}*/


void motor_set_duty_cycle(float duty_cycle) {
	duty_cycle_goal = duty_cycle;
	HAL_ADC_Start(&hadc1); // Lance une conversion du courant
	HAL_TIM_Base_Start_IT(&htim7);
}

void asserv_duty_cycle(void) {
	uint32_t Iadc = HAL_ADC_GetValue(&hadc1); // Lit le courant

	//static float duty_cycle_current = DUTY_CYCLE_INITIAL;
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
	duty_cycle_current += delta;/*
	if (duty_cycle_goal > duty_cycle_current) {
		duty_cycle_current += ALPHA_DUTY_CYCLE;
	} else {
		duty_cycle_current -= ALPHA_DUTY_CYCLE;
	}*/
	int ccr1 = ARR * duty_cycle_current;
	int ccr2 = ARR * (1 - duty_cycle_current);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1, ccr1);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2, ccr2);

	HAL_ADC_Start(&hadc1); // Lance une conversion du courant
}
