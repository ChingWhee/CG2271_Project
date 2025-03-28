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

#define MASK(x) (1 << (x))

int isMoving = 1;
int isFinish = 0;

void decode_packet(serialData* recv_packet) {
	if (recv_packet -> forward > 0) {
		OnLed(GREEN);
	} else if (recv_packet -> forward < 0) {
		OnLed(BLUE);
	}
	if (recv_packet -> right > 0) {
		OnLed(WHITE);
	} else if (recv_packet -> right < 0) {
		OnLed(RED);
	}
}

/*----------------------------------------------------------------------------
 * MOTOR
 *---------------------------------------------------------------------------*/
int forward_speed = 0;
int left_speed = 0;

/*----------------------------------------------------------------------------
 * ESP32 Packet Parsing Thread
 *---------------------------------------------------------------------------*/
void parse_command_thread(){
	for(;;){	
		serialData b;
		osStatus_t status = osMessageQueueGet(tParserMessageQueue, &b, NULL, osWaitForever);
		if (status == osOK) {
			decodeMessage(&b, &forward_speed, &left_speed);
		}
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
 * MAIN
 *---------------------------------------------------------------------------*/

int main (void) {
  // System Initialization
  SystemCoreClockUpdate();
  InitGPIO();
	initMotor();
	
	Init_UART2(BAUD_RATE);
	OffRGB();	
 
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
