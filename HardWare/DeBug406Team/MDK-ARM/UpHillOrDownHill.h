#include "gyro.h"
#include "main.h"
/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/line.h 
  * @author  Debug406 Team
  * @date    10-March-2019
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
	*	This file is used to describe the functions called when 
	* hunting and how the related algorithms work.
  ******************************************************************************
  */
	#ifndef DEBUGROB_UpDown_H
	#define DEBUGROB_UpDown_H
typedef enum 
{
  UP = 1,
  DOWN	= -1,
	FlatGround = 0
	
} UpHillOrDownHill;
typedef enum 
{
		BALANCE=1,
		UNBALANCE=0
} IsBalance;
UpHillOrDownHill _UpHillOrDownHillFeedBack(void);
IsBalance _IsBalanceFeedBack(void);
void _UpHillOrDownHillCheck(void);
void _GetPrimaryRollAngle(void);
void _IsBalance(void);
void _ResetUpDown(void);
void _InitNowYawAngle(void);

extern float _NowGyroYawAngle;
#endif

