// SwitchPA5.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize a GPIO as an input pin 
// as an external switch on PA5.
// Use bit-banded I/O.
// Daniel and Jonathan Valvano
// Jan 2, 2020

/* This example accompanies the book
   "Embedded Systems: Introduction to ARM Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2020
   Section 4.2    Program 4.2

  "Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2020
   Example 2.3, Program 2.9, Figure 2.36

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
#include "../inc/tm4c123gh6pm.h"
#include "../inc/SysTick.h"


#define PA5                     (*((volatile uint32_t *)0x40004080))


//------------SwitchPA5_Init------------
// Initialize GPIO Port A bit 5 for input
// Input: none
// Output: none
void SwitchPA5_Init(void){ 
  SYSCTL_RCGCGPIO_R |= 0x00000001;     // 1) activate clock for Port A
  while((SYSCTL_PRGPIO_R&0x01) == 0){};// ready?
  GPIO_PORTA_AMSEL_R &= ~0x20;      // 3) disable analog on PA5
  GPIO_PORTA_PCTL_R &= ~0x00F00000; // 4) PCTL GPIO on PA5
  GPIO_PORTA_DIR_R &= ~0x20;        // 5) direction PA5 input
  GPIO_PORTA_AFSEL_R &= ~0x20;      // 6) PA5 regular port function
  GPIO_PORTA_DEN_R |= 0x20;         // 7) enable PA5 digital port
}
//------------SwitchPA5_Input------------
// Read and return the status of GPIO Port A bit 5 
// Input: none
// Output: 0x20 if PA5 is high
//         0x00 if PA5 is low
uint32_t SwitchPA5_Input(void){
  return PA5; // return 0x20(pressed) or 0(not pressed)
}
uint32_t SwitchPA5_Input2(void){
  return (GPIO_PORTA_DATA_R&0x20); // 0x20(pressed) or 0(not pressed)
}


// Program 2.9 from Volume 2
#define PB1 (*((volatile uint32_t *)0x40005008))
//------------Switch_Init3------------
// Initialize GPIO Port B bit 1 for input
// Input: none
// Output: none
void Switch_Init3(void){
  SYSCTL_RCGCGPIO_R |= 0x02;        // 1) activate clock for Port B
  while((SYSCTL_PRGPIO_R&0x02) == 0){};// ready?
  GPIO_PORTB_DIR_R &= ~0x02;        // PB1 is an input
  GPIO_PORTB_AFSEL_R &= ~0x02;      // regular port function
  GPIO_PORTB_AMSEL_R &= ~0x02;      // disable analog on PB1 
  GPIO_PORTB_PCTL_R &= ~0x000000F0; // PCTL GPIO on PB1 
  GPIO_PORTB_DEN_R |= 0x02;         // PB3-0 enabled as a digital port
}
//------------Switch_Input3------------
// Read and return the status of GPIO Port B bit 1 
// Input: none
// Output: 0x02 if PB1 is high
//         0x00 if PB1 is low
uint32_t Switch_Input3(void){ 
  return PB1;      // 0x02 if pressed, 0x00 if not pressed
}

#define DELAY10MS 160000
#define DELAY10US 160
//------------Switch_Debounce------------
// Read and return the status of the switch 
// Input: none
// Output: 0x02 if PB1 is high
//         0x00 if PB1 is low
// debounces switch
uint32_t Switch_Debounce(void){
uint32_t in,old,time; 
  time = 1000; // 10 ms
  old = Switch_Input();
  while(time){
    SysTick_Wait(DELAY10US); // 10us
    in = Switch_Input();
    if(in == old){
      time--; // same value 
    }else{
      time = 1000;  // different
      old = in;
    }
  }
  return old;
}

//------------Switch_Debounce------------
// wait for the switch to be touched 
// Input: none
// Output: none
// debounces switch
void Switch_WaitForTouch(void){
// wait for release
  while(Switch_Input()){};
  SysTick_Wait(DELAY10MS); // 10ms
// wait for touch
  while(Switch_Input()==0){};
  SysTick_Wait(800000); // 10ms
}
  
