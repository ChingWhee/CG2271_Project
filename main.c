/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

#include "motor.h"
 
/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
void app_main (void *argument) {
 
	move_right_forward_wheel(7500, 0);
  // ...
  for (;;) {
		
		//PTD->PCOR = MASK(0);
		//PTD->PSOR = MASK(1);
		//TPM0_C0V = 7500;
		//TPM0_C1V = 0;
		
		//move(PWM_PERIOD, 0, 0);
		//osDelay(5000);
		//move(0, 0, 0);
		// osDelay(2000);
		// move(3000, 5, 0);
		// osDelay(2000);
	}
}
 
int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
	initMotor();
  // ...
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
  osThreadNew(app_main, NULL, NULL);    // Create application main thread
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
