
/*----------------------------------------------------------------------------
 * UART
 *---------------------------------------------------------------------------*/
#include "uart.h"


volatile uint8_t buf[16];
osMessageQueueId_t tParserMessageQueue;        //Parser message queue for parser thread to receive serial data from UART1_IRQ_Handler

osMessageQueueId_t Init_Serial_MsgQueue(void) {
  return osMessageQueueNew(10, sizeof(serialData), NULL);
}

// Define TX and RX queues
Q_T TxQ, RxQ;

// Initialize the queue
void Q_Init(Q_T *q) {
    for (unsigned int i = 0; i < Q_SIZE; i++) {
        q->Data[i] = 0; // Initialize data to 0 for debugging clarity
    }
    q->Head = 0;
    q->Tail = 0;
    q->Size = 0;
}

// Check if queue is empty
int Q_Empty(Q_T *q) {
    return q->Size == 0;
}

// Check if queue is full
int Q_Full(Q_T *q) {
    return q->Size == Q_SIZE;
}

// Enqueue (add) element to queue
int Q_Enqueue(Q_T *q, unsigned char d) {
    if (!Q_Full(q)) { // Ensure queue is not full
        q->Data[q->Tail] = d;
        q->Tail = (q->Tail + 1) % Q_SIZE; // Wrap around using modulus
        q->Size++;
        return 1; // Success
    }
    return 0; // Failure (queue full)
}

// Dequeue (remove) element from queue
unsigned char Q_Dequeue(Q_T *q) {
    unsigned char t = 0;
    if (!Q_Empty(q)) { // Ensure queue is not empty
        t = q->Data[q->Head];
        q->Data[q->Head] = 0; // Clear data for debugging
        q->Head = (q->Head + 1) % Q_SIZE; // Wrap around using modulus
        q->Size--;
    }
    return t; // Return dequeued value
}


void Init_UART2(uint32_t baud_rate) {
	uint32_t divisor;
	// enable clock to UART and Port E
	SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	
	// Page 162 - PTE22 Alt 4 = UART2 TX
	PORTE->PCR[UART_TX_PORTE22] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[UART_TX_PORTE22] |= PORT_PCR_MUX(4);
	// Page 162 - PTE23 Alt 4 = UART2 RX
	PORTE->PCR[UART_RX_PORT23] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[UART_RX_PORT23] |= PORT_PCR_MUX(4);
	
	// UART2->C2 - UART Control Register 2 (Page 753)
	// This turns them off (TX/RX Enable) 
	// ensure tx and rx are disabled before configuration
	UART2->C2 &= ~((UART_C2_TE_MASK) | (UART_C2_RE_MASK));
	
	// Set baud rate to 9600 baud
	divisor = DEFAULT_SYSTEM_CLOCK/(baud_rate*16*2);
	UART2->BDH = UART_BDH_SBR(divisor>>8);
	UART2->BDL = UART_BDL_SBR(divisor);
	
	// Page 751 - These are just to disable parity errors
	UART2->C1 = 0;  // Sets last bit PT (Parity) to 0 (no parity)
	UART2->S2 = 0;  // RAF Receiver Active Flag set to 0 - receiver waiting for start bit
	UART2->C3 = 0;  // Parity Error Interrupt set to 0 - disabled
	
	// Enabling interrupts
	NVIC_SetPriority(UART2_IRQn, 128);
	NVIC_ClearPendingIRQ(UART2_IRQn);
	NVIC_EnableIRQ(UART2_IRQn);
	
   Q_Init(&RxQ);
	
	// Enable transmitter and receiver
	UART2->C2 |= UART_C2_RIE_MASK;
	UART2->C2 |= UART_C2_RE_MASK;
}

uint8_t UART2_Receive_Poll(void) {
	// wait until receive data register is full
	while (!(UART2->S1 & UART_S1_RDRF_MASK)); // poll until receive register is full
	return UART2->D;
}


void UART2_IRQHandler(void) {
    NVIC_ClearPendingIRQ(UART2_IRQn);
		static size_t index = 0;
		static serialData packet;
    // Receive interrupt handling - receive data register is full =
    if (UART2->S1 & UART_S1_RDRF_MASK) {
        uint8_t byte = UART2->D;
				uint8_t *ptr = (uint8_t *)&packet;  // Pointer to struct memory
			
				if(byte == 0xFE){
					index = 0;
					ptr[index++] = byte;
				} else if(index >= (PACKET_SIZE - 1) && byte == 0xFF) {
					ptr[index] = byte;
					index = 0;
					osStatus_t status = osMessageQueuePut(tParserMessageQueue, ptr, 0U, 0U);
				} else if (index > 0 && index < PACKET_SIZE) {
					ptr[index++] = byte;
				} else {
					index = -1;
						// Error handling: Queue is full
						// Consider a better approach instead of infinite loop
						// Example: Set a flag or discard the oldest entry
				}
		}

    // Error handling (Overrun, Noise, Framing, Parity errors)
    if (UART2->S1 & (UART_S1_OR_MASK | UART_S1_NF_MASK | UART_S1_FE_MASK | UART_S1_PF_MASK)) {
        // Clear error flags by reading S1 and D registers
        (void)UART2->D;  // Read D register to clear the error flags
    }
}