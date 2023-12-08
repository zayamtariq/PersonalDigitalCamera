// LaunchPad.c
// Runs on TM4C123
// Input from switches, output to LED
// Jonathan Valvano
// Jan 12, 2020

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
// negative logic switches connected to PF0 and PF4 on the Launchpad
// red LED connected to PF1 on the Launchpad
// blue LED connected to PF2 on the Launchpad
// green LED connected to PF3 on the Launchpad
// NOTE: The NMI (non-maskable interrupt) is on PF0.  That means that
// the Alternate Function Select, Pull-Up Resistor, Pull-Down Resistor,
// and Digital Enable are all locked for PF0 until a value of 0x4C4F434B
// is written to the Port F GPIO Lock Register.  After Port F is
// unlocked, bit 0 of the Port F GPIO Commit Register must be set to
// allow access to PF0's control registers.  On the LM4F120, the other
// bits of the Port F GPIO Commit Register are hard-wired to 1, meaning
// that the rest of Port F can always be freely re-configured at any
// time.  Requiring this procedure makes it unlikely to accidentally
// re-configure the JTAG pins as GPIO, which can lock the debugger out
// of the processor and make it permanently unable to be debugged or
// re-programmed.


#include <stdint.h>
#include "../inc/LaunchPad.h"
#include "../inc/tm4c123gh6pm.h"
#include "../inc/CortexM.h"

//------------LaunchPad_Init------------
// Initialize Switch input and LED output
// Input: none
// Output: none
void LaunchPad_Init(void){
  SYSCTL_RCGCGPIO_R |= 0x00000020;  // 1) activate clock for Port F
  while((SYSCTL_PRGPIO_R&0x20) == 0){};// allow time for clock to stabilize
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
  // only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 in, PF3-1 out
  GPIO_PORTF_PUR_R = 0x11;          // enable pull-up on PF0 and PF4
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital I/O on PF4-0
  GPIO_PORTF_DATA_R = 0;            // LEDs off
}


//------------LaunchPad_Input------------
// Input from Switches, 
// Convert hardware negative logic to software positive logic 
// Input: none
// Output: 0x00 none
//         0x01 SW2 pressed (from PF4)
//         0x02 SW1 pressed (from PF1)
//         0x03 both SW1 and SW2 pressed
uint8_t LaunchPad_Input(void){
  uint32_t data;
  // read PF4,PF0 inputs
  data = ~GPIO_PORTF_DATA_R; // convert to positive logic
  // shift bits so switches are in bits 1,0
  return (((data&0x10)>>3)|((data&0x01)));   
}
// DelayMs
//  - busy wait n milliseconds
// Input: time to wait in msec
// Outputs: none
void static DelayMs(uint32_t n){
  volatile uint32_t time;
  while(n){
    time = 6665;  // 1msec, tuned at 80 MHz
    while(time){
      time--;
    }
    n--;
  }
}
void LaunchPad_WaitForTouch(void){
  while((PF0==0x01)&&(PF4==0x10)){
   DelayMs(10); // debounce
  }
}

void LaunchPad_WaitForRelease(void){
  while((PF0==0x01)&&(PF4==0x10)){};  // wait for switch touch
  DelayMs(10); // debounce
  while((PF0!=0x01)||(PF4!=0x10)){};  // wait for both release
  DelayMs(10); // debounce
}
//------------LaunchPad__Output------------
// Output to LaunchPad LEDs 
// Positive logic hardware and positive logic software
// Input: 0 off, bit0=red,bit1=blue,bit2=green
// Output: none
void LaunchPad_Output(uint8_t data){  // write three outputs bits of P2
  GPIO_PORTF_DATA_R = data<<1;
}
