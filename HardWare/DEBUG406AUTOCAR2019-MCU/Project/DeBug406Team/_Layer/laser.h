/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/laser.h 
  * @author  Debug406 Team
  * @date    2-January-2019
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
    # Laser(NPN)   High E-Level Distance: Xcm ~ Inf 
                 * Low  E-Level Distance: 0cm ~ Ycm            
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DEBUGROB_LASER_H
#define DEBUGROB_LASER_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
typedef enum laserState__
{
  COVERED = 0,
  OPENNING
} LaserState_t;
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions (F12)-------------------------------------------------- */
void Laser_Init(void);
LaserState_t GetLaserState(void);

/* Reference Only -> (F12) EXTI0_IRQHandler(); */
/*static*/ inline void _LaserEdgeTrigger_Interrupt(void);

#endif /* !DEBUGROB_LASER_H */
