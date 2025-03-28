// buffer.h
#ifndef BUFFER_H
#define BUFFER_H

#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

typedef struct {
	uint8_t start;
  int8_t forward;
  int8_t right; 
	uint8_t end;
} serialData;

void decodeMessage(serialData* b, int* forward_speed, int* left_speed);

#endif // BUFFER_H
