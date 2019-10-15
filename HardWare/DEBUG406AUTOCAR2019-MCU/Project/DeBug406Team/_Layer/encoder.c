/** See a brief introduction (right-hand button) */
#include "encoder.h"
/* Private include -----------------------------------------------------------*/
#include "stm32f4xx.h"
#include "motor.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const static uint32_t _timerClock = 84000000;

/* Right Front encoder */
static uint32_t _speedValueRF = 0;
static uint8_t  _updateTimesRF = 0;
static uint32_t _triggerCountRF = 0;

/* Left Rear encoder */
static uint32_t _speedValueLR = 0;
static uint8_t  _updateTimesLR = 0;
static uint32_t _triggerCountLR = 0;

/* Private function prototypes -----------------------------------------------*/
/*static*/ inline void _Timer5Capture_Interrupt(void);
/*static*/ inline void _Timer2Capture_Interrupt(void);
static void _CaptureFrequency(TIM_TypeDef* TIMx, uint32_t captureNum, uint8_t timerOverFlow);

/* Private functions ---------------------------------------------------------*/

/** 
  * @brief  Initialization of Timer-Input Capture
  * @param  None
  * @retval None
  */
void Encoder_Init(void)
{  
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_ICInitTypeDef TIM_ICInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Enable TIM5, TIM2 and GPIOA clocks **********************************/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  /* GPIO configuration **************************************************/
  /* GPIO configured as follows:
        - Pin -> PA0 & PA15 
        - Alternate function (AF) Mode -> TIM5 CH1 & TIM2 CH1
        - GPIO speed = 100MHz
        - Push pull output mode
        - Pull-Down
  */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_TIM2);
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);                                

  /* Time Base configuration *********************************************/
  /* Timer4 Base configured as follows:
        - Prescaler (Psc) = _timerClock / 500000 -> 500 KHz
        - Count up mode
        - Auto-Reload Register (ARR) value = 50000 -> 50KHz -> 0.1s
        - not divide system clock
     ## - Repetition Counter
  */
	TIM_TimeBaseStructure.TIM_Prescaler = (_timerClock / 500000) - 1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 50000 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  
  /* Channel 1 Configuration in Input Capture mode *************************/
  /* TIM5 Input Capture configured as follows:
        - TIM5 channel1
        - Capture rising signal
        
                   CNT ^      N times overflow
                   ARR |___________________________
                       |         /     /     /        
                       |        /!    /!    /!    / 
                 CCRx1 |---/   / !   / !   / !   / 
                       |  /!  /  !  /  !  /  !  /  
                 CCRx2 |-/-!-/---!-/---!-/---!-/ 
                       |/  !/    !/    !/    !/!   
                 Timer |----------------------------> t
                       |   t1                  t2       [Input signal's frequency]
                     1 |   !_______            !____     = (N * ARR + CCRx2) * T
                       |   |       |           |
                       |   |       |           |          
                     0 |___|       |___________|
          Input signal |---------------------------->

     ** - Direct Timer Compare
        - No Input Capture Prescaler
        - Filter set : Fre^sampling = Fre^input, Sampling High e-level times = 4
  */
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x02;
  
	TIM_ICInit(TIM5, &TIM_ICInitStructure);
  TIM_ICInit(TIM2, &TIM_ICInitStructure);  
  
  /* NVIC configuration ***************************************************/
  /* NVIC configured as follows:
        - Interrupt function name = TIM5_IRQHandler & TIM2_IRQHandler
        - pre-emption priority = 2 (low)
     ** - subpriority level = 0 (Very high)
        - NVIC_IRQChannel enable
  */  
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		   
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			     
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
	NVIC_Init(&NVIC_InitStructure);

  TIM_Cmd(TIM5, ENABLE);
	TIM_Cmd(TIM2, ENABLE); 
}  

/**----------------------------------------------------------** 
  * Right Front encoder ***************************************
  * @{
  */

/**
  * @brief  
  * @param  None
  * @retval None
  */
uint32_t GetRFSpeed(uint8_t nTimesAverage)
{
  uint32_t totalSpeed = 0;
  _updateTimesRF = 0;
  
  TIM_ClearITPendingBit(TIM5, TIM_IT_CC1 | TIM_IT_Update);
  TIM_ITConfig(TIM5, TIM_IT_Update | TIM_IT_CC1, ENABLE);
  
  for(int times = 1; times <= nTimesAverage; times++)
  {
    while(_updateTimesRF <= times) {}
    totalSpeed += _speedValueRF;
  }
  
  TIM_ITConfig(TIM5, TIM_IT_Update | TIM_IT_CC1, DISABLE);
  return ( totalSpeed / nTimesAverage );
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
uint32_t GetRFTriggerTimes(void)
{
  return _triggerCountRF;
}

/**
  * @brief  TIM5_IRQn's interrupt function (F12) TIM5_IRQHandler().
  * @param  None
  * @retval None
  */
void _Timer5Capture_Interrupt(void)
{
  static uint8_t _timerOverFlow = 0;
  
  /* Timer5 Update (overflow) interrupt ***********************************/
  if(TIM_GetITStatus(TIM5, TIM_IT_Update) == SET) {
    if(_timerOverFlow++ < 10) {}
    else {
      _CaptureFrequency(TIM5, TIM_GetCapture1(TIM5), _timerOverFlow);
      _timerOverFlow = 0;
    }
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
  }
  
  /* Timer5 Capture/Compare interrupt *************************************/  
  if(TIM_GetITStatus(TIM5, TIM_IT_CC1) == SET) {
    _CaptureFrequency(TIM5, TIM_GetCapture1(TIM5), _timerOverFlow);
    _timerOverFlow = 0;
    _triggerCountRF ++;
    TIM_SetCounter(TIM5, 0);
    TIM_ClearITPendingBit(TIM5, TIM_IT_CC1);
  }
}

/** 
  * @}
  *----------------------------------------------------------*/

/**----------------------------------------------------------**
  * Left Rear encoder *****************************************
  * @{
  */

/**
  * @brief  
  * @param  None
  * @retval None
  */
uint32_t GetLRSpeed(uint8_t nTimesAverage)
{
  uint32_t totalSpeed = 0;
  _updateTimesLR = 0;
  
  TIM_ClearITPendingBit(TIM2, TIM_IT_CC1 | TIM_IT_Update);
  TIM_ITConfig(TIM2, TIM_IT_Update | TIM_IT_CC1, ENABLE);
  
  for(int times = 1; times <= nTimesAverage; times++)
  {
    while(_updateTimesLR <= times) {}
    totalSpeed += _speedValueLR;
  }
  
  TIM_ITConfig(TIM2, TIM_IT_Update | TIM_IT_CC1, DISABLE);
  return ( totalSpeed / nTimesAverage );
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
uint32_t GetLRTriggerTimes(void)
{
  return _triggerCountLR;
}

/**
  * @brief  TIM2_IRQn's interrupt function (F12) TIM2_IRQHandler().
  * @param  None
  * @retval None
  */
void _Timer2Capture_Interrupt(void)
{
  static uint8_t _timerOverFlow = 0;
  
  /* Timer5 Update (overflow) interrupt ***********************************/
  if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
    if(_timerOverFlow++ < 10) {}
    else {
      _CaptureFrequency(TIM2, TIM_GetCapture1(TIM2), _timerOverFlow);
      _timerOverFlow = 0;
    }
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  }
  
  /* Timer5 Capture/Compare interrupt *************************************/  
  if(TIM_GetITStatus(TIM2, TIM_IT_CC1) == SET) {
    _CaptureFrequency(TIM2, TIM_GetCapture1(TIM2), _timerOverFlow);
    _timerOverFlow = 0;
    _triggerCountLR ++;
    TIM_SetCounter(TIM2, 0);
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
  }
}

/**
  * @}
  *----------------------------------------------------------*/

/**
  * @brief  
  * @param  None
  * @retval None
  */
int WaittingTrigger(uint16_t pluseCount)
{
  _triggerCountRF  = _triggerCountLR = 0;
  
  TIM_ClearITPendingBit(TIM5, TIM_IT_Update | TIM_IT_CC1);
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update | TIM_IT_CC1);
  TIM_ITConfig(TIM5, TIM_IT_Update | TIM_IT_CC1, ENABLE);
  TIM_ITConfig(TIM2, TIM_IT_Update | TIM_IT_CC1, ENABLE);
  
  while(_triggerCountRF < pluseCount && _triggerCountLR < pluseCount) {}
  
  TIM_ITConfig(TIM5, TIM_IT_Update | TIM_IT_CC1, DISABLE);
  TIM_ITConfig(TIM2, TIM_IT_Update | TIM_IT_CC1, DISABLE);
  return 1;
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
int WaittingTriggerWithFunc(uint16_t pluseCount, void (* func)(void))
{
  _triggerCountRF  = _triggerCountLR = 0;
  
  TIM_ClearITPendingBit(TIM5, TIM_IT_Update | TIM_IT_CC1);
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update | TIM_IT_CC1);
  TIM_ITConfig(TIM5, TIM_IT_Update | TIM_IT_CC1, ENABLE);
  TIM_ITConfig(TIM2, TIM_IT_Update | TIM_IT_CC1, ENABLE);
  
  func();
  
  while(_triggerCountRF < pluseCount || _triggerCountLR < pluseCount) {}
  
  TIM_ITConfig(TIM5, TIM_IT_Update | TIM_IT_CC1, DISABLE);
  TIM_ITConfig(TIM2, TIM_IT_Update | TIM_IT_CC1, DISABLE);
  return 1;
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void _CaptureFrequency(TIM_TypeDef* TIMx, uint32_t captureNum, uint8_t timerOverFlow)
{
  if(TIMx == TIM5)
  {
    if(timerOverFlow >= 10) {
      _updateTimesRF = 0xff;
      _speedValueRF = 0;
      return;
    }
    _speedValueRF = timerOverFlow * 50000 + captureNum + 1;
    _updateTimesRF++;
  }
  else if(TIMx == TIM2)
  {
    if(timerOverFlow >= 10) {
      _updateTimesLR = 0xff;
      _speedValueLR = 0;
      return;
    }
    _speedValueLR = timerOverFlow * 50000 + captureNum + 1;
    _updateTimesLR++;
  }
}
