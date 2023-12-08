/**
 * @file TLV5616.h
 * @author your name (you@domain.com)
 * @brief Low level driver for the TLV5616 12-bit SPI DAC.
 * @version 0.2.0
 * @date 2022-09-26
 * 
 * @copyright Copyright (c) 2022
 * @note Reference datasheet: 
 *       https://www.ti.com/lit/ds/symlink/tlv5616.pdf?ts=1644622732627&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FTLV5616
 */

#include <stdint.h>

/**
 * TODO: THIS
 * @brief Design your driver here! There are two things your driver needs to do:
 *
 * 1. Initialize SPI for your specific hardware (see page 965 on the TM4C
 *    datasheet).
 * 2. Accept a digital value (from 0 to (2^12 - 1)) and output a 16-bit data
 *    word via SPI that will be sent to the chip (see page 12 on the TLV5616
 *    datasheet).
 * 
 * Provided are two possible function declarations. You may modify this based on
 * your implementation and API design.
 */


/**
 * @brief tlv5616_init initializes SPI at pins <TODO: STATE PINS HERE> to
 *        communicate with the TLV5616 12-bit SPI DAC.
 * 
 * @return int 0 if initialization was successful or a positive integer if an
 *         error occurred. <TODO: LIST ERROR CODES AND POSSIBLE ERROR EVENTS>
 */
int tlv5616_init();

/**
 * @brief tlv5616_output tells the TLV5616 to output a specified voltage.
 * 
 * @param data 12-bit data value representing the voltage that should be output
 *        to the TLV4616.
 * @return int 0 if initialization was successful or a positive integer if an
 *         error occurred. <TODO: LIST ERROR CODES AND POSSIBLE ERROR EVENTS>
 */
int tlv5616_output(uint16_t data);
