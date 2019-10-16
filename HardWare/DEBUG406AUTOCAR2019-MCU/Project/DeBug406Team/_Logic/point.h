#include "laser.h"
#include "buttom.h"
#include "gyro.h"
#include "UpHillOrDownHill.h"
#include "SKILL.h"
#include "magic.h"
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
	#ifndef DEBUGROB_POINT_H
	#define DEBUGROB_POINT_H
	void _FindPointGo(void);
	void _FindPointStop(void);
	void _ArrivePlatform(void);
	
	void _ArrivePlatformBackUp_Gray(void);
	void _ArrivePlatformBackUp_Delay(void);
	void _ArrivePlatformBackUp_Encoder(void);
	
	uint32_t _CalculationADC(void);
	uint32_t _UpdateButtomValue(void);
	uint32_t _FindMax(uint32_t L[]);
	extern int PointFlag;
	
	#endif
