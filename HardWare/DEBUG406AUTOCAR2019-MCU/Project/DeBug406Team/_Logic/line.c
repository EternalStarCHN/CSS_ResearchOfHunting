#include "line.h"   /*giaogiao*/
#include "SHELL.h"
static float previous_errorByButtom = 0;
static float previous_error = 0;
extern int8_t LeftCountTimes;
extern int8_t RightCountTimes;
static float LastRError = 0;
static float LastLError = 0;
static float GyroLastError = 0;

/* 测试代码5.16 */
uint8_t _Is_PID_DownBridge_Activated = 0;


/* 巡线相关调整 jcbCKhyF58RkX0sc */
/*
** 0.获取灰度误差函数 NfxSyIc2eNmvgN9G
** 1.正常速度巡线控制 kP1sM5cEl6bSYxgm
** 2.中等速度巡线执行 ejke2AABCoexrDLj
** 3.低速巡线控制     sx1Fdj6qmWiiXR1P
** 4.圆上巡线控制     djC6P0V949VBavfH
** 5.正常速度巡线执行 SJXMdbp7DNmTKG3F
** 6.魔鬼速度巡线执行 xHLPg9bLToP5pzCN
** 7.低速巡线实际执行 9jUTtgPGiaku7DFv
** 8.下长坡实际控制   R86k6xedefOkFYMA
** 9.过桥惯性控制     8bEXBQ56TXj9avr7
**10.过桥惯性误差获取 lJyj6MWZc2jNejvX
**11.过桥惯性实际执行 qXHA9f0RGh1zYZaq
**12.速度锁误差获取   Qui8uit8juUgcJDZ
**13.速度所执行函数   XcoNapCZlJm0Amtm
**14.桥上巡线误差获取 hwlBMBRgX9PfXqai
**15.桥上巡线控制     dWC9IisdSQbtx8YR
**16.桥上巡线执行     pM1n2uVWgICs85qb
**17.OpenMV巡线控制   asfhakfcafja6sda
**18.OpenMV巡线执行   asfhadkaiafhalad

**17.带参数的正常速度巡线函数 BbVL9LNyaoFhbLws
*/


/* 桥上灰度误差获取 在桥上灰度反应和在地面上刚好相反 所以要单独写一组误差获取函数 */
//hwlBMBRgX9PfXqai
double _GetADCErrorInBridge(void)
{
	double Error, Bias;
#ifdef Robot_1
  double _TURE = 0.000106; //此参数如何得出 需要公式
#endif

//第二组真值需要调整
#ifdef Robot_2
  double _TURE = -0.000022; //此参数如何得出 需要公式
#endif

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


//在桥上的巡线控制函数 注意 在地上执行该函数将立即寻出线外 表现为哪边偏就往哪边调
//dWC9IisdSQbtx8YR
double _TrackingCoreAlgorithmInBridgeByButtom(void)
{
	#ifdef Robot_1
  float Kp = 520, Ki = 15, Kd = 30;
#endif

	//由于桥上差值的降低 即使是用老灰度也需要大幅提高巡线的激进程度 5.21需要调整
	#ifdef Robot_2
  float Kp = 520, Ki = 15, Kd = 15;
#endif

  float error = 0, P = 0.0, I = 0., D = 0, PID_value = 0;
  error = _GetADCErrorInBridge();
  P = error;
  I = I + error;
  D = error - previous_errorByButtom;
  PID_value = (Kp * P) + (Ki * I) + (Kd * D);
  previous_errorByButtom = error;
  return PID_value;
}

//在桥上的巡线执行函数 注意 在地上执行该函数将立即寻出线外 表现为哪边偏就往哪边调
//pM1n2uVWgICs85qb
void _GoBridgeByButtom(void)
{
	#ifdef Robot_1
	SetMotorDutyRatio(0.21 - _TrackingCoreAlgorithmInBridgeByButtom(), 0.21 + _TrackingCoreAlgorithmInBridgeByButtom());
	#endif
	
	#ifdef Robot_2 //桥上巡线倒过来 左就是右 右就是左 所以这里增益也倒过来
	SetMotorDutyRatio(0.14 - 0.8*_TrackingCoreAlgorithmInBridgeByButtom(), 0.09 + 1.2*_TrackingCoreAlgorithmInBridgeByButtom());
	#endif
}
/* 获取灰度误差函数 用来给PID巡线算法提供误差 NfxSyIc2eNmvgN9G */
double _GetADCError(void)
{
  double Error, Bias;
#ifdef Robot_1
  double _TURE = 0.000048; //此参数如何得出 需要公式
#endif

#ifdef Robot_2
  double _TURE = -0.00005680; //此参数如何得出 需要公式
#endif

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

/* 正常速度巡线的PID控制函数 kP1sM5cEl6bSYxgm */
double _TrackingCoreAlgorithm(void)
{
#ifdef Robot_1
  float Kp = 134, Ki = 7.5, Kd = 20;
#endif
#ifdef Robot_2
  float Kp = 130.763, Ki = 8.5, Kd = 7.5;
#endif
  float error = 0, P = 0.0, I = 0.0, D = 0, PID_value = 0;
  error = _GetADCError();
  P = error;
  I = I + error;
  D = error - previous_error;
  PID_value = (Kp * P) + (Ki * I) + (Kd * D);
  previous_error = error;
  return PID_value;
}

/*OpenMV巡线控制的PID控制函数 asfhakfcafja6sda*/
double _TrackingCoreAlgorithmOpenMV(void)
{
	float Kp = 0, Ki = 0, Kd = 0;
	float error = 0, P = 0.0, I = 0, D = 0, PID_value = 0;
	error = _DealRhoErrorSign();
	P = error;
	I = I + error;
	D = error - previous_error;
	PID_value = (Kp * P) + (Ki * I) + (Kd * D);
  previous_error = error;
	return PID_value;
}
/* 以中等速度巡线实际执行函数 ejke2AABCoexrDLj */
void _GoLineMSpeed(void)
{

#ifdef Robot_1
  SetMotorDutyRatio(0.14 + _TrackingCoreAlgorithm(), 0.14 - _TrackingCoreAlgorithm());
#endif

#ifdef Robot_2
  SetMotorDutyRatio(0.15 + _TrackingCoreAlgorithm(), 0.09 - _TrackingCoreAlgorithm());
#endif
}
/* 低速巡线的PID控制函数 */
double _TrackingCoreAlgorithmLowSpeed(void)
{

#ifdef Robot_1
  float Kp = 89, Ki = 1.2, Kd = 9.4;
#endif

#ifdef Robot_2
  float Kp = 89.763, Ki = 1.189, Kd = 9.0;
#endif
  float error = 0, P = 0.0, I = 0., D = 0, PID_value = 0;
  error = _GetADCError();
  P = error;
  I = I + error;
  D = error - previous_error;
  PID_value = (Kp * P) + (Ki * I) + (Kd * D);
  previous_error = error;
  return PID_value;
}

/* 在圆上的寻线控制函数 djC6P0V949VBavfH */
double _TrackingCoreAlgorithmInCricle(void)
{
	#ifdef Robot_1
  float Kp = 174.763, Ki = 0.689, Kd = 1.1;
	#endif
	
	#ifdef Robot_2
	float Kp = 174.763, Ki = 0.689, Kd = 1.1;
	#endif
	
  float error = 0, P = 0.0, I = 0., D = 0, PID_value = 0;
  error = _GetADCError();
  P = error;
  I = I + error;
  D = error - previous_error;
  PID_value = (Kp * P) + (Ki * I) + (Kd * D);
  previous_error = error;
  return PID_value;
}

/* 正常速度巡线的实际执行函数 SJXMdbp7DNmTKG3F */
void _GoLine(void)
{
#ifdef Robot_1
  SetMotorDutyRatio(0.30 + _TrackingCoreAlgorithm(), 0.30 - _TrackingCoreAlgorithm());
#endif

#ifdef Robot_2
  SetMotorDutyRatio(0.23 + 1.2*(_TrackingCoreAlgorithm()), 0.17 -  0.8*(_TrackingCoreAlgorithm()));
#endif
}


/* 带参数的正常速度巡线函数 BbVL9LNyaoFhbLws */
void _GoLine_UserAdjust(float LPWM,float RPWM)
{
	#ifdef Robot_1
	SetMotorDutyRatio(LPWM + _TrackingCoreAlgorithm(), RPWM - _TrackingCoreAlgorithm());
	#endif
	
	#ifdef Robot_2
	SetMotorDutyRatio(LPWM + _TrackingCoreAlgorithm(), RPWM - _TrackingCoreAlgorithm());
	#endif
}


/* 魔鬼速度巡线的实际执行函数 xHLPg9bLToP5pzCN */
void _GoLineHighSpeed()
{
	#ifdef Robot_1
  SetMotorDutyRatio(0.95 + _TrackingCoreAlgorithm(), 0.95 - _TrackingCoreAlgorithm());
	#endif
	
	#ifdef Robot_2
	SetMotorDutyRatio(0.95 + _TrackingCoreAlgorithm(), 0.95 - _TrackingCoreAlgorithm());
	#endif
}

/* 低速巡线的实际执行函数 9jUTtgPGiaku7DFv */
void _GoLineLowSpeed(void)
{
	#ifdef Robot_1
  SetMotorDutyRatio(0.07 + _TrackingCoreAlgorithmLowSpeed(), 0.07 - _TrackingCoreAlgorithmLowSpeed());
	#endif
	
	#ifdef Robot_2
//	SetMotorDutyRatio(0.05 + _TrackingCoreAlgorithmLowSpeed(), 0.05 - _TrackingCoreAlgorithmLowSpeed());
	SetMotorDutyRatio(0.06 + _TrackingCoreAlgorithmLowSpeed(), 0.06 - _TrackingCoreAlgorithmLowSpeed());

	#endif
}

/* 下长坡低速巡线PID的实际执行函数 该函数使用低速PID控制函数 R86k6xedefOkFYMA */
void _GoLineLowSpeedDownBridge(void)
{
  float temp = 0;

  static uint8_t counter = 0;

  counter++;

  //自动适应编码器节拍
  if (counter >= (100 / State_MachineTickTime) + 1)
    {
      counter = 0;
      temp = (Encoder_GetLWheelSpeed() + Encoder_GetRWheelSpeed()) / 2;
      if (temp > (float)0.35)
        {
          //Speed = 0.04;
          UpdateMotorState(MOTOR_STOP);
          SysTickDelay(2);
          UpdateMotorState(MOTOR_FRONT);
        }
    }

#ifdef Robot_1
  SetMotorDutyRatio(0.05 + _TrackingCoreAlgorithmLowSpeed(), 0.05 - _TrackingCoreAlgorithmLowSpeed());
#endif
#ifdef Robot_2
  SetMotorDutyRatio(0.06 + _TrackingCoreAlgorithmLowSpeed(), 0.04 - _TrackingCoreAlgorithmLowSpeed());
#endif
}


/* 新版下台函数 */
void _GoLineLowSpeedDownBridgeTest(void)
{

	#ifdef Robot_1
  SetMotorDutyRatio(0.05 + _TrackingCoreAlgorithmLowSpeed(), 0.05 - _TrackingCoreAlgorithmLowSpeed());
	#endif
		
	#ifdef Robot_2
	SetMotorDutyRatio(0.05 + _TrackingCoreAlgorithmLowSpeed(), 0.05 - _TrackingCoreAlgorithmLowSpeed());
	#endif

}

/* 过桥PID控制函数 该函数用陀螺仪的Yaw角作为判断 8bEXBQ56TXj9avr7 */
double _TrackingCoreAlgorithmInBridge(void)
{

#ifdef Robot_1
  float Kp = 0.52, Ki = 0.0032, Kd = 0.0035;
#endif

#ifdef Robot_2
  float Kp = 0.32, Ki = 0.0032, Kd = 0.0035;
#endif
  float error = 0, P = 0.0, I = 0, D = 0, PID_value = 0;

  error = (float)_GetGYROError();
  //printf("%f\r\n",error);

  P = error;
  I = I + error;
  D = error - GyroLastError;
  PID_value = (Kp * P) + (Ki * I) + (Kd * D);

  GyroLastError = error;
  //return PID_value;
  if (PID_value <= (float)0.86 || PID_value >= (float)-0.86)
    {
      return PID_value;
    }
  else
    {
      if (PID_value > (float)0.86)
        {
          UpdateMotorState(MOTOR_STOP);
          Motor_TurnRightBlockingMode(error + 1);
          UpdateMotorState(MOTOR_FRONT);
          GyroLastError = 0;
        }
      else if (PID_value < (float)-0.86)
        {
          UpdateMotorState(MOTOR_STOP);
          Motor_TurnLeftBlockingMode(error + 1);
          UpdateMotorState(MOTOR_FRONT);
          GyroLastError = 0;
        }
      return 0;
    }
}

/* 过桥陀螺仪Yaw角误差获取函数 lJyj6MWZc2jNejvX */
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

/* 过桥PID的实际执行函数 233实际上是老方案陀螺仪的实际执行函数  qXHA9f0RGh1zYZaq */
void _GoBridge()
{
	#ifdef Robot_1
  SetMotorDutyRatio(0.07 + _TrackingCoreAlgorithmInBridge(), 0.07 - _TrackingCoreAlgorithmInBridge());
	#endif
	
	#ifdef Robot_1
	SetMotorDutyRatio(0.07 + _TrackingCoreAlgorithmInBridge(), 0.07 - _TrackingCoreAlgorithmInBridge());
	#endif

}

/* 速度锁误差获取函数 Qui8uit8juUgcJDZ */
float _GetLSpeedError()
{
  return (Encoder_GetLWheelSpeed() - (float)0.2);
}
float _GetRSpeedError(void)
{
  return (Encoder_GetRWheelSpeed() - (float)0.2);
}

/* 速度锁PID控制函数 XcoNapCZlJm0Amtm */
void _BlockSpeed(void)
{
	#ifdef Robot_1
  float LKp = 0.475, LKi = 0.04, LKd = 0.70;
  float RKp = 0.475, RKi = 0.04, RKd = 0.70;
	#endif
	
	#ifdef Robot_2
  float LKp = 0.475, LKi = 0.04, LKd = 0.70;
  float RKp = 0.475, RKi = 0.04, RKd = 0.70;	
	#endif
 
	float Lerror = 0, LP = 0.0, LI = 0.,LD = 0, LPID_value = 0;
  float Rerror = 0, RP = 0.0, RI = 0.,RD = 0, RPID_value = 0;
 // printf("left: %f\r\n right: %f\r\n",LastLError,LastRError);
  Lerror = 0 - (float)_GetLSpeedError();
  Rerror = 0 - (float)_GetRSpeedError();
  //printf("%f\r\n",error);
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
  SetMotorDutyRatio((float)0.06 + LPID_value, (float)0.06 + RPID_value);

}

/* 圈内平地低速巡线PID执行函数 */
void _GoCricle(void)
{
	#ifdef Robot_2
  SetMotorDutyRatio(0.04 + _TrackingCoreAlgorithmInCricle(), 0.055 - 2.0*(_TrackingCoreAlgorithmInCricle()));
	#endif
  #ifdef Robot_1
  SetMotorDutyRatio(0.07 + _TrackingCoreAlgorithmInCricle(), 0.07 - _TrackingCoreAlgorithmInCricle());
	#endif
}

/* 圈内过障碍低速巡线带轮速锁的PID执行函数 */
void _GoCricleSpeedLock(void)
{
  static float Speed = 0;
  static uint8_t counter = 0;
  static float PWM = 0.05;

  if (counter > (100 / State_MachineTickTime) + 1)
    {
      counter = 0;

      //平均轮速 = (左边轮速 + 右边轮速) / 2.0
      Speed = Encoder_GetLWheelSpeed() + Encoder_GetRWheelSpeed();
      Speed /= (float)2.0;

      //速度过快就减小基PWM
      if (Speed > (float)0.30)
        {
          PWM -= (float)0.006;
        }

      //速度过慢就加大基PWM
      if (Speed < (float)0.50)
        {
          PWM += (float)0.002;
        }
    }

  SetMotorDutyRatio(PWM + _TrackingCoreAlgorithmInCricle(), (PWM - _TrackingCoreAlgorithmInCricle()));
  counter++;
}

