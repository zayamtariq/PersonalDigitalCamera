/**
 * @file Blynk.c
 * @author Jonathan Valvano, Matthew Yu (matthewjkyu@gmail.com)
 * @brief API for sending commands to and from Blynk using the ESP8266.
 *        TM4C <- UART -> ESP8266 <- WiFi -> Blynk Server
 * @version 0.1
 * @date 2022-09-07
 * @note Hardware setup:
 * TM4C123       ESP8266-ESP01 (2 by 4 header)
 * PE5 (U5TX) to Pin 1 (Rx)
 * PE4 (U5RX) to Pin 5 (TX)
 * PE3 output debugging
 * PE2 nc
 * PE1 output    Pin 7 Reset
 * PE0 input     Pin 3 Rdy IO2
 *               Pin 2 IO0, 10k pullup to 3.3V  
 *               Pin 8 Vcc, 3.3V (separate supply from LaunchPad 
 * Gnd           Pin 4 Gnd  
 * Place a 4.7uF tantalum and 0.1 ceramic next to ESP8266 3.3V power pin
 * Use LM2937-3.3 and two 4.7 uF capacitors to convert USB +5V to 3.3V for the ESP8266
 * http://www.ti.com/lit/ds/symlink/lm2937-3.3.pdf
 */

#include <stdlib.h>
#include <string.h>
#include "./inc/Timer2A.h"
#include "./inc/Blynk.h"
#include "./inc/esp8266_blynk.h"
#include "./inc/ST7735.h"

#define PF1   (*((volatile uint32_t *)0x40025008)) // RED LED
#define PF2   (*((volatile uint32_t *)0x40025010)) // BLUE LED
#define PF3   (*((volatile uint32_t *)0x40025020)) // GREEN LED

void EnableInterrupts(void);    // Defined in startup.s
void DisableInterrupts(void);   // Defined in startup.s
void WaitForInterrupt(void);    // Defined in startup.s

#define NUM_QUEUE_ELEMENTS 50
static blynk_info_t info_queue[NUM_QUEUE_ELEMENTS];
static uint8_t info_queue_head, info_queue_tail;

void tm4c_to_blynk(const blynk_info_t info) {
    if (info.pin_number < 70 || info.pin_number > 99) return;

    /* Output CSV string to ESP8266 in format <Virtual pin number>,<value>,0.0\n */
    ESP8266_OutInteger(info.pin_number);
    ESP8266_OutChar(',');
    ESP8266_OutInteger(info.integer_value);
    ESP8266_OutChar(',');
    ESP8266_OutInteger(info.float_value);
    ESP8266_OutChar('\n');

#ifdef DEBUG
    /* Output struct info debug. */
    UART_OutString("SEND:\n\tPin number = ");
    UART_OutString(info.pin_number);
    UART_OutString("\n\tPin integer = ");
    UART_OutString(info.pin_integer);
    UART_OutString("\n\tPin float = ");
    UART_OutString(info.pin_float);
    UART_OutString("\n\r");
#endif
}

blynk_info_t blynk_to_tm4c(void) {
    char serial_buffer[64] = { '\0' };
    char pin_number[8] = { '\0' };
    char pin_integer[8] = { '\0' };
    char pin_float[8] = { '\0' };

    if (!ESP8266_GetMessage(serial_buffer)) {
        blynk_info_t info = {255, 0, 0}; // 255 is our indicator that we have no data.
        return info;
    }

    /* Capture each file from the CSV string provided by the ESP8266. This is in
       the format <Virtual pin number>,<integer value>,<float value> */
    strcpy(pin_number, strtok(serial_buffer, ","));
    strcpy(pin_integer, strtok(NULL, ","));
    strcpy(pin_float, strtok(NULL, ","));

    blynk_info_t info = {
        .pin_number = atoi(pin_number),
        .integer_value = atoi(pin_integer),
        .float_value = atoi(pin_float)
    };

#ifdef DEBUG
    /* LED select button */
    if (info.pin_number == 0x1) {
        PF2 = info.pin_number;
        ST7735_SetTextColor(ST7735_CYAN);
        ST7735_OutString("Rcv VP1 data = ");
        ST7735_OutDec(LED);
        ST7735_OutChar('\n');
    }

    /* Output struct info debug. */
    UART_OutString("RECEIVE:\n\tPin number = ");
    UART_OutString(info.pin_number);
    UART_OutString("\n\tPin integer = ");
    UART_OutString(info.pin_integer);
    UART_OutString("\n\tPin float = ");
    UART_OutString(info.pin_float);
    UART_OutString("\n\r");
#endif

    return info;
}

void blynk_handler(void) {
    blynk_info_t info = blynk_to_tm4c();

    DisableInterrupts();

    /* Early exit if no data. */
    if (info.pin_number == 255) {
        EnableInterrupts();
        return;
    }

    /* Early exit if queue is full. */
    if (info_queue_head == (info_queue_tail + NUM_QUEUE_ELEMENTS - 1) % NUM_QUEUE_ELEMENTS) {
        PF1 = 0x2;
        PF3 = 0x0;
        EnableInterrupts();
        return;
    } else {
        PF1 = 0x0;
        PF3 = 0x8;
    }

    info_queue[info_queue_head].pin_number = info.pin_number;
    info_queue[info_queue_head].integer_value = info.integer_value;
    info_queue[info_queue_head].float_value = info.float_value;
    info_queue_head = (info_queue_head + 1) % NUM_QUEUE_ELEMENTS;
    EnableInterrupts();
}

void blynk_init(char *wifi_ssid, char *wifi_pass, char *blynk_auth_token, bool use_timer_interrupt) {
    ESP8266_Init();
    ESP8266_Reset();
    ESP8266_Connect(wifi_ssid, wifi_pass, blynk_auth_token);

    if (use_timer_interrupt) {
        /* Check for data from the Blynk app every 10 ms. */
        Timer2A_Init(&blynk_handler, 800000, 4);
    }
}

bool blynk_get_data_from_queue(blynk_info_t *info) {
    DisableInterrupts();

    /* Queue is empty. */
    if (info_queue_head == info_queue_tail) {
        EnableInterrupts();
        return false;
    }

    info->pin_number = info_queue[info_queue_tail].pin_number;
    info->integer_value = info_queue[info_queue_tail].integer_value;
    info->float_value = info_queue[info_queue_tail].float_value;
    info_queue_tail = (info_queue_tail + 1) % NUM_QUEUE_ELEMENTS;

    EnableInterrupts();
    return true;
}
