#include "point.h"

//如果要使用magic.h 把下面一行解注释
//#define Use_Magic


static int _times=5;
static int _LastTimeButtomValue=0;
uint32_t _DValue[7];
int PointFlag=0;
uint32_t _FindMax(uint32_t L[])
{
	uint32_t _max=0;
	for(int i=0;i<=6;i++)
	{
		if(L[i]>=_max)
		{
			_max=L[i];
		}
	}
	return _max;
}
#ifdef Use_Magic
#ifndef Robot_3
#define Magic
#endif
#endif
uint32_t _CalculationADC(void)
{
		const AdcData_t * adcData;
	adcData = UpdateButtom();
	uint32_t temp=0;
	for(int i=0; i<=7; i++)
	{
		temp+=adcData->array[0][i];
	}
	return temp;
}
uint32_t _UpdateButtomValue(void)
{
	if(_times==5)
	{
		_times=0;
		return _CalculationADC();
	}
	else
	{
		_times++;
		return _LastTimeButtomValue;
	}
	
}
void _FindPointStop(void)
{
	const AdcData_t * adcData;
	adcData = UpdateButtom();
	uint32_t MaxDvalue;
	uint32_t _Time=0;
	_LastTimeButtomValue=_UpdateButtomValue();
	for(int i=0; i<=6; i++)
	{
		_DValue[i]=abs(adcData->array[0][i]-adcData->array[0][i+1]);
		//printf("%d,",_DValue[i]);
	}
	//printf("\r\n");
	MaxDvalue=_FindMax(_DValue);
	//printf("%d\r\n",MaxDvalue);
	
	for(int i=0;i<=6;i++)
	{
		
		if(MaxDvalue/2<_DValue[i])
		{
			_Time++;
		}
	}
//	printf("%d",_Time);
			if(_Time>2)
		{
			PointFlag++;
			/* 到点停灵敏度调整 调高了灵敏度降低 低了灵敏度提高 */
			/* FindPointStopSensitivity */
			/* jNFDd3lSDTRLlU04 */
			#ifndef Magic
			if(PointFlag>=4)
			#else
			if(PointFlag>=FindPointStopSensitivity)
			#endif
			{
			//printf("1");
			FlagPoint=1;
			UpdateMotorState(MOTOR_STOP);
			ClearLLaserChangePendingBit();
			ClearRLaserChangePendingBit();
			PointFlag=0;
			}
			
		}
		else
		{
			ClearLLaserChangePendingBit();
			ClearRLaserChangePendingBit();
		}
}
void _FindPointGo()
{
	const AdcData_t * adcData;
	adcData = UpdateButtom();
	uint32_t MaxDvalue;
	uint32_t _Time=0;
	_LastTimeButtomValue=_UpdateButtomValue();
	for(int i=0; i<=6; i++)
	{
		_DValue[i]=abs(adcData->array[0][i]-adcData->array[0][i+1]);
		//printf("%d,",_DValue[i]);
	}
	//printf("\r\n");
	MaxDvalue=_FindMax(_DValue);
	//printf("%d\r\n",MaxDvalue);
	
	for(int i=0;i<=6;i++)
	{
		if(MaxDvalue/2<_DValue[i])
		{
			_Time++;
		}
	}
	if(_Time>2||adcData->array[0][0]<3300||adcData->array[0][7]<3300)
	{
		PointFlag++;
		/* 到点走灵敏度调整 调高将降低灵敏度 调低将提高灵敏度 */
		/* FindPointGoSensitivity */
		/* qR9Ifrq4I9Jmbv5b */
		#ifndef Magic
			if(PointFlag>=3)
		#else
			if(PointFlag>=FindPointGoSensitivity)
		#endif
			{
		PointFlag++;
		FlagPoint=2;
		SetMotorDutyRatio(0.06,0.06);
		//for(long long i=0;i <= 8400000000;i++){}
		SysTickDelay(50);
		ClearLLaserChangePendingBit();
		ClearRLaserChangePendingBit();
		PointFlag=0;
			}
	}
	else
	{
		ClearLLaserChangePendingBit();
		ClearRLaserChangePendingBit();
	}
	
}




void _ArrivePlatform(void)
{
	float Distant = 0;
	
	
	Distant = SuperSonic_GetDistant();
	
	//如果返回的是错误的距离
	if(Distant >= (float)1)
	{
		//等100ms然后立即返回函数
		SysTickDelay(100);
		return;
	}
	
				if(Distant<=(float)0.30)
					{
					if(Distant>=SuperSonic_MinDistant&&Distant<=SuperSonic_MaxDistant)
						{
							UpdateMotorState(MOTOR_STOP);
							SysTickDelay(300); //wlys 5.16注释 不懂啥用处 有用加回去
							
						}
					else if(Distant>=SuperSonic_MaxDistant)
						{
							UpdateMotorState(MOTOR_FRONT);
							SetMotorDutyRatio(0.1,0.1);
							SysTickDelay(SuperSonic_StepTime);
							SetMotorDutyRatio(0,0);
							SysTickDelay(SuperSonic_WaitTime);
						}
					else if(Distant<=SuperSonic_MinDistant)
						{
							UpdateMotorState(MOTOR_BACK);
							SetMotorDutyRatio(0.1,0.1);
							SysTickDelay(SuperSonic_StepTime);
							SetMotorDutyRatio(0,0);
							SysTickDelay(SuperSonic_WaitTime);
						}
					}
					else
					{
						UpdateMotorState(MOTOR_FRONT);
						_GoLineDelay(100);
					}
		///
}


//新加的到台停函数有问题
//已经进行修正 4.22 wlys
//原因分析 可能是原来的函数只延迟了50ms便进行下一轮的采样 导致超声波射到墙壁的回波影响距离判断


//上台停车备份方案


/*函数名称：_ArrivePlatformBackUp_Gray()
**函数输入：无
**函数输出：无
**函数作用：上台停的备用方案 用直行+灰度实现台停
**函数说明：该函数可以直接替代原本的到台函数 该函数只有停到位置了才会
**函数说明：将电机状态置位 MOTOR_STOP 因此直接替换是安全的
*/

/* 需要PCH进一步修改 2019.5.11 By WLYS */
void _ArrivePlatformBackUp_Gray(void)
{
	  const AdcData_t *adcData;
    adcData = UpdateButtom();
    uint32_t _Temp = 0;
		uint32_t Time=0;
    for (int i = 0; i <= 6; i++)
    {
        _Temp += adcData->array[0][i];
    }
		if(_Temp<=27000)
		{
			Time++;
			if(Time==3)
			{
				UpdateMotorState(MOTOR_STOP);
			}
			
		}
}



/*函数名称：_ArrivePlatformBackUp_Delay()
**函数输入：无
**函数输出：无
**函数作用：上台停的备用方案 用直行+延迟实现台停
**函数说明：该函数可以直接替代原本的到台函数 该函数只有停到位置了才会
**函数说明：将电机状态置位 MOTOR_STOP 因此直接替换是安全的
*/
void _ArrivePlatformBackUp_Delay(void)
{
	UpdateMotorState(MOTOR_STOP);
	
	SysTickDelay(100);
	
	//如果上台上歪了的话尝试修改左右电机的PWM值 相关接口会暴露在magic.h中
	UpdateMotorState(MOTOR_FRONT);
	SetMotorDutyRatio(0.1,0.1);
	
	//该时间决定了上台后车的前后位置 相关接口会暴露在magic.h中
	SysTickDelay(1350);
	
	//到位置了 停车
	UpdateMotorState(MOTOR_STOP);
	SetMotorDutyRatio(0,0);
	
}



/*函数名称：_ArrivePlatformBackUp_Encoder()
**函数输入：无
**函数输出：无
**函数作用：上台停的备用方案 用直行+编码器实现台停
**函数说明：该函数可以直接替代原本的到台函数 该函数只有停到位置了才会
**函数说明：将电机状态置位 MOTOR_STOP 因此直接替换是安全的
*/
void _ArrivePlatformBackUp_Encoder(void)
{
	//让车辆完全停下来
	UpdateMotorState(MOTOR_STOP);
	SysTickDelay(100);
	
	Encoder_StopDistantCount();
	Encoder_ResetDistant();
	Encoder_StartDistantCount();
	
	UpdateMotorState(MOTOR_FRONT);
	//如果车辆在上台的时候走不直 更改这个参数 这个参数也会暴露在magic.h中
	SetMotorDutyRatio(0.1,0.1);
	
	
	//这里决定车辆上台走的深浅程度 这个参数会暴露在magic.h中
	while(Encoder_GetCurrentDistantCount() < (float)0.1);
	
	
	//走到位置了 停车
	UpdateMotorState(MOTOR_STOP);
	SetMotorDutyRatio(0,0);
	
}


//远古版本的到台停函数 在有一定的灰尘 以及较新今年胶胎下工作正常

//void _ArrivePlatform(void)
//{
//				if(SuperSonic_GetDistant()<=(float)0.4)
//					{
//					if(SuperSonic_GetDistant()>=(float)0.13&&SuperSonic_GetDistant()<=(float)0.20)
//						{
//							UpdateMotorState(MOTOR_STOP);
//						}
//					else if(SuperSonic_GetDistant()>=(float)0.20)
//						{
//							UpdateMotorState(MOTOR_FRONT);
//							SetMotorDutyRatio(0.1,0.1);
//							SysTickDelay(100);
//							SetMotorDutyRatio(0,0);
//						}
//					else if(SuperSonic_GetDistant()<=(float)0.13)
//						{
//							UpdateMotorState(MOTOR_BACK);
//							SetMotorDutyRatio(0.1,0.1);
//							SysTickDelay(100);
//							SetMotorDutyRatio(0,0);
//						}
//					}
//					else
//					{
//						_GoLineDelay(400);
//					}
//		///
//}
