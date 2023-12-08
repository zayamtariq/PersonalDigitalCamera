// SwitchExternal.h
// Runs on LM4F120/TM4C123
// Provide functions that initialize a GPIO as an input pin and
// as external switches on PA5 and PB1.
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

#define PA5  (*((volatile uint32_t *)0x40004080))
//------------SwitchPA5_Init------------
// Initialize GPIO Port A bit 5 for input
// Input: none
// Output: none
void SwitchPA5_Init(void);
//------------SwitchPA5_Input------------
// Read and return the status of GPIO Port A bit 5 
// Input: none
// Output: 0x20 if PA5 is high
//         0x00 if PA5 is low
uint32_t SwitchPA5_Input(void);
uint32_t SwitchPA5_Input2(void);
//------------Switch_Debounce------------
// Read and return the status of the switch 
// Input: none
// Output: 0x20 if PA5 is high
//         0x00 if PA5 is low
// debounces switch
uint32_t SwitchPA5_Debounce(void);

//------------SwitchPA5_WaitForTouch------------
// wait for the switch to be touched 
// Input: none
// Output: none
// debounces switch
void SwitchPA5_WaitForTouch(void);
  
// Program 2.9 from Volume 2
#define PB1 (*((volatile uint32_t *)0x40005008))
//------------SwitchPB1_Init------------
// Initialize GPIO Port B bit 1 for input
// Input: none
// Output: none
void SwitchPB1_Init(void);
//------------SwitchPB1_Input------------
// Read and return the status of GPIO Port B bit 1 
// Input: none
// Output: 0x02 if PB1 is high
//         0x00 if PB1 is low
uint32_t SwitchPB1_Input(void);


//------------SwitchPB1_Debounce------------
// Read and return the status of the switch 
// Input: none
// Output: 0x02 if PB1 is high
//         0x00 if PB1 is low
// debounces switch
uint32_t SwitchPB1_Debounce(void);
//------------SwitchPB1_WaitForTouch------------
// wait for the switch to be touched 
// Input: none
// Output: none
// debounces switch
void SwitchPB1_WaitForTouch(void);

