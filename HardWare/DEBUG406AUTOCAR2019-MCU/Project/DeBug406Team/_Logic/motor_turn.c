#include "motor_turn.h"


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
			SetMotorDutyRatio(MOTOR_TurnLeft_LeftPWM,MOTOR_TurnLeft_RightPWM);
			
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
			SetMotorDutyRatio(MOTOR_TurnLeft_LeftPWM,MOTOR_TurnLeft_RightPWM);
			
			while(fabs(TurnAngle - Gyro_GetYawAngle()) > 15)
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
		SetMotorDutyRatio(MOTOR_TurnLeft_LeftPWM,MOTOR_TurnLeft_RightPWM);
		
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
			SetMotorDutyRatio(MOTOR_TurnLeft_LeftPWM,MOTOR_TurnLeft_RightPWM);
			
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
			SetMotorDutyRatio(MOTOR_TurnLeft_LeftPWM,MOTOR_TurnLeft_RightPWM);
			
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

      while(fabs(Gyro_GetYawAngle() - TurnAngle) > 15)
        {
        }
    }
  else if(Angle <= CurrentAngle)
    {
      TurnAngle = CurrentAngle - Angle;
      UpdateMotorState(MOTOR_TURN_RIGHT);
      SetMotorDutyRatio(MOTOR_TurnRight_LeftPWM,MOTOR_TurnRight_RightPWM);
      while(fabs(Gyro_GetYawAngle() - TurnAngle) > 15)
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

/* 测试函数 */

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

  SetMotorDutyRatio(0.085, 0.1);

  while(fabs(Gyro_GetYawAngle() - aimAngle) > 2.0)
    {
    }

  SysTickDelay(150);


  while(fabs(Gyro_GetYawAngle() - aimAngle) > 2.0)
    {
      if(aimAngle - Gyro_GetYawAngle() > (float)2.0)
        {
          UpdateMotorState(MOTOR_TURN_LEFT);
          SetMotorDutyRatio(0.05,0.05);
        }
      else if(Gyro_GetYawAngle() - aimAngle > (float)2.0)
        {
          UpdateMotorState(MOTOR_TURN_RIGHT);
          SetMotorDutyRatio(0.05,0.05);
        }

    }


  SetMotorDutyRatio(0,0);
  UpdateMotorState(MOTOR_STOP);

}
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
      SetMotorDutyRatio(0.2,0);
    }
  SetMotorDutyRatio(0,0);
  UpdateMotorState(MOTOR_STOP);
}
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
      SetMotorDutyRatio(0,0.2);
    }
  SetMotorDutyRatio(0,0);
  UpdateMotorState(MOTOR_STOP);
}
