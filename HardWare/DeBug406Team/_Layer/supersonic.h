/**
  ******************************************************************************
  * @file    Project/_Layer/supersonic.h
  * @author  Debug406 Team
  * @date    29-march-2019
  * @brief   Header for supersonic.c module
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SUPERSONIC_H_
#define SUPERSONIC_H_

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"

#include "systick.h"

#include "magic.h"

/* Exported functions --------------------------------------------------------*/

void SuperSonic_Init(void);
float SuperSonic_GetDistant(void);

#endif
