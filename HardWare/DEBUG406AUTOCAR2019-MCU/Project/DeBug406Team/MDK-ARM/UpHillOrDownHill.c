#include "UpHillOrDownHill.h"
#include "magic.h"
//#define NegativeNumber
static UpHillOrDownHill _UpOrDown=FlatGround;
static IsBalance _Isbalance=BALANCE;
static float PrimaryRollAngle=0;
float _NowGyroYawAngle=0;
UpHillOrDownHill _UpHillOrDownHillFeedBack()
{
	//printf("1=%f\r\n",1.0*(Gyro_GetPitchAngle()-PrimaryRollAngle));
	//printf("%d",_UpOrDown);
	return _UpOrDown;
}
IsBalance _IsBalanceFeedBack(void)
{
	//printf("1=%f\r\n",1.0*(Gyro_GetPitchAngle()-PrimaryRollAngle));
	//printf("%d",_UpOrDown);
	return _Isbalance;
}


#ifdef Use_Magic
 #ifndef Robot_3
	#define Magic
#endif
#endif

void _UpHillOrDownHillCheck(void)
{
	#ifdef NegativeNumber

	if((Gyro_GetPitchAngle()-PrimaryRollAngle<-2)&&(Gyro_GetPitchAngle()-PrimaryRollAngle>-100))
	{
		_UpOrDown=UP;
	}

	else if((Gyro_GetPitchAngle()-PrimaryRollAngle<-300)&&(Gyro_GetPitchAngle()-PrimaryRollAngle>-200))
	{
		_UpOrDown=DOWN;
	}
		else if((Gyro_GetPitchAngle()-PrimaryRollAngle>-1)&&(Gyro_GetPitchAngle()-PrimaryRollAngle<1))
	{
		_UpOrDown=FlatGround;
	}
	#else
	
	//LV7yRKlQDzAIU4zi
	//上坡临界判断角度 陀螺仪从中点往上仰头 角度从359逐渐减少 
	#ifndef Magic
		if((Gyro_GetPitchAngle()-PrimaryRollAngle>300)&&(Gyro_GetPitchAngle()-PrimaryRollAngle<358))
	#else
	if((Gyro_GetPitchAngle()-PrimaryRollAngle>UpHillOrDownHill_UpHill_Max_PitchAngle)&&(Gyro_GetPitchAngle()-PrimaryRollAngle<UpHillOrDownHill_UpHill_Min_PitchAngle))
	#endif
	{
		_UpOrDown=UP;
	}

	//下坡临界判断角度 陀螺仪从中点往下探头 角度从0增加到360
	#ifndef Magic
	else if((Gyro_GetPitchAngle()-PrimaryRollAngle>6)&&(Gyro_GetPitchAngle()-PrimaryRollAngle<100))
	#else
	else if((Gyro_GetPitchAngle()-PrimaryRollAngle>6)&&(Gyro_GetPitchAngle()-PrimaryRollAngle<100))
	#endif
	{
		_UpOrDown=DOWN;
	}
	else
	{
		_UpOrDown=FlatGround;
	}
	//roll角是否平衡
	//#ifndef Magic
	if(Gyro_GetRollAngle()>355||Gyro_GetRollAngle()<5)
	//#else
	//if(Gyro_GetRollAngle()<342||Gyro_GetRollAngle()>355)
	//#endif
	{
		_Isbalance=BALANCE;
	}
	else
	{
		_Isbalance=UNBALANCE;
	}
	#endif
}
void _GetPrimaryRollAngle(void)
{
	PrimaryRollAngle=Gyro_GetPitchAngle();
}
void _ResetUpDown(void)
{
	_UpOrDown=DOWN;
}
void _InitNowYawAngle()
{
	_NowGyroYawAngle=Gyro_GetYawAngle();
}
