/**
  ******************************************************************************
  * @file    Project/Debug406Team/_Logic/motor_turn.h
  * @author  Debug406 Team
  * @date    17-March-2019
  * @brief   Header for motor_turn.c module
  ******************************************************************************
  * @attention
	*	This file contains a set of funtion for vehicle heading adjustment.
	* 
  ******************************************************************************
  */
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MOTOR_TURN_H
#define MOTOR_TURN_H


/* Includes ------------------------------------------------------------------*/
#include "motor.h"
#include "gyro.h"
#include "systick.h"
#include "stdlib.h"
#include "math.h"

#include "magic.h"


/* Exported functions ------------------------------------------------------- */
void Motor_TurnLeftBlockingMode(float Angle);
void Motor_TurnRightBlockingMode(float Angle);
void Motor_TurnToAbsoluteYawAngle(float Angle);
void Motor_TurnRightNoInFlatGround(float Angle);
void Motor_TurnLeftNoInFlatGround(float Angle);
void Motor_TurnLeft180OnPlatform(void);

void Motor_TurnLeftDelayBlockingMode(uint16_t ms, float LPWM, float RPWM);
void Motor_TurnRightDelayBlockingMode(uint16_t ms, float LPWM, float RPWM);

void Motor_TurnLeft180OnPlatform_UserAdjust(float LPWM,float RPWM,float AimAngle);
void Motor_TurnGoMidByGray(void);

#endif
