/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
 
#define RED_LED 18 // PortB Pin 18
#define GREEN_LED 19 // PortB Pin 19
#define BLUE_LED 1 // PortD Pin 1
#define SWITCH 6 // PortD Pin 6
#define MASK(x) (1 << (x))

osMutexId_t myMutex;

typedef enum {
    RED,
    GREEN,
    BLUE
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
	SIM->SCGC5 |= ((SIM_SCGC5_PORTB_MASK) | (SIM_SCGC5_PORTD_MASK));
	// Configure MUX settings to make all 3 pins GPIO
	PORTB->PCR[RED_LED] &= ~PORT_PCR_MUX_MASK; // 0x700u = 0111 0000 0000 
	PORTB->PCR[RED_LED] |= PORT_PCR_MUX(1); // x << 8 
	PORTB->PCR[GREEN_LED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED] |= PORT_PCR_MUX(1);
	PORTD->PCR[BLUE_LED] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED] |= PORT_PCR_MUX(1);
	// Set Data Direction Registers for PortB and PortD
	PTB->PDDR |= (MASK(RED_LED) | MASK(GREEN_LED));
	PTD->PDDR |= MASK(BLUE_LED);
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
		}
}

void OnLed(colour_t colour) { 
	switch (colour){
		case RED: 
			PTB->PCOR |= MASK(RED_LED);
			break;
		case GREEN: 
			PTB->PCOR |= MASK(GREEN_LED);
			break;
		case BLUE:
			PTD->PCOR |= MASK(BLUE_LED);
			break;
		}
}

void OffRGB()
{
		PTB->PSOR |= MASK(RED_LED);
		PTB->PSOR |= MASK(GREEN_LED);
		PTD->PSOR |= MASK(BLUE_LED);
}

/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
void app_main (void *argument) {
 
  // ...
  for (;;) {}
}

void red_led_thread (void *argument) {
  for (;;) {
		osMutexAcquire(myMutex, osWaitForever); 
		
		OnLed(RED);
		osDelay(1000); 
		// delay(0x80000);
		OffLed(RED);
		osDelay(1000); 
		// delay(0x80000);
		
		osMutexRelease(myMutex);
	}
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
 
int main (void) {
  // System Initialization
  SystemCoreClockUpdate();
  InitGPIO();
	OffRGB();	
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
  myMutex = osMutexNew(NULL);
	// osThreadNew(app_main, NULL, NULL);    // Create application main thread
	osThreadNew(red_led_thread, NULL, NULL); 
	osThreadNew(green_led_thread, NULL, NULL); 
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
