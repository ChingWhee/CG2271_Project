#include "green_led.h"

extern volatile int moving_flag;
	
void InitLed(void)
{
	// Enable Clock to PORTC
	SIM->SCGC5 |= (SIM_SCGC5_PORTC_MASK);

	// Configure MUX settings to make all green LED pins `
	// PTC0 - 7
	for (int i = 0; i < 9; i++) {
		PORTC->PCR[i] &= ~PORT_PCR_MUX_MASK;
		PORTC->PCR[i] |= PORT_PCR_MUX(1);
	}
	// PTC10 - 11
	for (int i = 10; i < 12; i++) {
		PORTC->PCR[i] &= ~PORT_PCR_MUX_MASK;
		PORTC->PCR[i] |= PORT_PCR_MUX(1);
	}

	// Set Data Direction Registers for PortC
	for (int i = 0; i < 9; i++) {
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

//active-low
void onRedLED(){
	PTC->PCOR |= MASK(RED_LED);
}

//active-low
void offRedLED(){
	PTC->PSOR |= MASK(RED_LED);
}

void red_led_thread(void *argument){
	offRedLED();
	for(;;){
		onRedLED();
		uint16_t red_led_delay = isMoving ? 500 : 250 ;
		osDelay(red_led_delay);
		offRedLED();
		osDelay(red_led_delay);
	}	
}

void green_led_thread (void *argument) {
	int count = 0;
	InitLed();
  OffAllGreenLed();
	offRedLED();
	
  for (;;) {
		if (isMoving) {
			OffAllGreenLed();
			OnGreenLed(count);
			count++;
			// If count is 8, skip to 10
			// If count is 12, reset to 0
			count = (count == 8) ? 10 : ((count == 12) ? 0 : count);
			osDelay(LED_DELAY);
		} else {
			OnAllGreenLed();
			osDelay(LED_DELAY);
		}
	}
}
