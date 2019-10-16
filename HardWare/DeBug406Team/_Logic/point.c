#include "point.h"
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
	//printf("%d",_Time);
			if(_Time>2)
		{
			PointFlag++;
			if(PointFlag>=3)
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
			if(PointFlag>=3)
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



//有问题的到台停函数 5.6wlys注释掉
void _ArrivePlatform(void)
{
	float Distant = 0;
	
	Distant = SuperSonic_GetDistant();
	
				if(Distant<=(float)0.30)
					{
					if(Distant>=SuperSonic_MinDistant&&Distant<=SuperSonic_MaxDistant)
						{
							UpdateMotorState(MOTOR_STOP);
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
void _ArrivePlatformBackUp(void)
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
