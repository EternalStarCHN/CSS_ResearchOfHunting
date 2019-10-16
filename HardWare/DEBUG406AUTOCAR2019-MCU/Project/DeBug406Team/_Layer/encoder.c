/** See a brief introduction (right-hand button) */
#include "encoder.h"
/* Private include -----------------------------------------------------------*/
#include "stm32f4xx.h"
#include "motor.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
int16_t _LeftEncoderFeedback  = 0;
int16_t _RightEncoderFeedback = 0;

//路程统计相关变量
uint8_t _Encoder_CountControl = 0;
float _Encoder_DistantCount = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initialization of Timer-Input Capture
  * @param  None
  * @retval None
  */
void Encoder_Init(void)
{
    GPIO_InitTypeDef GPIO_EncoderInit;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    //GPIO时钟开启
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);

    //定时器时钟开启
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);

    //系统配置器时钟开启
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);

    //引脚复用配置
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5);

    GPIO_PinAFConfig(GPIOA,GPIO_PinSource15,GPIO_AF_TIM2);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_TIM2);

    //GPIO配置
    //GPIOA相关
    GPIO_EncoderInit.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_EncoderInit.GPIO_Pin   = GPIO_Pin_0;
    GPIO_EncoderInit.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_EncoderInit.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA,&GPIO_EncoderInit);

    GPIO_EncoderInit.GPIO_Pin   = GPIO_Pin_1;
    GPIO_Init(GPIOA,&GPIO_EncoderInit);

    GPIO_EncoderInit.GPIO_Pin   = GPIO_Pin_15;
    GPIO_Init(GPIOA,&GPIO_EncoderInit);


    //GPIOB相关
    GPIO_EncoderInit.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOB,&GPIO_EncoderInit);

    //TIM设置
    TIM_DeInit(TIM2);
    TIM_DeInit(TIM5);

    //即使是外部输入模式 也要设置重载计时器 否则 CNT的计数将被限制

    TIM_TimeBaseStructure.TIM_Period = 65535; //设置下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler = 0; //设置用来作为TIMx时钟频率除数的预分频值  不分频
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
    TIM_EncoderInterfaceConfig(TIM5,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);

    TIM2->CNT = 0;
    TIM5->CNT = 0;

    TIM_Cmd(TIM2,ENABLE);
    TIM_Cmd(TIM5,ENABLE);

}

//轮子直径72mm 预计上场由于受压的关系会变小一些 这里采用70mm


/*
** 函数名称：Encoder_GetLWheelSpeed()
** 函数作用：获取左边轮子在最近一个100ms内统计到的速度
** 函数输入：无
** 函数输出：轮子转速 以m/s为单位 例如0.5m/s

** 注意事项：轮子的速度是100ms更新一次的
** 注意事项：因此100ms内的重复调用会得到相同的结果 没有意义
*/
float Encoder_GetLWheelSpeed(void)
{
    float Speed = 0;

    Speed = (float)_LeftEncoderFeedback / (float)8.0;
    Speed *= 10;
    Speed /= 80;

    Speed *= (float)0.2199; //圈转速 * 每圈转的距离 = 米每秒

#ifdef Robot_1
    return Speed;
#endif

#ifdef Robot_2
    return Speed;
#endif

#ifdef Robot_3

#endif


}


/*
** 函数名称：Encoder_GetRWheelSpeed()
** 函数作用：获取右边轮子在最近一个100ms内统计到的速度
** 函数输入：无
** 函数输出：轮子转速 以m/s为单位 例如0.5m/s

** 注意事项：轮子的速度是100ms更新一次的
** 注意事项：因此100ms内的重复调用会得到相同的结果 没有意义
*/
float Encoder_GetRWheelSpeed(void)
{
    float Speed = 0;

    Speed = (float)_RightEncoderFeedback / (float)8.0;
    Speed *= 10;
    Speed /= 80;

    Speed *= (float)0.2199; //圈转速 * 每圈转的距离 = 米每秒

#ifdef Robot_1
    return - Speed;
#endif

#ifdef Robot_2
    return Speed;
#endif

#ifdef Robot_3

#endif

}


/*
** 函数名称：Encoder_ResetDistant()
** 函数作用：重置积分来的距离
** 函数输入：无
** 函数输出：无

** 函数说明：该函数不会停止路程积分 该函数仅仅是清空路程积分的结果
** 函数说明：想停止路程积分应该调用下面那个Stop函数
*/
void Encoder_ResetDistant(void)
{
    _Encoder_DistantCount = 0;
}



/*
** 函数名称：Encoder_StartDistantCount()
** 函数作用：从下一个最近的100ms开始进行路程积分
** 函数输入：无
** 函数输出：无

** 函数说明：统计的距离是两轮的平均路程
*/
void Encoder_StartDistantCount(void)
{
    _Encoder_CountControl = 1;
}

/*
** 函数名称：Encoder_StopDistantCount()
** 函数作用：立即停止路程积分 但不会清空已经行走的路程
** 函数输入：无
** 函数输出：无

** 函数说明：统计的距离是两轮的平均路程
*/
void Encoder_StopDistantCount(void)
{
    _Encoder_CountControl = 0;
}


/*
** 函数名称：Encoder_GetCurrentDistantCount()
** 函数作用：获取当前已经行走过的路程 100ms刷新一次
** 函数输入：无
** 函数输出：float类型数据 单位是米

** 函数说明：统计的路程是两轮的平均路程 而不是左边或者右边的
** 函数说明：因此寻线的摆动会影响路程计算 使得最终位移小于路程
*/
float Encoder_GetCurrentDistantCount(void)
{
    return _Encoder_DistantCount;
}
void BeginCount(void)
{
    Encoder_ResetDistant();
    Encoder_StartDistantCount();
}
void StopCount(void)
{
    Encoder_StopDistantCount();
}
