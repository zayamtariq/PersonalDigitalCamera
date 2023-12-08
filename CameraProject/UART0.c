// UART.c
// Runs on LM4F120, TM4C123
// Simple device driver for the UART.
// Daniel Valvano
// Jan 3, 2020

/* This example accompanies the books
  "Embedded Systems: Introduction to ARM Cortex M Microcontrollers",
  ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2020

"Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2020
 
 Copyright 2020 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// U0Rx (VCP receive) connected to PA0
// U0Tx (VCP transmit) connected to PA1

#include <stdint.h>
#include <stdio.h>
#include "UART0.h"
#include "inc/tm4c123gh6pm.h"


#define UART0_FR_TXFF            0x00000020  // UART Transmit FIFO Full
#define UART0_FR_RXFE            0x00000010  // UART Receive FIFO Empty
#define UART0_LCRH_WLEN_8        0x00000060  // 8 bit word length
#define UART0_LCRH_FEN           0x00000010  // UART Enable FIFOs
#define UART0_CTL_UARTEN         0x00000001  // UART Enable

//------------UART0_Init------------
// Initialize the UART for 115,200 baud rate (assuming 80 MHz bus clock),
// 8 bit word length, no parity bits, one stop bit, FIFOs enabled
// Input: none
// Output: none
void UART0_Init(void){
  SYSCTL_RCGCUART_R |= 0x01;            // activate UART0
  SYSCTL_RCGCGPIO_R |= 0x01;            // activate port A
  UART0_CTL_R &= ~UART0_CTL_UARTEN;      // disable UART
  UART0_IBRD_R = 43;                    // IBRD = int(80,000,000 / (16 * 115,200)) = int(43.403)
  UART0_FBRD_R = 26;                    // FBRD = round(0.4028 * 64 ) = 26
                                        // 8 bit word length (no parity bits, one stop bit, FIFOs)
  UART0_LCRH_R = (UART0_LCRH_WLEN_8|UART0_LCRH_FEN);
  UART0_CTL_R |= UART0_CTL_UARTEN;       // enable UART
  GPIO_PORTA_AFSEL_R |= 0x03;           // enable alt funct on PA1-0
  GPIO_PORTA_DEN_R |= 0x03;             // enable digital I/O on PA1-0
                                        // configure PA1-0 as UART
  GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFFFFFF00)+0x00000011;
  GPIO_PORTA_AMSEL_R &= ~0x03;          // disable analog functionality on PA
}

//------------UART0_InChar------------
// Wait for new serial port input
// Input: none
// Output: ASCII code for key typed
char UART0_InChar(void){
  while((UART0_FR_R&UART0_FR_RXFE) != 0);
  return((char)(UART0_DR_R&0xFF));
}
//------------UART0_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void UART0_OutChar(char data){
  while((UART0_FR_R&UART0_FR_TXFF) != 0);
  UART0_DR_R = data;
}

//------------UART0_OutString------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void UART0_OutString(char *pt){
  while(*pt){
    UART0_OutChar(*pt);
    pt++;
  }
}

//------------UART0_InUDec------------
// InUDec accepts ASCII input in unsigned decimal format
//     and converts to a 32-bit unsigned number
//     valid range is 0 to 4294967295 (2^32-1)
// Input: none
// Output: 32-bit unsigned number
// If you enter a number above 4294967295, it will return an incorrect value
// Backspace will remove last digit typed
uint32_t UART0_InUDec(void){
uint32_t number=0, length=0;
char character;
  character = UART0_InChar();
  while(character != CR){ // accepts until <enter> is typed
// The next line checks that the input is a digit, 0-9.
// If the character is not 0-9, it is ignored and not echoed
    if((character>='0') && (character<='9')) {
      number = 10*number+(character-'0');   // this line overflows if above 4294967295
      length++;
      UART0_OutChar(character);
    }
// If the input is a backspace, then the return number is
// changed and a backspace is outputted to the screen
    else if((character==BS) && length){
      number /= 10;
      length--;
      UART0_OutChar(character);
    }
    character = UART0_InChar();
  }
  return number;
}

//-----------------------UART0_OutUDec-----------------------
// Output a 32-bit number in unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1-10 digits with no space before or after
void UART0_OutUDec(uint32_t n){
// This function uses recursion to convert decimal number
//   of unspecified length as an ASCII string
  if(n >= 10){
    UART0_OutUDec(n/10);
    n = n%10;
  }
  UART0_OutChar(n+'0'); /* n is between 0 and 9 */
}

//---------------------UART0_InUHex----------------------------------------
// Accepts ASCII input in unsigned hexadecimal (base 16) format
// Input: none
// Output: 32-bit unsigned number
// No '$' or '0x' need be entered, just the 1 to 8 hex digits
// It will convert lower case a-f to uppercase A-F
//     and converts to a 16 bit unsigned number
//     value range is 0 to FFFFFFFF
// If you enter a number above FFFFFFFF, it will return an incorrect value
// Backspace will remove last digit typed
uint32_t UART0_InUHex(void){
uint32_t number=0, digit, length=0;
char character;
  character = UART0_InChar();
  while(character != CR){
    digit = 0x10; // assume bad
    if((character>='0') && (character<='9')){
      digit = character-'0';
    }
    else if((character>='A') && (character<='F')){
      digit = (character-'A')+0xA;
    }
    else if((character>='a') && (character<='f')){
      digit = (character-'a')+0xA;
    }
// If the character is not 0-9 or A-F, it is ignored and not echoed
    if(digit <= 0xF){
      number = number*0x10+digit;
      length++;
      UART0_OutChar(character);
    }
// Backspace outputted and return value changed if a backspace is inputted
    else if((character==BS) && length){
      number /= 0x10;
      length--;
      UART0_OutChar(character);
    }
    character = UART0_InChar();
  }
  return number;
}

//--------------------------UART0_OutUHex----------------------------
// Output a 32-bit number in unsigned hexadecimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1 to 8 digits with no space before or after
void UART0_OutUHex(uint32_t number){
// This function uses recursion to convert the number of
//   unspecified length as an ASCII string
  if(number >= 0x10){
    UART0_OutUHex(number/0x10);
    UART0_OutUHex(number%0x10);
  }
  else{
    if(number < 0xA){
      UART0_OutChar(number+'0');
     }
    else{
      UART0_OutChar((number-0x0A)+'A');
    }
  }
}
//--------------------------UART0_OutUHex2----------------------------
// Output a 32-bit number in unsigned hexadecimal format
// Input: 32-bit number to be transferred
// Output: none
// Fixed format 2 digits with no space before or after
void outnibble(uint32_t n){
    if(n < 0xA){
   UART0_OutChar(n+'0');
  }
  else{
    UART0_OutChar((n-0x0A)+'A');
  }
}
void UART0_OutUHex2(uint32_t number){
  outnibble(number/0x10); // ms digit
  outnibble(number%0x10); // ls digit
}

//------------UART0_InString------------
// Accepts ASCII characters from the serial port
//    and adds them to a string until <enter> is typed
//    or until max length of the string is reached.
// It echoes each character as it is inputted.
// If a backspace is inputted, the string is modified
//    and the backspace is echoed
// terminates the string with a null character
// uses busy-waiting synchronization on RDRF
// Input: pointer to empty buffer, size of buffer
// Output: Null terminated string
// -- Modified by Agustinus Darmawan + Mingjie Qiu --
void UART0_InString(char *bufPt, uint16_t max) {
int length=0;
char character;
  character = UART0_InChar();
  while(character != CR){
    if(character == BS){
      if(length){
        bufPt--;
        length--;
        UART0_OutChar(BS);
      }
    }
    else if(length < max){
      *bufPt = character;
      bufPt++;
      length++;
      UART0_OutChar(character);
    }
    character = UART0_InChar();
  }
  *bufPt = 0;
}

// Print a character to UART.
int fputc(int ch, FILE *f){
  if((ch == 10) || (ch == 13) || (ch == 27)){
    UART0_OutChar(13);
    UART0_OutChar(10);
    return 1;
  }
  UART0_OutChar(ch);
  return 1;
}
// Get input from UART, echo
int fgetc (FILE *f){
  char ch = UART0_InChar();  // receive from keyboard
  UART0_OutChar(ch);            // echo
  return ch;
}
// Function called when file error occurs.
int ferror(FILE *f){
  /* Your implementation of ferror */
  return EOF;
}

// Abstraction of general output device
// Volume 2 section 3.4.5


#ifdef __TI_COMPILER_VERSION__
	//Code Composer Studio Code
#include "file.h"
int UART0_open(const char *path, unsigned flags, int llv_fd){
  UART0_Init();
  return 0;
}
int UART0_close( int dev_fd){
	return 0;
}
int UART0_read(int dev_fd, char *buf, unsigned count){char ch;
  ch = UART0_InChar();    // receive from keyboard
  ch = *buf;         // return by reference
  UART0_OutChar(ch);  // echo
  return 1;
}
int UART0_write(int dev_fd, const char *buf, unsigned count){ unsigned int num=count;
	while(num){
		UART0_OutChar(*buf);
		buf++;
		num--;
	}
	return count;
}
off_t UART0_lseek(int dev_fd, off_t ioffset, int origin){
	return 0;
}
int UART0_unlink(const char * path){
	return 0;
}
int UART0_rename(const char *old_name, const char *new_name){
	return 0;
}

//------------Output_Init------------
// Initialize the UART for 115,200 baud rate (assuming 3 MHz bus clock),
// 8 bit word length, no parity bits, one stop bit
// Input: none
// Output: none
void Output_Init(void){int ret_val; FILE *fptr;
  UART0_Init();
  ret_val = add_device("uart", _SSA, UART0_open, UART0_close, UART0_read, UART0_write, UART0_lseek, UART0_unlink, UART0_rename);
  if(ret_val) return; // error
  fptr = fopen("uart","w");
  if(fptr == 0) return; // error
  freopen("uart:", "w", stdout); // redirect stdout to uart
  setvbuf(stdout, NULL, _IONBF, 0); // turn off buffering for stdout

}
#else
//Keil uVision Code
//------------Output_Init------------
// Initialize the Nokia5110
// Input: none
// Output: none
//------------Output_Init------------
// Initialize the UART for 115,200 baud rate (assuming 16 MHz bus clock),
// 8 bit word length, no parity bits, one stop bit, FIFOs enabled
// Input: none
// Output: none
void Output_Init(void){
  UART0_Init();
}
#endif