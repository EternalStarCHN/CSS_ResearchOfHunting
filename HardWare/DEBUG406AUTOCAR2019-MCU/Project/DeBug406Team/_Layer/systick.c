/** See a brief introduction (right-hand button) */
#include "systick.h"
/* Private include -----------------------------------------------------------*/
#include "stm32f4xx.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static volatile uint32_t _timingDelay;

/* Private function prototypes -----------------------------------------------*/
/*static*/ inline void _DelayOneMs_Interrupt(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Choice source of the SysTick clock.
  * @param  None
  * @retval None
  */
void SysTick_Init(void)
{
  /* SystemCoreClock = 1s -> 1s / 1000 = 1ms */
  SysTick_Config(SystemCoreClock / 1000);
  NVIC_SetPriority(SysTick_IRQn, 0x0);
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void SysTickDelay(uint32_t nTime)
{
  _timingDelay = nTime;

  while(_timingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable. (F12) SysTick_Handler();
  * @param  None
  * @retval None
  */
void _DelayOneMs_Interrupt(void)
{
  if (_timingDelay != 0)
  { 
    _timingDelay--;
  }
}
