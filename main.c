/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

#include "green_led.h"
#include "audio.h"
 
osMutexId_t myMutex;
int isMoving = 1;
int isFinish = 0;

/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
 
int main (void) {
  // System Initialization
  SystemCoreClockUpdate();
  osKernelInitialize();                 // Initialize CMSIS-RTOS
  myMutex = osMutexNew(NULL);
	osThreadNew(green_led_thread, NULL, NULL); 
	osThreadNew(red_led_thread, NULL, NULL); 
	osThreadNew(audio_thread, NULL, NULL); 
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
