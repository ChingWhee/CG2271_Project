#include "buffer.h"
#include "motor.h"

void decodeMessage(BUFFER* b, int* forward_speed, int* left_speed) {
	uint8_t forward_percentage = b->forward;
	*forward_speed = (forward_percentage / 100) * PWM_PERIOD;
	uint8_t left_percentage = b->left;
	*left_speed = (left_percentage / 100) * PWM_PERIOD;
}