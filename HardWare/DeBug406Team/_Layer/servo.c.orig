/** See a brief introduction (right-hand button) */
#include "servo.h"
/* Private include -----------------------------------------------------------*/
#include "stm32f4xx.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const static uint32_t _timerClock = 84000000;
const static uint32_t _pwmPulseDefault = 600;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Servo initialization
  * @param  None
  * @retval None
  */
void ServoInit(void)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	
  /* Enable TIM4 and GPIOD clocks ****************************************/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 
	
  /* GPIO configuration **************************************************/
  /* GPIO configured as follows:
        - Pin -> PD12 & PD13 & PD14 
        - Alternate function (AF) Mode -> TIM4 CH1 & CH2 & CH3
        - GPIO speed = 100MHz
        - Push pull output mode
        - Pull-up
  */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;      
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;      
	GPIO_Init(GPIOD, &GPIO_InitStructure);             

  /* Time Base configuration *********************************************/
  /* Timer4 Base configured as follows:
        - Prescaler (Psc) = 84 -> 1000 KHz
        - Count up mode
        - Auto-Reload Register (ARR) value = 20000 -> 20ms
        - not divide system clock
     ## - RepetitionCounter 
  */
	TIM_TimeBaseStructure.TIM_Prescaler = (_timerClock / 1000000) - 1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 20000 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  
  /* Channel 1,2 and 3 Configuration in PWM mode *************************/
  /* TIM Output Compare configured as follows:
        - Use PWM mode 1 : PWM mode 2 is contrary to mode 1
        - Output polarity High level
          ## PWM mode 1 is When CNT < CCRx, output the polarity e-level
             our e-level is High level. 
        
                       CNT ^
                           |
                       ARR |-----/-----/-----/-----/---
                           |    /!    /!    /!    /
                      CCRx |---/-!---/-!---/-!---/-----
                           |  /! !  /! !  /! !  /!
                           | / ! ! / ! ! / ! ! / !
                           |/  ! !/  ! !/  ! !/  !
                           |----------------------------> t
                           |   ! !   ! !   ! !   !
                         1 |___! !___! !___! !___! 
                           |   | |   | |   | |   |
                           |   | |   | |   | |   |
                         0 |   |_|   |_|   |_|   |_
                  IO Logic |---------------------------->

        - Capture Compare Register (CCR) = 0
        - Output Compare state enable
     ## - Complementary Output Compare state
     ## - Complementary output polarity
     ## - Output Compare pin state
     ## - Complementary Output Compare pin state
  */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
  TIM_OCInitStructure.TIM_Pulse = 0;
  
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	
  /* Use Preload Registor ************************************************/
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
  TIM_ARRPreloadConfig(TIM4, ENABLE);
	
	TIM_Cmd(TIM4, ENABLE);
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void SetServoPWM(Servo_t servoType, uint32_t pwmPulse)
{
  if(pwmPulse <= 2400 && pwmPulse >= 500){
    switch(servoType){
      case SERVO_ARM_LEFT:  TIM_SetCompare3(TIM4, pwmPulse); break;
      case SERVO_ARM_RIGHT: TIM_SetCompare2(TIM4, pwmPulse); break;
      case SERVO_HEAD:      TIM_SetCompare1(TIM4, pwmPulse); break;
      default:;
    }
  }
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void ResetServoPWM(Servo_t servoType)
{
  switch(servoType){
    case SERVO_ARM_LEFT:  TIM_SetCompare3(TIM4, _pwmPulseDefault); break;
    case SERVO_ARM_RIGHT: TIM_SetCompare2(TIM4, _pwmPulseDefault); break;
    case SERVO_HEAD:      TIM_SetCompare1(TIM4, _pwmPulseDefault); break;
    default:;
  }
}
