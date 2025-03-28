// buffer.h
#ifndef BUFFER_H
#define BUFFER_H

#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

typedef struct {
	uint8_t start;
	uint8_t forward;
	uint8_t left;
	uint8_t end;
} BUFFER;

void decodeMessage(BUFFER* b, int* forward_speed, int* left_speed);

#endif // BUFFER_H
