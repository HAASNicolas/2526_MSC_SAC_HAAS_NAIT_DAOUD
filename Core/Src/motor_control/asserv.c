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

#define K_SPEED			0.15 // P_vitesse
#define KI_SPEED		0.06 // I_vitesse
#define T_MEAS_SPEED	0.005; // T = 1/F = 85/170000 = 0.005

#define K_CURRENT	0.15 // P_courant
#define KI_CURRENT	0.06 // I_courant
#define NB_PHASE	2 // U et V

int8_t timer_it_asserv_speed = 0;
int8_t timer_it_asserv_current = 0;
uint32_t current_meas[NB_PHASE];

float duty_cycle_goal;
float duty_cycle_current = DUTY_CYCLE_STOP;

void asserv_init() {
	// Current measure
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
	HAL_ADC_Start_DMA(&hadc1, current_meas, NB_PHASE);
	//HAL_TIM_Base_Start(&htim1);

	// Start TIM2_IT to asserv speed
	HAL_TIM_Base_Start_IT(&htim2);
}

void correcteur(float k, float ki, float input, float* oldValue) {
	float b0 = k + ki * T_MEAS_SPEED;
	float b1 = -k;
	float a0 = 1;
	float a1 = -1;

	oldValue[1] = - a1 * oldValue[1] + b0 * input + b1 * oldValue[0];
	oldValue[0] = input;
}

void asserv_speed() {
	timer_it_asserv_speed = 1;
}

void asserv_current() {
	timer_it_asserv_current = 1;
}

void asserv_loop() {
	static float temp_speed[2], temp_current[2]; // temp = oldValue
	// speed_consigne = temp_speed[1]

	if (timer_it_asserv_speed == 1) { // Asserv tension
		unsigned int cnt = htim3.Instance->CNT; // Get encoder count
		htim3.Instance->CNT = 0; // reset count
		correcteur(K_SPEED, KI_SPEED, (float) cnt, temp_speed); // output = temp_...[1]
		//setValue(output);
		timer_it_asserv_speed = 0;
		timer_it_asserv_current = 1; // Force current asserv after tension asserv
	}
	if (timer_it_asserv_current == 1) { // asserv current
		float input_current = temp_speed[1] - (current_meas[0] + current_meas[1]);
		correcteur(K_CURRENT, KI_CURRENT, input_current, temp_current); // output = temp_...[1]
		motor_set_duty_cycle(temp_current[1]);
		timer_it_asserv_current = 0;
	}
}

void motor_set_duty_cycle(float duty_cycle) {
	duty_cycle_goal = duty_cycle;
	HAL_ADC_Start(&hadc1); // Lance une conversion du courant
	HAL_TIM_Base_Start_IT(&htim7);
}

void asserv_duty_cycle(void) {
	//uint32_t Iadc = HAL_ADC_GetValue(&hadc1); // Lit le courant

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

	//HAL_ADC_Start(&hadc1); // Lance une conversion du courant
}
