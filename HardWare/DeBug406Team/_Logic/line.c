#include "line.h"

static float previous_error = 0;
extern int8_t LeftCountTimes;
extern int8_t RightCountTimes;
static float LastRError=0;
static float LastLError=0;
static float GyroLastError=0;
/* 4.22 By WLYS 如果想直接手动改代码的话注释掉下面一行 */
#define USE_Magic

#ifdef USE_Magic

/* 获取灰度误差函数 用来给PID巡线算法提供误差 */
double _GetADCError(void)
{
    double Error, Bias;
    double _TURE = 0.000082755733553;
    const AdcData_t *adcData;
    uint16_t temp1 = 0, temp2 = 0;
    adcData = UpdateButtom();
    for (int i = 0; i <= 3; i++)
        temp1 += adcData->array[0][i];
    for (int i = 4; i <= 7; i++)
        temp2 += adcData->array[0][i];
    Bias = (sqrt(temp1 * 1.0) - sqrt(temp2 * 1.0)) / (temp1 + temp2);
    Error = _TURE - Bias;
    return Error;
}

/* 正常速度(高速)巡线的PID控制函数 */
double _TrackingCoreAlgorithm(void)
{
    float Kp = KpHightSpeed, Ki = KiHightSpeed, Kd = KdHightSpeed;
    float error = 0, P = 0.0, I = 0., D = 0, PID_value = 0;
    error = _GetADCError();
    P = error;
    I = I + error;
    D = error - previous_error;
    PID_value = (Kp * P) + (Ki * I) + (Kd * D);
    previous_error = error;
    return PID_value;
}

/* 低速巡线的PID控制函数 */
double _TrackingCoreAlgorithmLowSpeed(void)
{
    float Kp = KpLowSpeed, Ki = KiLowSpeed, Kd = KdLowSpeed;
    float error = 0, P = 0.0, I = 0., D = 0, PID_value = 0;
    error = _GetADCError();
    P = error;
    I = I + error;
    D = error - previous_error;
    PID_value = (Kp * P) + (Ki * I) + (Kd * D);
    previous_error = error;
    return PID_value;
}

/* 正常速度巡线的实际执行函数 */
void _GoLine(void)
{
    SetMotorDutyRatio(HightSpeedPID_PWM + _TrackingCoreAlgorithm(), HightSpeedPID_PWM - _TrackingCoreAlgorithm());
}

/* 低速巡线的实际执行函数 */
void _GoLineLowSpeed(void)
{
    SetMotorDutyRatio(LowSpeedPID_PWM + _TrackingCoreAlgorithmLowSpeed(), LowSpeedPID_PWM - _TrackingCoreAlgorithmLowSpeed());
}

/* 下长坡低速巡线PID的实际执行函数 该函数使用低速PID控制函数 */
void _GoLineLowSpeedDownBridge(void)
{
    float Speed = DownBridgePWM;
    float temp = 0;

    static uint8_t counter = 0;

    counter++;

    if (counter >= 7)
    {
        counter = 0;
        temp = (Encoder_GetLWheelSpeed() + Encoder_GetRWheelSpeed()) / 2;
        if (temp > (float)DownBridgeBrakeSpeed)
        {
            //Speed = 0.04;
            UpdateMotorState(MOTOR_STOP);
            SysTickDelay(DownBridgeBrakeTime);
            UpdateMotorState(MOTOR_FRONT);
        }
    }

    SetMotorDutyRatio(Speed + _TrackingCoreAlgorithmLowSpeed(), Speed - _TrackingCoreAlgorithmLowSpeed());
}

/* 过桥PID控制函数 该函数用陀螺仪的Yaw角作为判断 */
double _TrackingCoreAlgorithmInBridge(void)
{
    float Kp = KpBridge, Ki = KiBridge, Kd = KdBridge;
    float error = 0, P = 0.0, I = 0., D = 0, PID_value = 0;

    error = (float)_GetGYROError();
    //printf("%f\r\n",error);
    P = error;
    I = I + error;
    D = error - previous_error;
    PID_value = (Kp * P) + (Ki * I) + (Kd * D);
    previous_error = error;
    return PID_value;
}

/* 过桥陀螺仪Yaw角误差获取函数 */
double _GetGYROError()
{

    if (_NowGyroYawAngle < 180)
    {
        if (Gyro_GetYawAngle() > 180)
        {
            if (fabs(Gyro_GetYawAngle() - _NowGyroYawAngle) <= 180)
            {
                return (Gyro_GetYawAngle() - _NowGyroYawAngle);
            }
            else

                return (-(_NowGyroYawAngle + 360 - Gyro_GetYawAngle()));
        }
        else
            return (Gyro_GetYawAngle() - _NowGyroYawAngle);
    }
    else
    {
        if (Gyro_GetYawAngle() < 180)
        {
            if (fabs(Gyro_GetYawAngle() - _NowGyroYawAngle) <= 180)
            {
                return (Gyro_GetYawAngle() - _NowGyroYawAngle);
            }
            else

                return ((Gyro_GetYawAngle() + 360) - _NowGyroYawAngle);
        }
        else if (fabs(Gyro_GetYawAngle() - _NowGyroYawAngle) <= 180)
        {
            return (Gyro_GetYawAngle() - _NowGyroYawAngle);
        }
        else
            return ((Gyro_GetYawAngle() + 360) - _NowGyroYawAngle);
    }
}

/* 过桥PID的实际执行函数 */
void _GoBridge()
{
    //printf("b");
    SetMotorDutyRatio(BridgePID_PWM + _TrackingCoreAlgorithmInBridge(), BridgePID_PWM - _TrackingCoreAlgorithmInBridge());
}

/* 速度锁误差获取函数 */
float _GetSpeedError()
{
    return (((Encoder_GetLWheelSpeed() + Encoder_GetRWheelSpeed()) / 2) - (float)BlockSpeed_TargetSpeed);
}

/* 速度锁PID控制函数 */
void _BlockSpeed(void)
{
    float Kp = BlockSpeed_Kp, Ki = BlockSpeed_Ki, Kd = BlockSpeed_Kd;
    float error = 0, P = 0.0, I = 0, D = 0, PID_value = 0;

    error = 0 - (float)_GetSpeedError();
    //printf("%f\r\n",error);
    if (error < (float)-0.1)
    {
        UpdateMotorState(MOTOR_STOP);
        SysTickDelay(BlockSpeed_BrakeTime);
        UpdateMotorState(MOTOR_FRONT);
        SetMotorDutyRatio(0, 0);
    }
    else
    {
        P = error;
        I = I + error;
        D = error - previous_error;
        PID_value = (Kp * P) + (Ki * I) + (Kd * D);
        previous_error = error;
        SetMotorDutyRatio((float)0 + PID_value, (float)0 + PID_value);
    }
}

#endif

// 不使用magic.h文件
#ifndef USE_Magic

/* 获取灰度误差函数 用来给PID巡线算法提供误差 */
double _GetADCError(void)
{
    double Error, Bias;
    double _TURE = 0.000082755733553;
    const AdcData_t *adcData;
    uint16_t temp1 = 0, temp2 = 0;
    adcData = UpdateButtom();
    for (int i = 0; i <= 3; i++)
        temp1 += adcData->array[0][i];
    for (int i = 4; i <= 7; i++)
        temp2 += adcData->array[0][i];
    Bias = (sqrt(temp1 * 1.0) - sqrt(temp2 * 1.0)) / (temp1 + temp2);
    Error = _TURE - Bias;
    return Error;
}

/* 正常速度(高速)巡线的PID控制函数 */
double _TrackingCoreAlgorithm(void)
{
    float Kp = 132.763, Ki = 10, Kd = 22.5;
    float error = 0, P = 0.0, I = 0., D = 0, PID_value = 0;
    error = _GetADCError();
    P = error;
    I = I + error;
    D = error - previous_error;
    PID_value = (Kp * P) + (Ki * I) + (Kd * D);
    previous_error = error;
    return PID_value;
}
void _GoLineMSpeed(void)
{
	SetMotorDutyRatio(0.14 + _TrackingCoreAlgorithm(), 0.14 - _TrackingCoreAlgorithm());
}
/* 低速巡线的PID控制函数 */
double _TrackingCoreAlgorithmLowSpeed(void)
{
    float Kp = 84.763, Ki = 1.489, Kd = 9.4;
    float error = 0, P = 0.0, I = 0., D = 0, PID_value = 0;
    error = _GetADCError();
    P = error;
    I = I + error;
    D = error - previous_error;
    PID_value = (Kp * P) + (Ki * I) + (Kd * D);
    previous_error = error;
    return PID_value;
}
/* 在圆上执行的寻线函数 */
double _TrackingCoreAlgorithmInCricle(void)
{
    float Kp = 104.763, Ki = 0.789, Kd = 0.9;
    float error = 0, P = 0.0, I = 0., D = 0, PID_value = 0;
    error = _GetADCError();
    P = error;
    I = I + error;
    D = error - previous_error;
    PID_value = (Kp * P) + (Ki * I) + (Kd * D);
    previous_error = error;
    return PID_value;
}

/* 正常速度巡线的实际执行函数 */
void _GoLine(void)
{
    SetMotorDutyRatio(0.21 + _TrackingCoreAlgorithm(), 0.21 - _TrackingCoreAlgorithm());
}

/* 低速巡线的实际执行函数 */
void _GoLineLowSpeed(void)
{
    SetMotorDutyRatio(0.06 + _TrackingCoreAlgorithmLowSpeed(), 0.06 - _TrackingCoreAlgorithmLowSpeed());
}


/* 下长坡低速巡线PID的实际执行函数 该函数使用低速PID控制函数 */
void _GoLineLowSpeedDownBridge(void)
{
    float Speed = 0.06;
    float temp = 0;

    static uint8_t counter = 0;

    counter++;

    //自动适应编码器节拍
    if (counter >= (100 / State_MachineTickTime) + 1)
    {
        counter = 0;
        temp = (Encoder_GetLWheelSpeed() + Encoder_GetRWheelSpeed()) / 2;
        if (temp > (float)0.40)
        {
            //Speed = 0.04;
            UpdateMotorState(MOTOR_STOP);
            SysTickDelay(20);
            UpdateMotorState(MOTOR_FRONT);
        }
    }

    SetMotorDutyRatio(Speed + _TrackingCoreAlgorithmLowSpeed(), Speed - _TrackingCoreAlgorithmLowSpeed());
}

/* 过桥PID控制函数 该函数用陀螺仪的Yaw角作为判断 */
double _TrackingCoreAlgorithmInBridge(void)
{
    float Kp = 0.32, Ki = 0.0012, Kd = 0.0025;
    float error = 0, P = 0.0, I = 0, D = 0, PID_value = 0;

    error = (float)_GetGYROError();
    //printf("%f\r\n",error);

    P = error;
    I = I + error;
    D = error - GyroLastError;
    PID_value = (Kp * P) + (Ki * I) + (Kd * D);
	
    GyroLastError = error;
		//return PID_value;
	if(PID_value<=(float)0.86||PID_value>=(float)-0.86)
	{
    return PID_value;
	}
	else 
	{
		if(PID_value>(float)0.86)
		{
			UpdateMotorState(MOTOR_STOP);
			Motor_TurnRightBlockingMode(error+1);
			UpdateMotorState(MOTOR_FRONT);
			GyroLastError=0;
		}
		else if (PID_value<(float)-0.86)
		{
			UpdateMotorState(MOTOR_STOP);
			Motor_TurnLeftBlockingMode(error+1);
			UpdateMotorState(MOTOR_FRONT);
			GyroLastError=0;
		}
		return 0;
	}
}

/* 过桥陀螺仪Yaw角误差获取函数 */
double _GetGYROError()
{

    if (_NowGyroYawAngle < 180)
    	{
    		if (Gyro_GetYawAngle() > 180)
    		{
    			if (fabs(Gyro_GetYawAngle() - _NowGyroYawAngle) <= 180)
    			{
    				return (Gyro_GetYawAngle() - _NowGyroYawAngle);
    			}
    			else

    			return (-(_NowGyroYawAngle + 360 - Gyro_GetYawAngle()));
    			}
    		else
    			return (Gyro_GetYawAngle() - _NowGyroYawAngle);
    	}
    	else
    	{
    		if (Gyro_GetYawAngle() < 180)
    		{
    			if (fabs(Gyro_GetYawAngle() - _NowGyroYawAngle) <= 180)
    			{
    				return (Gyro_GetYawAngle() - _NowGyroYawAngle);
    			}
    			else

    			return ( (Gyro_GetYawAngle() + 360)- _NowGyroYawAngle);
    		}
    		else
    			if (fabs(Gyro_GetYawAngle() - _NowGyroYawAngle) <= 180)
    			{
    				return (Gyro_GetYawAngle() - _NowGyroYawAngle);
    			}
    			else
    				return ((Gyro_GetYawAngle() + 360) - _NowGyroYawAngle);
    	}
//    if (_NowGyroYawAngle < 180)
//    {
//        if (Gyro_GetYawAngle() > 180)
//        {
//            return (-(_NowGyroYawAngle + 360 - Gyro_GetYawAngle()));
//        }
//        else
//            return (Gyro_GetYawAngle() - _NowGyroYawAngle);
//    }
//    else
//    {
//        if (Gyro_GetYawAngle() < 180)
//        {
//            return (_NowGyroYawAngle - (Gyro_GetYawAngle() + 360));
//        }
//        else
//            return (Gyro_GetYawAngle() - _NowGyroYawAngle);
//    }
}

/* 过桥PID的实际执行函数 */
void _GoBridge()
{
    //printf("%f\r\n",_TrackingCoreAlgorithmInBridge());
    SetMotorDutyRatio(0.14 + _TrackingCoreAlgorithmInBridge(), 0.14 - _TrackingCoreAlgorithmInBridge());
}

/* 速度锁误差获取函数 */
float _GetLSpeedError()
{
    return (Encoder_GetLWheelSpeed()  - (float)0.4);
}
float _GetRSpeedError(void)
{
		return (Encoder_GetRWheelSpeed()  - (float)0.4);
}
/* 速度锁PID控制函数 */
void _BlockSpeed(void)
{
    float LKp = 0.425, LKi = 0.05, LKd = 0.80;
		float RKp = 0.825, RKi = 0.05, RKd = 0.20;
    float Lerror = 0, LP = 0.0, LI = 0.,LD = 0, LPID_value = 0;
		float Rerror = 0, RP = 0.0, RI = 0.,RD = 0, RPID_value = 0;
		//printf("left: %f\r\n right: %f\r\n",LastLError,LastRError);
    Lerror = 0 - (float)_GetLSpeedError();
		Rerror = 0-(float)_GetRSpeedError();
    //printf("%f\r\n",error);
    if (Lerror < (float)-0.1)
    {
        UpdateMotorState(MOTOR_STOP);
        SysTickDelay(75);
        UpdateMotorState(MOTOR_FRONT);
        SetMotorDutyRatio(0, 0);
    }
    else
    {
        LP = Lerror;
        LI = LI + Lerror;
				LD = Lerror - LastLError;
        LPID_value = (LKp * LP) + (LKi * LI) + (LKd * LD);
        LastLError = Lerror;
				RP = Rerror;
        RI = RI + Rerror;
				RD = Rerror - LastRError;
        RPID_value = (RKp * RP) + (RKi * RI) + (RKd * RD);
        LastRError = Rerror;
        SetMotorDutyRatio((float)0.05 + LPID_value, (float)0.05 + RPID_value);
    }
}

/* 圈内平地低速巡线PID执行函数 */
void _GoCricle(void)
{
  SetMotorDutyRatio(0.04 + _TrackingCoreAlgorithmInCricle(), (float)4.5* (0.04 - _TrackingCoreAlgorithmInCricle()));
}


/* 圈内过障碍低速巡线带轮速锁的PID执行函数 */
void _GoCricleSpeedLock(void)
{
	static float Speed = 0;
	static uint8_t counter = 0;
	static float PWM = 0.05;
	
	if(counter > (100 / State_MachineTickTime) + 1)
	{
		counter = 0;
		
		//平均轮速 = (左边轮速 + 右边轮速) / 2.0
		Speed = Encoder_GetLWheelSpeed() + Encoder_GetRWheelSpeed();
		Speed /= (float)2.0;
		
		//速度过快就减小基PWM
		if(Speed > (float)0.40)
		{
			PWM -= (float)0.006;
		}
		
		//速度过慢就加大基PWM
		if(Speed < (float)0.50)
		{
			PWM += (float)0.002;
		}

		
	}
	
  SetMotorDutyRatio(PWM + _TrackingCoreAlgorithmInCricle(), (PWM - _TrackingCoreAlgorithmInCricle()));	
	counter ++;
	
}


#endif
