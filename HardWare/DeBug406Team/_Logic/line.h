#include "encoder.h"
#include "buttom.h"
#include "math.h"
#include "gyro.h"
#include "encoder.h"

#include "magic.h"
/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/line.h 
  * @author  Debug406 Team
  * @date    1-March-2019
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
	*	This file is used to describe the functions called when 
	* hunting and how the related algorithms work.
  ******************************************************************************
  */
	#ifndef DEBUGROB_LOGIC_H
	#define DEBUGROB_LOGIC_H
	/*Hunting with laser tube and gray sensor return values*/
	/*
		This algorithm looks for two eigenvalues of the function fitted by the array
		of grayscale sensors returned via the ADC, namely the small value median and 
		the small value width.
	******************************************************************************
	*/
	/*Find the closest number in the data and return its PWM duty cycle*/
	double _FindRightSpeedLR(int NowSpeed);
//	/*Store PWM duty cycle versus speed()*/
//	double _SpeedToDutyCycleL(void);
	/*Use the core part of the grayscale sensor and laser tube algorithm. If you need to modify it, please refer to the README.MD file under the LOGIC file.*/
	double _TrackingCoreAlgorithm(void);
	double _TrackingCoreAlgorithmLowSpeed(void);
	double _TrackingCoreAlgorithmInBridge(void);
//	double _GoBridgeADC(void);
//	double _GoBridgePID(void);
	void _GoBridge(void);
	double _GetADCError(void);
	double _GetGYROError(void);
	void _GoLine(void);
	void _GoLineLowSpeed(void);
	void _GoLineMSpeed(void);
	void _GoLineLowSpeedDownBridge(void);
	float _GetLSpeedError(void);
	float _GetRSpeedError(void);
	void _GoCricle(void);
	void _GoCricleSpeedLock(void);
	double _TrackingCoreAlgorithmInCricle(void);
	void _BlockSpeed(void);
	
	#endif
