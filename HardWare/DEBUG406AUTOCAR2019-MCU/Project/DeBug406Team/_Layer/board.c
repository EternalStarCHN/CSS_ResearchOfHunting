/** See a brief introduction (right-hand button) */
#include "board.h"
/* Private include -----------------------------------------------------------*/
#include "stm32f4xx.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct timeOfIndicatorLight__
{
    uint32_t _timeBase;
    uint32_t _timeSet;
} IndicatorTimeBase_t;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static IndicatorTimeBase_t _timeBaseStructure = { 0, 0 };
static uint32_t _timeTick = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Init mini board's LED, You can use SetIndicatorTimeBase()
            To set led lighting frequency.
  * @param  None
  * @retval None
  */
void STMMiniBoard_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

    /* GPIO configuration **************************************************/
    /* GPIO configured as follows:
          - Pin -> PG15 - a led on mini board.
          - Output Mode
          - GPIO speed = 50MHz
          - Push pull output mode
          - Pull-Down
    */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    SetIndicatorTimeBase(5000, 4000);
}

/**
  * @brief  Reference Only -> (F12) SysTick_Handler();
  * @param  None
  * @retval None
  */
void _updateSysTick_Interrupt()
{
    _timeTick++;

    if(_timeTick >= _timeBaseStructure._timeBase)
    {
        _timeTick = 0;
    }
    else if(_timeTick >= _timeBaseStructure._timeSet)
    {
        GPIO_SetBits(GPIOG, GPIO_Pin_15);
    }
    else
    {
        GPIO_ResetBits(GPIOG, GPIO_Pin_15);
    }
}

/**
  * @brief  LED will light up timeSet(s) in timeBase(s)
  * @param  timeBase: Period.
            timeSet:  High level time.
  * @retval None
  */
void SetIndicatorTimeBase(uint32_t timeBase, uint32_t timeSet)
{
    _timeBaseStructure._timeBase = timeBase;
    _timeBaseStructure._timeSet = timeSet;
}
