/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

#include "motor.h"
#include "buffer.h"

int forward_speed = 0;
int left_speed = 0;

BUFFER b;

/*----------------------------------------------------------------------------
 * ESP32 Packet Parsing Thread
 *---------------------------------------------------------------------------*/
void parse_command_thread(){
	for(;;){
		decodeMessage(&b, &forward_speed, &left_speed);
	}
}

/*----------------------------------------------------------------------------
 * Motor Control Thread
 *---------------------------------------------------------------------------*/
void motor_thread (void *argument) {
  for (;;) {
		move(forward_speed, left_speed);
	}
}
 
/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
void app_main (void *argument) {
 
	
  // ...
  for (;;) {
		//turn_right_smooth(7500, 0, 1);
		//turn_right_smooth(7500, 0, 0);
		
		//move(-3000, 0); // forward
		//osDelay(5000);
		//move(3000, 0); // backward
		//osDelay(5000);
		move(0, 2000); // right
		//osDelay(5000);
		//move(-3000, -1000); // left
		//osDelay(5000);
		
		//move_right_backward_wheel(7500, 0);
		
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
  
	//osThreadNew(motor_thread, NULL, NULL);
	//osThreadNew(parse_command_thread, NULL, NULL);
	
	osKernelStart();                      // Start thread execution
  for (;;) {}
}
