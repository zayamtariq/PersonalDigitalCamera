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

// picture of a stick figure to send to the lcd display
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
	LCD_WriteSector(LCD_TestImage); 
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
	while (array[0] != 0xAA || array[1] != 0x0E || array[2] != 0x0D || array[4] != 0x00 || array[5] != 0x00) { 
		Camera_HardwareReset(); 
		UART_InData();  
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
	LCD_Clear(); 
	
	UART_OutInitial(); 
	if (array[0] != 0xAA || array[1] != 0x0E || array[2] != 0x01 || array[4] != 0x00 || array[5] != 0x00) { 
		LCD_Clear(); 
		LCD_WriteString("Image Parameters have gone wrong. Please shut down system. \n"); 
		while (1) {} 
	}
	
	UART_OutPackageSize(); 
	if (array[0] != 0xAA || array[1] != 0x0E || array[2] != 0x06 || array[4] != 0x00 || array[5] != 0x00) { 
		LCD_Clear(); 
		LCD_WriteString("Package Size has gone wrong. Please shut down system. \n"); 
		while (1) {} 
	}
	
	UART_OutSnapshot(); 
	if (array[0] != 0xAA || array[1] != 0x0E || array[2] != 0x05 || array[4] != 0x00 || array[5] != 0x00) { 
		LCD_Clear(); 
		LCD_WriteString("Snapshot has gone wrong. Please shut down system. \n"); 
		while (1) {} 
	}
	
	UART_OutGetPic(); 
	if (array[0] != 0xAA || array[1] != 0x0E || array[2] != 0x04 || array[4] != 0x00 || array[5] != 0x00) { 
		LCD_Clear(); 
		LCD_WriteString("Get Picture has gone wrong. Please shut down system. \n"); 
		while (1) {} 
	}	
	
	// get the number of 512-byte transfers: 
	UART_InData(); 
	if (array[0] != 0xAA || array[1] != 0x0A || array[2] != 0x01) {
		LCD_Clear(); 
		LCD_WriteString("Transferring has gone wrong. Please shut down system. \n"); 
		while (1) {} 
	} 
	int32_t NUM_BYTES = (((array[5] & 0xFF) << 16) + ((array[4] & 0xFF) << 8) + array[3]);  
	int32_t NUM_TRANSFERS = NUM_BYTES / 512;  
	
	uint16_t current_package_number = 0; 
	int32_t num_bytes_left = NUM_BYTES; 
	while (current_package_number < NUM_TRANSFERS) { 
		UART_OutCUSTOMACK(current_package_number); 
		if (num_bytes_left >= 512) { 
			UART_InNBytes(512); // populating image array 
		}	else { 
			UART_InNBytes(num_bytes_left);
		}					
				
		// now write to sector in SD card: 	
		LCD_SetSectorAddress(current_package_number); 
		LCD_WriteSector(image_array); 
		LCD_FlushMedia(); 	
		
		++current_package_number; 
		num_bytes_left -= 512; 
		
		// and then finally: clear image array in order to be ready for next transfer 
		for (int32_t k = 0; k < 512; ++k) {  
			image_array[k] = 0; 
		}
	}
	LCD_WriteString("Done taking photo... \n"); 
	/***** TAKING THE PHOTO END*****/ 	
	
	/***** SHOWING THE PHOTO START *****/ 
	
	LCD_SetSectorAddress(0); 
	LCD_DisplayImage(0, 0); 
	
	/***** SHOWING THE PHOTO END *****/ 
	
}


int main() { 
	LCD_Camera_main2(); 
/****** SYNCING AND INITIALIZATION ******/
	/*
	DisableInterrupts(); 
	PLL_Init(Bus80MHz);  
	Unified_Port_Init(); // initialize all ports 
	// 1. first need to send a message and do this at a 5 ms interval.
	// do this by initializing a timer to go with this. 
	Timer1A_Init(UART_OutSync, SyncTime * 80000, 2); 
	UART_Init(); // initialize camera uart 
	LCD_UART_Init(); // initialize lcd communication 
	UART0_Init(); 
	EnableInterrupts(); 
	
	// 2. receive data from UART (waiting for the ACK) 
	// 3. and then the same thing with the SYNC 
	UART_InData();
	LCD_WriteString("Syncing..."); 	
	while (array[0] != 0xAA || array[1] != 0x0E || array[2] != 0x0D || array[4] != 0x00 || array[5] != 0x00) { 
		UART_InData();  
	} 
	// once we've gotten here, we've read our ACK signal, and can stop sending sync signals. 
	Timer1A_Stop(); 
	// now we need to read our CAMERA'S SYNC signal 
	UART_InData(); 
	while (array[0] != 0xAA || array[1] != 0x0D || array[2] != 0x00 || array[3] != 0x00 || array[4] != 0x00 || array[5] != 0x00) { 
		UART_InData(); 
	}		
	UART0_OutString("received ack");  
	// finally, we send our OWN ACK signal 
	UART_OutACK(); 
// and now we're done syncing, if we've made it this far! 
	*/ 
/******* SYNCING ********/ 
// WE MADE IT HERE. 
	/*
	LCD_Clear(); 
	LCD_InData(); 
	// so now steps we need to take 
	// 1. Send the initial command 
	// we should make this an interface we can choose from at startup? 
	// for now, JPEG is totally fine 
	
	// initial host signal 
	UART_OutInitial();
	// camera ack signal 
	UART_InData(); 
	while (array[0] != 0xAA || array[1] != 0x0E || array[2] != 0x01 || array[4] != 0x00 || array[5] != 0x00) { 
		UART_InData(); 
	}
	
	// set package size
	UART_OutPackageSize(); 
	// camera ack signal 
	UART_InData(); 
	while (array[0] != 0xAA || array[1] != 0x0E || array[2] != 0x06 || array[4] != 0x00 || array[5] != 0x00) { 
		UART_InData(); 
	}

	while (1) { 
		// if they press a button (pf0 = sw2, pf4 = sw1) 
		if (PF4 == 0) { 
			for (int x = 0; x < 800000; x++) {}
			// send out a snap shot 
			UART_OutSnapshot(); 
			LCD_WriteString("Sent Snapshot"); 
			
			// camera ack signal 
			UART_InData(); 
			while (array[0] != 0xAA || array[1] != 0x0E || array[2] != 0x05 || array[4] != 0x00 || array[5] != 0x00) { 
				LCD_WriteString("Snapshot acknowledge"); 
				UART_InData(); 
			}
			
			for (int x = 0; x < 800000; x++) {} 
			
			// and then also GET the picture and [send it to the SD card] -> just do something with it  
			UART_OutGetPic(); 
			LCD_WriteString("Getting Picture");  
			
			// camera ack signal 
			UART_InData(); 
			while (array[0] != 0xAA || array[1] != 0x0E || array[2] != 0x04 || array[4] != 0x00 || array[5] != 0x00) { 
				// gonna need to be careful that this doesn't eat the data up. 
				UART_InData(); 
			}	
			LCD_WriteString("Done getting picture"); 
			
			// and now we ALSO need to read the data using UART_InData
			// the first UART_InData will give us the LENGTH of our data
			
			UART_InData(); 
			while (array[0] != 0xAA || array[1] != 0x0A || array[2] != 0x01) {} 
			int32_t NUM_BYTES = (((array[5] & 0xFF) << 16) + ((array[4] & 0xFF) << 8) + array[3]);  
				// TODO: check that NUM_TRANSFERS is around ~6-7 (actually should be EXACTLY 7, should NEVER be 8) 
			int32_t NUM_TRANSFERS = NUM_BYTES / 512;  
			LCD_WriteString("Now reading image"); 

			// READING IMAGE HERE 

				// we should mount the file system here:
				LCD_FileMount(); 
				
				// and then open a new file HERE (making sure to grab the file handle): 
				// first need to create the file name 
				uint16_t CurrentHandle = LCD_FileOpen("IMG_2", 'w'); 
				
				uint16_t current_package_number = 0; 
				int32_t num_bytes_left = NUM_BYTES; 
				while (current_package_number < NUM_TRANSFERS) { 
					UART_OutCUSTOMACK(current_package_number); 
					if (num_bytes_left >= 512) { 
						UART_InNBytes(512);
						// write 512 bytes to file: 
						LCD_FileWrite(512, CurrentHandle); 
					}	else { 
						UART_InNBytes(num_bytes_left);
						// write N bytes to file: 
						LCD_FileWrite(num_bytes_left, CurrentHandle); 
					}					
					++current_package_number; 
					num_bytes_left -= 512; 
				
					// and then finally: clear image array in order to be ready for next transfer 
					for (int32_t k = 0; k < 512; ++k) {  
						image_array[k] = 0; 
					}
				
					// close file: 
					LCD_FileClose(CurrentHandle); 
				
					// reopen to append mode: 
					LCD_FileOpen("IMG", 'a'); 
				}
			
				// lets show the user the most recently taken image: 
				LCD_DisplayImage(CurrentHandle); 
				
				// unmount file here? need to do it somewhere. in theory we have a power off button that will do it for us. 
				
				LCD_FileUnmount(); 	
				
				// TODO: right now we are mounting and unmounting every time we take a picture which... is bad design
		}
		// if they press a different button, view all photos in SD card 
		if (PF0 == 0) { // switch 2 !!! TWO!!! 
			char toObserve = 0x0; 
			while (toObserve != 0x06) { 
				LCD_Clear(); 
				toObserve = LCD_InData();  
			}
			LCD_WriteString("Viewing Photos");
		}
		// if they press ANOTHER another button, we can go to the light settings 
	}
	*/ 
	
	while (1) {} 
	
	return 0; 
} 
