/**
 * @file      Fuzzy.h
 * @brief     Fuzzy Logic functions
 * @details   <b>Fuzzy logic</b> is a design pattern for digital controllers.
 * The key principle is true/false conditions can take on a spectrum
 * of possibilities from definitely true (255) to definitely false (0).
 * For example, a fuzzy logic value of 128 means maybe.
 * Like any controller, a fuzzy logic controller: collects information
 * from input sensors, performs calculations, makes decisions, and affects
 * the system by outputting to actuators. Also similar to other controllers,
 * the goal is to drive the system into a desired state.
 * In a fuzzy logic controller,  <b>crisp inputs</b> are the sensor measurements
 * with units, and <b>crisp outputs</b> are the actuator commands with units.
 * A <b>fuzzy membership set</b> is a collection of variables, having values from<br>
 * true (255,254,253,...,2,1,0) false.<br>
 * The <b>input membership set</b> defines the status of the system, and the fuzzy values
 * are derived from the sensors. The <b>output membership set</b> defines the
 * status of the actuators, and the values describe what the controller
 * intends to do in order to drive the system into the desired state.
 * More specifically, a fuzzy logic performs these steps:<br>
 1) <b>Measurements</b>: collect <b>crisp inputs</b> from sensors. <br>
 2) <b>Fuzzification</b>: converts crisp inputs to input fuzzy variables.<br>
 3) <b>Fuzzy logic</b>: converts input fuzzy variables to output fuzzy variables. <br>
 4) <b>Defuzzification</b>: converts output fuzzy variables to actuator commands, and perform outputs.<br>
 * @version   Any
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2019 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      December 22, 2019
 * */
/* This example accompanies the book
   "Embedded Systems: Introduction to Robotics,
   Jonathan W. Valvano, ISBN: 9781074544300, copyright (c) 2019
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/

Simplified BSD License (FreeBSD License)
Copyright (c) 2019, Jonathan Valvano, All rights reserved.

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
/**
 * \brief 8-bit fuzzy variable having values from true (255,254,253,...,2,1,0) false
 */
typedef uint8_t fuz_t;

/**
 * Fuzzy logic complement
 * @param u1 is a fuzzy variable having values from 0(false) to 255(true)
 * @return 255-u1
 * @brief  Complement
 */
fuz_t not(fuz_t u1);


/**
 * Fuzzy logic or
 * @param u1 is a fuzzy variable
 * @param u2 is a fuzzy variable
 * @return maximum of u1 u2
 * @brief  Fuzzy or
 */
fuz_t or(fuz_t u1,fuz_t u2);

/**
 * Fuzzy logic or
 * @param u1 is a fuzzy variable
 * @param u2 is a fuzzy variable
 * @param u3 is a fuzzy variable
 * @return maximum of u1 u2 u3
 * @brief  Fuzzy or
 */
fuz_t or3(fuz_t u1,fuz_t u2,fuz_t u3);

/**
 * Fuzzy logic and
 * @param u1 is a fuzzy variable
 * @param u2 is a fuzzy variable
 * @return minimum of u1 u2
 * @brief  Fuzzy and
 */
fuz_t and(fuz_t u1,fuz_t u2);

/**
 * Fuzzy logic and
 * @param u1 is a fuzzy variable
 * @param u2 is a fuzzy variable
 * @param u3 is a fuzzy variable
 * @return minimum of u1 u2 u3
 * @brief  Fuzzy and
 */
fuz_t and3(fuz_t u1,fuz_t u2,fuz_t u3);

/**
 * 32-bit unsigned minimum
 * @param u1 is a 32-bit unsigned number
 * @param u2 is a 32-bit unsigned number
 * @return minimum of u1 u2
 * @brief  minimum
 */
uint32_t umin32(uint32_t u1,uint32_t u2);

/**
 * 32-bit signed minimum
 * @param n1 is a 32-bit signed number
 * @param n2 is a 32-bit signed number
 * @param n3 is a 32-bit signed number
 * @return minimum of n1 n2 n3
 * @brief  minimum
 */
int32_t min32(int32_t n1,int32_t n2,int32_t n3);

/**
 * Fuzzy logic fuzzification, converting crisp input to fuzzy variable.<br>
 * If crisp is less than MIN, then fuzzy is true.<br>
 * If crisp is between MIN and MAX, then fuzzy is linear.<br>
 * If crisp is greater than MAX, then fuzzy is false.<br>
 *255                                 <br>
 * |----------                        <br>
 * |........... \                     <br>
 * |.............. \                  <br>
 * |................. \               <br>
 * |.................... \            <br>
 * |....................... ----------<br>
 * 0..........MIN.......MAX           <br>
 *                                    <br>
 * @note MIN must be less than MAX. Constants must not be equal.
 * @param crisp is a crisp input
 * @param MIN is threshold value for the crisp input
 * @param MAX is another threshold value for the crisp input
 * @return fuzzy value
 * @brief  Fuzzification
 */
fuz_t MinFuzzification(int32_t crisp, const int32_t MIN, const int32_t MAX);

/**
 * Fuzzy logic fuzzification, converting crisp input to fuzzy variable.<br>
 * If crisp is less than MIN, then fuzzy is false.<br>
 * If crisp is between MIN and CENTER, then fuzzy is linear.<br>
 * If crisp is CENTER, then fuzzy is true.<br>
 * If crisp is between CENTER and max, then fuzzy is linear.<br>
 * If crisp is greater than MAX, then fuzzy is false.<br>
 *255                                         <br>
 * |.................... -                    <br>
 * |................. / ...\                  <br>
 * |.............. / ........ \               <br>
 * |........... / .............. \            <br>
 * |........ / .................... \         <br>
 * |-------- ........................---------<br>
 * 0.....MIN ... CENTER ....... MAX      <br>
 *                                            <br>
 * @note MIN must be less than CENTER, and CENTER must be less than MAX. Constants must not be equal.
 * @param crisp is a crisp input
 * @param MIN is threshold value for the crisp input
 * @param CENTER is a second threshold value for the crisp input
 * @param MAX is a third threshold value for the crisp input
 * @return fuzzy value
 * @brief  Fuzzification
 */
fuz_t CenterFuzzification(int32_t crisp, const int32_t MIN, const int32_t CENTER, const int32_t MAX);

/**
 * Fuzzy logic fuzzification, converting crisp input to fuzzy variable.<br>
 * If crisp is less than MIN, then fuzzy is false.           <br>
 * If crisp is between MIN and LEFT, then fuzzy is linear.   <br>
 * If crisp is between LEFT and RIGHT, then fuzzy is true.   <br>
 * If crisp is between RIGHT and max, then fuzzy is linear.  <br>
 * If crisp is greater than MAX, then fuzzy is false.        <br>
 *255                                            <br>
 * |.................... ----                    <br>
 * |................. / ......\                  <br>
 * |.............. / ........... \               <br>
 * |........... / ................. \            <br>
 * |........ / ....................... \         <br>
 * |-------- ...........................---------<br>
 * 0.....MIN ... LEFT RIGHT.... MAX      <br>
 *                                            <br>
 * @note Constants must be ordered: MIN<LEFT<RIGHT<MAX. Constants must not be equal.
 * @param crisp is a crisp input
 * @param MIN is threshold value for the crisp input
 * @param LEFT is a second threshold value for the crisp input
 * @param RIGHT is a third threshold value for the crisp input
 * @param MAX is a fourth threshold value for the crisp input
 * @return fuzzy value
 * @brief  Fuzzification
 */
fuz_t LeftRightFuzzification(int32_t crisp, const int32_t MIN,
    const int32_t LEFT, const int32_t RIGHT, const int32_t MAX);

/**
 * Fuzzy logic fuzzification, converting crisp input to fuzzy variable.<br>
 * If crisp is less than MIN, then fuzzy is false.<br>
 * If crisp is between MIN and MAX, then fuzzy is linear.<br>
 * If crisp is greater than MAX, then fuzzy is true.<br>
 *255                                 <br>
 * |........................--------- <br>
 * |...................... /          <br>
 * |................... /             <br>
 * |................ /                <br>
 * |............. /                   <br>
 * |------------                      <br>
 * 0..........MIN.......MAX           <br>
 *                                    <br>
 * @note MIN must be less than MAX
 * @param crisp is a crisp input
 * @param MIN is threshold value for the crisp input
 * @param MAX is another threshold value for the crisp input
 * @return fuzzy value
 * @brief  Fuzzification
 */
fuz_t MaxFuzzification(int32_t crisp, const int32_t MIN, const int32_t MAX);

/**
 * Fuzzy logic display helper function
 * return 'a' + (25*fuz)/255;
 * @param fuz is a fuzzy variable having values from 0(false) to 255(true)
 * @return ASCII character from 'a' to 'z'
 * @brief  convert fuzzy variable to ASCII character
 */
char FuzzyChar(fuz_t fuz);

