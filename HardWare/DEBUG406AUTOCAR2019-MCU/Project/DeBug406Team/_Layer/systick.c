/** See a brief introduction (right-hand button) */
#include "systick.h"
/* Private include -----------------------------------------------------------*/
#include "stm32f4xx.h"
#include "magic.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static volatile uint32_t _timingDelay;
static volatile uint32_t _GoLineTime;
static volatile uint32_t _CTime=10;
/* Public variables ---------------------------------------------------------*/
uint8_t _UpdateTick = 0;


/* Private function prototypes -----------------------------------------------*/
/*static*/ inline void _DelayOneMs_Interrupt(void);
void _StateMachineTick_Interrupt(void);
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


void _StateMachineTick_Interrupt(void)
{
    static uint16_t counter = 0;

    counter ++;

    if(counter > (State_MachineTickTime - 1))
    {
        counter = 0;
        _UpdateTick = 1;
    }
}
void _GoLineDelay_Interrupt(void)
{
    if(_GoLineTime!=0)
    {
        _GoLineTime--;
    }
}
void _GoLineDelay(uint16_t MsTime)
{
    _GoLineTime=MsTime;
    while(_GoLineTime!= 0)
    {
        _GoLineMSpeed();
        //_FindPointGo();
    };
    if(_GoLineTime==0)
    {
        //UpdateMotorState(MOTOR_STOP);
    }
}

/*
** 函数名称：_UpdateEncoderFeedback()
** 函数作用：轮速 行驶路程反馈函数
** 函数输入：无
** 函数输出：无

** 注意事项：此函数不应该由你调用 而应该由systick的中断服务函数调用
** 注意事项：该函数实现了100ms内平均速度的计算 以及距离的计算

** 平均速度计算：假设电机向前转 则CNT每一个脉冲 +4 倒退则是 -4 该变量是int类型的
** 然后我们的电机每圈反馈4个脉冲(推算值 不一定准 但是结果是对的) 因此CNT / 4反馈的就是真实
** 计数脉冲 然后让计数脉冲 / 4 / 80(电机减速比)代表100ms内轮子转动的角度 最后*10 得到R/s

** 距离的积分计算：现在我们已经能获取电机速度 则左边+右边速度/2可得平均速度
** 距离 = 速度 * 时间 轮子R/s知道  R/s * 0.1 =轮子在100ms内转了多少圈 然后再 * 每圈轮子长度
** 得到上一个100ms轮子跑了多远 最后用+=做累加运算就能吧距离积分出来
*/
void _UpdateEncoderFeedback(void)
{
    static uint8_t counter = 0;

    if(counter < 99)
    {
        counter ++;
    }
    else
    {
        _LeftEncoderFeedback  = TIM2->CNT;
        _RightEncoderFeedback = TIM5->CNT;

        //距离统计
        if(_Encoder_CountControl == 1)
        {
            _Encoder_DistantCount +=  ((Encoder_GetLWheelSpeed()+Encoder_GetRWheelSpeed())/(float)2.0)*(float)0.1;
        }

        TIM2->CNT = 0;
        TIM5->CNT = 0;
        counter = 0;
    }


}

/*
** 函数名称：_Laser_AntiJitterFeedback()
** 函数作用：左右两边激光传感器反馈函数 用来消除抖动
** 函数输入：无
** 函数输出：无

** 注意事项：由于采用了干扰抑制 所以该函数会比不带干扰抑制的状态读取函数慢一些 最多慢5ms
** 注意事项：你不应该调用该函数 该函数应再systick中断服务函数重自己执行

** 算法实现：分别对 左 右激光传感器引脚读10次 如果读超过5次低电平认为在白线上 否则在绿毯上
** 检测间隔由systick的中断服务函数决定 1ms检测一次
*/
void _Laser_AntiJitterFeedback(void)
{
    //采用int类型确保即使出问题也有恢复的可能
    static int8_t LeftCountTimes = 0;
	static int8_t RightCountTimes = 0;

    //如果读到左边低电平(在线上)
    if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_13) == 0)
    {
        //小于10就增加一次在线上的计数 否则不增加该值
        if(LeftCountTimes < 10)
        {
            LeftCountTimes ++;
        }
    }
    else //不是低就是高 左边读到高电平(在毯子上)
    {
        if(LeftCountTimes > 0)
        {
            //大于0就增加一次不在线上(或者降低在线上)的计数 不大于0就不减少
            LeftCountTimes --;
        }
    }


    //如果读到右边低电平(在线上)
    if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_11) == 0)
    {
        //小于10就增加一次在线上的计数 否则不增加该值
        if(RightCountTimes < 10)
        {
            RightCountTimes ++;
        }
    }
    else //不是低就是高 右边读到高电平(在毯子上)
    {
        if(RightCountTimes > 0)
        {
            //大于0就增加一次不在线上(或者降低在线上)的计数 不大于0就不减少
            RightCountTimes --;
        }
    }

    /* 完成采样 开始分析究竟反馈在线上还是不在线上 */

    //左边
    //统计到 > 4次在线上(也就是5次) 认为的却在线上
    if(LeftCountTimes > 4)
    {
        //在线上LED亮 LOW
        _LLaserAntiJitterState = LOW;
    }
    else
    {
        //不在线上 HIGH
        _LLaserAntiJitterState = HIGH;
    }


    //右边
    //统计到 > 4次在线上(也就是5次) 认为的却在线上
    if(RightCountTimes > 4)
    {
        //在线上LED亮 LOW
        _RLaserAntiJitterState = LOW;
    }
    else
    {
        //不在线上LED灭 HIGH
        _RLaserAntiJitterState = HIGH;
    }



}

//void _FindPointL()
//{
//	if(_CTime==0)
//	{
//		_CTime=10;
//		ClearLLaserChangePendingBit();
//		if(GetLeftLaserState()==Changed)
//		{
//			UpdateMotorState(MOTOR_STOP);
//		}
//	}
//}
//void _FindPointR()
//{
//	if(_CTime==0)
//	{
//		_CTime=10;
//		ClearRLaserChangePendingBit();
//		if(GetRightLaserState()==Changed)
//		{
//			UpdateMotorState(MOTOR_STOP);
//		}
//	}
//}
//void _FindPoint_Interrupt()
//{
//	while(_CTime!=0)
//	{
//		_CTime--;
//	}
//
//}
