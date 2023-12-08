#include <stdint.h>
#include "inc/tm4c123gh6pm.h" 
#include "inc/CortexM.h"

extern uint8_t flag; 

// initialize all the dma uart4 stuff 
void DMA_UART_Init(void); 

// get the status of DMA (busy waiting) 
uint32_t DMA_UART_Status(void); 

// transfer data from source to destination
// we're reading from an i/o port, so that's 32 bit 
// destination is 32 bit if 32 bit array, 8 bit if char array, etc. 
// count is number of BYTES to transfer 
void DMA_UART_Transfer(volatile uint32_t *source, uint8_t *destination, uint32_t count); 

void DMA_UART_Enable(void); 

void DMA_UART_Disable(void); 
