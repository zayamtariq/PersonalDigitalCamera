/**
 * @file      IRDistance.h
 * @brief     Take infrared distance measurements
 * @details   Provide mid-level functions that convert raw ADC
 * values from the GP2Y0A21YK0F infrared distance sensors to
 * distances in mm.
 * @version   derived from TI-RSLK MAX v1.1
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2020 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      January 15, 2020

 ******************************************************************************/

/* This example accompanies the book
   "Embedded Systems: Introduction to Robotics,
   Jonathan W. Valvano, ISBN: 9781074544300, copyright (c) 2020
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/

Simplified BSD License (FreeBSD License)
Copyright (c) 2020, Jonathan Valvano, All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are
those of the authors and should not be interpreted as representing official
policies, either expressed or implied, of the FreeBSD Project.
*/

// 5V connected to all GP2Y0A21YK0F Vcc's (+5V)
// 5V connected to positive side of 10 uF capacitors physically near the sensors
// ground connected to all GP2Y0A21YK0F grounds and microcontroller ground
// ground connected to negative side of all 10 uF capacitors



#ifndef IRDISTANCE_H_
#define IRDISTANCE_H_


/**
 * Convert ADC sample into distance for the GP2Y0A21YK0F
 * infrared distance sensor.  Conversion uses a calibration formula<br>
 * D = A/(adcSample + B) + C
 * @param adcSample is the 12-bit ADC sample 0 to 4095
 * @param sensor is sensor number 0 to 3
 * @return distance from robot to wall (units mm)
 * @brief  Convert infrared distance measurement
 */
int32_t IRDistance_Convert(int32_t adcSample, uint32_t sensor);


#endif /* IRDISTANCE_H_ */
