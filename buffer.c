#include "buffer.h"
#include "motor.h"

extern int isMoving;

void decodeMessage(serialData* b, int* forward_speed, int* left_speed) {
	int forward_percentage = b->forward;
	*forward_speed = (forward_percentage / 100.0) * PWM_PERIOD;
	
	int left_percentage = b->right;
	if (left_percentage < 0) left_percentage = left_percentage - 30;
	else if (left_percentage > 0) left_percentage = left_percentage + 30;
	
	if (left_percentage < -100) left_percentage = -100;
	else if (left_percentage > 100) left_percentage = 100;
		
	*left_speed = (left_percentage / 100.0) * PWM_PERIOD;
	
	if (*forward_speed == 0 && *left_speed == 0) {
		isMoving = 0;
	} else {
		isMoving = 1;
	}
}
