#include "cpu_test.h"


void CPU_UsageTestInit(void)
{
	GPIO_InitTypeDef GPIO_CPUTestInit;
	
	GPIO_CPUTestInit.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_CPUTestInit.GPIO_OType = GPIO_OType_PP;
	GPIO_CPUTestInit.GPIO_Pin   = GPIO_Pin_14;
	GPIO_CPUTestInit.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_CPUTestInit.GPIO_Speed = GPIO_Speed_100MHz;
	
	GPIO_Init(GPIOD,&GPIO_CPUTestInit);
	
	GPIO_ResetBits(GPIOD,GPIO_Pin_14);
}
