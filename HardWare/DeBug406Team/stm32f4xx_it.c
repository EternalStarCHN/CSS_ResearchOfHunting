/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{

    //如需测量该中断函数CPU占用 解除下一行代码注释
//#define CPU_USAGE_TEST_Systick
#ifdef CPU_USAGE_TEST_Systick
    GPIO_SetBits(GPIOD,GPIO_Pin_14);
#endif

    //编码器反馈中断服务
    _UpdateEncoderFeedback();

    //抗干扰激光创安其反馈服务
    _Laser_AntiJitterFeedback();

    //板载LED指示灯中断服务
    _updateSysTick_Interrupt();

    //systickdelay()函数的中断服务函数
    _DelayOneMs_Interrupt();

    //华乾写的电机向前测试中断服务函数 目前不用
//  _forwardTime_Interrupt();

    //主函数内10ms轮询计数函数
    _StateMachineTick_Interrupt();

    //巡线一定时间，单位ms
    _GoLineDelay_Interrupt();

#ifdef CPU_USAGE_TEST_Systick
    GPIO_ResetBits(GPIOD,GPIO_Pin_14);
#endif


}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles TIM2 interrupt request.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{

}

/**
  * @brief  This function handles TIM5 interrupt request.
  * @param  None
  * @retval None
  */
void TIM5_IRQHandler(void)
{

}



/**
  * @brief  This function handles EXTI15-10 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
    _LaserEdgeTrigger_Interrupt();
}

/**
  * @brief  This function handles DMA1Stream5 interrupt request.
  * @param  None
  * @retval None
  */
void DMA1_Stream5_IRQHandler(void)
{
    Gyro_Usart_Rx_Interrupt();
}


/**
  * @brief  This function handles DMA2Stream5 interrupt request.
  * @param  None
  * @retval None
  */
void DMA2_Stream5_IRQHandler(void)
{
    Command_Usart_Rx_Interupt();
    DMA_ClearFlag(DMA2_Stream5,DMA_FLAG_TCIF5);
}

/**
  * @brief  This function handles DMA1Stream1 interrupt request.
  * @param  None
  * @retval None
  */
void DMA1_Stream1_IRQHandler(void)
{
    Command_Usart_Rx_Interupt();
    DMA_ClearFlag(DMA1_Stream1,DMA_FLAG_TCIF1);
}


/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
