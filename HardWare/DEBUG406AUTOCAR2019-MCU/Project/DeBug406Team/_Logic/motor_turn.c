#include "motor_turn.h"
#include "line.h"

/*
* 函数名称：Motor_TurnLeftBlockingMode()
* 函数作用：控制车辆左转一定的角度
* 函数输入：要左转的角度值 取值范围为 10-270 超出这个范围可能有魔法
* 函数输出：无
* 注意事项：该函数时阻塞函数 在执行左转的时候单片机做不了其他事(只有中断服务能运行)
* 魔法相关：该函数可能需要调整的值已经放到magic.h内
*/
void Motor_TurnLeftBlockingMode(float Angle)
{
  float CurrentAngle = 0;
	float TurnAngle = 0;
	
	SysTickDelay(MOTOR_TurnLeft_StableTime);
	
	CurrentAngle = Gyro_GetYawAngle();
	
	if(Angle > CurrentAngle)
	{
		TurnAngle = Angle + CurrentAngle;
		UpdateMotorState(MOTOR_TURN_LEFT);
		SetMotorDutyRatio(MOTOR_TurnLeft_LeftPWM,MOTOR_TurnLeft_RightPWM);
		
		while(fabs(TurnAngle - Gyro_GetYawAngle()) > 20)
		{
		}
		UpdateMotorState(MOTOR_STOP);
		SetMotorDutyRatio(0,0);
		
		
	}
	else if(Angle < CurrentAngle)
	{
		
		TurnAngle = Angle + CurrentAngle;
		
		if(TurnAngle > 360)
		{
			TurnAngle -= 360;
			
			UpdateMotorState(MOTOR_TURN_LEFT);
			SetMotorDutyRatio(MOTOR_TurnLeft_LeftPWM,MOTOR_TurnLeft_RightPWM);
			
			while(Gyro_GetYawAngle() < 357)
			{
			}
			SysTickDelay(70);
			
			while(fabs(TurnAngle - Gyro_GetYawAngle()) > 20)
			{
			}
			UpdateMotorState(MOTOR_STOP);
			SetMotorDutyRatio(0,0);
			
		}
		else
		{
			UpdateMotorState(MOTOR_TURN_LEFT);
			SetMotorDutyRatio(MOTOR_TurnLeft_LeftPWM,MOTOR_TurnLeft_RightPWM);
			
			while(fabs(TurnAngle - Gyro_GetYawAngle()) > 20)
			{
			}
			UpdateMotorState(MOTOR_STOP);
			SetMotorDutyRatio(0,0);			
			
		}
	}
	
	SysTickDelay(MOTOR_TurnLeft_WaitTime);
	
	//微调
	
	while(fabs(TurnAngle - Gyro_GetYawAngle()) > 1)
	{
		if(TurnAngle > Gyro_GetYawAngle())
		{
			UpdateMotorState(MOTOR_TURN_LEFT);
			SetMotorDutyRatio(MOTOR_TurnLeft_LeftPWM_ADJ,MOTOR_TurnLeft_RightPWM_ADJ);
		}
		
		if(Gyro_GetYawAngle() > TurnAngle)
		{
			UpdateMotorState(MOTOR_TURN_RIGHT);
			SetMotorDutyRatio(MOTOR_TurnLeft_LeftPWM_ADJ,MOTOR_TurnLeft_RightPWM_ADJ);
		}
	}
	
	
	UpdateMotorState(MOTOR_STOP);
	SetMotorDutyRatio(0,0);			
	
}




/*
* 函数名称：Motor_TurnLeft180OnPlatform()
* 函数作用：控制车辆在平台上左转180°
* 函数输入：无
* 函数输出：无
* 注意事项：该函数时阻塞函数 在执行左转的时候单片机做不了其他事(只有中断服务能运行)
* 注意事项：该函数应仅在平台上使用
* 魔法相关：该函数可能需要调整的值已经放到magic.h内
* 魔法相关：该函数左右轮PWM可以与常规左右转函数不一样 具体在magic.h内调整
*/
void Motor_TurnLeft180OnPlatform(void)
{
  float CurrentAngle = 0;
	float TurnAngle = 0;
	
	//要转的角度 写入magic.h内
	float Angle = MOTOR_TurnAroundOnPlatformAngle;
	
	SysTickDelay(MOTOR_TurnLeft_StableTime);
	
	CurrentAngle = Gyro_GetYawAngle();
	
	if(Angle > CurrentAngle)
	{
		TurnAngle = Angle + CurrentAngle;
		UpdateMotorState(MOTOR_TURN_LEFT);
		SetMotorDutyRatio(MOTOR_TurnAroundOnPlatformLPWM,MOTOR_TurnAroundOnPlatformRPWM);
		
		while(fabs(TurnAngle - Gyro_GetYawAngle()) > 15)
		{
		}
		UpdateMotorState(MOTOR_STOP);
		SetMotorDutyRatio(0,0);
		
		
	}
	else if(Angle < CurrentAngle)
	{
		
		TurnAngle = Angle + CurrentAngle;
		
		if(TurnAngle > 360)
		{
			TurnAngle -= 360;
			
			UpdateMotorState(MOTOR_TURN_LEFT);
			SetMotorDutyRatio(MOTOR_TurnAroundOnPlatformLPWM,MOTOR_TurnAroundOnPlatformRPWM);
			
			#ifdef Robot_1
			while(Gyro_GetYawAngle() < 357)
			{
			}
			SysTickDelay(70);
			#endif
			
			
			#ifdef Robot_2
			while(Gyro_GetYawAngle() < 358)
			{
			}
			SysTickDelay(20);			
			#endif
			
			while(fabs(TurnAngle - Gyro_GetYawAngle()) > 15)
			{
			}
			UpdateMotorState(MOTOR_STOP);
			SetMotorDutyRatio(0,0);
			
		}
		else
		{
			UpdateMotorState(MOTOR_TURN_LEFT);
			SetMotorDutyRatio(MOTOR_TurnAroundOnPlatformLPWM,MOTOR_TurnAroundOnPlatformRPWM);
			
			while(fabs(TurnAngle - Gyro_GetYawAngle()) > 15)
			{
			}
			UpdateMotorState(MOTOR_STOP);
			SetMotorDutyRatio(0,0);			
			
		}
	}
	
	SysTickDelay(MOTOR_TurnLeft_WaitTime);
		
}




/*
* 函数名称：Motor_TurnLeft180OnPlatform_UserAdjust()
* 函数作用：控制车辆在平台上左转180° 包含用户每个台可能需要的调整值
* 函数输入：每个台子用户自己的左右粗调PWM 以及转弯角度
* 函数输出：无
* 注意事项：该函数时阻塞函数 在执行左转的时候单片机做不了其他事(只有中断服务能运行)
* 注意事项：该函数应仅在平台上使用
* 魔法相关：该函数可能需要调整的值已经放到magic.h内
* 魔法相关：该函数左右轮PWM可以与常规左右转函数不一样 具体在magic.h内调整
*/
void Motor_TurnLeft180OnPlatform_UserAdjust(float LPWM,float RPWM,float AimAngle)
{
  float CurrentAngle = 0;
	float TurnAngle = 0;
	
	//要转的角度 写入magic.h内
	float Angle = AimAngle;
	
	SysTickDelay(MOTOR_TurnLeft_StableTime);
	
	CurrentAngle = Gyro_GetYawAngle();
	
	if(Angle > CurrentAngle)
	{
		TurnAngle = Angle + CurrentAngle;
		UpdateMotorState(MOTOR_TURN_LEFT);
		SetMotorDutyRatio(LPWM,RPWM);
		
		while(fabs(TurnAngle - Gyro_GetYawAngle()) > 15)
		{
		}
		UpdateMotorState(MOTOR_STOP);
		SetMotorDutyRatio(0,0);
		
		
	}
	else if(Angle < CurrentAngle)
	{
		
		TurnAngle = Angle + CurrentAngle;
		
		if(TurnAngle > 360)
		{
			TurnAngle -= 360;
			
			UpdateMotorState(MOTOR_TURN_LEFT);
			SetMotorDutyRatio(LPWM,RPWM);
			
			while(Gyro_GetYawAngle() < 357)
			{
			}
			SysTickDelay(70);
			
			while(fabs(TurnAngle - Gyro_GetYawAngle()) > 15)
			{
			}
			UpdateMotorState(MOTOR_STOP);
			SetMotorDutyRatio(0,0);
			
		}
		else
		{
			UpdateMotorState(MOTOR_TURN_LEFT);
			SetMotorDutyRatio(LPWM,RPWM);
			
			while(fabs(TurnAngle - Gyro_GetYawAngle()) > 15)
			{
			}
			UpdateMotorState(MOTOR_STOP);
			SetMotorDutyRatio(0,0);			
			
		}
	}
	
	SysTickDelay(MOTOR_TurnLeft_WaitTime);
		
}



/*
* 函数名称：Motor_TurnRightBlockingMode()
* 函数作用：控制车辆右转一定的角度
* 函数输入：要右转的角度值 取值范围为 10-270 超出这个范围可能有魔法
* 函数输出：无
* 注意事项：该函数时阻塞函数 在执行右转的时候单片机做不了其他事(只有中断服务能运行)
* 魔法相关：该函数可能需要调整的值已经放到magic.h内
*/
void Motor_TurnRightBlockingMode(float Angle)
{
  float CurrentAngle = 0;
  float TurnAngle = 0;
	
	SysTickDelay(MOTOR_TurnRight_StableTime);

  CurrentAngle = Gyro_GetYawAngle();

  if(Angle > CurrentAngle)
    {
      TurnAngle = (float)360.0 - (float)(Angle - CurrentAngle);
      UpdateMotorState(MOTOR_TURN_RIGHT);
      SetMotorDutyRatio(MOTOR_TurnRight_LeftPWM,MOTOR_TurnRight_RightPWM);

      while(Gyro_GetYawAngle() > (float)2)
        {
        }
      SysTickDelay(150);

      while(fabs(Gyro_GetYawAngle() - TurnAngle) > 20)
        {
        }
    }
  else if(Angle <= CurrentAngle)
    {
      TurnAngle = CurrentAngle - Angle;
      UpdateMotorState(MOTOR_TURN_RIGHT);
      SetMotorDutyRatio(MOTOR_TurnRight_LeftPWM,MOTOR_TurnRight_RightPWM);
      while(fabs(Gyro_GetYawAngle() - TurnAngle) > 20)
        {
        }
    }


  UpdateMotorState(MOTOR_STOP);
  SetMotorDutyRatio(0,0);
	
  SysTickDelay(MOTOR_TurnRight_WaitTime);
		
	
	//微调
  while(fabs((double)(TurnAngle - Gyro_GetYawAngle())) > 2)
    {
      if(TurnAngle - Gyro_GetYawAngle() > (float)2)
        {
          UpdateMotorState(MOTOR_TURN_LEFT);
          SetMotorDutyRatio(MOTOR_TurnRight_LeftPWM_ADJ,MOTOR_TurnRight_RightPWM_ADJ);
        }
      else if(Gyro_GetYawAngle() - TurnAngle > (float)2)
        {
          UpdateMotorState(MOTOR_TURN_RIGHT);
          SetMotorDutyRatio(MOTOR_TurnRight_LeftPWM_ADJ,MOTOR_TurnRight_RightPWM_ADJ);
        }
    }

  UpdateMotorState(MOTOR_STOP);
  SetMotorDutyRatio(0,0);

}



/*
* 函数名称：Motor_TurnToAbsoluteYawAngle()
* 函数作用：控制车辆转到一陀螺仪绝对角度 左右转由哪边进决定
* 函数输入：要转到的绝对陀螺仪角度值
* 函数输出：无
* 注意事项：该函数时阻塞函数 在执行右转的时候单片机做不了其他事(只有中断服务能运行)
* 注意事项：我们陀螺仪采用的是无磁场轴的算法 因此稳定性会随时间降低
* 魔法相关：该函数可能需要调整的值已经放到magic.h内
* 
*/
void Motor_TurnToAbsoluteYawAngle(float aimAngle)
{
  float currentAngle = Gyro_GetYawAngle(),
        distanceAngle = currentAngle - aimAngle,
        distanceAngle2;

  if(distanceAngle > 0)
    {
      distanceAngle2 = aimAngle + 360 - currentAngle;
      if((distanceAngle - distanceAngle2 ) < 0)
        {
          UpdateMotorState(MOTOR_TURN_RIGHT);
        }
      else
        {
          UpdateMotorState(MOTOR_TURN_LEFT);
        }
    }
  else
    {
      distanceAngle2 = aimAngle - 360 - currentAngle;
      if((distanceAngle - distanceAngle2 ) < 0)
        {
          UpdateMotorState(MOTOR_TURN_RIGHT);
        }
      else
        {
          UpdateMotorState(MOTOR_TURN_LEFT);
        }
    }

  SetMotorDutyRatio(0.18, 0.18);

  while(fabs(Gyro_GetYawAngle() - aimAngle) > 20.0)
    {
    }

  SysTickDelay(150);


  while(fabs(Gyro_GetYawAngle() - aimAngle) > 2.0)
    {
      if(aimAngle - Gyro_GetYawAngle() > (float)2.0)
        {
          UpdateMotorState(MOTOR_TURN_LEFT);
          SetMotorDutyRatio(0.16,0.16);
        }
      else if(Gyro_GetYawAngle() - aimAngle > (float)2.0)
        {
          UpdateMotorState(MOTOR_TURN_RIGHT);
          SetMotorDutyRatio(0.16,0.16);
        }

    }


  SetMotorDutyRatio(0,0);
  UpdateMotorState(MOTOR_STOP);

}


/*
* 函数名称：Motor_TurnRightNoInFlatGround()
* 函数作用：控制车辆在非平面上原地右转一定的角度
* 函数输入：要右转的角度值 取值范围为 10-270 超出这个范围可能有魔法
* 函数输出：无
* 注意事项：该函数是阻塞函数 在执行右转的时候单片机做不了其他事(只有中断服务能运行)
* 魔法相关：该函数可能需要调整的值还没有放到magic.h内 当然该函数也可能不用调整
*/
void Motor_TurnRightNoInFlatGround(float Angle)
{
  float CurrentAngle = 0;
  float TurnAngle = 0;
  SysTickDelay(200);

  CurrentAngle = Gyro_GetYawAngle();
  if(CurrentAngle-Angle<0)
    TurnAngle=CurrentAngle-Angle+360;
  else
    TurnAngle=CurrentAngle-Angle;
  while(Gyro_GetYawAngle()>=TurnAngle+(float)2.5||Gyro_GetYawAngle()<=TurnAngle-(float)2.5)
    {
      GPIO_SetBits(GPIOG,GPIO_Pin_5);
			#ifdef Robot_2
      SetMotorDutyRatio(0.2,0);
			#endif
			
			#ifdef Robot_1
      SetMotorDutyRatio(0.4,0);
			#endif
    }
  SetMotorDutyRatio(0,0);
  UpdateMotorState(MOTOR_STOP);
}


/*
* 函数名称：Motor_TurnLeftNoInFlatGround()
* 函数作用：控制车辆在非平面上原地左转一定的角度
* 函数输入：要右转的角度值 取值范围为 10-270 超出这个范围可能有魔法
* 函数输出：无
* 注意事项：该函数是阻塞函数 在执行右转的时候单片机做不了其他事(只有中断服务能运行)
* 魔法相关：该函数可能需要调整的值还没有放到magic.h内 当然该函数也可能不用调整
*/
void Motor_TurnLeftNoInFlatGround(float Angle)
{
  float CurrentAngle = 0;
  float TurnAngle = 0;
  SysTickDelay(200);

  CurrentAngle = Gyro_GetYawAngle();
  if(CurrentAngle+Angle>360)
    TurnAngle=CurrentAngle+Angle-360;
  else
    TurnAngle=CurrentAngle+Angle;
  while(Gyro_GetYawAngle()>=TurnAngle+(float)2.5||Gyro_GetYawAngle()<=TurnAngle-(float)2.5)
    {
      GPIO_SetBits(GPIOG,GPIO_Pin_2);
			#ifdef Robot_2
      SetMotorDutyRatio(0.0,0.2);
			#endif
			
			#ifdef Robot_1
      SetMotorDutyRatio(0.0,0.4);
			#endif

    }
  SetMotorDutyRatio(0,0);
  UpdateMotorState(MOTOR_STOP);
}


/*
* 函数名称：Motor_TurnRightDelayBlockingMode()
* 函数作用：以延时方式控制车辆右转一定的角度

* 函数输入：要右转的时间 以ms决定
* 函数输入：电机右转时候的PWM值 此值将控制车辆转弯后的相对位置

* 函数输出：无
* 注意事项：该函数是阻塞函数 在执行右转的时候单片机做不了其他事(只有中断服务能运行)
* 魔法相关：该函数自己就是魔法 既可以 也可以不配合magic.h使用
*/
void Motor_TurnRightDelayBlockingMode(uint16_t ms, float LPWM, float RPWM)
{
	//等待车辆稳定的延时 看情况决定延时大小与有无
	UpdateMotorState(MOTOR_STOP);
	SysTickDelay(100);
	
	//以当前的PWM和转弯时间执行转弯
	UpdateMotorState(MOTOR_TURN_RIGHT);
	SetMotorDutyRatio(LPWM,RPWM);
	SysTickDelay(ms);
	
	UpdateMotorState(MOTOR_STOP);
	SetMotorDutyRatio(0,0);
}


/*
* 函数名称：Motor_TurnLeftDelayBlockingMode()
* 函数作用：以延时方式控制车辆左转一定的角度

* 函数输入：要左转转的时间 以ms决定
* 函数输入：电机左转时候的PWM值 此值将控制车辆转弯后的相对位置

* 函数输出：无
* 注意事项：该函数是阻塞函数 在执行左转的时候单片机做不了其他事(只有中断服务能运行)
* 魔法相关：该函数自己就是魔法 既可以 也可以不配合magic.h使用
*/
void Motor_TurnLeftDelayBlockingMode(uint16_t ms, float LPWM, float RPWM)
{
	//等待车辆稳定的延时 看情况决定延时大小与有无
	UpdateMotorState(MOTOR_STOP);
	SysTickDelay(100);
	
	//以当前的PWM和转弯时间执行转弯
	UpdateMotorState(MOTOR_TURN_LEFT);
	SetMotorDutyRatio(LPWM,RPWM);
	SysTickDelay(ms);
	
	UpdateMotorState(MOTOR_STOP);
	SetMotorDutyRatio(0,0);
}




/*
* 函数名称：Motor_TurnLeftDelayBlockingMode()
* 函数作用：以灰度误差为参考将车辆朝向挪到灰度中间
* 函数输入：无
* 函数输出：无
* 注意事项：该函数是阻塞函数 在执行左转的时候单片机做不了其他事(只有中断服务能运行)
*/
void Motor_TurnGoMidByGray(void)
{
	float Error = _GetADCError() * 1000;
	
	
	
	while(fabs(Error) > (float)0.1)
	{
		Error = _GetADCError() * 1000;
		if(Error > (float)0.1)
		{
			UpdateMotorState(MOTOR_TURN_RIGHT);
			SetMotorDutyRatio(0.1,0.1);
			SysTickDelay(10);
		}
		else if(Error < (float)-0.1)
		{
			UpdateMotorState(MOTOR_TURN_LEFT);
			SetMotorDutyRatio(0.1,0.1);
			SysTickDelay(10);
		}
				
	}
	SetMotorDutyRatio(0,0);
	UpdateMotorState(MOTOR_STOP);
	
}

