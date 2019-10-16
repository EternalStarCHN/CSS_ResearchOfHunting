/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/motor.h
  * @author  Debug406 Team
  * @date    15-Fabrulary-2018
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
    ##  PC6 -> TIM3_CH1 -> Right motor
        PC7 -> TIM3_CH2 -> Left motor

        PG2 -> Right Motor enable-2
        PG3 -> Right Motor enable-1
        PG4 -> Left  Motor enable-2
        PG5 -> Left  Motor enable-1

    ## -----------------------------------------------------------------------
      TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles.

      In this example TIM3 input clock (TIM3CLK) is set to 2 * APB1 clock (PCLK1),
      since APB1 prescaler is different from 1.
        TIM3CLK = 2 * PCLK1
        PCLK1 = HCLK / 4
        => TIM3CLK = HCLK / 2 = SystemCoreClock /2 = 84MHz

      To get TIM3 counter clock at 21 MHz, the prescaler is computed as follows:
         Prescaler = (TIM3CLK / TIM3 counter clock) - 1
         Prescaler = ((SystemCoreClock /2) /1 MHz) - 1

      To get TIM3 output clock at 30 KHz, the period (ARR)) is computed as follows:
         ARR = (TIM3 counter clock / TIM3 output clock) - 1
             = 2000

      TIM3 Channel1 duty cycle = (1000 / TIM3_ARR)* 100 = 50%
      TIM3 Channel2 duty cycle = ( 750 / TIM3_ARR)* 100 = 37.5%
      TIM3 Channel3 duty cycle = ( 500 / TIM3_ARR)* 100 = 25%
      TIM3 Channel4 duty cycle = ( 250 / TIM3_ARR)* 100 = 12.5%

      Note:
       SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
       Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
       function to update SystemCoreClock variable value. Otherwise, any configuration
       based on this variable will be incorrect.
    ----------------------------------------------------------------------- ##
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DEBUGROB_MOTOR_H
#define DEBUGROB_MOTOR_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* test only */
#include "encoder.h"
/*           */


/* Exported types ------------------------------------------------------------*/
typedef enum motorMode__
{
    /* Default */
    MOTOR_STOP = 0,
    /* Go Front */
    MOTOR_FRONT,
    /* Turn Left */
    MOTOR_TURN_LEFT,
    /* Turn Right */
    MOTOR_TURN_RIGHT,
    /* Go Back */
    MOTOR_BACK

} MotorMode_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void MotorPWM_Init(void);

int SetMotorDutyRatio(double leftDutyRatio, double rightDutyRatio);
int SetMotorPulse(int32_t leftPulse, int32_t rightPulse);

/* state machine relative funtion, only use below funtion in state machine mode*/
void inline SetMotorState(MotorMode_t motorMode);
MotorMode_t UpgradeMotorState(void);
/* ************************************************************************** */

MotorMode_t UpdateMotorState(MotorMode_t motorMode);

int32_t inline GetMotorSpeed(int8_t leftOrRight);

/* Test functions ------------------------------------------------------------*/
void SetMotorForwardTime(uint32_t sec);
inline void _forwardTime_Interrupt(void);

void SetWheelSpeed(float LTarGetSpeed, float RTarGetSpeed);

#endif /* DEBUGROB_MOTOR_H */
