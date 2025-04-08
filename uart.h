// motor.h
#ifndef UART_H
#define UART_H

#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"


#define PACKET_SIZE 5

#define BAUD_RATE 9600
#define UART_TX_PORTE22 22  // Page 162
#define UART_RX_PORT23 23   // Page 162
#define UART2_INT_PRIO 128
#define MASK(x) (1 << (x))
#define Q_SIZE 30


typedef struct {
    unsigned char Data[Q_SIZE]; // Data storage
    unsigned int Head; // Points to the oldest data element
    unsigned int Tail; // Points to the next free space
    unsigned int Size; // Number of elements in queue
} Q_T;

osMessageQueueId_t Init_Serial_MsgQueue(void);
void Init_UART2(uint32_t baud_rate);
//void UART2_IRQHandler(void) ;
uint8_t UART2_Receive_Poll(void);
unsigned char Q_Dequeue(Q_T *q);

extern osMessageQueueId_t tParserMessageQueue;
extern Q_T RxQ;



#endif 
