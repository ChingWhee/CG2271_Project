/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "uart.h"
 
#define RED_LED 12 // PortB Pin 18
#define GREEN_LED 13 // PortB Pin 19
#define BLUE_LED 16 // PortD Pin 1
#define WHITE_LED 17
#define SWITCH 6 // PortD Pin 6
#define MASK(x) (1 << (x))


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

/**
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
**/

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
 * MAIN
 *---------------------------------------------------------------------------*/

int main (void) {
  // System Initialization
  SystemCoreClockUpdate();
  InitGPIO();
	//tParserMessageQueue = Init_Serial_MsgQueue();
	Init_UART2(BAUD_RATE);
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
	uint8_t *ptr = (uint8_t *)&packet;  // Pointer to struct memory
	size_t index = 0;
	
  for (;;) {
		uint8_t byte = Q_Dequeue(&RxQ);  // Receive one byte at a time
		if (byte != 0x00) {
			if(byte == 0xFE){
				index = 0;
				ptr[index++] = byte;
			} else if(index >= (PACKET_SIZE - 1) && byte == 0xFF) {
				ptr[index] = byte;
				decode_packet(&packet);
				delay(100);
				index = 0;
			} else if (index > 0 && index < PACKET_SIZE) {
				ptr[index++] = byte;
			} else { //invalid message, reset it
				index = -1;
			}
	}
		
	}
}
