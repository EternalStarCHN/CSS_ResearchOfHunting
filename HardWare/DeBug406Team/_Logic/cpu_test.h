/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/line.h 
  * @author  Debug406 Team
  * @date    20-March-2019
  * @brief   Header for cpu_test.c module
  ******************************************************************************
  * @attention
	*	This file is used to describe the functions called when measureing CPU Usage.
	* In order to measure the usage of cpu in certain application. You need to call
	* "CPU_UsageTestInit()" funtion to initialize the CPU usage test routine in your
	* code's initialize area. Then insert
	*																	    GPIO_SetBit(GPIOD,GPIO_Pin_14);
	* At the start positon of the program that you want to measure performance.
	* 												Then insert 
	*					                            GPIO_ResetBit(GPIOD,GPIO_Pin_14);
	* At the end positon of the program that you want to measure performance.
	* And use a oscilloscope measure the High/Low ratio of PD14. This ratio represent
	* the Percent of CPU usage.
	*             
  ******************************************************************************
  */
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CPU_TEST_H
#define CPU_TEST_H

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "stdint.h"
#include "stm32f4xx_gpio.h"


/* Exported functions ------------------------------------------------------- */
void CPU_UsageTestInit(void);


#endif
