/** See a brief introduction (right-hand button) */
#include "gyro.h"
/* Private include -----------------------------------------------------------*/
#include "stm32f4xx.h"
#include "systick.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_dma.h"
#include "UpHillOrDownHill.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static volatile uint8_t _Usart2_RxBuffer[11] = {0x00};

/* Private functions ---------------------------------------------------------	*/
void Gyro_Init(void)
{
    GPIO_InitTypeDef GPIO_USART2_Init;
    USART_InitTypeDef USART_USART2_Init;
    NVIC_InitTypeDef USART2_DMA_NVIC_Init;
    DMA_InitTypeDef USART2_Receive_DMA_Init;

    const uint8_t AutoCalibration[] = {0xff,0xaa,0x63,0x00,0x00};
    const uint8_t SaveConfig[] = {0xff,0xaa,0x00,0x00,0x00};

    uint8_t counter = 0;

    //时钟开启
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);

    //PD5 TX口配置
    GPIO_USART2_Init.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_USART2_Init.GPIO_OType = GPIO_OType_PP;
    GPIO_USART2_Init.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_USART2_Init.GPIO_Pin   = GPIO_Pin_5;
    GPIO_USART2_Init.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOD,&GPIO_USART2_Init);

    //PD6 RX口配置
    GPIO_USART2_Init.GPIO_Pin   = GPIO_Pin_6;
    GPIO_USART2_Init.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_USART2_Init.GPIO_OType = GPIO_OType_OD;
    GPIO_USART2_Init.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_USART2_Init.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOD,&GPIO_USART2_Init);

    //引脚复用处理
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2);

    //USART2初始化
    USART_USART2_Init.USART_BaudRate            = 115200;
    USART_USART2_Init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_USART2_Init.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
    USART_USART2_Init.USART_Parity              = USART_Parity_No;
    USART_USART2_Init.USART_StopBits            = USART_StopBits_1;
    USART_USART2_Init.USART_WordLength          = USART_WordLength_8b;

    USART_Init(USART2,&USART_USART2_Init);
    USART_Cmd(USART2,ENABLE);

    //DMA配置
    DMA_DeInit(DMA1_Stream5);

    USART2_Receive_DMA_Init.DMA_Channel            = DMA_Channel_4;
    USART2_Receive_DMA_Init.DMA_PeripheralBaseAddr = (uint32_t)(&(USART2->DR));
    USART2_Receive_DMA_Init.DMA_Memory0BaseAddr    = (uint32_t)_Usart2_RxBuffer;
    USART2_Receive_DMA_Init.DMA_DIR                = DMA_DIR_PeripheralToMemory;
    USART2_Receive_DMA_Init.DMA_BufferSize         = 11;
    USART2_Receive_DMA_Init.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    USART2_Receive_DMA_Init.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    USART2_Receive_DMA_Init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    USART2_Receive_DMA_Init.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    USART2_Receive_DMA_Init.DMA_Mode               = DMA_Mode_Circular;
    USART2_Receive_DMA_Init.DMA_Priority           = DMA_Priority_Medium;
    USART2_Receive_DMA_Init.DMA_FIFOMode           = DMA_FIFOMode_Disable;
    USART2_Receive_DMA_Init.DMA_FIFOThreshold      = DMA_FIFOThreshold_HalfFull;
    USART2_Receive_DMA_Init.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
    USART2_Receive_DMA_Init.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;

    //初始化DMA
    DMA_Init(DMA1_Stream5,&USART2_Receive_DMA_Init);

    //开启USART2 DMA接收
    USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);

    //使能DMA中断
    DMA_ITConfig(DMA1_Stream5,DMA_IT_TC,ENABLE);

    //配置NVIC优先级
    USART2_DMA_NVIC_Init.NVIC_IRQChannel                   = DMA1_Stream5_IRQn;
    USART2_DMA_NVIC_Init.NVIC_IRQChannelPreemptionPriority = 3;
    USART2_DMA_NVIC_Init.NVIC_IRQChannelSubPriority        = 2;
    USART2_DMA_NVIC_Init.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&USART2_DMA_NVIC_Init);

    //使能DMA
    DMA_Cmd(DMA1_Stream5,ENABLE);

    //陀螺仪自动校准
    for(counter = 0; counter < sizeof(AutoCalibration); counter ++)
    {
        USART_SendData(USART2,(uint16_t)AutoCalibration[counter]);
    }

    SysTickDelay(150);

    //保存参数
    for(counter = 0; counter < sizeof(SaveConfig); counter ++)
    {
        USART_SendData(USART2,(uint16_t)SaveConfig[counter]);
    }

}


/*
	函数名称：Gyro_Usart_Rx_Interrupt()
	函数作用：陀螺仪串口DMA接收中断
	函数输入：无
	函数输出：无
	函数参考：DMA1_Stream5_IRQHandler() (F12)
*/
void Gyro_Usart_Rx_Interrupt(void)
{
    uint8_t sum = 0;

    //DMA_Cmd(DMA1_Stream5,DISABLE);

    //计算校验和
    sum += _Usart2_RxBuffer[0];
    sum += _Usart2_RxBuffer[1];
    sum += _Usart2_RxBuffer[2];
    sum += _Usart2_RxBuffer[3];
    sum += _Usart2_RxBuffer[4];
    sum += _Usart2_RxBuffer[5];
    sum += _Usart2_RxBuffer[6];
    sum += _Usart2_RxBuffer[7];
    sum += _Usart2_RxBuffer[8];
    sum += _Usart2_RxBuffer[9];


    //校验和判断
    if(sum != _Usart2_RxBuffer[10])
    {
        //校验和不一致执行此处代码
        //重启系统
        NVIC_SystemReset();
    }
    _UpHillOrDownHillCheck();
    DMA_ClearFlag(DMA1_Stream5,DMA_FLAG_TCIF5);
    //DMA_Cmd(DMA1_Stream5,ENABLE);
}



/*
	函数名称：Gyro_GetPitchAngle()
	函数作用：获取陀螺仪的俯仰角
	函数输入：无
	函数输出：float类型俯仰角数据
	函数说明：陀螺仪文档貌似有点问题 Pitch角实际测下来是Roll角
	函数说明：这里已经对该情况进行了修正
	函数说明：PS 实际上是我们的陀螺仪安装问题 Pitch和Roll反过来了

	数据格式：车在校准平面上的角度为0 从车屁股看向车头方向 俯冲为从0增加到360
																		                     爬升为从360减少到0
*/
float Gyro_GetPitchAngle(void)
{
    uint32_t temp = 0;
    float output = 0;

    temp = _Usart2_RxBuffer[3];
    temp <<= 8;
    temp |= _Usart2_RxBuffer[2];

    output = (float)temp / (float)32768 * (float)180;

    return output;
}


/*
	函数名称：Gyro_GetRollAngle()
	函数作用：获取陀螺仪的滚转角
	函数输入：无
	函数输出：float类型滚转角数据
	函数说明：陀螺仪文档貌似有点问题 Pitch角实际测下来是Roll角
	函数说明：这里已经对该情况进行了修正
  函数说明：PS 实际上是我们的陀螺仪安装问题 Pitch和Roll反过来了

	数据格式：车在校准平面上的角度为0 从车屁股看向车头方向 左倾为从0增加到360
																												 右倾为从360减少到0
*/
float Gyro_GetRollAngle(void)
{
    uint32_t temp = 0;
    float output = 0;

    temp = _Usart2_RxBuffer[5];
    temp <<= 8;
    temp |= _Usart2_RxBuffer[4];

    output = (float)temp / (float)32768 * (float)180;

    return output;
}



/*
	函数名称：Gyro_GetYawAngle()
	函数作用：获取陀螺仪的偏航角
	函数输入：无
	函数输出：float类型偏航角数据
	函数说明：陀螺仪采用了六轴算法而不是九轴算法
	函数说明：意味着陀螺仪没法用地磁校准偏航轴
	函数说明：意思就是Yaw数据每次上电就是0 然后左加右减
	数据格式：车在校准平面上的角度为0 从车屁股看向车头方向 左转为从0增加到360
																												 右转为从360减少到0
*/
float Gyro_GetYawAngle(void)
{
    uint32_t temp = 0;
    float output = 0;

    temp = _Usart2_RxBuffer[7];
    temp <<= 8;
    temp |= _Usart2_RxBuffer[6];

    output = (float)temp / (float)32768 * (float)180;

    return output;
}


