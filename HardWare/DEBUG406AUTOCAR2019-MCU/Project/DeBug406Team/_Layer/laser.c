/** See a brief introduction (right-hand button) */
#include "laser.h"
/* Private include -----------------------------------------------------------*/
#include "stm32f4xx.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static LaserState_t _laserState = OPENNING;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Init laser's pin & interrupt.
  * @param  None
  * @retval None
  */
void Laser_Init(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Enable External port clocks *****************************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
  /* GPIO configuration *************************************************/
  /* GPIO configured as follows:
        - Pin -> PA0  
        - Input Mode
        - GPIO speed = 100MHz
        - Push pull output mode
        - Pull-up
  */  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* EXTI configuration *************************************************/
  /* EXTI configured as follows:
        - Source0 -> Line0
        - Interrupt Mode
        - Rising & Falling Interrupt
        - Enable
  */  
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
  /* NVIC configuration ***************************************************/
  /* NVIC configured as follows:
        - Interrupt function name = USART1_IRQHandler
        - pre-emption priority = 0 (Very low)
     ** - subpriority level = 2 (Very low)
        - NVIC_IRQChannel enable
  */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  EXTI_IRQn's interrupt function (F12) EXTI0_IRQHandler().
  * @param  None
  * @retval None
  */
void _LaserEdgeTrigger_Interrupt(void)
{
  _laserState = (
    (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == SET) ?
    OPENNING : COVERED
  );    
  
  EXTI_ClearITPendingBit(EXTI_Line0);
}

/**
  * @brief  Get Laser's state.
  * @param  None
  * @retval None
  */
LaserState_t GetLaserState(void)
{
  return _laserState;
}
