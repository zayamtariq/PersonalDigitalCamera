// #include <stdint.h>
// #include "inc/tm4c123gh6pm.h" 
// #include "inc/CortexM.h"
#include "DMA_UART.h"

extern uint8_t array[6]; 
extern char image_array[512]; 
// extern char image_array_two[512]; 

// initialize to baud rate of: 115200 bits per second   
void UART_Init(void); 

// read in 6 bytes of consecutive data 
void UART_InData(void); 

// read an image one package at a time
// input instance: tells us what number package we're on
void UART_ReadImage(uint32_t instance); 

// output SYNC message 
void UART_OutSync(void); 

// output ACK message 
void UART_OutACK(void); 

// output custom ACK message 
void UART_OutCUSTOMACK(uint16_t num_package); 

// output Initial message for JPEG images 
void UART_OutInitial(void); 

// tell the camera what package size we want
void UART_OutPackageSize(void); 

// take a snapshot - this will need to be triggered by a button press, eventually 
void UART_OutSnapshot(void); 

// get the picture back 
void UART_OutGetPic(void); 

// get a single byte 
char UART_ReadSingleByte(void); 

// read a packet of N bytes of data 
void UART_InNBytes(int32_t N); 

// hardware reset signal to send to camera 
void Camera_HardwareReset(void); 
