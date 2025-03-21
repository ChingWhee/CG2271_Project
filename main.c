/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
 
#define MASK(x) (1 << (x))

osMutexId_t myMutex;

void InitGreenLed(void)
{
	// Enable Clock to PORTC
	SIM->SCGC5 |= (SIM_SCGC5_PORTC_MASK);

	// Configure MUX settings to make all green LED pins `
	// PTC0 - 7
	for (int i = 0; i < 8; i++) {
		PORTC->PCR[i] &= ~PORT_PCR_MUX_MASK;
		PORTC->PCR[i] |= PORT_PCR_MUX(1);
	}
	// PTC10 - 11
	for (int i = 10; i < 12; i++) {
		PORTC->PCR[i] &= ~PORT_PCR_MUX_MASK;
		PORTC->PCR[i] |= PORT_PCR_MUX(1);
	}

	// Set Data Direction Registers for PortC
	for (int i = 0; i < 8; i++) {
		PTC->PDDR |= MASK(i);
	}
	for (int i = 10; i < 12; i++) {
		PTC->PDDR |= MASK(i);
	}
}

/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
 
int main (void) {
  // System Initialization
  SystemCoreClockUpdate();
  osKernelInitialize();                 // Initialize CMSIS-RTOS
  myMutex = osMutexNew(NULL);
	osThreadNew(green_led_thread, NULL, NULL); 
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
