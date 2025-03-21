// Green LEDs -- PTC0 - 7, 10 - 11
#define GREEN_LED_0 0
#define GREEN_LED_1 1
#define GREEN_LED_2 2
#define GREEN_LED_3 3
#define GREEN_LED_4 4
#define GREEN_LED_5 5
#define GREEN_LED_6 6
#define GREEN_LED_7 7
#define GREEN_LED_8 10
#define GREEN_LED_9 11

#define MASK(x) (1 << (x))

void InitGreenLed(void);
void OffAllGreenLed();
void OnAllGreenLed();
void OnGreenLed(int led);

void green_led_thread (void *argument);