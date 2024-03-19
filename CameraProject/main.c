#include <stdint.h>
#include "inc/PLL.h" 
// #include "UART.h" 
#include "LCD_UART.h" // LCD_UART.h -> UART.h 
#include "inc/Timer1A.h" 
#include "inc/Unified_Port_Init.h"
#include "UART0.h" 

// these things are mostly predetermined by the programmer, i think. see no purpose in giving user control of these things. 
//#define PACKAGE_SIZE 506 
//#define IMAGE_SIZE   76800 
//#define NUM_PACKAGES IMAGE_SIZE / (PACKAGE_SIZE - 6) 

// for DMA: 
// #define TRANSFER_SIZE 512 // sending 512 bytes 
// uint8_t Buffer[TRANSFER_SIZE]; 
// #define DMA_SOURCE 							((volatile uint32_t *)0x40010000)
// #define DMA_SOURCE       (*((volatile uint32_t *)0x40006040))    // PC4, apparently 

static uint8_t LCD_TestParagraph[512] = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. Vivamus e";

// picture of a stick figure to send to the lcd display
/*
const static uint8_t LCD_TestImage[512] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x0f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xfe, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 
	0xff, 0x80, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0x00, 0x03, 0xff, 0x80, 0x07, 0xff, 0xff, 
	0xfc, 0x00, 0xff, 0xff, 0xf0, 0x01, 0xff, 0xff, 0xf8, 0x0f, 0xff, 0xff, 0xfe, 0x00, 0xff, 0xff, 
	0xf8, 0x3f, 0xff, 0xff, 0xff, 0xe0, 0x7f, 0xff, 0xf0, 0x7f, 0xff, 0xff, 0xff, 0xf8, 0x1f, 0xff, 
	0xe0, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x0f, 0xff, 0xe1, 0xff, 0xc7, 0xff, 0xc3, 0xfe, 0x0f, 0xff, 
	0xe3, 0xff, 0xc1, 0xff, 0xc3, 0xff, 0x07, 0xff, 0xc3, 0xff, 0x81, 0xff, 0x80, 0xff, 0x83, 0xff, 
	0xc3, 0xff, 0x81, 0xff, 0x80, 0xff, 0xc3, 0xff, 0xc3, 0xff, 0x81, 0xff, 0x00, 0xff, 0xc3, 0xff, 
	0xc7, 0xff, 0x81, 0xff, 0x03, 0xff, 0xe1, 0xff, 0xc7, 0xff, 0x81, 0xff, 0x03, 0xff, 0xe1, 0xff, 
	0xc7, 0xff, 0xff, 0xff, 0x03, 0xff, 0xe1, 0xff, 0xc3, 0xff, 0xff, 0xff, 0x03, 0xff, 0xf1, 0xff, 
	0xc3, 0xff, 0xff, 0xff, 0x03, 0xff, 0xf1, 0xff, 0xe1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xff, 
	0xe1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xff, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xff, 
	0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xff, 0xf0, 0xff, 0xe1, 0xff, 0xff, 0xff, 0xf1, 0xff, 
	0xf8, 0x7f, 0xe0, 0x00, 0xff, 0xff, 0xf1, 0xff, 0xf8, 0x7f, 0xe0, 0x00, 0x0f, 0xff, 0xf1, 0xff, 
	0xfc, 0x3f, 0xf8, 0x00, 0x00, 0x7f, 0xe1, 0xff, 0xfc, 0x1f, 0xff, 0xfc, 0x00, 0x07, 0xe1, 0xff, 
	0xfe, 0x0f, 0xff, 0xff, 0xc0, 0x07, 0xc3, 0xff, 0xfe, 0x00, 0xff, 0xff, 0xfe, 0x07, 0x83, 0xff, 
	0xff, 0x80, 0x0f, 0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xc0, 0x00, 0xff, 0xff, 0xfc, 0x0f, 0xff, 
	0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x3f, 0xff, 
	0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x01, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x01, 0x00, 0x3f, 0xff, 0xff, 0xff, 
	0xfe, 0x00, 0x0f, 0x18, 0x07, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x7f, 0x1e, 0x00, 0xff, 0xff, 0xff, 
	0xfc, 0x0f, 0xff, 0x1f, 0x00, 0x3f, 0xff, 0xff, 0xfc, 0x7f, 0xff, 0x1f, 0xc0, 0x1f, 0xff, 0xff, 
	0xfc, 0x7f, 0xfe, 0x1f, 0xf8, 0x07, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x1f, 0xff, 0x00, 0x3f, 0xff, 
	0xff, 0xff, 0xfe, 0x3f, 0xff, 0x80, 0x3f, 0xff, 0xff, 0xff, 0xfe, 0x3f, 0xff, 0xc0, 0x3f, 0xff, 
	0xff, 0xff, 0xfe, 0x3f, 0xff, 0xfe, 0x3f, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x7f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xf0, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x1f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xfe, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x02, 0x07, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xf8, 0x0f, 0x81, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x7f, 0xc0, 0x7f, 0xff, 0xff, 0xff, 
	0xff, 0x00, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xfe, 0x01, 0xff, 0xf8, 0x07, 0xff, 0xff, 0xff}; 
*/ 

// main for lcd tests 
// intended purposes: 
// 1. should send photo data to LCD display via UART communication
// 2. save data to sd card 
// 3. read sd card data and show image on lcd display 
void LCD_main1() { 
	DisableInterrupts();
	PLL_Init(Bus80MHz);  
	Unified_Port_Init(); // initialize all ports 
	LCD_UART_Init(); // initialize lcd communication 
	EnableInterrupts(); 
	
	LCD_Clear(); 
	LCD_WriteString("Ready to write... \n"); 
	
	for (int x = 0; x < 16000000; ++x) {}
	
	// 1. need to issue a MEDIA_INIT command in order to initialize the sd card 
	// 2. then, we need to SET THE SECTOR ADDRESS of where the data array needs to go (where do we even get this sector address though) 
	// 3. then, we need to WRITE TO THE SECTOR address pointed to by the previous instruction 
	// 4. "after writing data to any sector, you should FLUSH MEDIA (some safety necessary thing)"
	// 5. then, finally, let's DISPLAY image (after we've written to the sd card)
	
	// note: in this extremely simple case, we will write to and display from the 0th sector, and thats it (only ONE 512 byte sector in this case) 
	LCD_MediaInit(); 		
	LCD_SetSectorAddress(0); 
	// LCD_WriteSector(LCD_TestImage); 
	LCD_FlushMedia(); 
		
	for (int x = 0; x < 16000000; ++x) {}	
		
	// LCD_Clear();
	LCD_WriteString("Now displaying image: \n"); 	
	// LCD_SetSectorAddress(0); 
	LCD_DisplayImage(0, 0); 
		
		// after testing: clearly, an image is being written to and displayed, but i think the lack of it being in RAW format is making this not work. 
		// next test main (LCD_Camera_main2) will try and get a RAW image, and display it on the lcd display 
}

void LCD_Camera_main2() { 
	DisableInterrupts();
	PLL_Init(Bus80MHz);  
	Unified_Port_Init(); // initialize all ports 
	LCD_UART_Init(); // initialize lcd communication 
	UART_Init(); 		// initialize camera here 
	Timer1A_Init(UART_OutSync, SyncTime * 80000, 2); // timer allows us to sync with camera module 
	EnableInterrupts(); 
	
	
	/***** CAMERA SET UP START*****/ 
	LCD_WriteString("Setting up camera... \n"); 
	// initialize the SD card to be ready to accept RAW image data 
	LCD_MediaInit(); 	
	
	// need to send a reset signal to camera before syncing 
	Camera_HardwareReset(); 
	UART_InData(); 
	LCD_WriteString("Syncing... \n"); 
	uint8_t SyncFailureCounter = 0; 
	while (array[0] != 0xAA || array[1] != 0x0E || array[2] != 0x0D || array[4] != 0x00 || array[5] != 0x00) { 
		UART_InData();  
		++SyncFailureCounter; 
		if (SyncFailureCounter == 10) { 
			Camera_HardwareReset(); 
			SyncFailureCounter = 0; 
		}
	} 
	LCD_WriteString("Syncing 2... \n"); 
	// once we've gotten here, we've read our ACK signal, and can stop sending sync signals: 
	Timer1A_Stop(); 
	// reading our CAMERA'S SYNC signal: 
	LCD_WriteString("Syncing 3... \n");
	UART_InData(); 
	LCD_WriteString("Syncing 4... \n"); 
	if (array[0] != 0xAA || array[1] != 0x0D || array[2] != 0x00 || array[3] != 0x00 || array[4] != 0x00 || array[5] != 0x00) { 
		LCD_Clear(); 
		LCD_WriteString("Sync has gone wrong. Please shut down system. \n"); 
		while (1) {} 
	}		
	UART_OutACK(); 
	LCD_WriteString("Done setting up camera... \n"); 
	/***** CAMERA SET UP END*****/ 
	
	/***** TAKING THE PHOTO START*****/ 
	LCD_WriteString("Taking photo... \n"); 
	
	UART_OutInitial(); 
	if (array[0] != 0xAA || array[1] != 0x0E || array[2] != 0x01 || array[4] != 0x00 || array[5] != 0x00) { 
		LCD_Clear(); 
		LCD_WriteString("Image Parameters have gone wrong. Please shut down system. \n"); 
		while (1) {} 
	}
	
	LCD_WriteString("Out Initial Done... \n"); 
	
	UART_OutPackageSize(); 
	if (array[0] != 0xAA || array[1] != 0x0E || array[2] != 0x06 || array[4] != 0x00 || array[5] != 0x00) { 
		LCD_Clear(); 
		LCD_WriteString("Package Size has gone wrong. Please shut down system. \n"); 
		while (1) {} 
	}
	
	LCD_WriteString("Out Package Done... \n"); 
	
	UART_OutSnapshot(); 
	if (array[0] != 0xAA || array[1] != 0x0E || array[2] != 0x05 || array[4] != 0x00 || array[5] != 0x00) { 
		LCD_Clear(); 
		LCD_WriteString("Snapshot has gone wrong. Please shut down system. \n"); 
		while (1) {} 
	}
	
	LCD_WriteString("Out Snapshot Done... \n"); 
	
	for (int z = 0; z < 800000; ++z) {} // 100 ms delay to allow camera to chill 
	
	UART_OutGetPic(); 
	if (array[0] != 0xAA || array[1] != 0x0E || array[2] != 0x04 || array[4] != 0x00 || array[5] != 0x00) { 
		LCD_Clear(); 
		LCD_WriteString("Get Picture has gone wrong. Please shut down system. \n"); 
		while (1) {} 
	}	
	
	LCD_WriteString("Out Get Pic Done... \n"); 
	
	for (int z = 0; z < 800000; ++z) {} // 100 ms delay to allow camera to chill 
	
	// get the number of 512-byte transfers: 
	UART_InData(); 
	if (array[0] != 0xAA || array[1] != 0x0A || array[2] != 0x02) {
		LCD_Clear(); 
		LCD_WriteString("Transferring has gone wrong. Please shut down system. \n"); 
		while (1) {} 
	} 
	int32_t NUM_BYTES = (((array[5] & 0xFF) << 16) + ((array[4] & 0xFF) << 8) + array[3]);  
	int32_t NUM_TRANSFERS = NUM_BYTES / 512;  
	
	LCD_WriteString("Get Transfer Size Done... \n"); 
	
	uint16_t current_package_number = 0; 
	LCD_SetSectorAddress(current_package_number); // start at beginning of SD card 
	int32_t num_bytes_left = NUM_BYTES; 
	while (current_package_number < NUM_TRANSFERS) { 
		UART_OutCUSTOMACK(current_package_number); 
		
		for (int z = 0; z < 800000; ++z) {} // 100 ms delay to allow camera to chill 
		
		if (num_bytes_left >= 512) { 
			UART_InNBytes(512); // populating image array 
		}	else { 
			UART_InNBytes(num_bytes_left);
		}					
				
		LCD_WriteString("Image Array populated! \n");
		
		// now write to sector in SD card: 	
		LCD_WriteSector(image_array); 
		LCD_FlushMedia(); 	
		
		//---------- BAD 
		
		
		if (current_package_number == 3) { 
			LCD_SetSectorAddress(0); 
			LCD_DisplayImage(0, 30); 
			LCD_SetSectorAddress(1); 
			LCD_DisplayImage(0, 30); 
			LCD_SetSectorAddress(2); 
			LCD_DisplayImage(0, 30); 
			LCD_SetSectorAddress(3); 
			LCD_DisplayImage(0, 30); 
		}
		
		//---------- BAD 	
			
		++current_package_number; 
		num_bytes_left -= 512; 
		
		// and then finally: clear image array in order to be ready for next transfer 
		for (uint16_t k = 0; k < 512; ++k) {  
			image_array[k] = 0; 
		}
	}
	
	LCD_WriteString("Done taking photo... \n"); 
	/***** TAKING THE PHOTO END*****/ 	
	
	/***** SHOWING THE PHOTO START *****/ 
	LCD_WriteString("Showing photo... \n"); 
	LCD_SetSectorAddress(0); 
	LCD_DisplayImage(0, 0); 
	
	/***** SHOWING THE PHOTO END *****/ 
	
}

void sd_card_main3() { 
	DisableInterrupts();
	PLL_Init(Bus80MHz);  
	Unified_Port_Init(); // initialize all ports 
	LCD_UART_Init(); // initialize lcd communication 
	EnableInterrupts(); 
	
	LCD_Clear(); 
	LCD_WriteString("Test"); 
	
	// initialize SD card for further use by LCD display 
	LCD_MediaInit(); 
	
	// set the sector address we're going to start writing to
	LCD_SetSectorAddress(0); 
	
	// write text to that sector 
	LCD_WriteSector(LCD_TestParagraph); 
	
	// set the sector address we're going to read from 
	LCD_SetSectorAddress(0); 
	
	// read text from that sector
	uint8_t LCD_ReadParagraph[512]; 
	
	LCD_ReadSector(&LCD_ReadParagraph);
	
	// write it back on the screen to make sure we aint buggin: 
	LCD_WriteString("\n"); 
	LCD_WriteString((char *) LCD_ReadParagraph); 
	
	while (1) {} 
}	


int main() { 
	
	while (1) {} 
	
	return 0; 
} 
