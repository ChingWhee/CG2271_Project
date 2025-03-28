// motor.h
#ifndef UART_H
#define UART_H

#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

#define PACKET_SIZE 6

typedef struct {
	uint8_t start;
  int16_t forward;
  int16_t right; 
	uint8_t end;
} serialData;

osMessageQueueId_t Init_Serial_MsgQueue(void);
void InitUART1(uint32_t baud_rate);
void UART1_IRQHandler(void) ;

extern osMessageQueueId_t tParserMessageQueue;


#endif 
