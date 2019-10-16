/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/board.h
  * @author  Debug406 Team
  * @date    15-Fabrulary-2018
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
     ## BOARD :  STM32F407ZGT6 mini
     ## LED   :  PG15
     ## USB   :  TX -> PD8
                 RX -> PD9
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DEBUGROB_BOARD_H
#define DEBUGROB_BOARD_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void STMMiniBoard_Init(void);
void SetIndicatorTimeBase(uint32_t timeBase, uint32_t timeSet);

/*static*/ inline void _updateSysTick_Interrupt(void);
#endif /* DEBUGROB_BOARD_H */
