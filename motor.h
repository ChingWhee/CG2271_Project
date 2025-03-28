// motor.h
#ifndef MOTOR_H
#define MOTOR_H

#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

#define MASK(x) (1 << (x))

/*----------------------------------------------------------------------------
 * PWM Pins for Motor Driver
 *---------------------------------------------------------------------------*/
 
#define RIGHT_FRONT_PWM_FWD		0		// TPM0_C0V Port D Pin 0 Alt 4
#define RIGHT_FRONT_PWM_BWD 	1		// TPM0_C1V Port D Pin 1 Alt 4
#define RIGHT_BACK_PWM_FWD  	4		// TPM0_C4V Port D Pin 4 Alt 4
#define RIGHT_BACK_PWM_BWD  	5		// TPM0_C5V Port D Pin 5 Alt 4

#define LEFT_FRONT_PWM_FWD		20	// TPM1_C0V Port E Pin 20 Alt 3
#define LEFT_FRONT_PWM_BWD  	21	// TPM1_C1V Port E Pin 21 Alt 3
#define LEFT_BACK_PWM_FWD			29	// TPM0_C2V Port E Pin 29 Alt 3
#define LEFT_BACK_PWM_BWD  		30	// TPM0_C3V Port E Pin 30 Alt 3

#define PWM_PERIOD 						7500

/*----------------------------------------------------------------------------
 * Helper Functions
 *---------------------------------------------------------------------------*/

void initMotor(void);
void move_right_forward_wheel(uint16_t speed, uint8_t direction);
void move_right_backward_wheel(uint16_t speed, uint8_t direction);
void move_left_forward_wheel(uint16_t speed, uint8_t direction);
void move_left_backward_wheel(uint16_t speed, uint8_t direction);

void move_forward_or_backward(uint16_t speed, uint8_t direction);

void turn_right_on_spot(uint16_t speed);
void turn_left_on_spot(uint16_t speed);
void turn_left_smooth(uint16_t base_speed, uint16_t turn_factor, uint8_t direction);
void turn_right_smooth(uint16_t base_speed, uint16_t turn_factor, uint8_t direction);
void stop_car(void);

void move(int base_speed, int turn_factor);

#endif // MOTOR_H