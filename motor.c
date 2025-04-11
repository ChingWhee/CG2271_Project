#include "motor.h"

void initMotor(void) {
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;
	
	// Enable Clock Gating for TPM0 and TPM1
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
  SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;
	
	// Configure correct modes for PWM Pin Operation
	PORTD->PCR[RIGHT_FRONT_PWM_FWD] &= ~PORT_PCR_MUX_MASK;
  PORTD->PCR[RIGHT_FRONT_PWM_FWD] |= PORT_PCR_MUX(4);

	PORTD->PCR[RIGHT_FRONT_PWM_BWD] &= ~PORT_PCR_MUX_MASK;
  PORTD->PCR[RIGHT_FRONT_PWM_BWD] |= PORT_PCR_MUX(4);
	
	
	PORTD->PCR[RIGHT_BACK_PWM_FWD] &= ~PORT_PCR_MUX_MASK;
  PORTD->PCR[RIGHT_BACK_PWM_FWD] |= PORT_PCR_MUX(4);
	
	PORTD->PCR[RIGHT_BACK_PWM_BWD] &= ~PORT_PCR_MUX_MASK;
  PORTD->PCR[RIGHT_BACK_PWM_BWD] |= PORT_PCR_MUX(4);
	
	
	PORTE->PCR[LEFT_FRONT_PWM_FWD] &= ~PORT_PCR_MUX_MASK;
  PORTE->PCR[LEFT_FRONT_PWM_FWD] |= PORT_PCR_MUX(3);
	
	PORTE->PCR[LEFT_FRONT_PWM_BWD] &= ~PORT_PCR_MUX_MASK;
  PORTE->PCR[LEFT_FRONT_PWM_BWD] |= PORT_PCR_MUX(3);
	
	
	PORTE->PCR[LEFT_BACK_PWM_FWD] &= ~PORT_PCR_MUX_MASK;
  PORTE->PCR[LEFT_BACK_PWM_FWD] |= PORT_PCR_MUX(3);
	
	PORTE->PCR[LEFT_BACK_PWM_BWD] &= ~PORT_PCR_MUX_MASK;
  PORTE->PCR[LEFT_BACK_PWM_BWD] |= PORT_PCR_MUX(3);
	
	// Select clock for TPM module
  SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
  SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1); // MCGFLLCLK or MCGPLLCLK/2
	
	// Set Modulo Value 48000000 / 128 = 375000 / 50Hz = 7500
	TPM0->MOD = PWM_PERIOD;
  TPM1->MOD = PWM_PERIOD;
	
	/* Edge-Aligned PWM */
  // Update Sns register: CMOD = 01, PS = 111 (128)
	TPM1->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM1->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	TPM1->SC &= ~(TPM_SC_CPWMS_MASK);
	
	TPM0->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM0->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	TPM0->SC &= ~(TPM_SC_CPWMS_MASK);
	
	//Enable PWM on TPM0 Channel 0 -> PTD0
	TPM0_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM0_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	//Enable PWM on TPM0 Channel 1 -> PTD1
	TPM0_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM0_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	//Enable PWM on TPM0 Channel 4 -> PTD4
	TPM0_C4SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM0_C4SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	//Enable PWM on TPM0 Channel 5 -> PTD5
	TPM0_C5SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM0_C5SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	//Enable PWM on TPM1 Channel 0 -> PTE20
	TPM1_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	//Enable PWM on TPM1 Channel 1 -> PTE21
	TPM1_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM1_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	//Enable PWM on TPM0 Channel 2 -> PTE29
	TPM0_C2SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM0_C2SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	//Enable PWM on TPM0 Channel 3 -> PTE30
	TPM0_C3SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM0_C3SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
}

void move_right_forward_wheel(int speed, int direction) {
	if (speed < 0) speed = 0;
	if (direction) {
		TPM0_C0V = 0;
		TPM0_C1V = speed;
	} else {
		TPM0_C0V = speed;
		TPM0_C1V = 0;
	}
}

void move_right_backward_wheel(int speed, int direction) {
	if (speed < 0) speed = 0;
	if (direction) {
		TPM0_C4V = 0;
		TPM0_C5V = speed;
	} else {
		TPM0_C4V = speed;
		TPM0_C5V = 0;
	}
}

void move_left_forward_wheel(int speed, int direction) {
	if (speed < 0) speed = 0;
	if (direction) {
		TPM1_C0V = 0;
		TPM1_C1V = speed;
	} else {
		TPM1_C0V = speed;
		TPM1_C1V = 0;
	}
}

void move_left_backward_wheel(int speed, int direction) {
	if (speed < 0) speed = 0;
	if (direction) {
		TPM0_C2V = 0;
		TPM0_C3V = speed;
	} else {
		TPM0_C2V = speed;
		TPM0_C3V = 0;
	}
}

void move_forward_or_backward(int speed, int direction) {
  move_right_forward_wheel(speed, direction);
	move_right_backward_wheel(speed, direction);
  move_left_forward_wheel(speed, direction);
	move_left_backward_wheel(speed, direction);
}

void turn_right_on_spot(int speed) {
	speed = speed / 2 - 500;
	if (speed < 0) {
		speed = 0;
	}
	
	move_right_forward_wheel(speed, 0);
	move_right_backward_wheel(speed, 0);
	move_left_forward_wheel(speed, 1);
	move_left_backward_wheel(speed, 1);
}

void turn_left_on_spot(int speed) {
	speed = speed / 2 - 500;
	if (speed < 0) {
		speed = 0;
	}
		
	move_right_forward_wheel(speed, 1);
	move_right_backward_wheel(speed, 1);
	move_left_forward_wheel(speed, 0);
	move_left_backward_wheel(speed, 0);
}

void turn_left_smooth(int base_speed, int turn_factor, int direction) {
  //move_left_forward_wheel(base_speed - turn_factor, direction);
	//move_left_backward_wheel(base_speed - turn_factor, direction);
	move_left_forward_wheel(0, direction);
	move_left_backward_wheel(0, direction);
  move_right_forward_wheel(base_speed, direction);
	move_right_backward_wheel(base_speed, direction);
}

void turn_right_smooth(int base_speed, int turn_factor, int direction) {
	move_left_forward_wheel(base_speed, direction);
	move_left_backward_wheel(base_speed, direction);
	//move_right_forward_wheel(base_speed - turn_factor, direction);
	//move_right_backward_wheel(base_speed - turn_factor, direction);
	move_right_forward_wheel(0, direction);
	move_right_backward_wheel(0, direction);
}

void stop_car() {
  move_left_forward_wheel(0, 1);
	move_left_backward_wheel(0, 1);
  move_right_forward_wheel(0, 1);
  move_right_backward_wheel(0, 1);
}


void move(int base_speed, int turn_factor){
	// Forward is Negative
	// Left is Negative
	int fwd_direction;
	if (base_speed < 0) {
		base_speed = -base_speed;
		fwd_direction = 1;
	} else {
		fwd_direction = 0;
	}
	
	int right_direction;
	if (turn_factor < 0) {
		turn_factor = -turn_factor;
		right_direction = 0;
	} else {
		right_direction = 1;
	}
	
	if (base_speed == 0) {
		if (turn_factor != 0) {
			if (right_direction) turn_right_on_spot(turn_factor);
			else turn_left_on_spot(turn_factor);
		} else {
			stop_car();
		}
	}
	
	else {
		if (turn_factor == 0) {
			move_forward_or_backward(base_speed, fwd_direction);
		} else {
			if (right_direction) { // turn right
				turn_right_smooth(base_speed, turn_factor, fwd_direction);
			} else {
				turn_left_smooth(base_speed, turn_factor, fwd_direction);
			}
		}
	}
	
}
