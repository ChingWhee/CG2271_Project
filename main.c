/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
 
#define RED_LED 12 // PortB Pin 18
#define GREEN_LED 13 // PortB Pin 19
#define BLUE_LED 16 // PortD Pin 1
#define WHITE_LED 17
#define SWITCH 6 // PortD Pin 6
#define MASK(x) (1 << (x))

#define UART_CLK 24000000 // Assuming a 48 MHz bus clock


osMutexId_t myMutex;

typedef enum {
    RED,
    GREEN,
    BLUE,
		WHITE
} colour_t;

const osThreadAttr_t thread_attr = {
	.priority = osPriorityAboveNormal1
};

static void delay(volatile uint32_t nof) {
    while(nof != 0) {
        __asm("NOP");
        nof--;
    }
}

void InitGPIO(void)
{
	// Enable Clock to PORTB and PORTD
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
	// Configure MUX settings to make all 3 pins GPIO
	PORTC->PCR[RED_LED] &= ~PORT_PCR_MUX_MASK; // 0x700u = 0111 0000 0000 
	PORTC->PCR[RED_LED] |= PORT_PCR_MUX(1); // x << 8 
	PORTC->PCR[GREEN_LED] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[GREEN_LED] |= PORT_PCR_MUX(1);
	PORTC->PCR[BLUE_LED] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[BLUE_LED] |= PORT_PCR_MUX(1);
	PORTC->PCR[WHITE_LED] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[WHITE_LED] |= PORT_PCR_MUX(1);
	// Set Data Direction Registers for PortB and PortD
	PTC->PDDR |= (MASK(RED_LED) | MASK(GREEN_LED) | MASK(WHITE_LED) | MASK(BLUE_LED));
}

void OffLed(colour_t colour)
{
		switch (colour){
		case RED: 
			PTB->PSOR |= MASK(RED_LED);
			break;
		case GREEN: 
			PTB->PSOR |= MASK(GREEN_LED);
			break;
		case BLUE:
			PTD->PSOR |= MASK(BLUE_LED);
			break;
		case WHITE:
			PTD->PSOR |= MASK(WHITE_LED);
			break;
		}
}

void OnLed(colour_t colour) { 
	switch (colour){
		case RED: 
			PTC->PSOR |= MASK(RED_LED);
			break;
		case GREEN: 
			PTC->PSOR |= MASK(GREEN_LED);
			break;
		case BLUE:
			PTC->PSOR |= MASK(BLUE_LED);
			break;
		case WHITE:
			PTC->PSOR |= MASK(WHITE_LED);
			break;
		}
}

void OffRGB()
{
		PTC->PCOR |= MASK(RED_LED);
		PTC->PCOR |= MASK(GREEN_LED);
		PTC->PCOR |= MASK(BLUE_LED);
		PTC->PCOR |= MASK(WHITE_LED);
		
}

/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
void app_main (void *argument) {
 
  // ...
  for (;;) {}
}

void green_led_thread (void *argument) {
 
  for (;;) {
		osMutexAcquire(myMutex, osWaitForever); 
		
		OnLed(GREEN);
		osDelay(1000); 
		// delay(0x80000);
		OffLed(GREEN);
		osDelay(1000); 
		// delay(0x80000);
		
		osMutexRelease(myMutex);
	}
}

/*----------------------------------------------------------------------------
 * UART
 *---------------------------------------------------------------------------*/

#define PACKET_SIZE 6

osMessageQueueId_t tParserMessageQueue;          //Parser message queue for parser thread to receive serial data from UART1_IRQ_Handler
volatile uint8_t buf[16];

 typedef struct {
	uint8_t start;
  int16_t forward;
  int16_t right; 
	uint8_t end;
} serialData;

void Init_Serial_MsgQueue(void) {
  tParserMessageQueue = osMessageQueueNew(10, sizeof(serialData), NULL);
  if(tParserMessageQueue == NULL){
    ;
  }
}


void InitUART1(uint32_t baudRate) {
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
	
  uint32_t sbr = (UART_CLK / (16 * baudRate)); // Baud rate calculation
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

void parser_thread(void *argument) {
    serialData recv_packet;

    while (1) {
        osStatus_t status = osMessageQueueGet(tParserMessageQueue, &recv_packet, NULL, osWaitForever);
        if (status == osOK) {
					if (recv_packet.forward > 0) {
						OnLed(GREEN);
					} else if (recv_packet.forward < 0) {
						OnLed(BLUE);
					}
					if (recv_packet.right > 0) {
						OnLed(WHITE);
					} else if (recv_packet.right < 0) {
						OnLed(RED);
					}
        }
				osDelay(1000);
				OffRGB();
    }
}

uint8_t UART1_Receive_Poll(void) {
	// wait until receive data register is full
	while (!(UART1->S1 & UART_S1_RDRF_MASK)); // poll until receive register is full
	return UART1->D;
}

void decode_packet(serialData* recv_packet) {
	if (recv_packet -> forward > 0) {
		OnLed(GREEN);
	} else if (recv_packet -> forward < 0) {
		OnLed(BLUE);
	}
	if (recv_packet -> right > 0) {
		OnLed(WHITE);
	} else if (recv_packet -> right < 0) {
		OnLed(RED);
	}
}


/*----------------------------------------------------------------------------
 * MAIN
 *---------------------------------------------------------------------------*/

int main (void) {
  // System Initialization
  SystemCoreClockUpdate();
  InitGPIO();
	InitUART1(9600);
	//Init_Serial_MsgQueue();
	OffRGB();	
 
  //osKernelInitialize();                 // Initialize CMSIS-RTOS
  //myMutex = osMutexNew(NULL);
	//osThreadNew(app_main, NULL, NULL);    // Create application main thread
	//osThreadNew(red_led_thread, NULL, NULL); 
	//osThreadNew(green_led_thread, NULL, NULL); 
	//osThreadNew(parser_thread, NULL, NULL); 
  //osKernelStart();                      // Start thread execution
	serialData packet;
	
  for (;;) {
		uint8_t *ptr = (uint8_t *)&packet;  // Pointer to struct memory
    for (size_t i = 0; i < PACKET_SIZE; i++) {
        ptr[i] = UART1_Receive_Poll();  // Receive one byte at a time
    }
		decode_packet(&packet);
		delay(100);
	}
}
