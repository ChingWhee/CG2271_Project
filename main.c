/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
 
#define MASK(x) (1 << (x))

osMutexId_t myMutex;

/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
 
int main (void) {
  // System Initialization
  SystemCoreClockUpdate();
  osKernelInitialize();                 // Initialize CMSIS-RTOS
  myMutex = osMutexNew(NULL);
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
