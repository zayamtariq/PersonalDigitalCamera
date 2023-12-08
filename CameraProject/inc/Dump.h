/**
 * @file Dump.h
 * @author your name (you@domain.com), Jonathan Valvano, Matthew Yu
 *    <TA NAME and LAB SECTION # HERE>
 * @brief 
 *    A set of debugging functions that capture data for later inspection.
 *    There are two primary methods:
 *       - DebugCapture will record data and time.
 *       - JitterMeasure will measure real time jitter.
 * @version 0.1
 * @date 2022-01-31 <REPLACE WITH DATE OF LAST REVISION>
 *
 * @copyright Copyright (c) 2022
 */
#pragma once

/** File includes. */
#include <stdint.h>

#define DUMPBUFSIZE 300

/**
 * @brief DumpInit initializes the debugging dump.
 * 
 * @note Holds 0 up to DUMPBUFSIZE-1 elements. Each element is 32-bit data and
 *       32-bit time.
 */
void DumpInit(void);

/**
 * @brief DumpCapture dumps a 32-bit unsigned integer into the array. Records
 *        time and data. Reads TIMER1_TAR_R to get the current 32-bit time.
 * 
 * @param data The data to be captured at TIMER1_TAR_R time.
 * @note This should be minimally intrusive.
 */
void DumpCapture(uint32_t data);

/**
 * @brief DumpCount returns how many elements are currently stored.
 * 
 * @return uint32_t A value from 0 to DUMPBUFSIZE-1.
 */
uint32_t DumpCount(void);

/**
 * @brief Allows users to look at debug data.
 * 
 * @return uint32_t* Pointer to the current debug data buffer.
 */
uint32_t* DumpData(void);

/**
 * @brief Allows users to look at debug times.
 * 
 * @return uint32_t* Pointer to the current debug time buffer.
 */
uint32_t* DumpTime(void);
 
/**
 * @brief JitterInit initializes jitter recordings to determine if task is real
 *        time.
 * 
 * @note Assume that JitterMeasure is called from a periodic real-time task.
 */
void JitterInit(void);

/**
 * @brief Measures jitter. Maintains the maximum and minimum elapsed times.
 * 
 * @note The first time called, JitterMeasure just measures current time. On
 *       subsequent calls, JitterMeasure measures the elapsed time from the
 *       previous call. 
 */
void JitterMeasure(void);

/**
 * @brief JitterGet returns the maximum minus minimum elapsed time. The result
 *        is the worst case from calling JitterInit to now. There is no error if
 *        this returns 0.
 * 
 * @return uint32_t The difference between maximum and minimum elapsed times.
 */
uint32_t JitterGet(void);
