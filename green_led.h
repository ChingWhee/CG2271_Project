#ifndef GREEN_LED_H
#define GREEN_LED_H

#include "MKL25Z4.h"
#include "cmsis_os2.h"

// Green LEDs -- PTC0 - 7, 10 - 11
#define GREEN_LED_0 // Port C Pin 0
#define GREEN_LED_1 // Port C Pin 1
#define GREEN_LED_2 // Port C Pin 2
#define GREEN_LED_3 // Port C Pin 3
#define GREEN_LED_4 // Port C Pin 4
#define GREEN_LED_5 // Port C Pin 5
#define GREEN_LED_6 // Port C Pin 6
#define GREEN_LED_7 // Port C Pin 7
#define GREEN_LED_8 // Port C Pin 10
#define GREEN_LED_9 // Port C Pin 11

#define MASK(x) (1 << (x))

#define LED_DELAY 200

void InitGreenLed(void);
void OffAllGreenLed(void);
void OnAllGreenLed(void);
void OnGreenLed(int led);

void green_led_thread (void *argument);

#endif
