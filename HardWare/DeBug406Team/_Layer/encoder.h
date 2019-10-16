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
#include <stdio.h>
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern int16_t _LeftEncoderFeedback;
extern int16_t _RightEncoderFeedback;

//路程统计相关变量
extern uint8_t _Encoder_CountControl;
extern float _Encoder_DistantCount;

/* Exported functions ------------------------------------------------------- */
void Encoder_Init(void);
float Encoder_GetLWheelSpeed(void);
float Encoder_GetRWheelSpeed(void);

/* 轮速反馈与距离计算相关函数 ----------------------------------------------- */
void Encoder_ResetDistant(void);
void Encoder_StartDistantCount(void);
void Encoder_StopDistantCount(void);
float Encoder_GetCurrentDistantCount(void);

void BeginCount(void);
void StopCount(void);
#endif /* DEBUGROB_ENCODER_H */
