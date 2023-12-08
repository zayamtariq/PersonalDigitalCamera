/**
 * @file RGB.c
 * @author Matthew Yu (matthewjkyu@gmail.com)
 * @brief Flashes the onboard TM4C RGB (PF1-3) many colors, using a state machine.
 * @version 0.1
 * @date 2022-02-06
 * @copyright Copyright (c) 2021
 */

/** General imports. */
#include <stdlib.h>
#include <stdbool.h>

/** Device specific imports. */
#include <inc/LED.h>
#include <inc/Timer0A.h>
#include <lib/RGB/RGB.h>
#define REALTIMEFREQ 15
#define REALTIMEPERIOD (80000000 / REALTIMEFREQ)

static uint8_t states[8][4] = {
    // R, B, G, next state      
      {0, 0, 0, 1}, // R + G and R+G+B doesn't lead to interesting colors.
      {1, 1, 0, 2},
      {1, 0, 0, 3},
      {0, 1, 1, 4}, 
      {1, 0, 0, 5},
      {0, 1, 0, 6},
      {1, 0, 0, 7},
      {0, 0, 1, 1}  // We never go back to the off state :)
};

static uint8_t state = 0;
static void RGBStep(void) {
    // Set the LEDs.

    // RED
    if (states[state][0]) {
        LED_RedOn();
    } else {
        LED_RedOff();
    }

    // BLUE
    if (states[state][1]) {
        LED_BlueOn();
    } else {
        LED_BlueOff();
    }

    // GREEN
    if (states[state][2]) {
        LED_GreenOn();
    } else {
        LED_GreenOff();
    }

    // Set new state.
    state = states[state][3];
}

void RGBInit(void) {
    LED_Init();
    Timer0A_Init(&RGBStep, REALTIMEPERIOD, 7); 
}

void RGBStart(void) {
    Timer0A_Init(&RGBStep, REALTIMEPERIOD, 7); 
}

void RGBStop(void) {
    Timer0A_Stop();
    state = 0;
}
#undef REALTIMEFREQ
#undef REALTIMEPERIOD
