/*
 * asserv.h
 *
 *  Created on: Nov 11, 2025
 *      Author: nicolas
 */

#ifndef INC_MOTOR_CONTROL_ASSERV_H_
#define INC_MOTOR_CONTROL_ASSERV_H_

#include "main.h"

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim7;

void asserv_init(void);
void motor_set_duty_cycle(float duty_cycle);
void asserv_duty_cycle(void);

#endif /* INC_MOTOR_CONTROL_ASSERV_H_ */
