
/*----------------------------------------------------------------------------
 * UART
 *---------------------------------------------------------------------------*/
#include "uart.h"


volatile uint8_t buf[16];
osMessageQueueId_t tParserMessageQueue;        //Parser message queue for parser thread to receive serial data from UART1_IRQ_Handler

osMessageQueueId_t Init_Serial_MsgQueue(void) {
  return osMessageQueueNew(10, sizeof(serialData), NULL);
}


void InitUART1(uint32_t baud_rate) {
	// Enable clock gating for UART1
  SIM->SCGC4 |= SIM_SCGC4_UART1_MASK;

  // Enable clock gating for PORTE (where UART1 TX/RX is located)
  SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

  // Set PTE1 (UART1 RX) and PTE0 (UART1 TX) to alternative function ALT 3 (UART)
  PORTE->PCR[1] = (PORTE->PCR[1] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(3); // RX
  PORTE->PCR[0] = (PORTE->PCR[0] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(3); // TX
  
  //Disable TX and RX before configuration
  UART1->C2 &= ~(UART_C2_RE_MASK | UART_C2_TE_MASK); 
  
  // Configure UART1: Set baud rate, word length, parity, etc.
	
  uint32_t sbr = DEFAULT_SYSTEM_CLOCK/(baud_rate*16*2);
  UART1->BDH = UART_BDH_SBR(sbr >> 8); // Set baud rate
  UART1->BDL = UART_BDL_SBR(sbr);
  
	// Configure UART1: data structure
  UART1->C1 = 0; // Normal Mode: start + 8 data (lsb first) + stop, parity disabled
  UART1->S2 = 0; // Reset flags
  UART1->C3 = 0; // Parity interrupt disabled
	
	

	//Enable TX RX
  UART1->C2 |= (UART_C2_RE_MASK | UART_C2_TE_MASK); 
      
  //NVIC_SetPriority(UART1_IRQn, 128); 
  //NVIC_ClearPendingIRQ(UART1_IRQn); 
  //NVIC_EnableIRQ(UART1_IRQn);
	//UART1->C2 |= UART_C2_RIE_MASK; 
}

void UART1_IRQHandler(void) {
    NVIC_ClearPendingIRQ(UART1_IRQn);
    static uint8_t index = 0;
    if (UART1->S1 & UART_S1_RDRF_MASK) {
      uint8_t b = UART1->D;
      if(b == 0xFE){
        index = 0;
        buf[index++] = b;
      } else if(index >= PACKET_SIZE && b == 0xFF) {
        buf[index] = b;
        osStatus_t status = osMessageQueuePut(tParserMessageQueue, &buf, 0U, 0U);
        index = 0;
      } else if (index < sizeof(buf) - 1) {
        buf[index++] = b;
      } else { //invalid message, reset it
				index = 0;
			}
    }
}
