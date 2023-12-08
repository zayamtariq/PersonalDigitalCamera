// LaunchPad.h
// Runs on TM4C123
// Input from switches, output to LED
// Jonathan Valvano
// Jan 2, 2020

/* This example accompanies the books
   "Embedded Systems: Introduction to ARM Cortex M Microcontrollers",
      ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2020
   "Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
      ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2020
   "Embedded Systems: Real-Time Operating Systems for ARM Cortex-M Microcontrollers",
      ISBN: 978-1466468863, Jonathan Valvano, copyright (c) 2020

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
 #include <stdint.h>

// negative logic built-in SW 1 connected to PF4
// negative logic built-in SW 2 connected to PF0
// built-in red LED connected to PF3
// built-in blue LED connected to PF2
// built-in green LED connected to PF1
// Function parameter              LED(s)   PortF
#define DARK      0x00   // dark     ---    0
#define RED       0x01   // red      R--    0x02
#define BLUE      0x02   // blue     --B    0x04
#define GREEN     0x04   // green    -G-    0x08
#define YELLOW    0x05   // yellow   RG-    0x0A
#define SKYBLUE   0x06   // sky blue -GB    0x0C
#define WHITE     0x07   // white    RGB    0x0E
#define PINK      0x03   // pink     R-B    0x06

#define SW1 2
#define SW2 1
#define PF4             (*((volatile uint32_t *)0x40025040))
#define PF3             (*((volatile uint32_t *)0x40025020))
#define PF2             (*((volatile uint32_t *)0x40025010))
#define PF1             (*((volatile uint32_t *)0x40025008))
#define PF0             (*((volatile uint32_t *)0x40025004))
//------------LaunchPad_Init------------
// Initialize Switch input and LED output
// Input: none
// Output: none
void LaunchPad_Init(void);


//------------LaunchPad_Input------------
// Input from Switches, 
// Convert hardware negative logic to software positive logic 
// Input: none
// Output: 0x00 none
//         0x01 SW2 pressed (from PF4)
//         0x02 SW1 pressed (from PF1)
//         0x03 both SW1 and SW2 pressed
uint8_t LaunchPad_Input(void);
void LaunchPad_WaitForTouch(void);
void LaunchPad_WaitForRelease(void);
//------------LaunchPad__Output------------
// Output to LaunchPad LEDs 
// Positive logic hardware and positive logic software
// Input: 0 off, bit0=red,bit1=blue,bit2=green
// Output: none
void LaunchPad_Output(uint8_t data);
