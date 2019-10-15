/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/encoder.h 
  * @author  Debug406 Team
  * @date    13-March-2018
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
    ## Encoder -* GROUP A *- 
               PA0  -> TIM5_CH1 - Right Front encoder
               PA15 -> TIM2_CH1 - Left Rear encoder

    ## Encoder -* GROUP B *-             
               PA1  -> TIM5_CH2 - Right Front encoder
               PB3  -> TIM2_CH2 - Left Rear encoder
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DEBUGROB_ENCODER_H
#define DEBUGROB_ENCODER_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Encoder_Init(void);
uint32_t inline GetRFSpeed(uint8_t nTimesAverage);
uint32_t inline GetLRSpeed(uint8_t nTimesAverage);
uint32_t GetRFTriggerTimes(void);
uint32_t GetLRTriggerTimes(void);
int WaittingTrigger(uint16_t pluseCount);
int WaittingTriggerWithFunc(uint16_t pluseCount, void (* func)(void));

/* Reference Only -> (F12) TIM5_IRQHandler(); */
/*static*/ inline void _Timer5Capture_Interrupt(void);
/* Reference Only -> (F12) TIM2_IRQHandler(); */
/*static*/ inline void _Timer2Capture_Interrupt(void);

#endif /* DEBUGROB_ENCODER_H */
