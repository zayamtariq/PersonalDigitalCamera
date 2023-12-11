#include <stdio.h>
#include "UART.h" 

// these arrays are, i hope, stored in RAM 
uint8_t array[6];
char image_array[512]; 
// char image_array_two[512]; 

#define UART_FR_TXFF            0x00000020  // UART Transmit FIFO Full
#define UART_FR_RXFE            0x00000010  // UART Receive FIFO Empty
#define UART_LCRH_WLEN_8        0x00000060  // 8 bit word length
#define UART_LCRH_FEN           0x00000010  // UART Enable FIFOs
#define UART_CTL_UARTEN         0x00000001  // UART Enable

// TODO: find a port for hardware reset
#define PF1                     (*((volatile uint32_t *)0x40025008))

void UART_Init() { 
	// 1. uart configurations 
	SYSCTL_RCGCUART_R |= 0x10; // activate UART4 
	SYSCTL_RCGCGPIO_R |=  0x04; 		 // activate port C 
	UART4_CTL_R &= ~UART_CTL_UARTEN; // disable UART 
	UART4_IBRD_R = 43; 
	UART4_FBRD_R = 26; // baud rate calculation stuffs 
	UART4_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN); // specifications of the UART 
	UART4_DMACTL_R |= 0x01; // enables to receive dma (i wonder if this should only be on when reading from image??) -> probs not necessary 
	UART4_CTL_R |= UART_CTL_UARTEN;       // re-enable UART
	// 2. gpio stuff with port c (using PC4 and PC5) 
	GPIO_PORTC_AFSEL_R |= 0x30; // enable alt func on PC4-5
	GPIO_PORTC_DEN_R |= 0x30; // enable digital I/O on PC4-5 
	GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0xFF00FFFF)+0x110000; 
	GPIO_PORTC_AMSEL_R &= ~0x03; // disable analog functionality 
	// for the camera: 
	PF1 = 0x02; 
}

// we want to receive an acknowledge signal most of the time 
void UART_InData() {   
	// 1
	while ((UART4_FR_R & UART_FR_RXFE) != 0); 
	array[0] = (UART4_DR_R&0xFF);
	// 2
	while ((UART4_FR_R & UART_FR_RXFE) != 0); 
	array[1] = (UART4_DR_R&0xFF); 
	// 3
	while ((UART4_FR_R & UART_FR_RXFE) != 0); 
	array[2] = (UART4_DR_R&0xFF);
	// 4
	while ((UART4_FR_R & UART_FR_RXFE) != 0); 
	array[3] = (UART4_DR_R&0xFF);
	// 5
	while ((UART4_FR_R & UART_FR_RXFE) != 0); 
	array[4] = (UART4_DR_R&0xFF); 
	// 6
	while ((UART4_FR_R & UART_FR_RXFE) != 0); 
	array[5] = (UART4_DR_R&0xFF); 
}

void UART_OutSync() { 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART4_DR_R = 0xAA; 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART4_DR_R = 0x0D; 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART4_DR_R = 0x00; 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART4_DR_R = 0x00; 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART4_DR_R = 0x00; 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART4_DR_R = 0x00; 
} 

void UART_OutACK() { 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART4_DR_R = 0xAA; 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART4_DR_R = 0x0E; 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART4_DR_R = 0x0D; 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART4_DR_R = 0x00; 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART4_DR_R = 0x00; 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART4_DR_R = 0x00; 
}

void UART_OutInitial() { 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART4_DR_R = 0xAA; 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART4_DR_R = 0x01; 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART4_DR_R = 0x00; 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait - decides image format (RAW) 
	UART4_DR_R = 0x03; 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait - decides raw resolution
	UART4_DR_R = 0x03; // 160 x 120 in RAW 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait - decides jpeg resolution 
	UART4_DR_R = 0x05; // we are using a 320 x 240 display 
	
	UART_InData(); 
} 

// choosing 512 bytes 
void UART_OutPackageSize() { 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART4_DR_R = 0xAA; 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART4_DR_R = 0x06; 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART4_DR_R = 0x08; 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART4_DR_R = 0x00; 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART4_DR_R = 0x02; 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART4_DR_R = 0x00; 
	
	UART_InData(); 
}

void UART_OutSnapshot() { 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART4_DR_R = 0xAA; 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART4_DR_R = 0x05; 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART4_DR_R = 0x01; // RAW  
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART4_DR_R = 0x00; 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART4_DR_R = 0x00; 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART4_DR_R = 0x00; 
	
	UART_InData(); 
}

void UART_OutGetPic() { 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART4_DR_R = 0xAA; 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART4_DR_R = 0x04; 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART4_DR_R = 0x02; // RAW  
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART4_DR_R = 0x00; 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART4_DR_R = 0x00; 
	while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART4_DR_R = 0x00; 
	
	UART_InData(); 
}

void UART_OutCUSTOMACK(uint16_t num_package) { 
	if (num_package <= 0xff) { 
		while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
		UART4_DR_R = 0xAA; 
		while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
		UART4_DR_R = 0x0E; 
		while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
		UART4_DR_R = 0x00; 
		while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
		UART4_DR_R = 0x00; 
		while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
		UART4_DR_R = (num_package & 0x00FF); 
		while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
		UART4_DR_R = 0x00; 
	}
	else { 
		while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
		UART4_DR_R = 0xAA; 
		while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
		UART4_DR_R = 0x0E; 
		while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
		UART4_DR_R = 0x00; 
		while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
		UART4_DR_R = 0x00; 
		while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
		UART4_DR_R = (num_package & 0x00ff); 
		while ((UART4_FR_R & UART_FR_TXFF) != 0); // busy wait 
		UART4_DR_R = (num_package & 0xff00) >> 8; 
	}
}

char UART_ReadSingleByte() { 
	while ((UART4_FR_R & UART_FR_RXFE) != 0); 
	return (UART4_DR_R&0xFF);
}

void UART_InNBytes(int32_t N) { 
	for (int32_t k = 0; k < N; ++k) { 
		while ((UART4_FR_R & UART_FR_RXFE) != 0); 
		image_array[k] = (UART4_DR_R&0xFF);
	}
}	


void Camera_HardwareReset() { 
	DisableInterrupts(); 
	
	int i; 
	PF1 ^= 0x02; 
	for (i = 0; i < 20000000; ++i) {} 
	PF1 ^= 0x02; 
		
	EnableInterrupts(); 
}
