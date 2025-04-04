/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "uart.h"
#include "motor.h"
#include "buffer.h"
#include "green_led.h"
#include "audio.h"

int isMoving = 0;
int isFinish = 0;

/*----------------------------------------------------------------------------
 * MOTOR
 *---------------------------------------------------------------------------*/
int forward_speed = 0;
int left_speed = 0;

/*----------------------------------------------------------------------------
 * ESP32 Packet Parsing Thread
 *---------------------------------------------------------------------------*/
void parse_command_thread(void *argument){
	for(;;){	
		serialData b;
		osStatus_t status = osMessageQueueGet(tParserMessageQueue, &b, NULL, osWaitForever);
		if (status == osOK) {
			decodeMessage(&b, &forward_speed, &left_speed);
		}
		osDelay(1);
	}
}

/*----------------------------------------------------------------------------
 * Motor Control Thread
 *---------------------------------------------------------------------------*/
void motor_thread (void *argument) {
  for (;;) {
		move(forward_speed, left_speed);
		osDelay(1);
	}
}

/*----------------------------------------------------------------------------
 * MAIN
 *---------------------------------------------------------------------------*/

int main (void) {
  // System Initialization
  SystemCoreClockUpdate();
	initMotor();
	
	Init_UART2(BAUD_RATE);
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
	tParserMessageQueue = Init_Serial_MsgQueue();
	osThreadNew(motor_thread, NULL, NULL);
	osThreadNew(parse_command_thread, NULL, NULL);
	osThreadNew(green_led_thread, NULL, NULL); 
	osThreadNew(red_led_thread, NULL, NULL); 
	osThreadNew(audio_thread, NULL, NULL); 
  osKernelStart();                      // Start thread execution
	
  for (;;) {
	}
		
}
