/**
 * @file      odometry.h
 * @brief     Count wheel rotations and maintain robot position and angle
 * @details   Inputs to the system are left and right tachometer counts.
 * System is called periodically, e.g., every 20 to 40 ms.
 * Private global variables from the system are position (x,y) and orientation theta.
 * @version   TI-RSLK MAX v1.1
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2019 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @warning   This system will need calibration to your particular robot, adjust N D W C for your robot
 * @date      July 31, 2019
 *
 * Although tested with the Pololu #3542 Romi Encoder connected to Pololu RSLK 1.1 interface board,
 * the module should operate with any 2 wheel differential drive robot.
 ******************************************************************************/


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
#ifndef _ODOMETRY_H_
#define _ODOMETRY_H_
#include <stdint.h>
#define N 360     ///< counts/rotation, just one edge of one tach
#define D 70000   ///< wheel diameter 0.0001cm
#define W 140000  ///< wheel base 0.0001 cm
#define C 219910  ///< wheel circumference 0.0001cm
#define PI 8192   ///< representation of pi radians
#define TWOPI (2*PI) ///< 6.28...
#define NORTH 4096   ///< direction that is north
#define EAST  0      ///< direction that is east
#define SOUTH -4096  ///< direction that is south
#define WEST -8192   ///< direction that is west
#define XYTOLERANCE 10000      ///< tolerance in x,y goal seeking, 1 cm
#define THETATOLERANCE (4096/90)  ///< tolerance in angle goal seeking, 1 deg
/**
 * Initialize odometry module by specifying the initial position/angle of the robot.
 * Angles range from (-pi to +pi) signified by -8192 to +8191.
 * @param initx initial x position of the robot (units of 0.0001cm)
 * @param inity initial y position of the robot (units of 0.0001cm)
 * @param initTheta initial angle of the robot (units of 2*pi/16384 radians)
 * @return none
 * @brief  Initialize odometry module
 */
void Odometry_Init(int32_t initx, int32_t inity, int32_t initTheta);

/**
 * Inputs are wheel tachometer counts (n counts/rotation), and
 * outputs are private global variable of new position/angle
 * @param LCount number of left wheel tachometer counts from the last update
 * @param RCount number of right wheel tachometer counts from the last update
 * @return none
 * @brief  Compute new position/angle of the robot.
 */
void Odometry_Update(int32_t LCount, int32_t RCount);

/**
 * Specifies two power levels to use during odometry task, and
 * PWM levels must be between 0 to 14,998.
 * Obviously, the power level must be high enough to move robot
 * @param fast PWM level for moving forward
 * @param slow PWM level for turning
 * @return none
 * @brief  Set power levels
 */
void Odometry_SetPower(uint32_t fast, uint32_t slow);

/**
 * Fetch value of the current X position of the robot
 * @param none
 * @return position in units of 0.0001cm
 * @brief  Get X position
 */
int32_t Odometry_GetX(void);

/**
 * Fetch value of the current y position of the robot
 * @param none
 * @return position in units of 0.0001cm
 * @brief  Get y position
 */
int32_t Odometry_GetY(void);

 /**
  * Fetch value of the current theta angle of the robot
  * @param none
  * @return position units of 2*pi/16384 radians
  * @brief  Get theta angle
  */
int32_t Odometry_GetAngle(void);


/**
 * Get the current robot position.
 * @param x is pointer to store x position of the robot (units of 0.0001cm)
 * @param y is pointer to store y position of the robot (units of 0.0001cm)
 * @param theta is pointer to store angle (or direction) of the robot (units of 2*pi/16384 radians)
 * @return none
 * @note Assumes Odometry_Init() has been called at least once<br>
 * @note Typically Odometry_Update() is being called periodically (so LCount and RCount are less than 30)
 * @brief Get the current robot position
 */
void Odometry_Get(int32_t *x, int32_t *y, int32_t *theta);


/**
 * \enum RobotState
 * These are possible robot motions for odometry system
 * @brief robot state
 */
enum RobotState{
  ISSTOPPED, ///< stopped
  GOFORWARD, ///< going forward
  HARDRIGHT, ///< turning hard right
  HARDLEFT,  ///< turning hard left
  SOFTRIGHT, ///< turning soft right
  SOFTLEFT   ///< turning soft left
};



/**
 * Take wheel counts and update position
 * @param none
 * @return none
 * @note Must be called periodically so wheel counts are 5 to 20.
 * If both wheel counts are routinely below 5, update less frequently.
 * If either wheel count is routinely above 20, update more frequently.
 * @brief  update position
 */
void UpdatePosition(void);

/**
 * Show current state on OLED
 * @param none
 * @return none
 * @note uses SSD1306.c
 * @brief OLED output
 */
void Display(void);

/**
 * Wait until bumper touched
 * @param none
 * @return none
 * @note uses bump.c
 * @brief Wait until bumper touched
 */
void WaitUntilBumperTouched(void);

/**
 * Stop and wait until bumper touched
 * @param none
 * @return none
 * @note uses bump.c
 * @brief Stop and wait  until bumper touched
 */
void StopUntilBumperTouched(void);

/**
 * Go straight until bumper touched (crash)
 * @param none
 * @return none
 * @note uses bump.c
 * @brief Go until bumper touched
 */
uint32_t ForwardUntilCrash(void);

/**
 * Odometry command to Go straight until X goal has been reached.
 * This routine will run until goal is met.
 * @param desiredX desired X position in units of 0.0001cm
 * @return 0 for success, nonzero is failure (crash=bump sensor or 0xFF=going wrong way)
 * @brief Forward Until X goal
 */
uint32_t ForwardUntilX(int32_t desiredX);

/**
 * Odometry command to Go straight until Y goal has been reached.
 * This routine will run until goal is met.
 * @param desiredY desired Y position in units of 0.0001cm
 * @return 0 for success, nonzero is failure (crash=bump sensor or 0xFF=going wrong way)
 * @brief Forward Until Y goal
 */
uint32_t ForwardUntilY(int32_t desiredY);

/**
 * Odometry command to soft left until theta goal has been reached.
 * This routine will run until goal is met.
 * @param desiredTh desired theta position in units of 2*pi/16384 radians
 * @return 0 for success, nonzero is failure (crash=bump sensor or 0xFF=going wrong way)
 * @brief Soft Left Until Theta
 */
uint32_t SoftLeftUntilTh(int32_t desiredTh);


/**
 * Odometry command to Go straight until X goal has been reached.
 * This routine will start the command, but return immediately
 * @param thedesiredX desired X position in units of 0.0001cm
 * @return none
 * @brief start going Forward Until X
 */
void ForwardUntilXStart(int32_t thedesiredX);

/**
 * Odometry command to check status of Go straight until X command.
 * Assumes ForwardUntilXStart has been issued
 * @param none
 * @return true if done or error, false if still running ok
 * @brief check status of Forward Until X command
 */
uint32_t ForwardUntilXStatus(void);
/**
 * Odometry command to Go straight until Y goal has been reached.
 * This routine will start the command, but return immediately
 * @param thedesiredY desired Y position in units of 0.0001cm
 * @return none
 * @brief start going Forward Until Y
 */
void ForwardUntilYStart(int32_t thedesiredY);

/**
 * Odometry command to check status of Go straight until Y command.
 * Assumes ForwardUntilYStart has been issued
 * @param none
 * @return true if done or error, false if still running ok
 * @brief check status of Forward Until Y command
 */
uint32_t ForwardUntilYStatus(void);

/**
 * Odometry command to soft left until theta goal has been reached.
 * This routine will start the command, but return immediately
 * @param thedesiredTh desired theta position in units of 2*pi/16384 radians
 * @return none
 * @brief start going Forward Until Y
 */
void SoftLeftUntilThStart(int32_t thedesiredTh);

/**
 * Odometry command to check status of soft left until theta  command.
 * Assumes SoftLeftUntilThStart has been issued
 * @param none
 * @return true if done or error, false if still running ok
 * @brief check status of soft left until theta command
 */
uint32_t  ForwardUntilThStatus(void);

/**
 * \enum OdometryCommand
 * These are possible odometry commands
 * @brief Odometry Commands
 */
enum OdometryCommand{
  STOP,       ///< stop robot
  FORWARDTOX, ///< move forward straight until X is matched within tolerance XYTOLERANCE
  FORWARDTOY, ///< move forward straight until Y is matched within tolerance XYTOLERANCE
  LEFTTOTH,   ///< turn left until theta is matched within tolerance THETATOLERANCE
  RIGHTTOTH   ///< turn right until theta is matched within tolerance THETATOLERANCE
};

/**
 * Odometry command to check status of any odometry command.
 * Assumes ForwardUntilXStart ForwardUntilYStart or SoftLeftUntilThStart has been issued
 * @param none
 * @return true if done or error, false if still running ok
 * @brief check status of odometry command
 */
uint32_t CheckGoal(void);
#endif
