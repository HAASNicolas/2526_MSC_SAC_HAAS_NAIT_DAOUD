/*
 * motor.h
 *
 *  Created on: Nov 11, 2025
 *      Author: nicolas
 */

#ifndef INC_MOTOR_CONTROL_MOTOR_H_
#define INC_MOTOR_CONTROL_MOTOR_H_

#include "main.h"
#include <stdint.h>

extern TIM_HandleTypeDef htim1;

void motor_init(void);
void motor_start(void);
void motor_stop(void);
void motor_set_duty_cycle(float duty); // Définit le rapport cyclique (entre 0 et 1)
void set_speed(int speed);

#endif /* INC_MOTOR_CONTROL_MOTOR_H_ */


