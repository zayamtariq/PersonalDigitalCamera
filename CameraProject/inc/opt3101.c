// opt3101.c
// Runs on TM4C123
// Busy-wait and interrupt device driver for the I2C0.
// Daniel and Jonathan Valvano
// Jan 12, 2020
// This file originally comes from the TIDA-010021 Firmware (tidcf48.zip) and
// was modified by Pololu to support the MSP432P401R. Modified again for TM4C123

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

#include <stdint.h>
#include "../inc/opt3101.h"
#include "../inc/tm4c123gh6pm.h"
#include "../inc/I2CB1.h"
#include "../inc/PLL.h"
#include "../inc/CortexM.h"

// EE445M robot hardware connections
// edited by Valvano and Valvano 1/3/2020
// OPT3101 hardware
// GND    ground
// VIN    3.3V
// SDA    OPT3101 pin 14 <> PB3 I2C data SDA_S
// SCL    OPT3101 pin 13 <- PB2 I2C clock SCL_S
// GP1    OPT3101 pin 11 -> PC4 input from the DATA_RDY
// GP2    OPT3101 pin 12 ->  Not used
// RST_MS OPT3101 pin 17 <- PC5 output low to reset the OPT3101
/*
 *  I2C0 Conncection | I2C1 Conncection | I2C2 Conncection | I2C3 Conncection
 *  ---------------- | ---------------- | ---------------- | ----------------
 *  SCL -------- PB2 | SCL -------- PA6 | SCL -------- PE4 | SCL -------- PD0
 *  SDA -------- PB3 | SDA -------- PA7 | SDA -------- PE5 | SDA -------- PD1
 */
// **********ST7735 TFT and SDC*******************
// ST7735
// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) connected to PB0
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

// HC-SR04 Ultrasonic Range Finder 
// J9X  Trigger0 to PB7 output (10us pulse)
// J9X  Echo0    to PB6 T0CCP0
// J10X Trigger1 to PB5 output (10us pulse)
// J10X Echo1    to PB4 T1CCP0
// J11X Trigger2 to PB3 output (10us pulse) ***used by OPT3101***
// J11X Echo2    to PB2 T3CCP0              ***used by OPT3101***
// J12X Trigger3 to PC5 output (10us pulse) ***used by OPT3101***
// J12X Echo3    to PF4 T2CCP0

// Ping))) Ultrasonic Range Finder 
// J9Y  Trigger/Echo0 to PB6 T0CCP0
// J10Y Trigger/Echo1 to PB4 T1CCP0
// J11Y Trigger/Echo2 to PB2 T3CCP0   ***used by OPT3101***
// J12Y Trigger/Echo3 to PF4 T2CCP0

// IR distance sensors
// J5/A3/PE3
// J6/A2/PE2
// J7/A1/PE1
// J8/A0/PE0   

#define I2C_ADDRESS 0x58
typedef struct{
  uint32_t sub_frame_count;
  uint32_t xtalk_filt_time_const;
  uint32_t crosstalk_settling_time_ms;
} OPT3101Speed_t;
const OPT3101Speed_t OPT3101Choices[10]={
{  1,  10,   10}, //   didn't work
{  2,  9,    20}, //   4.02ms
{  4,  8,    40}, //   7.04ms
{  8,  7,    80}, //   6.03ms
{ 16,  6,   160}, //   8.05ms
{ 32,  5,   320}, //   8.05ms
{ 64,  4,   640}, //  16.1ms
{128,  3,  1280}, //  32.3ms
{256,  2,  2560}, //  64.4ms
{512,  1,  5120}  // 128.8ms
};

// (speed of light) / (2 * 10 MHz * 0x10000) = 0.22872349395 mm
// Valvano removed floating point, converted to binary fixed point
#define MM_PER_PHASE_COUNT 0.22872349395
#define BinFixMM_PER_PHASE_COUNT 14990 // divided by 65536
#define BinFix 16
// We will do 128 sub-frames per frame.
uint32_t SUB_FRAME_COUNT=128;

// Time constant of the crosstalk filter.  This is set according to equation 6,
// section 4.2.1, of sbau310.pdf, and depends on SUB_FRAME_COUNT above.
uint32_t XTALK_FILT_TIME_CONST=3;

// Assuming SUB_VD_CLK_CNT has not been changed, each sub-frame takes 0.25 ms,
// and each frame will take 32 ms.  The datasheet documentation of
// ILLUM_XTALK_CALIB and INT_XTALK_CALIB says we should wait at least
// 5 << XTALK_FILT_TIME_CONST frames to get good crosstalk readings.
// (5 << 3) * 128 * (0.25 ms) = 1280
uint32_t CROSSTALK_SETTLING_TIME_MS=1280;

static uint32_t reg08, reg09;
//uint32_t r1,r2,d3;
uint32_t OPT3101_ReadRegister(uint8_t address){
  uint8_t buffer[3];
  I2C0_Send1(I2C_ADDRESS, address);
  I2C0_Recv3(I2C_ADDRESS, buffer);
  return buffer[0] + ((uint32_t)buffer[1] << 8) + ((uint32_t)buffer[2] << 16);
}
//uint32_t OPT3101_ReadRegister(uint8_t address){
//  uint8_t buffer[3];
//  I2C0_Send(I2C_ADDRESS, &address, 1);
//  I2C0_Recv(I2C_ADDRESS, buffer, 3);
//  return buffer[0] + ((uint32_t)buffer[1] << 8) + ((uint32_t)buffer[2] << 16);
//}
void OPT3101_WriteRegister(uint8_t address, uint32_t data){
  I2C0_Send4(I2C_ADDRESS,  
    address,
    data & 0xFF,
    (data >> 8) & 0xFF,
    (data >> 16)& 0xFF);
}
//void OPT3101_WriteRegister(uint8_t address, uint32_t data){
//  uint8_t buffer[] = {
//    address,
//    data & 0xFF,
//    data >> 8 & 0xFF,
//    data >> 16 & 0xFF
//  };

//  I2C0_Send(I2C_ADDRESS, buffer, sizeof(buffer));
//}
//int c=0;
#define PC4   (*((volatile uint32_t *)0x40006040))
#define PC5   (*((volatile uint32_t *)0x40006080))
void OPT3101_Init(uint32_t speed){
  if((speed==0)||(speed>9)) speed = 7;
  SUB_FRAME_COUNT            = OPT3101Choices[speed].sub_frame_count;
  XTALK_FILT_TIME_CONST      = OPT3101Choices[speed].xtalk_filt_time_const;
  CROSSTALK_SETTLING_TIME_MS = OPT3101Choices[speed].crosstalk_settling_time_ms;
	
  // Drive P6.3/AUXL/nRST_MS low to reset the OPT3101, then drive it high.
  SYSCTL_RCGCGPIO_R |= 0x00000004;  // 1) activate clock for Port C
  while((SYSCTL_PRGPIO_R&0x04) == 0){};// allow time for clock to stabilize
  GPIO_PORTC_DIR_R |= 0x20;       // PC5 out
  GPIO_PORTC_DIR_R &= ~0x10;      // PC4 input
  GPIO_PORTC_DEN_R |= 0x30;       // enable digital I/O on PC4 PC5
  PC5 = 0;    // RST_MS=0, reset
  Clock_Delay1ms(1);
  PC5 = 0x20; // RST_MS=1, reset released
  Clock_Delay1ms(1);

  // Make PC4 is an input for the DATA_RDY signal.
  // Set up P6.2/AUXR to detect low-to-high transitions.
  GPIO_PORTC_IS_R &= ~0x10;    // PC4 is edge-sensitive 
  GPIO_PORTC_IBE_R &= ~0x10;   // PC4 is not both edges 
  GPIO_PORTC_IEV_R |= 0x10;    // PC4 rising edge event 
  GPIO_PORTC_ICR_R = 0x10;     // clear flag4
  GPIO_PORTC_IM_R &= ~0x10;    // disarm interrupt on PC4

  // Wait until INIT_LOAD_DONE gets set to 1.
  while(!(OPT3101_ReadRegister(0x03) & 0x100)){
    Clock_Delay1ms(1);
  }
//  while(1){
//    OPT3101_ReadRegister(0x03);
//    c++;
//    Clock_Delay(2374);
//  }
}
//uint32_t Reg2a;
//uint32_t Reg27;
void OPT3101_Setup(void){
  // Set the overload flag observation window (TG_OVL_WINDOW_START).
  // This choice comes from the OPT3101 Configurator Tool version 0.8.0.
  OPT3101_WriteRegister(0x89, 7000);

  // Enable the temperature sensor.
  // This choice comes from the OPT3101 Configurator Tool version 0.8.0.
  {
    uint32_t reg6e = OPT3101_ReadRegister(0x6e);
    reg6e |= 0x80000;  // EN_TEMP_CONV = 1
    OPT3101_WriteRegister(0x6e, reg6e);
  }

  // Turn on clip mode for frequency-correction to phase.
  // This choice comes from the OPT3101 Configurator Tool version 0.8.0.
  {
    uint32_t reg50 = OPT3101_ReadRegister(0x50);
    reg50 |= 1;
    OPT3101_WriteRegister(0x50, reg50);
  }

  // Set NUM_SUB_FRAMES to SUB_FRAME_COUNT - 1.
  // Set NUM_AVG_SUB_FRAMES to the same value so we are averaging together
  // every sub-frame.
  OPT3101_WriteRegister(0x9f,
    (uint32_t)(SUB_FRAME_COUNT - 1) << 12 | (SUB_FRAME_COUNT - 1));

  // Set XTALK_FILT_TIME_CONST to the corresponding value.
  {
    uint32_t reg2e = OPT3101_ReadRegister(0x2e);
    reg2e = (reg2e & ~0xF00000) | (uint32_t)XTALK_FILT_TIME_CONST << 20;
    OPT3101_WriteRegister(0x2e, reg2e);
  }

  // Set up the GPIO1 pin (which is connected to P6.2/AUXR) to be
  // a data-ready signal.
  {
    uint32_t reg78 = OPT3101_ReadRegister(0x78);
    uint32_t reg0b = OPT3101_ReadRegister(0x0b);
    reg78 |= 0x1000;                   // GPIO1_OBUF_EN = 1
    reg78 = (reg78 & ~0x1C0) | 0x080;  // GPO1_MUX_SEL = 2 (DIG_GPO_0)
    reg0b = (reg0b & ~0xF) | 9;        // DIG_GPO_SEL0 = 9 (DATA_RDY)
    OPT3101_WriteRegister(0x78, reg78);
    OPT3101_WriteRegister(0x0b, reg0b);
  }

  {
    uint32_t reg2a = OPT3101_ReadRegister(0x2a);
    reg2a |= 0x8000;  // EN_ADAPTIVE_HDR = 1 : Adaptive HDR
    reg2a |= 1;       // EN_TX_SWITCH = 1    : Automatic channel-switching.
    OPT3101_WriteRegister(0x2a, reg2a);
//    Reg2a = OPT3101_ReadRegister(0x2a); // debugging
  }

  // Settings from the OPT3101 Configurator Tool 0.8.0 for monoshot mode.
  // No deep sleep, using the minimal startup delay allowed by the tool.
  {
    uint32_t reg27 = OPT3101_ReadRegister(0x27);
    reg27 |= 3;                             // MONOSHOT_MODE = 3
    reg27 = (reg27 & 0xFFFF03) | (1 << 2);  // MONOSHOT_NUMFRAME = 1
    OPT3101_WriteRegister(0x27, reg27);
//    Reg27 = OPT3101_ReadRegister(0x27); // debugging

  }
  {
    uint32_t reg76 = OPT3101_ReadRegister(0x76);
    reg76 |= 0x001;  // DIS_GLB_PD_REFSYS = 1
    reg76 |= 0x020;  // DIS_GLB_PD_AMB_DAC = 1
    reg76 |= 0x100;  // DIS_GLB_PD_OSC = 1
    OPT3101_WriteRegister(0x76, reg76);
  }
  {
    uint32_t reg26 = OPT3101_ReadRegister(0x26);
    reg26 = (reg26 & 0x0003FF) | (95 << 10);  // POWERUP_DELAY = 95
    OPT3101_WriteRegister(0x26, reg26);
  }
}

void OPT3101_CalibrateInternalCrosstalk(void){
  // Clear TG_EN because the OPT3101 datasheet says EN_SEQUENCER should only be
  // changed while TG_EN is 0.
  uint32_t reg80 = OPT3101_ReadRegister(0x80);
  reg80 &= ~1;  // TG_EN = 0
  OPT3101_WriteRegister(0x80, reg80);

  uint32_t orig_reg2a = OPT3101_ReadRegister(0x2a);
  uint32_t orig_reg2e = OPT3101_ReadRegister(0x2e);

  uint32_t reg2e = orig_reg2e;
  reg2e &= ~(1 << 6);      // USE_XTALK_REG_INT = 0
  reg2e |= 1 << 5;         // USE_XTALK_FILT_INT = 1 : Select filtered IQ.
  reg2e = reg2e & ~0xE00;  // IQ_READ_DATA_SEL = 0
  OPT3101_WriteRegister(0x2e, reg2e);

  uint32_t reg2a = orig_reg2a;
  reg2a &= ~(1 << 15);     // EN_ADAPTIVE_HDR = 0
  OPT3101_WriteRegister(0x2a, reg2a);

  uint32_t reg14 = OPT3101_ReadRegister(0x14);
  reg14 &= ~(1 << 16);     // EN_SEQUENCER = 0
  reg14 &= ~(1 << 17);     // EN_PROCESSOR_VALUES = 0
  OPT3101_WriteRegister(0x14, reg14);

  reg80 |= 1;  // TG_EN = 1
  OPT3101_WriteRegister(0x80, reg80);

  reg2e |= 1 << 4;  // INT_XTALK_CALIB = 1 : Start the calibration.
  OPT3101_WriteRegister(0x2e, reg2e);

  Clock_Delay1ms(CROSSTALK_SETTLING_TIME_MS);

  // The internal crosstalk values could be read from registers 0x3b and 0x3c
  // at this point.

  reg80 &= ~1;  // TG_EN = 0
  OPT3101_WriteRegister(0x80, reg80);

  OPT3101_WriteRegister(0x2a, orig_reg2a);
  OPT3101_WriteRegister(0x2e, orig_reg2e);

  reg80 |= 1;   // TG_EN = 1
  OPT3101_WriteRegister(0x80, reg80);
}

void OPT3101_StartMeasurement(void){
  // Set MONOSHOT_BIT to 1 to trigger a new measurement.
  // Assumption: The other bits in register 0 should be 0.
  OPT3101_WriteRegister(0x00, 0x800000);
}
// ch is 0,1,2 for channel
void OPT3101_StartMeasurementChannel(uint32_t ch){
  if(ch <= 2){
    uint32_t reg2a = OPT3101_ReadRegister(0x2a);
    reg2a |= 0x8000;  // EN_ADAPTIVE_HDR = 1 : Adaptive HDR
    reg2a &= ~0x07;   // EN_TX_SWITCH = 0    : Manual channel-switching.
    reg2a |= ch<<1;   // SEL_TX_CH bits 2:1 is 0,1,2
    OPT3101_WriteRegister(0x2a, reg2a);  // Set MONOSHOT_BIT to 1 to trigger a new measurement.
  }
  // Assumption: The other bits in register 0 should be 0.
  OPT3101_WriteRegister(0x00, 0x800000);
}
void OPT3101_ReadMeasurement(void){
  reg08 = OPT3101_ReadRegister(0x08);
  reg09 = OPT3101_ReadRegister(0x09);
}

bool OPT3101_MeasurementError(void){
  if (!((reg08 >> 20) & 0x0001)){
    // FRAME_STATUS is 0: The frame is invalid.
    return true;
  }

  if (reg09 >> 18 & 3){
    // PHASE_OVER_FLOW_F2 or SIG_OVL_FLAG is 1.
    return true;
  }

  return false;
}

uint32_t OPT3101_GetTxChannel(void){
  return (reg08 >> 18) & 0x0003;  // TX_CHANNEL
}

uint32_t OPT3101_GetAmplitude(void){
  return reg09 & 0xFFFF;  // AMP_OUT
}
//
uint32_t OPT3101_GetPhase(void){
  return reg08 & 0xFFFF;  // PHASE_OUT
}

uint32_t OPT3101_GetDistanceMillimeters(void){
// return OPT3101_GetPhase() * MM_PER_PHASE_COUNT; // removed floating point
  return (OPT3101_GetPhase() * BinFixMM_PER_PHASE_COUNT)>>BinFix;
}

// Returns true if we have a new distance measurement ready to be read, false otherwise.
bool OPT3101_CheckDistanceSensor(void){
  if ((GPIO_PORTC_RIS_R&0x10) == 0){
    // Data is not ready yet.
    return false;
  }
  return true;
}
uint32_t *PTxChan;
uint32_t *Pdistances;
uint32_t *Pamplitudes;
uint32_t ChannelCount[3]; // debugging monitor

uint32_t OPT3101_GetMeasurement(uint32_t distances[3], uint32_t amplitudes[3]){
  uint32_t channel,distance,amplitude;
  OPT3101_ReadMeasurement();

  distance  = OPT3101_GetDistanceMillimeters();
  amplitude = OPT3101_GetAmplitude();
  channel   = OPT3101_GetTxChannel();

  if (OPT3101_MeasurementError()){
    // Something went wrong getting the measurement.
      distance = 65535;
  }
  else if (amplitude < 150){
    // Low amplitude: ignore the distance.
      distance = 65534;
  }
  else if (distance > 10000){
    // The distance measurement probably underflowed and wrapped around
    // to a really big number (because of imperfect phase offset
    // calibration), so report 65533.
    distance = 65533;
  }

  // Clear the pin-change interrupt flag.
  GPIO_PORTC_ICR_R = 0x10;
  if (channel <= 2){
    ChannelCount[channel]++;
    distances[channel]  = distance;
    amplitudes[channel] = amplitude;
  }
  return channel;
}
void OPT3101_ArmInterrupts(uint32_t *pTxChan, uint32_t distances[3], uint32_t amplitudes[3]){
    // write this as part of Lab 15
  PTxChan = pTxChan;
  Pdistances = distances;
  Pamplitudes = amplitudes;
  // Make PC4/AUXR be an input for the DATA_RDY signal.
  GPIO_PORTC_DIR_R &= ~0x10;   // (c) make PC4 in 
  GPIO_PORTC_DEN_R |= 0x10;    //     enable digital I/O on PC4
  GPIO_PORTC_IS_R &= ~0x10;    // (d) PC4 is edge-sensitive 
  GPIO_PORTC_IBE_R &= ~0x10;   //     PC4 is not both edges 
  GPIO_PORTC_IEV_R |= 0x10;    //     PC4 rising edge event 
  GPIO_PORTC_ICR_R = 0x10;     // (e) clear flag4
  GPIO_PORTC_IM_R |= 0x10;     // (f) arm interrupt on PC4
  NVIC_PRI0_R = (NVIC_PRI0_R&0xFF00FFFF)|0x00A00000; // (g) priority 5
  NVIC_EN0_R = 4;              // (h) enable interrupt 2 in NVIC

}
// *PTxChan set to 0,1,2 when measurement done
void GPIOPortC_Handler(void){
	static uint32_t channel = 0;

  *PTxChan = OPT3101_GetMeasurement(Pdistances,Pamplitudes);
  GPIO_PORTC_ICR_R = 0xFF;     // clear all flags
	
	channel = (channel+1)%3;
  OPT3101_StartMeasurementChannel(channel);

}
