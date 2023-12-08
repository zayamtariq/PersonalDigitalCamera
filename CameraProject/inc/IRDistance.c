// IRDistance.c
// Runs on any microcontroller
// Provide mid-level functions that convert raw ADC
// values from the GP2Y0A21YK0F infrared distance sensors to
// distances in mm. STUDENTS WILL NEED TO CALIBRATE THEIR OWN SENSORS
// Jonathan Valvano
// Jan 15, 2020

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



#include <stdint.h>

/* Calibration data
distance measured from front of the sensor to the wall                
d(cm) 1/d    bL     al     aR   bR  adcSample d (0.01cm)  error
10    0.100  2813  2830  2820  2830  2823.25  1006        0.06
15    0.067  1935  1976  1986  1978  1968.75  1482       -0.18
20    0.050  1520  1500  1520  1550  1522.5   1966       -0.34
30    0.033  1040  1096  1028   933  1024.25  3099        0.99
  
      adcSample = 26813/d+159      2681300    
      d = 26813/(adcSample-159)      -159    
*/
const int32_t A[4] = { 268130,268130,268130,268130};
const int32_t B[4] = { -159,-159,-159,-159};
const int32_t C[4] = { 0,0,0,0};
const int32_t IRmax[4] = { 494,494,494,494};
int32_t IRDistance_Convert(int32_t adcSample, uint32_t sensor){        // returns left distance in mm
  if(adcSample < IRmax[sensor]){
    return 800;
  }
  return A[sensor]/(adcSample + B[sensor]) + C[sensor];
}



