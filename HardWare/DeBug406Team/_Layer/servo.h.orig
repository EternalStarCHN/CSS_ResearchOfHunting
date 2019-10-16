/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/servo.h 
  * @author  Debug406 Team
  * @date    15-Fabrulary-2018
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
    ## PD12 -> TIM4_CH1 -> Head  servo
       PD13 -> TIM4_CH2 -> Right servo
       PD14 -> TIM4_CH3 -> Left  servo
       
    ## -----------------------------------------------------------------------
      TIM4 Configuration: generate 4 PWM signals with 4 different duty cycles.
      
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
             = 20000
                    
      TIM3 Channel1 duty cycle = (100 / TIM3_ARR)* 100 = 0.5%
      TIM3 Channel2 duty cycle = (300 / TIM3_ARR)* 100 = 1.5%
      TIM3 Channel3 duty cycle = (400 / TIM3_ARR)* 100 = 2.0%
      TIM3 Channel4 duty cycle = (500 / TIM3_ARR)* 100 = 2.5%

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
#ifndef DEBUGROB_SERVO_H
#define DEBUGROB_SERVO_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
typedef enum servo__ 
{
  SERVO_ARM_LEFT,
  SERVO_ARM_RIGHT,
  SERVO_HEAD
} Servo_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void ServoInit(void);
void SetServoPWM(Servo_t servoType, uint32_t pwmPulse);
void ResetServoPWM(Servo_t servoType);

#endif /* DEBUGROB_SERVO_H */
