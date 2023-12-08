/**
 * @file      I2CB1.h
 * @brief     Low-level software driver for the OPT3101 time of flight sensor
 * @details   TM4C123 is master. I2C0 is connected to OPT3101<br>
 * This file originally comes from the TIDA-010021 Firmware (tidcf48.zip) and<br>
 * was modified by Pololu to support the MSP432P401R. . Modified again for TM4C123
 * @version   EE445M robot
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2020 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      Nov 7, 2020
<table>
<caption id="I2Cports">I2C connected to OPT3101</caption>
<tr><th>Pin   <th>signal
<tr><td>GND   <td>ground
<tr><td>VIN   <td>3.3V
<tr><td>PB3   <td>I2C SDA data (bidirectional)
<tr><td>PB2   <td>I2C SCL clock (TM4C123 output)
</table>
 ******************************************************************************/


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

// TM4C123 hardware
// SDA    PB3 I2C data
// SCL    PB2 I2C clock
/*
 *  I2C0 Conncection | I2C1 Conncection | I2C2 Conncection | I2C3 Conncection
 *  ---------------- | ---------------- | ---------------- | ----------------
 *  SCL -------- PB2 | SCL -------- PA6 | SCL -------- PE4 | SCL -------- PD0
 *  SDA -------- PB3 | SDA -------- PA7 | SDA -------- PE5 | SDA -------- PD1
 */
 #ifndef _I2CB1_H_
#define _I2CB1_H_
#include <stdint.h>

/**
 * I2C0_Init
 * ----------
 * let t be bus period, let F be bus frequency,
 * let f be I2C frequency.
 * at F=80 MHz, I2C period = (TPR+1)*250ns.<br> 
 * f=400kHz,    I2C period = 20*(TPR+1)*12.5ns = 2.5us, with TPR=9<br>
 * I2C period, 1/f = 20*(TPR+1)*t <br>
 * F/f = 20*(TPR+1)<br>
 * TPR = (F/f/20)-1 
 * @param  I2Cfreq  desired frequency of I2C in Hz.
 * @param  busFreq  bus frequency in Hz.
 * ----------
 * @brief write 1 or more bytes to I2C0.
 */
void I2C0_Init(uint32_t I2Cfreq, uint32_t busFreq);

/**
 * I2C0_Send
 * ----------
 * @param  slaveAddr  address of slave device.
 * @param  pData      data address of data to be writen.
 * @param  count      number of bytes to be writen.
 * ----------
 * @brief write 1 or more bytes to I2C0.
 */
int I2C0_Send(uint8_t slaveAddr, uint8_t *pData, uint32_t count);

/**
 * I2C0_Send1
 * ----------
 * @param  slaveAddr  address of slave device.
 * @param  data       data to be writen.
 * ----------
 * @brief write 1 byte to I2C0.
 */
 int I2C0_Send1(uint8_t slaveAddr, uint8_t data);
 
 /**
 * I2C0_Send2
 * ----------
 * @param  slaveAddr  address of slave device.
 * @param  data1      first data to be writen.
 * @param  data2      second data to be writen.
 * ----------
 * @brief write 1 byte to I2C0.
 */
 int I2C0_Send2(int8_t slave, uint8_t data1, uint8_t data2);
/**
 * I2C0_Send4
 * ----------
 * @param  slaveAddr  address of slave device.
 * @param  data1      data to be writen.
 * @param  data2      data to be writen.
 * @param  data3      data to be writen.
 * @param  data4      data to be writen.
 * ----------
 * @brief write 4 bytes to I2C0.
 */
 int I2C0_Send4(uint8_t slaveAddr, uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4);

/**
 * I2C0_Recv
 * ----------
 * @param  slaveAddr  address of slave device.
 * @param  pData      data address to store read data.
 * @param  count      number of bytes to be read.
 * ----------
 * @brief read 1 or more bytes from I2C0.
 */
int I2C0_Recv(uint8_t slaveAddr, uint8_t *pData, uint32_t count);

/**
 * I2C0_Recv3
 * ----------
 * @param  slaveAddr  address of slave device.
 * @param  data       data address to store read data.
 * @param  count      number of bytes to be read.
 * @brief read 2 or more bytes from I2C0.
 */
int I2C0_Recv3(uint8_t slaveAddr, uint8_t data[3]);

/**
 * I2C0_SendData
 * ----------
 * @param  slaveAddr  address of slave device.
 * @param  pData      data address of data to be writen.
 * @param  count      number of bytes to be writen.
 * ----------
 * @brief write many bytes to I2C0.
 */
int I2C0_SendData(uint8_t slaveAddr, uint8_t *pData, uint32_t count);

#endif // _I2CB1_H_

