// WTimer0A.c
// Runs on LM4F120/TM4C123
// Use WTimer0 in 64-bit periodic mode to request interrupts at a periodic rate
// Jonathan Valvano
// Jan 9, 2020

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2020
  Program 7.5, example 7.6

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

void (*WidePeriodicTask0)(void);   // user function

// ***************** WideTimer0A_Init ****************
// Activate WideTimer0 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
//          priority 0 (highest) to 7 (lowest)
// Outputs: none
void WideTimer0A_Init(void(*task)(void), uint32_t period, uint32_t priority){
  SYSCTL_RCGCWTIMER_R |= 0x01;   // 0) activate WTIMER0
  WidePeriodicTask0 = task;      // user function
  WTIMER0_CTL_R = 0x00000000;    // 1) disable WTIMER0A during setup
  WTIMER0_CFG_R = 0x00000000;    // 2) configure for 64-bit mode
  WTIMER0_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  WTIMER0_TAILR_R = period-1;    // 4) reload value
	WTIMER0_TBILR_R = 0;           // bits 63:32
  WTIMER0_TAPR_R = 0;            // 5) bus clock resolution
  WTIMER0_ICR_R = 0x00000001;    // 6) clear WTIMER0A timeout flag TATORIS
  WTIMER0_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI23_R = (NVIC_PRI23_R&0xFF00FF00)|(priority<<21); // priority 
// interrupts enabled in the main program after all devices initialized
// vector number 110, interrupt number 94
  NVIC_EN2_R = 1<<30;            // 9) enable IRQ 94 in NVIC
  WTIMER0_CTL_R = 0x00000001;    // 10) enable TIMER5A
}

void WideTimer0A_Handler(void){
  WTIMER0_ICR_R = TIMER_ICR_TATOCINT;// acknowledge WTIMER5A timeout
  (*WidePeriodicTask0)();            // execute user task
}
void WideTimer0_Stop(void){
  NVIC_DIS2_R = 1<<30;          // 9) disable interrupt 94 in NVIC
  WTIMER0_CTL_R = 0x00000000;    // 10) disable wtimer0A
}
