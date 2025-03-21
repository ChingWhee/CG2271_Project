#include "led.h"
#include "MKL25Z4.h"
#include "cmsis_os2.h"

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

void OffAllGreenLed()
{
	for (int i = 0; i < 8; i++) {
		PTC->PCOR |= MASK(i);
	}
	for (int i = 10; i < 12; i++) {
		PTC->PCOR |= MASK(i);
	}
}

void OnAllGreenLed() 
{
	for (int i = 0; i < 8; i++) {
		PTC->PSOR |= MASK(i);
	}
	for (int i = 10; i < 12; i++) {
		PTC->PSOR |= MASK(i);
	}
}

void OnGreenLed(int led)
{
	PTC->PSOR |= MASK(led);
}

void green_led_thread (void *argument) {
 
	int count = 0;
  for (;;) {
		OffAllGreenLed();
		OnGreenLed(count);
		// If count is 8, skip to 10
		// If count is 12, reset to 0
		count++;
		count = (count == 8) ? 10 : ((count == 12) ? 0 : count);
		osDelay(500);
	}
}