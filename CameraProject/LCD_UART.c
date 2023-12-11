#include <stdio.h>
#include "LCD_UART.h"

#define UART_FR_TXFF            0x00000020  // UART Transmit FIFO Full
#define UART_FR_RXFE            0x00000010  // UART Receive FIFO Empty
#define UART_LCRH_WLEN_8        0x00000060  // 8 bit word length
#define UART_LCRH_FEN           0x00000010  // UART Enable FIFOs
#define UART_CTL_UARTEN         0x00000001  // UART Enable

void LCD_UART_Init() { 
	// 1. uart configurations 
	SYSCTL_RCGCUART_R |= 0x08; // activate UART3
	SYSCTL_RCGCGPIO_R |= 0x04; // activate port C
	UART3_CTL_R &= ~UART_CTL_UARTEN; //disable UART 
	UART3_IBRD_R = 520; 
	UART3_FBRD_R = 53; // baud rate calculation stuff -> we want a baud rate of 9600! 
	UART3_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN); // specifications of uart 
	UART3_CTL_R |= UART_CTL_UARTEN; // re-enable uart 
	// 2. gpio stuff with port c (using pc6 and pc7) 
	GPIO_PORTC_AFSEL_R |= 0xC0; // enable alt func on pc6-7 
	GPIO_PORTC_DEN_R |= 0xC0; // enable digital i/o on pc6-7
	GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0x00FFFFFF)+0x11000000; 
	GPIO_PORTC_AMSEL_R &= ~0x03; // disable analog functionality 
}

void LCD_Clear() { 
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 0xFF;
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 0xCD; 	
	
	LCD_InData(); 
}

void LCD_DrawRectangle() { 
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 0x0; 
}

char LCD_InData() { 
	while ((UART3_FR_R & UART_FR_RXFE) != 0); 
	return (UART3_DR_R&0xFF);
}

void LCD_WriteString(char * string) { 
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 0x00;
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 0x18;
	for (int i = 0; string[i] != '\0'; i++) { 
		while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
		UART3_DR_R = string[i];
	}
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = '\0'; // string needs to be null terminated 
	LCD_InData(); 
	LCD_InData(); 
	LCD_InData(); 
}

/********* SD CARD FUNCTIONS **************/ 

void LCD_MediaInit() { 
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 0xFF;
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 0x89;
	
	if (LCD_InData() != 0x06) LCD_WriteString("Media Init Command Unsuccessful \n"); 
	if (LCD_InData() == 1) LCD_WriteString("Actually you just misread \n"); 
	if (LCD_InData() == 0) LCD_WriteString("SD Card not present! \n"); 
}

void LCD_SetSectorAddress(uint32_t sector_location) { 
	// command 
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 0xFF;
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 0x92;
	// hiword
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = (sector_location & 0xFF000000) >> 24;
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = (sector_location & 0x00FF0000) >> 16;
	// loword
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = (sector_location & 0x0000FF00) >> 8;
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = (sector_location & 0x000000FF);
	
	LCD_InData(); 
}

void LCD_WriteSector(const uint8_t source[]) { 
	int i;
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 0x00;
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 0x17;
	for (i = 0; i < 512; ++i) { 
		while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
		UART3_DR_R = source[i]; // iterating through the source array
	}
	
	LCD_InData(); 
	LCD_InData(); 
	if (LCD_InData() == 0) LCD_WriteString("Write Media Attempt Failed \n");  
}

void LCD_FlushMedia() { 
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 0xFF;
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 0x8A;
	
	LCD_InData(); 
	LCD_InData(); 
	if (LCD_InData() == 0) LCD_WriteString("Flush Media Attempt Failed \n");  
}

void LCD_DisplayImage(uint16_t x_pos, uint16_t y_pos) { 
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 0xFF;
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 0x8B;
	
	// x
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = (x_pos & 0xFF00) >> 8;
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = (x_pos & 0x00FF);
	
	// y
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = (y_pos & 0xFF00) >> 8;
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = (y_pos & 0x00FF);
	
	if (LCD_InData() != 0x06) LCD_WriteString("Unable to Display Image \n"); 
}


/********* FILE SYSTEM FUNCTIONS **********/ 
/*
void LCD_FileMount() { 
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 0xFF; 
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 0x03; 
	
	if (LCD_InData() != 0x06) LCD_WriteString("file mount is fucked");  
	LCD_InData(); 
	LCD_InData(); 
} 

// unmount the file system 
void LCD_FileUnmount() { 
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 0xFF; 
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 0x02; 
	
	if (LCD_InData() != 0x06) LCD_WriteString("file unmount is fucked");
}

// opening the file, setting it to designated mode 
// returns the handle 
uint16_t LCD_FileOpen(char * file_name, char mode) { 
	// command
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 0x00;
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 0x0A;
	
	// file name 
	for (int i = 0; file_name[i] != '\0'; ++i) { 
		while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
		UART3_DR_R = file_name[i]; 
	}
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = '\0'; // all strings are null terminated, including file names 
	
	// set the mode 
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = mode; // write mode  
	
	// get ack signal: 
	if (LCD_InData() != 0x06) LCD_WriteString("file open is fucked");  
	
	uint16_t handle = 0x0000;  
	handle |= LCD_InData(); 
	handle = handle << 8; 
	handle |= LCD_InData(); 
	return handle;
}

// writing to file once it is opened 
void LCD_FileWrite(uint16_t num_bytes, uint16_t handle) { 
	// command
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait
	UART3_DR_R = 0x00; 
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait
	UART3_DR_R = 0x10; 
	
	// how many bytes are we sending 
	if (num_bytes > 0xFF) { 
		while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait
		UART3_DR_R = (num_bytes & 0xFF00) >> 8;
		while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait
		UART3_DR_R = (num_bytes & 0xFF); 		
	} else { 
		while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait
		UART3_DR_R = 0x00;
		while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait
		UART3_DR_R = (num_bytes & 0xFF); 		
	}
	
	// actually WRITE the data 
	for (int z = 0; z < num_bytes; ++z) { 
		while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait
		UART3_DR_R = image_array[z]; 
	}
	
	// what is the handle of the file that we're sending to
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait
	UART3_DR_R = (handle & 0xFF00) >> 8; 	
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait
	UART3_DR_R = (handle & 0xFF); 
	
	if (LCD_InData() != 0x06) LCD_WriteString("file write is fucked"); 
	LCD_InData(); 
	LCD_InData(); 
}
 
// close the file 
void LCD_FileClose(uint16_t handle) { 
	// command
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 0xFF; 
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 0x18;
	
	// handle 
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = (handle & 0xFF00) >> 8; 
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = (handle & 0xFF); 	
	
	if (LCD_InData() != 0x06) LCD_WriteString("file close is fucked"); 
	LCD_InData(); 
	LCD_InData();
}

// we want to match with "IMG_*" 
int LCD_CountFiles() { 
	// command
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 0x00; 
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 0x02;
	
	// file we looking for 
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 'I'; 
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 'M';
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 'G'; 
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = '_';
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = '*'; 
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = '\0';
	
	LCD_InData();
	
	int num_files = 0; 
	num_files |= LCD_InData(); 
	num_files = num_files << 8; 
	num_files |= LCD_InData(); 
	
	return num_files; 
}	


void LCD_DisplayImage(uint16_t handle) { 
	// command
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 0xFF; 
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 0x11;
	
	// position x bits (0,0)
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 0x00; 
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 0x00;
	
	// position y bits (0,0) 
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 0x00; 
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = 0x00;
	
	// handle bits: 
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = (handle & 0xFF00) >> 8; 
	while ((UART3_FR_R & UART_FR_TXFF) != 0); // busy wait 
	UART3_DR_R = (handle & 0xFF);
	
	if (LCD_InData() != 0x06) LCD_WriteString("Display Image is fucked"); 
	LCD_InData();
	LCD_InData(); 
}
*/