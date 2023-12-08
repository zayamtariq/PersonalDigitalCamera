#include <stdint.h>
#include "inc/tm4c123gh6pm.h" 
#include "inc/CortexM.h"
#include "UART.h"

// will initialize UART3 - using pc6 (u3rx) and pc7 (u3tx) 
void LCD_UART_Init(void); 

// clear the screen 
void LCD_Clear(void); 

// just a test function to work with for now, make sure communication is good 
void LCD_DrawRectangle(void); 

// receiving serial data from the picaso processor in the lcd display 
char LCD_InData(void); 

// use this to write a string to the LCD display 
void LCD_WriteString(char * string); 

/**** SD CARD *********/ 

// initialize sd card to perform instructions on 
void LCD_MediaInit(void);

// assuming no file system, use this to say which 512-byte section of the sd card to write to 
void LCD_SetSectorAddress(uint32_t sector_location); 

// write to address defined by most previous set sector address instruction 
void LCD_WriteSector(uint8_t * source); 

// flush after writing 
void LCD_FlushMedia(void);

// display image defined by most previous set sector address instruction 
void LCD_DisplayImage(uint16_t x_pos, uint16_t y_pos);

/**** FILE SYSTEM ****/ 

// mount the file system 
void LCD_FileMount(void); 

// open and write mode for a file 
// handle - associated with the file
uint16_t LCD_FileOpen(char * file_name, char mode); 

// actually write data to the file
// num_bytes: number of bytes we send to the file 
// handle: associated w file 
void LCD_FileWrite(uint16_t num_bytes, uint16_t handle); 

// close the file 
// input: handle associated w file 
void LCD_FileClose(uint16_t handle); 

// unmount the file system 
// no inputs 
void LCD_FileUnmount(void); 

// count files in the file system  
int LCD_CountFiles(void); 

// display image associated with certain handle 
void LCD_DisplayImage(uint16_t handle); 
