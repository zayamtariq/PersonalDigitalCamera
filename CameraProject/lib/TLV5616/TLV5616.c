/**
 * @file TLV5616.c
 * @author your name (you@domain.com)
 * @brief Low level driver for the TLV5616 12-bit SPI DAC.
 * @version 0.2.0
 * @date 2022-09-26
 *
 * @copyright Copyright (c) 2022
 * @note Reference datasheet:
 *     https://www.ti.com/lit/ds/symlink/tlv5616.pdf?ts=1644622732627&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FTLV5616
 */

#include "./lib/TLV5616/TLV5616.h"
#include "./inc/tm4c123gh6pm.h"

int tlv5616_init() {
    /**
     * Unified_Port_Init in Lab5.c calls Port_D_Init, which initializes the Port
     * D GPIOs for the appropriate alternate functionality (SSI).
     *
     * According to Table 15-1. SSI Signals in the datasheet, this corresponds
     * to SSI1. The corresponding Valvanoware register defines are at L302 and
     * L2670 in inc/tm4c123gh6pm.h. Use this in combination with the datasheet
     * or any existing code to write your driver! An example of how to
     * initialize SSI is found in L741 in inc/ST7735.c.
     */
    return 1; // UNIMPLEMENTED
}

int tlv5616_output(uint16_t data) {
    // An example of how to send data via SSI is found in L534 of inc/ST7735.c.
    return 1; // UNIMPLEMENTED
}
