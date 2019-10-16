/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/systick.h
  * @author  Debug406 Team
  * @date    15-Fabrulary-2018
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
    ## SystemClock's initialization see @func SystemInit() -> SetSysClock();
    ## (F12) @SysTick is a Timer count backwords, We set 21MHz as frequency.
       To see (F12) SysTick_Type know how to control it.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DEBUGROB_SYSTICK_H
#define DEBUGROB_SYSTICK_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "line.h"
#include "encoder.h"
#include "laser.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/**
  * Keil MDK is not support true or false.
  * but if it support that, use true and false.
  */
#define TRUE  1
#define FALSE 0

/* Exported functions (F12)-------------------------------------------------- */
void SysTick_Init(void);
void SysTickDelay(uint32_t nTime);
void _StateMachineTick_Interrupt(void);
void _GoLineDelay_Interrupt(void);
void _GoLineDelay(uint16_t MsTime);

void _Laser_AntiJitterFeedback(void);
void _UpdateEncoderFeedback(void);
//
/* Reference Only -> (F12) SysTick_Handler(); */
/*static*/ inline void _DelayOneMs_Interrupt(void);
inline void _GoLineDelay_Interrupt(void);
#endif /* !DEBUGROB_SYSTICK_H */
