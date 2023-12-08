/**
 * @file    esp8266_blynk.h
 * @author  Mark McDermott, Jonathan Valvano, Matthew Yu (matthewjkyu@gmail.com)
 * @brief   Bridges communication between the TM4C and the ESP8266, which talks to
 *          Blynk. 
 * @version 0.1
 * @date    2022-09-12 (orig 2018-05-21)
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

#include <stdint.h>

// Comment out either/both to remove debugging
#define __DEBUG_UART__		// UART0 Debug output
#define __DEBUG_ST7735__	// ST7735 Debug output

/**
 * @brief Initialize the interface with the ESP8266.
 * 
 * @note Uses UART5 (PE4, PE5) for talking to the ESP8266 serial port
 *       PE1 is used for RST and PE0 is used for GPIO_2.
 */
void ESP8266_Init(void);

/**
 * @brief Toggles the RST pin (PE1) over the course of 10 seconds to reset the
 *        ESP8266.
 * 
 * @note Uses UART0 and ST7735 if __DEBUG_UART__ and __DEBUG_ST7735__ is defined.
 */
void ESP8266_Reset(void);

/**
 * @brief Sets up the WiFi connection between the TM4C and the hotspot. It also
 *        instructs the ESP8266 to connect to Blynk.
 * 
 * @param wifi_ssid Network ID.
 * @param wifi_pass Network password.
 * @param blynk_auth_token Auth token used by Blynk to authenticate the user.
 * @note Uses UART0 and ST7735 if __DEBUG_UART__ and __DEBUG_ST7735__ is defined.
 */
void ESP8266_Connect(char *wifi_ssid, char *wifi_pass, char *blynk_auth_token);

/**
 * @brief Get a message from the ESP8266.
 * 
 * @param datapt Points to an empty data buffer of MESSAGESIZE chars. This is
 *        filled on success.
 * @return int FIFOSUCCESS if success, FIFOFAIL if no message can be retrieved.
 */
int ESP8266_GetMessage(char *datapt);

/**
 * @brief Sends an ASCII character to the ESP8266 via UART5. 
 * 
 * @param data Character to send.
 * @note Uses interrupts and therefore interrupts must be enabled. Stalls if
 *       internal storage is full. 
 */
void ESP8266_OutChar(char data);

/**
 * @brief Sends an ASCII, null terminated string to the ESP8266 via UART5.
 * 
 * @param pt Pointer to a null terminated string to send.
 * @note Uses interrupts and therefore interrupts must be enabled. Stalls if
 *       internal storage is full. 
 */
void ESP8266_OutString(char *pt);

/**
 * @brief Sends an unsigned, 32-bit integer to the ESP8266 via UART5.
 * 
 * @param n An unsigned, 32-bit integer to be sent.
 * @note Uses interrupts and therefore interrupts must be enabled. Stalls if
 *       internal storage is full. 
 */
void ESP8266_OutInteger(uint32_t n);
