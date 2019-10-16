#include "SKILL.h"
#include "magic.h"
#define TEST2


/* 使用Magic.h就把下一行解注释 */
//#define Use_Magic


int FlagUp = 0;
int FlagDown = 0;
int _FlagBack = 0;
int FlagPoint = 0;
int TurnFlag = 0;
int FlagStop = 0;
int FlagBalance = 0;
int FlagBridge = 0;
int FlagDistant = 0;
int Flagstart = 0;
extern int8_t LeftCountTimes;
extern int8_t RightCountTimes;

extern volatile uint8_t _Is_PID_DownBridge_Activated;

void _resetlaser(void)
{
  if (_UpHillOrDownHillFeedBack() != FlatGround || _IsBalanceFeedBack() != BALANCE)
    {
      ClearLLaserChangePendingBit();
      ClearRLaserChangePendingBit();
    }
}


void InitFlag()
{
  FlagUp = 0;
  FlagDown = 0;
  _FlagBack = 0;
  FlagPoint = 0;
  TurnFlag = 0;
  FlagStop = 0;
  FlagBalance = 0;
  FlagBridge = 0;
  FlagDistant = 0;
}


#ifdef Use_Magic
#ifndef Robot_3
#define Magic
#endif
#endif


/*
* 函数名称：OneTOTwo()
* 函数作用：控制车辆从一台走到二台 惯性导航版本
* 函数输入：无
* 函数输出：无
* 注意事项：该函数运行速度由系统节拍决定 目前系统节拍是10ms
* 魔法相关：该函数可能需要调整的值还没有放到magic.h内 当然该函数也可能不用调整
*/

/* 5.14号 要把下桥判断改成纯距离的判断 */
#ifdef INERTIALNAVIGATION //使用惯性导航
void OneTOTwo(void)
{
  if (_FlagBack == 0)
    {
      if (FlagDown == 0)
        {
          _GoLineLowSpeed();
          if (_UpHillOrDownHillFeedBack() == DOWN)
            {
              FlagDown = 1;
            }
        }
      else
        {
          if(_UpHillOrDownHillFeedBack()==DOWN)
            {
              _GoLineLowSpeed();
            }
          else
            {
              if (FlagUp == 0)
                {

                  _FindPointGo();
                  if(FlagPoint==2)
                    {
                      _GoLineLowSpeed();
                    }
                  else
                    {
                      _GoLine();
                    }
                  if (_UpHillOrDownHillFeedBack() == UP)
                    {
                      UpdateMotorState(MOTOR_STOP);
                      BeginCount();

                      /* NFYMCQgYukXJtPmr */
                      /* OneToTwo_GyroStableTime */
#ifndef Magic
                      SysTickDelay(500);
#else
                      SysTickDelay(OneToTwo_GyroStableTime);
#endif

                      _InitNowYawAngle();
                      FlagUp = 1;
                      UpdateMotorState(MOTOR_FRONT);
                    }
                }
              else if (FlagUp == 1)
                {
                  //1到2 上桥后距离屏蔽 eEYIjRWy5DlDoIvl
#ifndef Magic
                  while (Encoder_GetCurrentDistantCount() <= (float)2)
#else
                  while (Encoder_GetCurrentDistantCount() <= OneToTwo_OnBridgeDistant)
#endif
                    {
                      _GoBridge();
                    }
                  UpdateMotorState(MOTOR_FRONT);
                  FlagBridge = 0;
                  _FlagBack = 1;
                  FlagUp = 0;
                  FlagPoint=0;
                }
            }
        }
    }
  else
    {
      if(FlagPoint==0||FlagUp==1)
        {
          _GoLine();
        }
      else if (FlagPoint==2&&FlagUp==0)
        {
          _GoLineLowSpeed();
        }
      if (_UpHillOrDownHillFeedBack() == UP)
        {
          FlagUp = 1;
        }
      if (FlagUp == 0)
        {
          _FindPointGo();
        }
      else
        {
          _ArrivePlatform();
          if (UpgradeMotorState() == MOTOR_STOP)
            {
              Stop = 1;
            }
        }
    }
}
#endif


/*
* 函数名称：OneTOTwo()
* 函数作用：控制车辆从一台走到二台 桥上灰度巡线版本
* 函数输入：无
* 函数输出：无
* 注意事项：该函数运行速度由系统节拍决定 目前系统节拍是10ms
* 魔法相关：该函数可能需要调整的值还没有放到magic.h内 当然该函数也可能不用调整
*/
#ifndef INERTIALNAVIGATION //不使用惯性导航
void OneTOTwo(void)
{
  if (_FlagBack == 0)
    {
      if (FlagDown == 0)
        {
          _GoLineLowSpeed();
          if (_UpHillOrDownHillFeedBack() == DOWN)
            {
              FlagDown = 1;
            }
        }
      else
        {
          if(_UpHillOrDownHillFeedBack()==DOWN)
            {
              _GoLineLowSpeed();
            }
          else
            {
              if (FlagUp == 0)
                {
                  //1到2 过0点延迟巡线
                  //需要确认这几句的作用 PSoVUrROsjoqsYpD
                  _GoLineDelay(500);
                  _GoLine();
                  _FindPointGo();
                  if (_UpHillOrDownHillFeedBack() == UP)
                    {
                      UpdateMotorState(MOTOR_STOP);
                      BeginCount();

                      //_InitNowYawAngle();  灰度版本不需要初始化Yaw角
                      FlagUp = 1;
                      UpdateMotorState(MOTOR_FRONT);
                    }
                }
              else if (FlagUp == 1)
                {
                  Crycle=0;
                  //1到2 上桥后距离屏蔽 al4oxNYUmsKd23qX
#ifndef Magic
                  while (Encoder_GetCurrentDistantCount() <= (float)1.375)
#else
                  while (Encoder_GetCurrentDistantCount() <= OneToTwo_GraySolution_OnBridgeDistant)
#endif
                    {
                      //刚开始上桥那段时间应该用正常函数 否则飞出去
#ifndef Magic
                      if(Crycle<=400)
#else
                      if(Crycle<=OneToTwo_GraySolution_UpBridgeGolineTime)
#endif
                        {
													#ifdef Robot_1
                          _GoLine();
													#endif
													
													#ifdef Robot_2
													_GoLine_UserAdjust(0.21,0.21);
													#endif
                        }
                      else
                        {
                          _GoBridgeByButtom();
                        }

                    }										
                  UpdateMotorState(MOTOR_FRONT);
                  FlagBridge = 0;
                  _FlagBack = 1;
                  FlagUp = 0;
                  FlagPoint=0;
                }
            }
        }
    }
  else
    {
      if(FlagPoint==0||FlagUp==1)
        {
          _GoLine();
        }
      else if (FlagPoint==2&&FlagUp==0)
        {
          _GoLineLowSpeed();
        }
      if (_UpHillOrDownHillFeedBack() == UP)
        {
          FlagUp = 1;
        }
      if (FlagUp == 0)
        {
          _FindPointGo();
        }
      else
        {
          _ArrivePlatform();
          if (UpgradeMotorState() == MOTOR_STOP)
            {
              Stop = 1;
            }
        }
    }
}

#endif

/*
* 函数名称：TwoTOThree()
* 函数作用：控制车辆从二台走到三台
* 函数输入：无
* 函数输出：无
* 注意事项：该函数运行速度由系统节拍决定 目前系统节拍是10ms
* 魔法相关：该函数可能需要调整的值还没有放到magic.h内 当然该函数也可能不用调整
*/
void TwoTOThree(void)
{

  /* rG2wkjTY1yylRc5z */

  if (UpgradeMotorState() == MOTOR_STOP && FlagStop != 2)
    {
      UpdateMotorState(MOTOR_FRONT);
      FlagStop++;
      /* TwoTOThree_PointCoverSpeed */
#ifndef Magic
      SetMotorDutyRatio(0.1, 0.1);
#else
      SetMotorDutyRatio(TwoTOThree_PointCoverSpeed, TwoTOThree_PointCoverSpeed);
#endif

      /*TwoTOThree_PointCoverTime */
#ifndef Magic
      SysTickDelay(300); //开一定的时间来屏蔽多余的点
#else
      SysTickDelay(TwoTOThree_PointCoverTime);
#endif
      //FlagStop++;
    }
  else if (UpgradeMotorState() == MOTOR_STOP && FlagStop == 2)
    {
      Stop = 1;
    }
  if (FlagStop == 1)
    {
      if (FlagDown == 1)
        {
          _GoLineLowSpeed();
          _FindPointStop();
          if (UpgradeMotorState() == MOTOR_STOP)
            {
              FlagPoint = 1;
            }
          if (FlagPoint == 1)
            {

              UpdateMotorState(MOTOR_FRONT);

              //过了障碍以后到B点要往后退一些 lGcWKjU4fM0l8XG9 2到3
              /* TwoTOThree_B_PointBackSpeed */
#ifndef Magic
              SetMotorDutyRatio(0.18, 0.18);//退后速度
#else
              SetMotorDutyRatio(TwoTOThree_B_PointBackSpeed,TwoTOThree_B_PointBackSpeed);//退后速度
#endif

              ClearLLaserChangePendingBit();
              while (UpgradeMotorState() == MOTOR_FRONT)
                {
                  if (IsLLaserChange() == Changed)
                    {
                      UpdateMotorState(MOTOR_BACK);
                      /* TwoTOThree_B_PointBackTime */
#ifndef Magic
                      SysTickDelay(310);//退后时间
#else
                      SysTickDelay(TwoTOThree_B_PointBackTime);//退后时间
#endif
                      UpdateMotorState(MOTOR_STOP);
                      /* TwoTOThree_B_PointTurnAngle */
#ifndef Magic
                      Motor_TurnRightBlockingMode(146);//转向3台的角度
#else
                      Motor_TurnRightBlockingMode(TwoTOThree_B_PointTurnAngle);//转向3台的角度
#endif
                      FlagBalance=0;
                      FlagDown = 0;
                    }
                }
            }
        }
      else
        {

          /* oS7ogBfylETMiD18 2到3 */
          /* TwoTOThree_PointAtoB_GolineDelayTime */
#ifndef Magic
          _GoLineDelay(3500);//通过障碍物延迟
#else
          _GoLineDelay(TwoTOThree_PointAtoB_GolineDelayTime);//通过障碍物延迟
#endif
          UpdateMotorState(MOTOR_FRONT);
          ClearLLaserChangePendingBit();
          FlagDown = 1;
        }
    }
  else if (FlagStop == 2)
    {
      if(FlagBalance==0)
        {
          if(FlagBalance==0)
            {
              Crycle=0;
              while(Crycle<1500)
                {
                  _GoLineLowSpeed();
                }
            }//完成在B点的转角 需要低速巡线修正
          FlagBalance=1;
        }
      _GoLine();
      _FindPointGo(); //刚才好像删了，但是现在不知道是不是他的原因
      if (_UpHillOrDownHillFeedBack() == UP)
        {
          FlagUp = 1;
        }
      if (FlagUp == 1)
        {
          _ArrivePlatform();
        }
    }
  else if (FlagStop == 0)
    {
      if (_FlagBack == 0)
        {
          _FlagBack = 1;
        }
      else if (_FlagBack == 1)
        {
          _GoLineLowSpeed();
          if (_UpHillOrDownHillFeedBack() == DOWN)
            {
              _GoLineLowSpeed();
              FlagDown = 1;
              _FlagBack=2;
            }
        }
      if (FlagDown == 1)
        {
          _GoLineLowSpeed();
          //2到3  A点转弯相关 osNlUTo9kCsr9Ldm
          if (_UpHillOrDownHillFeedBack() == FlatGround)
            {
              _FindPointStop();
              if (UpgradeMotorState() == MOTOR_STOP)
                {
#ifndef Magic
                  SetMotorDutyRatio(0.14,0.14);
#else
                  SetMotorDutyRatio(TwoTOThree_A_PointBackSpeed,TwoTOThree_A_PointBackSpeed);
#endif
                  UpdateMotorState(MOTOR_FRONT);
#ifndef Magic
                  SysTickDelay(50);
#else
                  SysTickDelay(TwoTOThree_A_PointBackTime);
#endif
                  UpdateMotorState(MOTOR_STOP);
#ifndef Magic
                  Motor_TurnRightBlockingMode(37);
#else
                  Motor_TurnRightBlockingMode(TwoTOThree_A_PointTurnAngle);
#endif
                  FlagDown = 0;
                }
            }
          else if (_UpHillOrDownHillFeedBack() == DOWN)
            {
              _GoLineLowSpeed();
            }
        }
    }
}




/*
* 函数名称：TwoTOFour()
* 函数作用：控制车辆从二台走到四台
* 函数输入：无
* 函数输出：无
* 注意事项：该函数运行速度由系统节拍决定 目前系统节拍是10ms
* 魔法相关：该函数可能需要调整的值还没有放到magic.h内 当然该函数也可能不用调整
*/
void TwoTOFour()
{
  if (UpgradeMotorState() == MOTOR_STOP && FlagStop != 2)
    {
      //点停转弯后屏蔽相同点往前开的速度以及时间 1Xe4BV3ApFnHYPXt
      UpdateMotorState(MOTOR_FRONT);
      FlagStop++;
#ifndef Magic
      SetMotorDutyRatio(0.1, 0.1);
      SysTickDelay(500);
#else
      SetMotorDutyRatio(TwoTOFour_PointCoverSpeed, TwoTOFour_PointCoverSpeed);
      SysTickDelay(TwoTOFour_PointCoverTime);
#endif
      //FlagStop++;
    }
  else if (UpgradeMotorState() == MOTOR_STOP && FlagStop == 2)
    {
      Stop = 1;
    }
  if (FlagStop == 1)
    {
      if (FlagDown == 1)
        {
          _GoLine();
          _FindPointStop();
          if (UpgradeMotorState() == MOTOR_STOP)
            {
              FlagPoint = 1;
            }
          if (FlagPoint == 1)
            {
              //B点动作 2到4 LN997ancgNnSZiNA
              UpdateMotorState(MOTOR_FRONT);
#ifndef Magic
              SetMotorDutyRatio(0.15, 0.15);
#else
              SetMotorDutyRatio(TwoTOFour_B_PointBackSpeed, TwoTOFour_B_PointBackSpeed);
#endif
              ClearLLaserChangePendingBit();
              while (UpgradeMotorState() == MOTOR_FRONT)
                {
                  if (IsLLaserChange() == Changed)
                    {
                      UpdateMotorState(MOTOR_BACK);
#ifndef Magic
                      SysTickDelay(300);
#else
                      SysTickDelay(TwoTOFour_B_PointBackTime);
#endif
                      UpdateMotorState(MOTOR_STOP);
#ifndef Magic
                      Motor_TurnLeftBlockingMode(33);
#else
                      Motor_TurnLeftBlockingMode(TwoTOFour_B_PointTurnAngle);
#endif
                      FlagDown = 0;
                    }
                }
            }
        }
      else
        {
          /* 1jF57tb2cnPVO5UJ 2到4 过障碍时间 */
#ifndef Magic
          _GoLineDelay(3500);//过障碍
#else
          _GoLineDelay(TwoTOFour_PointAtoB_GolineDelayTime);//过障碍
#endif
          UpdateMotorState(MOTOR_FRONT);
          ClearLLaserChangePendingBit();
          FlagDown = 1;
        }
    }
  else if (FlagStop == 2)
    {
      if(FlagBalance==0)
        {


          if(FlagBalance==0)
            {
              Crycle=0;
              while(Crycle<1500)
                {
                  _GoLineLowSpeed();
                }
            }//完成在B点的转角 需要低速巡线修正
          FlagBalance=1;
        }
      _GoLine();
      _FindPointGo();
      if (_UpHillOrDownHillFeedBack() == UP)
        {
          FlagUp = 1;
        }
      if (FlagUp == 1)
        {
          _ArrivePlatform();
        }
    }
  else if (FlagStop == 0)
    {
      if (_FlagBack == 0)
        {
//          Motor_TurnRightBlockingMode(180);
//          UpdateMotorState(MOTOR_FRONT);
          _FlagBack = 1;
        }
      else if (_FlagBack == 1)
        {
          _GoLineLowSpeed();
          if (_UpHillOrDownHillFeedBack() == DOWN)
            {
              FlagDown = 1;
            }
        }
      if (FlagDown == 1)
        {
          if (_UpHillOrDownHillFeedBack() == FlatGround)
            {
              _GoLineLowSpeed();
              _FindPointStop();
              if (UpgradeMotorState() == MOTOR_STOP)
                {
                  /* Ebabpx9IvphwcCYo */
                  //A点 2到4
#ifndef Magic
                  SetMotorDutyRatio(0.14,0.14);
#else
                  SetMotorDutyRatio(TwoTOFour_A_PointBackSpeed,TwoTOFour_A_PointBackSpeed);
#endif
                  
                  
                  UpdateMotorState(MOTOR_FRONT);
#ifndef Magic
                  SysTickDelay(100);
#else
                  SysTickDelay(TwoTOFour_A_PointBackTime);
#endif
                  UpdateMotorState(MOTOR_STOP);
#ifndef Magic
                  Motor_TurnRightBlockingMode(37);
#else
                  Motor_TurnRightBlockingMode(TwoTOFour_A_PointTurnAngle);
#endif
                  FlagDown = 0;
                }
            }
          else if (_UpHillOrDownHillFeedBack() == DOWN)
            {
              _GoLineLowSpeed();
            }
        }
    }
}



/*
* 函数名称：FourTOFive()
* 函数作用：控制车辆从四台走到五台
* 函数输入：无
* 函数输出：无
* 注意事项：该函数运行速度由系统节拍决定 目前系统节拍是10ms
* 魔法相关：该函数可能需要调整的值还没有放到magic.h内 当然该函数也可能不用调整
*/
void FourTOFive(void)
{
  const AdcData_t *adcData;
  adcData = UpdateButtom();
  uint32_t _Temp = 0;
  for (int i = 0; i <= 6; i++)
    {
      _Temp += adcData->array[0][i];
    }
  if (UpgradeMotorState() == MOTOR_STOP && FlagStop != 2)
    {
      //每次点停转弯以后开一定时间屏蔽重复点 4到5
      /* A8gFEeGJO9W46kmZ */
      UpdateMotorState(MOTOR_FRONT);
      FlagStop++;
#ifndef Magic
      SetMotorDutyRatio(0.1, 0.1);
      SysTickDelay(300);
#else
      SetMotorDutyRatio(FourTOFive_PointCoverSpeed, FourTOFive_PointCoverSpeed);
      SysTickDelay(FourTOFive_PointCoverTime);
#endif
    }
  else if (UpgradeMotorState() == MOTOR_STOP && FlagStop == 2)
    {
      Stop = 1;
    }
  if (_FlagBack == 1)
    {
      if (FlagStop == 0)
        {
          if (FlagDown == 0)
            {
              _GoLineLowSpeed();
              if (_UpHillOrDownHillFeedBack() == DOWN)
                {
                  FlagBalance = 1;
                }
              if (FlagBalance == 1)
                {
                  if (_UpHillOrDownHillFeedBack() == FlatGround)
                    {
                      FlagDown = 1;
											FlagBalance=0;
                    }
										else
										{
											_GoLineLowSpeed();
										}
                }
            }
          else if (FlagDown == 1)
            {
              if (TurnFlag == 0)
                {
                  if(FlagBalance==0)
                    {
                      Crycle=0;
                      while(Crycle<=1500)
                        {
                          _GoLineLowSpeed();
                        }
                      FlagBalance=1;
                    }
                  _GoLine();
                  _FindPointStop();
                  if (FlagPoint == 1)
                    {
                      /* 4到5 D点转弯相关  8TYvIapnyyipa3Mz */
                      UpdateMotorState(MOTOR_FRONT);
#ifndef Magic
                      SetMotorDutyRatio(0.18, 0.18);
#else
                      SetMotorDutyRatio(FourTOFive_D_PointBackSpeed, FourTOFive_D_PointBackSpeed);
#endif
                      ClearLLaserChangePendingBit();
                      while (UpgradeMotorState() == MOTOR_FRONT)
                        {
                          _GoLine();
                          if (IsLLaserChange() == Changed)
                            {
                              UpdateMotorState(MOTOR_BACK);
#ifndef Magic
                              SysTickDelay(200);
#else
                              SysTickDelay(FourTOFive_D_PointBackTime);
#endif

                              UpdateMotorState(MOTOR_STOP);
#ifndef Magic
                              Motor_TurnLeftBlockingMode(94);
#else
                              Motor_TurnLeftBlockingMode(FourTOFive_D_PointTurnAngle);
#endif
                              FlagDown = 0;
                            }
                        }
                    }
                  //此处需要测试
                }
            }
        }
      else if (FlagStop == 1)
        {
          // printf("1");
          _GoLine();
          if (_UpHillOrDownHillFeedBack() == UP)
            {
              _GoLineDelay(1500);
              FlagUp = 1;
              UpdateMotorState(MOTOR_FRONT);
            }
          if (FlagUp == 1)
            {
              _GoLine();
              if (_Temp <= 10000)
                {
                  UpdateMotorState(MOTOR_STOP);
                  FlagUp = 0;
                  TurnFlag = 1;
                }
            }
          if (TurnFlag == 1)
            {
              /* 4到5 G点转弯相关 OKEwq2AMELMr0XUD */
              ClearLLaserChangePendingBit();
              ClearRLaserChangePendingBit();
              UpdateMotorState(MOTOR_FRONT);
							FlagBalance=0;
#ifndef Magic
              SetMotorDutyRatio(0.14, 0.14);
#else
              SetMotorDutyRatio(FourTOFive_G_PointBackSpeed, FourTOFive_G_PointBackSpeed);
#endif
              ClearLLaserChangePendingBit();
              while (UpgradeMotorState() == MOTOR_FRONT)
                {
                  if (IsLLaserChange() == Changed)
                    {
                      UpdateMotorState(MOTOR_BACK);
#ifndef Magic
                      SysTickDelay(250);
#else
                      SysTickDelay(FourTOFive_G_PointBackTime);
#endif
                      UpdateMotorState(MOTOR_STOP);
#ifndef Magic
                      Motor_TurnLeftBlockingMode(90);
#else
                      Motor_TurnLeftBlockingMode(FourTOFive_G_PointTurnAngle);
#endif
                      FlagDown = 0;
                    }
                }
            }
        }
      else if (FlagStop == 2)
        {
					if(FlagBalance==0)
					{
						_GoLineDelay(500);
					}
          _GoLine();
          if (_UpHillOrDownHillFeedBack() == UP)
            {
              FlagUp = 1;
						}
          while(FlagUp == 1)
            {
							if(UpgradeMotorState() == MOTOR_STOP)
							{
								FlagUp = 0;
							}
              _ArrivePlatform();
            }
        }
    }
  else if (_FlagBack == 0)
    {
      //      Motor_TurnLeftBlockingMode(180);
      //      UpdateMotorState(MOTOR_FRONT);
      _FlagBack = 1;
    }
}


/*
* 函数名称：ThreeTOFive()
* 函数作用：控制车辆从三台走到五台
* 函数输入：无
* 函数输出：无
* 注意事项：该函数运行速度由系统节拍决定 目前系统节拍是10ms
* 魔法相关：该函数可能需要调整的值还没有放到magic.h内 当然该函数也可能不用调整
*/
void ThreeTOFive(void)
{
  const AdcData_t *adcData;
  adcData = UpdateButtom();
  uint32_t _Temp = 0;
  for (int i = 0; i <= 7; i++)
    {
      _Temp += adcData->array[0][i];
    }
  if (UpgradeMotorState() == MOTOR_STOP && FlagStop != 2)
    {
      //每次过点转弯以后走一定时间屏蔽相同点
      /* 3到5 IxU2oCKayK8p5D6z */
      UpdateMotorState(MOTOR_FRONT);
      FlagStop++;
#ifndef Magic
      SetMotorDutyRatio(0.1, 0.1);
      SysTickDelay(300);
#else
      SetMotorDutyRatio(ThreeTOFive_PointCoverSpeed, ThreeTOFive_PointCoverSpeed);
      SysTickDelay(ThreeTOFive_PointCoverTime);
#endif
      FlagPoint = 0;
    }
  else if (UpgradeMotorState() == MOTOR_STOP && FlagStop == 2)
    {
      Stop = 1;
    }
  if (_FlagBack == 1)
    {
      if (FlagStop == 0)
        {
          if (FlagDown == 0)
            {
              _GoLineLowSpeed();
              if (_UpHillOrDownHillFeedBack() == DOWN)
                {
                  FlagBalance = 1;
                }
              if (FlagBalance == 1)
                {
                  if (_UpHillOrDownHillFeedBack() == FlatGround)
                    {
                      FlagDown = 1;
											FlagBalance=0;
                    }
                }
            }
          else if (FlagDown == 1)
            {
              // _GoLine();
              if (TurnFlag == 0)
                {
                  if(FlagBalance==0)
                    {
                      Crycle=0;
                      while(Crycle<=1500)
                        {
                          _GoLineLowSpeed();
                        }
                      FlagBalance=1;
                    }
                  _GoLine();
                  _FindPointStop();
                  if(UpgradeMotorState()==MOTOR_STOP)
                    {
                      TurnFlag=1;
                      UpdateMotorState(MOTOR_FRONT);
                    }
                  //同4到5需测试
                }
              else
                {

                  /* 3到5 C点转弯相关 PRF9Cbzp8Sy82CQk */
                  UpdateMotorState(MOTOR_FRONT);
                  SetMotorDutyRatio(0.1, 0.1);
                  if(GetRLaserStateAntiJitter()==LOW)
                    {
                      UpdateMotorState(MOTOR_BACK);
#ifndef Magic
                      SetMotorDutyRatio(0.1, 0.1);
                      SysTickDelay(200);
#else
                      SetMotorDutyRatio(ThreeTOFive_C_PointBackSpeed, ThreeTOFive_C_PointBackSpeed);
                      SysTickDelay(ThreeTOFive_C_PointBackTime);
#endif
                      UpdateMotorState(MOTOR_STOP);
#ifndef Magic
                      Motor_TurnRightBlockingMode(40);
#else
                      Motor_TurnRightBlockingMode(ThreeTOFive_C_PointTurnAngle);
#endif
                      TurnFlag=0;
                      BeginCount();
                    }

                }
            }
        }
      else if (FlagStop == 1)
        {
          if (FlagDistant == 0)
            {

              if (Encoder_GetCurrentDistantCount() <= (float)2)
                {
                  _GoLine();
                }
              else
                {
                  _GoLine();
                  _FindPointStop();
                  if (UpgradeMotorState() == MOTOR_STOP)
                    {
                      //printf("1");
                      UpdateMotorState(MOTOR_FRONT);
                      FlagDistant = 1;
                      StopCount();
                    }
                }
            }
          else
            {
              /* 3到5 G点转弯相关 e0lVNGajHjzbHyvY */
							SetMotorDutyRatio(0.14,0.14);
              UpdateMotorState(MOTOR_FRONT);
#ifndef Magic
              SysTickDelay(0);
#else
              SysTickDelay(ThreeTOFive_G_PointFrontTime);
#endif
              UpdateMotorState(MOTOR_STOP);
#ifndef Magic
              Motor_TurnLeftBlockingMode(45);
#else
              Motor_TurnLeftBlockingMode(ThreeTOFive_G_PointTurnAngle);
#endif
              BeginCount();
            }
        }
      else if (FlagStop == 2)
        {
          //printf("2");
          _GoLine();
          if (_UpHillOrDownHillFeedBack() == UP)
            {
              FlagUp = 1;
            }
          if (FlagUp == 1)
            {
              _ArrivePlatform();
            }
        }
    }
  else if (_FlagBack == 0)
    {
      //      Motor_TurnLeftBlockingMode(180);
      //      UpdateMotorState(MOTOR_FRONT);
      _FlagBack = 1;
    }
}


/*
* 函数名称：FiveTOSeven()
* 函数作用：控制车辆从五台走到七台
* 函数输入：无
* 函数输出：无
* 注意事项：该函数运行速度由系统节拍决定 目前系统节拍是10ms
* 魔法相关：该函数可能需要调整的值还没有放到magic.h内 当然该函数也可能不用调整
*/
void FiveTOSeven(void)
{
  const AdcData_t *adcData;
  adcData = UpdateButtom();
  uint32_t _Temp = 0;

  for (int i = 0; i <= 7; i++)
    {
      _Temp += adcData->array[0][i];
    }
  if (UpgradeMotorState() == MOTOR_STOP && FlagStop == 0)
    {
      //5到7 点停转弯屏蔽相同点直行速度 时间 A94xE4UEGtN3JV0v

      UpdateMotorState(MOTOR_FRONT);
      FlagStop++;
#ifndef Magic
      SetMotorDutyRatio(0.07, 0.07);
      SysTickDelay(500);
#else
      SetMotorDutyRatio(FiveTOSeven_PointCoverSpeed, FiveTOSeven_PointCoverSpeed);
      SysTickDelay(FiveTOSeven_PointCoverTime);
#endif
    }
  //5到7 上坡后转弯并不需要往前开一段距离屏蔽相同点
  else if(UpgradeMotorState() == MOTOR_STOP && FlagStop == 1)
    {
      SetMotorDutyRatio(0.00, 0.00);
      UpdateMotorState(MOTOR_FRONT);
      FlagStop++;
    }
  else if (UpgradeMotorState() == MOTOR_STOP && FlagStop == 2)
    {
      Stop = 1;
    }
  if (_FlagBack == 1)
    {
      if (FlagStop == 0)
        {
          if (FlagBalance == 0)
            {
              _GoLineLowSpeed();
              //printf("1");
              if (_UpHillOrDownHillFeedBack() == DOWN)
                {
                  //printf("2");
                  FlagDown = 1;
                }
              if (FlagDown == 1)
                {
                  if (_UpHillOrDownHillFeedBack() == FlatGround)
                    {
                      //printf("3");
                      FlagBalance = 1;
                      Encoder_ResetDistant();
                      Encoder_StartDistantCount();
                    }
                }
            }
          else
            {
              _GoLine();
              //5到7 从5台开到J点过程中距离屏蔽G点
              //2NHQPJvSayey56T9
#ifndef Magic
              if(Encoder_GetCurrentDistantCount() > (float)2.7)
#else
              if(Encoder_GetCurrentDistantCount() > (float)FiveTOSeven_5_G_DistantCover)
#endif
                {
                  FlagDistant = 1;
                  Encoder_StopDistantCount();
                  Encoder_ResetDistant();
                }
              if (FlagDistant == 1)
                {
                  _GoLine();
                  if (GetLLaserStateAntiJitter() == LOW)
                    {
                      // 5到7 J点转弯前后退调整 Wt7p07dgHaM70lSb
                      UpdateMotorState(MOTOR_BACK);
#ifndef Magic
                      SetMotorDutyRatio(0.15, 0.15);
                      SysTickDelay(150);
#else
                      SetMotorDutyRatio(FiveTOSeven_J_PointBackSpeed, FiveTOSeven_J_PointBackSpeed);
                      SysTickDelay(FiveTOSeven_J_PointBackTime);
#endif
                      UpdateMotorState(MOTOR_STOP);
#ifndef Magic
                      Motor_TurnLeftBlockingMode(90);
#else
                      Motor_TurnLeftBlockingMode(FiveTOSeven_J_PointTurnAngle);
#endif
                      BeginCount();
                    }
                }
            }
        }
      else if (FlagStop == 1)
        {
          _GoLineLowSpeed();
          // 5到7 J点到K点的距离屏蔽 M6BhHpTalxMjxt4V
#ifndef Magic
          if(Encoder_GetCurrentDistantCount()>=(float)1)
#else
          if(Encoder_GetCurrentDistantCount()>=FiveTOSeven_J_K_DistantCover)
#endif
            {
              if (_UpHillOrDownHillFeedBack() == UP)
                {
                  UpdateMotorState(MOTOR_STOP);
                  //5到7 上斜坡后车辆稳定时间
                  SysTickDelay(500);
                  BeginCount();
                  FlagUp = 1;
                }
              if (FlagUp == 1)

                {
                  //5到7 K1点 转弯前调整 往上开一段距离防止自己掉下来
                  //rP8iDAFMCIuVqy48
                  UpdateMotorState(MOTOR_FRONT);
#ifndef Magic
                  SetMotorDutyRatio(0.07,0.07);
                  while(Encoder_GetCurrentDistantCount()<=(float)0.04)
#else
                  SetMotorDutyRatio(FiveTOSeven_K1_PiontUp_Speed,FiveTOSeven_K1_PiontUp_Speed);
                  while(Encoder_GetCurrentDistantCount()<=FiveTOSeven_K1_PiontUp_Distant)
#endif
                    {

                    };
                  UpdateMotorState(MOTOR_STOP);
                  //5到7 K1点到L2点半锁定转弯角度
#ifndef Magic
                  Motor_TurnLeftNoInFlatGround(90);
#else
                  Motor_TurnLeftNoInFlatGround(FiveTOSeven_K1_PointHalfLockTurnAngle);
#endif
                  StopCount();
                  FlagDistant=0;
                }
            }
        }
      else if (FlagStop == 2)
        {
          if (FlagBridge == 0)
            {
              FlagBridge = 1;
            }
          else
            {
              if (FlagDistant == 0)
                {
                  //5到7  在坡上定点转弯后执行的动作 9YTn95VTOsmEJNIo
                  UpdateMotorState(MOTOR_STOP);
                  _InitNowYawAngle();
                  Crycle=0;
                  UpdateMotorState(MOTOR_FRONT);

#ifndef Magic     //过障碍到台前总时间
                  while(Crycle<=5000)
                    {
                      //前200ms惯性导航
                      if(Crycle<200)
#else
                  while(Crycle<=FiveTOSeven_K1_N2_PointTotalTime)
                    {
                      if(Crycle<FiveTOSeven_K1_N2_PointGyroTime)
#endif
                        {
                          _GoBridge();

                        }
#ifndef Magic
                      else if(Crycle>=200&&Crycle<3000)
#else
                      else if(Crycle>=FiveTOSeven_K1_N2_PointLowSpeedStartTime&&Crycle<FiveTOSeven_K1_N2_PointLowSpeedStopTime)
#endif
                        {
                          //低速前进 保证寻到线上
                          _GoLineLowSpeed();
                        }
#ifndef Magic
                      else if(Crycle>=3000&&Crycle<3700)
#else
                      else if(Crycle>=FiveTOSeven_K1_N2_PointInsameStartTime&&Crycle<FiveTOSeven_K1_N2_PointInsameStopTime)
#endif
                        {
                          //魔鬼速度前进 保证腾空过第一个障碍
                          _GoLineHighSpeed();
                        }
#ifndef Magic
                      else if(Crycle>=3700&&Crycle<5000)
#else
                      else if(Crycle>=FiveTOSeven_K1_N2_PointNormalSpeedStartTime&&Crycle<FiveTOSeven_K1_N2_PointNormalSpeedStoptTime)
#endif
                        {
                          //正常速度前进 过完障碍的同时防止寻出去
                          _GoLine();
                        }
                    }
                  FlagDistant = 1;
                  StopCount();

                }
              else
                {
                  //过完障碍上台 等上坡然后开超声波
                  _GoLine();
                  if (_UpHillOrDownHillFeedBack() == UP)
                    {
                      FlagUp = 1;
                    }
                  if (FlagUp == 1)
                    {
                      if (_UpHillOrDownHillFeedBack() == FlatGround)
                        {
                          _ArrivePlatform();
                        }
                    }
                }
            }
        }
    }
  else
    {
      //      Motor_TurnLeftBlockingMode(180);
      //      UpdateMotorState(MOTOR_FRONT);
      _FlagBack = 1;
    }
}

/*
* 函数名称：TurnBack()
* 函数作用：控制车辆在台上转头
* 函数输入：无
* 函数输出：无
* 注意事项：该函数运行速度由系统节拍决定 目前系统节拍是10ms
* 魔法相关：该函数可能需要调整的值还没有放到magic.h内 当然该函数也可能不用调整
*/
void TurnBack(void)
{
  Motor_TurnLeft180OnPlatform();
  if (UpgradeMotorState() == MOTOR_STOP)
    {
      Stop = 1;
    }
}



/*
* 函数名称：ThreeTOSix()
* 函数作用：控制车辆从三台走到六台
* 函数输入：无
* 函数输出：无
* 注意事项：该函数运行速度由系统节拍决定 目前系统节拍是10ms
* 魔法相关：该函数可能需要调整的值还没有放到magic.h内 当然该函数也可能不用调整
*/
void ThreeTOSix()
{
  const AdcData_t *adcData;
  adcData = UpdateButtom();
  uint32_t _Temp = 0;
  //计算灰度返回值之和
  for (int i = 0; i <= 7; i++)
    {
      _Temp += adcData->array[0][i];
    }
  //当由转弯造成的motor_stop到达下一轮询时使任务到达下一阶段
  if (UpgradeMotorState() == MOTOR_STOP && FlagStop != 4)
    {
      //屏蔽相同点 往前开一下 ttWvbxCzV2krZACv
      UpdateMotorState(MOTOR_FRONT);
      FlagStop++;
#ifndef Magic
      SetMotorDutyRatio(0.07, 0.07);
      SysTickDelay(350);
#else
      SetMotorDutyRatio(ThreeTOSix_PointCoverSpeed, ThreeTOSix_PointCoverSpeed);
      SysTickDelay(ThreeTOSix_PointCoverTime);
#endif
    }
  //此脚本执行完毕后将脚本stop置为1
  else if (UpgradeMotorState() == MOTOR_STOP && FlagStop == 4)
    {
      Stop = 1;
    }
  //当stop为0时，及从台上出发到第一个点停之前
  if (FlagStop == 0)
    {
      //之前_FlagBack用于转180°，现在已经无效
      if (_FlagBack == 1)
        {
          _GoLineLowSpeed();
          //当检测到出发台下坡时，将Flagdown置为1
          if (_UpHillOrDownHillFeedBack() == DOWN)
            {
              FlagDown = 1;
            }
          //当检测到下坡后，一直低速寻线知道陀螺仪认为已经到达平地
          if (FlagDown == 1)
            {
              //在陀螺仪检测到平地后，一直执行寻线和寻点
              if (_UpHillOrDownHillFeedBack() == FlatGround)
                {
                  if(FlagBalance==0)
                    {
                      Crycle=0;
                      while(Crycle<=1500)
                        {
                          _GoLineLowSpeed();
                        }
                      FlagBalance=1;
                    }
                  _GoLine();
                  _FindPointStop();
                  //当寻点函数将电机停下时，继续向前寻线，直到右侧激光传感器检测到白线
                  if (UpgradeMotorState() == MOTOR_STOP)
                    {
                      UpdateMotorState(MOTOR_FRONT);
                      while (GetRLaserStateAntiJitter() != LOW)
                        {

                          _GoLine();
                        }
                      //因为车辆有惯性，在激光找到线后需要向后做微调，保证车的几何中心与线的交叉点近乎重合
                      //3到6  C点转弯相关 ZMsAcwvH6qKcK9f0
                      UpdateMotorState(MOTOR_BACK);
#ifndef Magic
                      SetMotorDutyRatio(0.14, 0.14);
                      SysTickDelay(100);
#else
                      SetMotorDutyRatio(ThreeTOSix_C_PointBackSpeed, ThreeTOSix_C_PointBackSpeed);
                      SysTickDelay(ThreeTOSix_C_PointBackTime);
#endif
                      UpdateMotorState(MOTOR_STOP);
                      //找到后立刻停车，并转弯，并且将motor_stop状态传达至下一轮询
#ifndef Magic
                      Motor_TurnRightBlockingMode(85);
#else
                      Motor_TurnRightBlockingMode(ThreeTOSix_C_PointTurnAngle);
#endif
                      FlagBalance=0;
                    }
                }
              else
                {
                  _GoLineLowSpeed();
                }
            }
        }
      else
        {
          _GoLineLowSpeed();
          if (_UpHillOrDownHillFeedBack() == DOWN)
            {
              _FlagBack = 1;
            }
        }
    }
  //从第一个停点到第二个之间
  else if (FlagStop == 1)
    {
      //如果FlagDistant为0时即距离未到达2m时，只进行寻线。
      if (FlagDistant == 0)
        {
          Encoder_ResetDistant();
          Encoder_StartDistantCount();
          while (Encoder_GetCurrentDistantCount() <= (float)2)
            {
              _GoLine();
            }
          FlagDistant = 1;
          Encoder_StopDistantCount();
          Encoder_ResetDistant();
        }
      //当距离到达后，执行寻线以及一种临时的寻点方案，及底部灰度总和值小于10000时认为进入点，注意：这里使用这个方案一是边缘检测不适用，其次，这里的
      //实际到点总和值小于2000，与正常的32000有着极大的差距，故基本可忽略数字化的影响
      else
        {

          _GoLine();

          //F点找点停  临时点检测 Hh1wKW8dOMyAgvMa
#ifndef Magic
          if (_Temp <= 10000)
#else
          if (_Temp <= ThreeTOSix_F_PointDetectGraySum)
#endif
            {
              UpdateMotorState(MOTOR_STOP);
            }
          //临时点模式造成电机停状态时，执行继续向前寻线，并且向后微调，并且右转，并且将motor_stop状态带入下一轮询
          if (UpgradeMotorState() == MOTOR_STOP)
            {
              UpdateMotorState(MOTOR_FRONT);
              while (GetRLaserStateAntiJitter() != LOW)
                {
                  _GoLine();
                }
              //3到6 F点停转弯相关 y2FINQVL4sfx8rYq
              UpdateMotorState(MOTOR_STOP);
              UpdateMotorState(MOTOR_BACK);
#ifndef Magic
              SetMotorDutyRatio(0.1, 0.1);
              SysTickDelay(250);
#else
              SetMotorDutyRatio(ThreeTOSix_F_PointBackSpeed, ThreeTOSix_F_PointBackSpeed);
              SysTickDelay(ThreeTOSix_F_PointBackTime);
#endif
              UpdateMotorState(MOTOR_STOP);
#ifndef Magic
              Motor_TurnRightBlockingMode(90);
#else
              Motor_TurnRightBlockingMode(ThreeTOSix_F_PointTurnAngle);
#endif
            }
        }
    }
  //当从第二停点到第三停点时，不断执行寻线和寻点停函数
  else if (FlagStop == 2)
    {
      _GoLineLowSpeed();
      _FindPointStop();
      //点模式造成电机停状态时，执行继续向前寻线
      if (UpgradeMotorState() == MOTOR_STOP)
        {
          UpdateMotorState(MOTOR_FRONT);

          while (GetRLaserStateAntiJitter() != LOW)
            {
              _GoLine();
            }
          //3到6 H1点圆上玄学转弯 gGFhBKxUKnFykquW
          //当右侧激光传感器被触发时 前进一段距离 右转145°
						
					//FnVLyW1QWghfZRMN	
          UpdateMotorState(MOTOR_STOP);
					#ifndef Magic
          SetMotorDutyRatio(0.14,0.14);
          UpdateMotorState(MOTOR_FRONT);
          SysTickDelay(200);
					#else
          SetMotorDutyRatio(ThreeTOSix_H1_PointBackSpeed,ThreeTOSix_H1_PointBackSpeed);
          UpdateMotorState(MOTOR_FRONT);
          SysTickDelay(ThreeTOSix_H1_PointBeforeTurnGoTime);				
					#endif
						
          UpdateMotorState(MOTOR_STOP);
#ifndef Magic
          Motor_TurnRightBlockingMode(120);
#else
          Motor_TurnRightBlockingMode(ThreeTOSix_H1_PointTurnAngle);
#endif
          BeginCount();
          FlagDistant=0;
          FlagBridge=0;
        }
      else
        {
          UpdateMotorState(MOTOR_FRONT);
        }
    }
  //第三点停到第四点停时
  else if (FlagStop == 3)
    {
      if(FlagBridge==0)
        {

          //3到6 只对H1点生效的后退  MLp88MY89ZtaRsp7
          UpdateMotorState(MOTOR_BACK);
#ifndef Magic
          SetMotorDutyRatio(0.07, 0.07);
          SysTickDelay(350);
#else
          SetMotorDutyRatio(ThreeTOSix_H1_PointBackSpeed, ThreeTOSix_H1_PointBackSpeed);
          SysTickDelay(ThreeTOSix_H1_PointBackTime);
#endif
          UpdateMotorState(MOTOR_FRONT);
          FlagBridge=1;
        }
      _GoCricle();
      if(FlagDistant==0)
        {
          //H1到H3点1/4圆距离屏蔽 可能需要现场调整 EpJIv8k6ENnykNan

#ifndef Magic
          while(Encoder_GetCurrentDistantCount()<=(float)0.5)
#else
          while(Encoder_GetCurrentDistantCount()<=ThreeTOSix_H1_H3_DistantCover)
#endif
            {
              _GoCricle();
            }
          FlagDistant=1;
        }
      //当未检测上坡时，低速寻线
      if (FlagUp == 0)
        {
          _GoCricle();
          //当检测到上坡时，左转26矫正，保证上跷跷板时两前轮均能不悬空，并将flag置为1
          if (_UpHillOrDownHillFeedBack() == UP)
            {
              FlagUp = 1;
              FlagBalance=0;
              //H3点 上坡瞬间的切线与跷跷板实际方向不同 需纠正
              //3到6 H3点上跷跷板调整 BOYZVLk4YfmJ5olf
#ifndef Magic
              Motor_TurnLeftBlockingMode(25);
#else
              Motor_TurnLeftBlockingMode(ThreeTOSix_H3_PointTurnAngle);
#endif
              UpdateMotorState(MOTOR_FRONT);
            }
        }
      //当flagup为1时，实行轮速锁定，保证速度不会过低
      else if (FlagUp == 1)
        {
          _BlockSpeed();
          if(FlagBalance==0)
            {
              _BlockSpeed();
              Crycle=0;
              //在跷跷板上轮速锁走的距离 务必确保越过平衡位置
              //3到6 IYyvbgcEpFIllIWk
#ifndef Magic
              while(Crycle<3900)
#else
              while(Crycle<ThreeTOSix_OnSeesawDelayTime)
#endif
                {
                  _BlockSpeed();
                }
              FlagBalance=1;
              UpdateMotorState(MOTOR_STOP);
              SysTickDelay(500);
              UpdateMotorState(MOTOR_FRONT);
            }
          else
            {
              Crycle=0; //3到6 越过跷跷板中点后下跷跷板下桥PID巡线时间 要加
              //kXRGlmj3GMjfXGOH
#ifndef Maigc
              while(Crycle<=600)
#else
              while(Crycle<=ThreeTOSix_DownSeesawDownBridgeTime)
#endif
                {
                  _GoLineLowSpeedDownBridge();
                }
              if(_UpHillOrDownHillFeedBack()==FlatGround)
                {
                  FlagUp = 2;
                  FlagBalance=0;
                  //3到6 H4点下跷跷板以后需要调整的角度  6EUdPkXo1Qfth9rx
#ifndef Magic
                  Motor_TurnLeftBlockingMode(40);
#else
									UpdateMotorState(MOTOR_BACK);
									SetMotorDutyRatio(0.14, 0.14);
          SysTickDelay(100);
UpdateMotorState(MOTOR_FRONT);
                  Motor_TurnLeftBlockingMode(ThreeTOSix_H4_PointTurnAngle);
#endif

									
                  UpdateMotorState(MOTOR_FRONT);
                }
            }
          //一直保持转台直至回到平地。因为我们的陀螺仪因为防抖需要所以在有一部分还在跷跷板时就会认为在平地上，所以先需要向前行进一段距离做矫正
          //左转40°因为圆的线方向与上桥时相比已经有了变化。还需要将flagup置为2


        }

      //当flagup为2时执行低速寻线0.5，这里的执行距离是防止刚下跷跷板后的车处于非稳定状态误判入点
      else if (FlagUp == 2)
        {
          if (FlagBalance == 0)
            {
              //printf("%f\r\n",Encoder_GetCurrentDistantCount());
              _GoCricle();
              if (Encoder_GetCurrentDistantCount() >= (float)0.5)
                {
                  //printf("1\r\n");
                  //此处的入点方式为临时入点，原因为此处边缘仅为1，与点模式的通用描述不同
                  //I点到点检测灰度和 光照变化可能需要调整  UMZMT9OGErQStomk
#ifndef Magic
                  if (_Temp <= 20000)
#else
                  if (_Temp <= ThreeTOSix_I_PointDetectGraySum)
#endif
                    {
                      UpdateMotorState(MOTOR_STOP);
                      ClearLLaserChangePendingBit();
                    }
                }
              //由临时点模式造成的电机停有时，继续前进，直至左侧激光管检测到线，并将flagbalance置为1
              if (UpgradeMotorState() == MOTOR_STOP)
                {
                  //printf("2\r\n");
                  //需要改成防抖激光检测
                  Encoder_StopDistantCount();
                  Encoder_ResetDistant();
                  UpdateMotorState(MOTOR_FRONT);
                  while (IsLLaserChange() != Changed)
                    {
                      //printf("3\r\n");
                      _GoLineLowSpeed();
                    }
                  FlagBalance = 1;
                }
            }
          //如果flagbalance为1则左转90°，并且将flagup为0，将电机停带入下一轮询
          else
            {
              //3到6 I点上台转弯前退后调整 YysIWU1XhGBpnxB2
              UpdateMotorState(MOTOR_BACK);
#ifndef Magic
              SetMotorDutyRatio(0.18,0.18);
              SysTickDelay(175);
#else
              SetMotorDutyRatio(ThreeTOSix_I_PointBackSpeed,ThreeTOSix_I_PointBackSpeed);
              SysTickDelay(ThreeTOSix_I_PointBackTime);
#endif

              UpdateMotorState(MOTOR_STOP);
#ifndef Magic
              Motor_TurnLeftBlockingMode(85);
#else
              Motor_TurnLeftBlockingMode(ThreeTOSix_I_PointTurnAngle);
#endif
              FlagUp = 0;
            }
        }
    }
  //第四点到最后台
  else if (FlagStop == 4)
    {
      //当未检测到上坡时，执行寻线，检测到后将flagup置为1
      if (FlagUp == 0)
        {
          _GoLineLowSpeed();
          if (_UpHillOrDownHillFeedBack() == UP)
            {
              FlagUp = 1;
            }
        }
      //上坡后执行到台停止函数
      else
        {
          _GoLine();
          _ArrivePlatform();
        }
    }
}


/*
* 函数名称：FourTOSix()
* 函数作用：控制车辆从四台走到六台
* 函数输入：无
* 函数输出：无
* 注意事项：该函数运行速度由系统节拍决定 目前系统节拍是10ms
* 魔法相关：该函数可能需要调整的值还没有放到magic.h内 当然该函数也可能不用调整
*/
void FourTOSix()
{
  const AdcData_t *adcData;
  adcData = UpdateButtom();
  uint32_t _Temp = 0;
  for (int i = 0; i <= 7; i++)
    {
      _Temp += adcData->array[0][i];
    }
  if (UpgradeMotorState() == MOTOR_STOP && FlagStop != 4)
    {

      /* 屏蔽同一点 4到6 Pjp9eQs1l8618kJH */
      UpdateMotorState(MOTOR_FRONT);
      FlagStop++;
#ifndef Magic
      SetMotorDutyRatio(0.07, 0.07);
      SysTickDelay(350);
#else
      SetMotorDutyRatio(FourTOSix_PointCoverSpeed, FourTOSix_PointCoverSpeed);
      SysTickDelay(FourTOSix_PointCoverTime);
#endif
    }
  else if (UpgradeMotorState() == MOTOR_STOP && FlagStop == 4)
    {
      Stop = 1;
    }
  if (FlagStop == 0)
    {
      if (_FlagBack == 0)
        {
          _GoLineLowSpeed();
          if (FlagDown == 0)
            {

              if (_UpHillOrDownHillFeedBack() == DOWN)
                {
                  FlagDown = 1;
                }
            }
          else
            {
              if (_UpHillOrDownHillFeedBack() == FlatGround)
                {
                  _FlagBack = 1;
                }
            }
        }
      else
        {
          if(FlagBalance==0)
            {
              Crycle=0;
              while(Crycle<=1500)
                {
                  _GoLineLowSpeed();
                }
              FlagBalance=1;
            }
          _GoLine();
          _FindPointStop();
          if (UpgradeMotorState() == MOTOR_STOP)
            {
              UpdateMotorState(MOTOR_FRONT);
              while (IsLLaserChange() == UnChange)
                {
                  _GoLine();
                }

              //4到6 D点转弯动作 p9ZvVRZ1nL2REfKG
              UpdateMotorState(MOTOR_BACK);
#ifndef Magic
              SetMotorDutyRatio(0.14,0.14);
              SysTickDelay(0);
#else
              SetMotorDutyRatio(FourTOSix_D_PointBackSpeed,FourTOSix_D_PointBackSpeed);
              SysTickDelay(FourTOSix_D_PointBackTime);
#endif
              UpdateMotorState(MOTOR_STOP);
#ifndef Magic
              Motor_TurnLeftBlockingMode(40);
#else
              Motor_TurnLeftBlockingMode(FourTOSix_D_PointTurnAngle);
#endif
              BeginCount();
            }
        }
    }
  else if (FlagStop == 1)
    {
      if (FlagDistant == 0)
        {
          //距离屏蔽3号门
          if (Encoder_GetCurrentDistantCount() <= (float)2.5)
            {
              _GoLine();
            }
          else
            {
              _GoLine();
              _FindPointStop(); //F点找点停
              if (UpgradeMotorState() == MOTOR_STOP)
                {
                  //printf("1");
                  UpdateMotorState(MOTOR_FRONT);
                  FlagDistant = 1;
                  StopCount();
                }
            }
        }
      else
        {
          // 4到6 F点转弯动作 MgW1qp1zrjTQjVtN
					UpdateMotorState(MOTOR_STOP);
					SetMotorDutyRatio(0.14,0.14);
          UpdateMotorState(MOTOR_FRONT);
#ifndef Magic
          SysTickDelay(150);
#else
          SysTickDelay(FourTOSix_F_PointFrontTime);
#endif
          UpdateMotorState(MOTOR_STOP);
#ifndef Magic
          Motor_TurnRightBlockingMode(40);
#else
          Motor_TurnRightBlockingMode(FourTOSix_F_PointTurnAngle);
#endif
          BeginCount();
        }
    }
  else if (FlagStop == 2)
    {
      _GoLineLowSpeed();
      _FindPointStop();
      //点模式造成电机停状态时，执行继续向前寻线
      if (UpgradeMotorState() == MOTOR_STOP)
        {
          UpdateMotorState(MOTOR_FRONT);

          while (GetRLaserStateAntiJitter() != LOW)
            {
              _GoLine();
            }
          //当右侧激光传感器被触发时 前进一定时间速度 右转145°
          
					//4到6 H1点转弯前单独前进速度 时间 Lt6SeMwok8WmSZEw
					UpdateMotorState(MOTOR_STOP);
					#ifndef Magic
          SetMotorDutyRatio(0.14,0.14);
          UpdateMotorState(MOTOR_FRONT);
          SysTickDelay(200);
					#else
          SetMotorDutyRatio(FourTOSix_H1_PointBeforeTurnGoSpeed,FourTOSix_H1_PointBeforeTurnGoSpeed);
          UpdateMotorState(MOTOR_FRONT);
          SysTickDelay(FourTOSix_H1_PointBeforeTurnGoTime);					
					#endif
          UpdateMotorState(MOTOR_STOP);
					//4到6 H1点转弯 Xp4o4PxUggn8kfWn
#ifndef Magic
          Motor_TurnRightBlockingMode(120);
#else
          Motor_TurnRightBlockingMode(FourTOSix_H1_PointTurnAngle);
#endif
          FlagDistant=0;
          FlagBalance=0;
          BeginCount();
        }
      else
        {
          UpdateMotorState(MOTOR_FRONT);
        }
    }
  else if (FlagStop == 3)
    {
      if(FlagBridge==0)
        {
          //4到6 H1点单独后退一段距离修正屏蔽点寻出去的问题 IzZmLTHP95lhuhGY
          UpdateMotorState(MOTOR_BACK);
#ifndef Magic
          SetMotorDutyRatio(0.07, 0.07);
          SysTickDelay(350);
#else
          SetMotorDutyRatio(FourTOSix_H1_PointBackSpeed, FourTOSix_H1_PointBackSpeed);
          SysTickDelay(FourTOSix_H1_PointBackTime);
#endif
          UpdateMotorState(MOTOR_FRONT);
          FlagBridge=1;
        }
      _GoCricle();
      if(FlagDistant==0)
        {
          //4到6 屏蔽H1到H3点的1/4圆弧 9J0IPkCrRRcaVrng

#ifndef Magic
          while(Encoder_GetCurrentDistantCount()<=(float)0.5)
#else
          while(Encoder_GetCurrentDistantCount()<= FourTOSix_H1_H3_DistantCover)
#endif
            {
              _GoCricle();

            }
          FlagDistant=1;
        }
      //当未检测上坡时，低速寻线
      if (FlagUp == 0)
        {
          _GoCricle();
          //当检测到上坡时，左转26矫正，保证上跷跷板时两前轮均能不悬空，并将flag置为1
          if (_UpHillOrDownHillFeedBack() == UP)
            {
              FlagUp = 1;
              //4到6 H3点上跷跷板对正跷跷板转弯调整  qoh2fNPtd8JknFX4
#ifndef Magic
              Motor_TurnLeftBlockingMode(10);
#else
              Motor_TurnLeftBlockingMode(FourTOSix_H3_PointTurnAngle);
#endif
              UpdateMotorState(MOTOR_FRONT);
            }
        }
      //当flagup为1时，实行轮速锁定，保证速度不会过低
      else if (FlagUp == 1)
        {
          if(FlagBalance==0)
            {
              _BlockSpeed();
              Crycle=0;
#ifndef Magic
              while(Crycle<3900)  //4到6 上跷跷板轮速锁定时间  qF8jkqxf7Qm0yAie
#else
              while(Crycle<FourTOSix_OnSeesawDelayTime)
#endif
                {
                  _BlockSpeed();
                }
              FlagBalance=1;
              UpdateMotorState(MOTOR_STOP);
              SysTickDelay(500);
              UpdateMotorState(MOTOR_FRONT);
            }
          else
            {
              Crycle=0;
              //4到6 下跷跷板时间 tHNWddLXqeeEPm2e
#ifndef Magic
              while(Crycle<=900)
#else
              while(Crycle<=FourTOSix_DownSeesawDownBridgeTime)
#endif
                {
                  _GoLineLowSpeedDownBridge();
                }
              if(_UpHillOrDownHillFeedBack()==FlatGround)
                {
                  FlagUp = 2;
                  FlagBalance=0;
                  //4到6 下跷跷板后H4点转弯调整 p0KIeBItkUzrl5Y2
#ifndef Magic
                  Motor_TurnLeftBlockingMode(40);
#else
                  Motor_TurnLeftBlockingMode(FourTOSix_H4_PointTurnAngle);
#endif
                  UpdateMotorState(MOTOR_FRONT);
                }
            }
        }

      //当flagup为2时执行低速寻线0.5，这里的执行距离是防止刚下跷跷板后的车处于非稳定状态误判入点
      else if (FlagUp == 2)
        {
          if (FlagBalance == 0)
            {
              //printf("%f\r\n",Encoder_GetCurrentDistantCount());
              _GoCricle();
              if (Encoder_GetCurrentDistantCount() >= (float)0.5)
                {
                  //printf("1\r\n");
                  //此处的入点方式为临时入点，原因为此处边缘仅为1，与点模式的通用描述不同
                  //4到6 I点临时点检测算法 kKO5DeZMVCLcFA4U
#ifndef Magic
                  if (_Temp <= 20000)
#else
                  if (_Temp <= FourTOSix_I_PointDetectGraySum)
#endif
                    {
                      UpdateMotorState(MOTOR_STOP);
                      ClearLLaserChangePendingBit();
                    }
                }
              //由临时点模式造成的电机停有时，继续前进，直至左侧激光管检测到线，并将flagbalance置为1
              if (UpgradeMotorState() == MOTOR_STOP)
                {
                  //printf("2\r\n");
                  //改成防抖激光
                  Encoder_StopDistantCount();
                  Encoder_ResetDistant();
                  UpdateMotorState(MOTOR_FRONT);
                  while (IsLLaserChange() != Changed)
                    {
                      //printf("3\r\n");
                      _GoLineLowSpeed();
                    }
                  FlagBalance = 1;
                }
            }
          //如果flagbalance为1则左转90°，并且将flagup为0，将电机停带入下一轮询
          else
            {
              //4到6 I点停转弯上台 qNr40qZ62LSkf5Gs
              UpdateMotorState(MOTOR_BACK);
#ifndef Magic
              SetMotorDutyRatio(0.14,0.14);
              SysTickDelay(200);
#else
              SetMotorDutyRatio(FourTOSix_I_PointBackSpeed,FourTOSix_I_PointBackSpeed);
              SysTickDelay(FourTOSix_I_PointBackTime);
#endif
              UpdateMotorState(MOTOR_STOP);
#ifndef Magic
              Motor_TurnLeftBlockingMode(90);
#else
              Motor_TurnLeftBlockingMode(FourTOSix_I_PointTurnAngle);
#endif
              FlagUp = 0;
            }
        }

    }
  else if (FlagStop == 4)
    {
      if (FlagUp == 0)
        {
          _GoLineLowSpeed();
          if (_UpHillOrDownHillFeedBack() == UP)
            {
              FlagUp = 1;
            }
        }
      else
        {
          _GoLine();
          _ArrivePlatform();
        }
    }
}



/*
* 函数名称：FiveTOEight()
* 函数作用：控制车辆从五台走到八台
* 函数输入：无
* 函数输出：无
* 注意事项：该函数运行速度由系统节拍决定 目前系统节拍是10ms
* 魔法相关：该函数可能需要调整的值还没有放到magic.h内 当然该函数也可能不用调整
*/
void FiveTOEight(void)
{
  const AdcData_t *adcData;
  adcData = UpdateButtom();
  uint32_t _Temp = 0;

  for (int i = 0; i <= 7; i++)
    {
      _Temp += adcData->array[0][i];
    }
  if (UpgradeMotorState() == MOTOR_STOP && FlagStop == 0)
    {
      //5到8 屏蔽相同点 Icth53GNxEDlaipP
      UpdateMotorState(MOTOR_FRONT);
      FlagStop++;
#ifndef Magic
      SetMotorDutyRatio(0.07, 0.07);
      SysTickDelay(500);
#else
      SetMotorDutyRatio(FiveTOEight_PointCoverSpeed, FiveTOEight_PointCoverSpeed);
      SysTickDelay(FiveTOEight_PointCoverTime);
#endif
    }

  //T台上点停 应该不需要调整
  else if(UpgradeMotorState() == MOTOR_STOP && FlagStop == 1)
    {
      UpdateMotorState(MOTOR_FRONT);
      FlagStop++;
      SetMotorDutyRatio(0.0, 0.0);
    }
  else if (UpgradeMotorState() == MOTOR_STOP && FlagStop == 2)
    {
      Stop = 1;
    }
  if (_FlagBack == 1)
    {
      if (FlagStop == 0)
        {
          if (FlagBalance == 0)
            {
              _GoLineLowSpeed();
              //printf("1");
              if (_UpHillOrDownHillFeedBack() == DOWN)
                {
                  //printf("2");
                  FlagDown = 1;
                }
              if (FlagDown == 1)
                {
                  if (_UpHillOrDownHillFeedBack() == FlatGround)
                    {
                      //printf("3");
                      FlagBalance = 1;
                      Encoder_ResetDistant();
                      Encoder_StartDistantCount();
                    }
                }
            }
          else
            {
              _GoLine();
              //5到8 距离屏蔽G点 7BPA6eRugWZXB5TG
#ifndef Magic
              if (Encoder_GetCurrentDistantCount() > (float)2.7)
#else
              if (Encoder_GetCurrentDistantCount() > FiveTOEight_5_G_DistantCover)
#endif
                {
                  FlagDistant = 1;
                  TurnFlag=0;
                  Encoder_StopDistantCount();
                  Encoder_ResetDistant();
                }
              if (FlagDistant == 1)
                {
                  _GoLine();
                  //5到8 J点临时点检测算法阈值 LvZ9m40cag2urfKA
#ifndef Magic
                  if(_Temp<=20000)
#else
                  if(_Temp<=FourTOSix_J_PointDetectGraySum)
#endif
                    {
                      TurnFlag=1;
                      UpdateMotorState(MOTOR_STOP);
                      //while(1);
                    }
                  if(TurnFlag==1)
                    {
                      UpdateMotorState(MOTOR_FRONT);
                      if (GetLLaserStateAntiJitter() == LOW)
                        {
                          //5到8 J点点停转弯 1ppOxBugIh7xd7K0
                          UpdateMotorState(MOTOR_STOP);
                          //while(1);
                          UpdateMotorState(MOTOR_BACK);
#ifndef Magic
                          SetMotorDutyRatio(0.1,0.1);
                          SysTickDelay(200);
#else
                          SetMotorDutyRatio(FiveTOEight_J_PointBackSpeed,FiveTOEight_J_PointBackSpeed);
                          SysTickDelay(FiveTOEight_J_PointBackTime);
#endif
                          UpdateMotorState(MOTOR_STOP);
#ifndef Magic
                          Motor_TurnLeftBlockingMode(90);
#else
                          Motor_TurnLeftBlockingMode(FiveTOEight_J_PointTurnAngle);
#endif
                          BeginCount();
                        }
                    }
                }
            }
        }
      else if (FlagStop == 1)
        {
          _GoLineLowSpeed();
          //5到8 1m保护防止认为自己已经上坡  8kDsAhFv72AOhfvK
#ifndef Magic
          if(Encoder_GetCurrentDistantCount()>=(float)1)
#else
          if(Encoder_GetCurrentDistantCount()>=FiveTOEight_J_K_DistantCover)
#endif
            {
              if (_UpHillOrDownHillFeedBack() == UP)
                {
                  UpdateMotorState(MOTOR_STOP);
                  SysTickDelay(500);
                  FlagUp = 1;
                  BeginCount();
                }
            }
          if (FlagUp == 1)

            {
              // 5到8 K1上斜坡以后往上开一段距离 OER0kJlKC3AdrONB

              UpdateMotorState(MOTOR_FRONT);
#ifndef Magic
              SetMotorDutyRatio(0.07, 0.07);
              while(Encoder_GetCurrentDistantCount()<=(float)0.05);
#else
              SetMotorDutyRatio(FiveTOEight_K1_PiontUp_Speed, FiveTOEight_K1_PiontUp_Speed);
              while(Encoder_GetCurrentDistantCount()<=FiveTOEight_K1_PiontUp_Distant);
#endif
              UpdateMotorState(MOTOR_STOP);
              //5到8 K1点到L2转弯角度
#ifndef Magic
              Motor_TurnRightNoInFlatGround(85);
#else
              Motor_TurnRightNoInFlatGround(FiveTOEight_K1_PointHalfLockTurnAngle);
#endif
              FlagBridge=0;
              FlagDistant=0;
            }
        }
      else if (FlagStop == 2)
        {
          if (FlagBridge == 0)
            {
              FlagBridge = 1;
            }
          else
            {
              if (FlagDistant == 0)
                {
                  UpdateMotorState(MOTOR_STOP);
                  _InitNowYawAngle();
                  Crycle=0;
                  UpdateMotorState(MOTOR_FRONT);
                  //5到8 转向L1点后 上长坡前运行时间相关 oH85arhctcvIWnah
#ifndef Magic
                  while(Crycle<=2500)
                    {
                      if(Crycle<200)
#else
                  while(Crycle<=FiveTOEight_K1_8_PointToltalTime)
                    {
                      if(Crycle<FiveTOEight_K1_8_PointBridgeTime)
#endif
                        {
                          _GoBridge();

                        }
#ifndef Magic
                      else if(Crycle>=200&&Crycle<2500)
#else
                      else if(Crycle>=FiveTOEight_K1_8_PointLowSpeedStartTime&&Crycle<FiveTOEight_K1_8_PointLowSpeedStopTime)
#endif
                        {
                          _GoLineLowSpeed();
                        }
                    }
                  FlagDistant = 1;
                  StopCount();
									FlagBalance=0;
                }
              else
                {
									if(FlagBalance==0)
									{
										Crycle=0;
										//5到8 屏蔽一段桥面延时 fw3Ijj3Uf1CF6dwu
										#ifndef Magic
										while(Crycle<=1000)
										#else
										while(Crycle<=FiveTOEight_BridgeUpTime)
										#endif
										{
											_GoLine();
										}
										FlagBalance=1;
									}
                  _GoLine();
                  if (_UpHillOrDownHillFeedBack() == UP)
                    {
                      FlagUp = 1;
                    }
                  if (FlagUp == 1)
                    {
                      if (_UpHillOrDownHillFeedBack() == FlatGround)
                        {
                          _ArrivePlatform();
                        }
                    }
                }
            }
        }
    }
  else
    {
      _FlagBack = 1;
    }
}



/*
* 函数名称：SixTOSeven()
* 函数作用：控制车辆从六台走到七台
* 函数输入：无
* 函数输出：无
* 注意事项：该函数运行速度由系统节拍决定 目前系统节拍是10ms
* 魔法相关：该函数可能需要调整的值还没有放到magic.h内 当然该函数也可能不用调整
*/
void SixTOSeven(void)
{
  const AdcData_t *adcData;
  adcData = UpdateButtom();
  uint32_t _Temp = 0;
  for (int i = 0; i <= 7; i++)
    {
      _Temp += adcData->array[0][i];
    }
  if (UpgradeMotorState() == MOTOR_STOP && FlagStop != 3&&FlagStop!=2)
    {
      //6到7 开一段距离屏蔽相同点  YicJdmzfGpeHYoTV
      UpdateMotorState(MOTOR_FRONT);
      FlagStop++;
#ifndef Magic
      SetMotorDutyRatio(0.07, 0.07);
      SysTickDelay(500);
#else
      SetMotorDutyRatio(SixTOSeven_PointCoverSpeed, SixTOSeven_PointCoverSpeed);
      SysTickDelay(SixTOSeven_PointCoverTime);
#endif
    }
  else if(UpgradeMotorState() == MOTOR_STOP && FlagStop == 2)
    {
      SetMotorDutyRatio(0.0, 0.0);
      UpdateMotorState(MOTOR_FRONT);
      FlagStop++;
      SetMotorDutyRatio(0.0, 0.0);
    }
  else if (UpgradeMotorState() == MOTOR_STOP && FlagStop == 3)
    {
      Stop = 1;
    }
  if (FlagStop == 0)
    {
      if (_FlagBack == 0)
        {
          _GoLineLowSpeed();
          if (_UpHillOrDownHillFeedBack() == DOWN)
            {
              _FlagBack = 1;
            }
        }
      else
        {
          if (_UpHillOrDownHillFeedBack() != FlatGround)
            {
              _GoLineLowSpeed();
            }
          else
            {
              _GoLineLowSpeed();
              //6到7 I点临时点停算法 ZtcHAYg7aQ0gYoOw
#ifndef Magic
              if(_Temp<=10000)
#else
              if(_Temp<=SixTOSeven_I_PointDetectGraySum)
#endif
                {
                  UpdateMotorState(MOTOR_STOP);
                }
              if (UpgradeMotorState() == MOTOR_STOP)
                {
                  //printf("1");
                  UpdateMotorState(MOTOR_FRONT);
                  while (GetLLaserStateAntiJitter() != LOW)
                    {
                      _GoLine();
                    }
                  //6到7 I点转角 nXjsbfFhdTSckpAx
                  UpdateMotorState(MOTOR_BACK);
#ifndef Magic
                  SetMotorDutyRatio(0.14, 0.14);
                  SysTickDelay(200);
#else
                  SetMotorDutyRatio(SixTOSeven_I_PointBackSpeed, SixTOSeven_I_PointBackSpeed);
                  SysTickDelay(SixTOSeven_I_PointBackTime);
#endif
                  UpdateMotorState(MOTOR_STOP);
#ifndef Magic
                  Motor_TurnLeftBlockingMode(95);
#else
                  Motor_TurnLeftBlockingMode(SixTOSeven_I_PointTurnAngle);
#endif
                  UpdateMotorState(MOTOR_STOP);
                  BeginCount();
                  FlagPoint = 0;
                }
            }
        }
    }
  else if (FlagStop == 1)
    {
      if (FlagDistant == 0)
        {
          //6到7 I点出去半圆距离 m08NntLbZ1X01Oxe
#ifndef Magic
          while (Encoder_GetCurrentDistantCount() <= (float)3.2)
#else
          while (Encoder_GetCurrentDistantCount() <= SixTOSeven_I_PointOutCircleDistant)
#endif
            {
              //6到7 到波浪板应该用正常巡线的距离 W1nxYty6wpCWi0O4
#ifndef Magic
              if(Encoder_GetCurrentDistantCount() >= (float)1.15&&Encoder_GetCurrentDistantCount() <= (float)1.5)
#else
              if(Encoder_GetCurrentDistantCount() >= SixTOSeven_BarricadeDistantDownLimit&&Encoder_GetCurrentDistantCount() <= SixTOSeven_BarricadeDistantUpLimit)
#endif
                {
                  /* 6到7 波浪板前玄学调整角度 */
#ifndef Magic
                  Motor_TurnLeftBlockingMode(10);
#else
                  Motor_TurnLeftBlockingMode(10);
#endif
                  UpdateMotorState(MOTOR_FRONT);
                  Crycle=0;
                  //6到7 波浪板延迟巡线时间 RUWU0UgaxWsdbA9M
#ifndef Magic
                  while(Crycle<=1000)
#else
                  while(Crycle<=SixTOSeven_BarricadeGolineTime)
#endif
                    {
                      _GoLine();
                    }
										Motor_TurnLeftBlockingMode(20);
										UpdateMotorState(MOTOR_FRONT);
                }
              else
                {
                  _GoCricle();
                }
            }
          FlagBalance=0;
          StopCount();
          BeginCount();
          //6到7 剩余的出圈距离
#ifndef Magic
          while (Encoder_GetCurrentDistantCount() <= (float)1.8)
#else
          while (Encoder_GetCurrentDistantCount() <= (float)SixTOSeven_AfterBarricadeOutCircleDistant)
#endif
            {
              _GoCricle();
              if (FlagPoint == 0)
                {
                  _FindPointGo();
                  if (FlagPoint == 2)
                    {
                      //6到7 出圈以后屏蔽H1点 PkOku0t3KBaGjme9

#ifndef Magic
                      Motor_TurnRightBlockingMode(10);
#else
                      Motor_TurnRightBlockingMode(SixTOSeven_OutCircle_H1_PiontCoverAngle);
#endif
                      UpdateMotorState(MOTOR_FRONT);
#ifndef Magic
                      SetMotorDutyRatio(0.1, 0.1);
                      SysTickDelay(200);
                      //认为是屏蔽F点
                      _GoLineDelay(500);
#else
                      SetMotorDutyRatio(SixTOSeven_OutCircle_H1_PiontCoverSpeed, SixTOSeven_OutCircle_H1_PiontCoverSpeed);
                      SysTickDelay(SixTOSeven_OutCircle_H1_PiontCoverTime);
                      //认为是屏蔽F点
                      _GoLineDelay(SixTOSeven_OutCircle_F_PiontCoverGolineTime);
#endif
                    }
                }
              //printf("2");
            }
          FlagDistant = 1;
        }
      else
        {
          _GoLine();
          if (GetRLaserStateAntiJitter() == LOW)
            {
              //printf("1");
              //6到7 找到J点后后退点停转弯 oiWGjxaYnanbmE2J
              UpdateMotorState(MOTOR_BACK);
#ifndef Magic
              SetMotorDutyRatio(0.1, 0.1);
              SysTickDelay(250);
#else
              SetMotorDutyRatio(SixTOSeven_J_PointBackSpeed, SixTOSeven_J_PointBackSpeed);
              SysTickDelay(SixTOSeven_J_PointBackTime);
#endif
              UpdateMotorState(MOTOR_STOP);
#ifndef Magic
              Motor_TurnRightBlockingMode(90);
#else
              Motor_TurnRightBlockingMode(SixTOSeven_J_PointTurnAngle);
#endif
              BeginCount();
            }
        }
    }
  else if (FlagStop == 2)
    {
      //6到7 从J点到K点的距离屏蔽 SwOylLiHqPI8ZDAR
      _GoLineLowSpeed();
#ifndef Magic
      if(Encoder_GetCurrentDistantCount()>=(float)1)
#else
      if(Encoder_GetCurrentDistantCount()>=SixTOSeven_J_K_DistantCover)
#endif
        {
          if (_UpHillOrDownHillFeedBack() == UP)
            {
              UpdateMotorState(MOTOR_STOP);
              //6到7 上斜坡后车辆稳定时间
              SysTickDelay(500);
              BeginCount();
              FlagUp = 1;
            }
          if (FlagUp == 1)
            {
              //6到7 K1点 转弯前调整 往上开一段距离防止自己掉下来
              //CKjXE5IJ3tl4yjby
#ifndef Magic
              SetMotorDutyRatio(0.07,0.07);
#else
              SetMotorDutyRatio(SixTOSeven_K1_PiontUp_Speed,SixTOSeven_K1_PiontUp_Speed);
#endif
              UpdateMotorState(MOTOR_FRONT);
#ifndef Magic
              while(Encoder_GetCurrentDistantCount()<=(float)0.1);
#else
              while(Encoder_GetCurrentDistantCount()<=SixTOSeven_K1_PiontUp_Distant);
#endif
              UpdateMotorState(MOTOR_STOP);
              //6到7 K1点到L2点半锁定转弯角度
#ifndef Magic
              Motor_TurnLeftNoInFlatGround(90);
#else
              Motor_TurnLeftNoInFlatGround(SixTOSeven_K1_PointHalfLockTurnAngle);
#endif
              StopCount();
              FlagDistant=0;
            }
        }
    }
  else if (FlagStop == 3)
    {
      if (FlagDistant == 0)
        {
          //6到7 过障碍延迟运行相关调整  JEReGrTupcAJz4gG
          UpdateMotorState(MOTOR_STOP);
          _InitNowYawAngle();
          Crycle=0;
          UpdateMotorState(MOTOR_FRONT);
#ifndef Magic
          while(Crycle<=5000)
            {
              if(Crycle<200)
#else
          while(Crycle<=SixTOSeven_K1_N2_PointTotalTime)
            {
              if(Crycle<SixTOSeven_K1_N2_PointGyroTime)
#endif
                {
                  _GoBridge();
                }
#ifndef Magic
              else if(Crycle>=200&&Crycle<3000)
#else
              else if(Crycle>=SixTOSeven_K1_N2_PointLowSpeedStartTime&&Crycle<SixTOSeven_K1_N2_PointLowSpeedStopTime)
#endif
                {
                  _GoLineLowSpeed();
                }
#ifndef Magic
              else if(Crycle>=3000&&Crycle<3500)
#else
              else if(Crycle>=SixTOSeven_K1_N2_PointInsameStartTime&&Crycle<SixTOSeven_K1_N2_PointInsameStopTime)
#endif
                {
                  _GoLineHighSpeed();
                }
#ifndef Magic
              else if(Crycle>=3500&&Crycle<5000)
#else
              else if(Crycle>=SixTOSeven_K1_N2_PointNormalSpeedStartTime&&Crycle<SixTOSeven_K1_N2_PointNormalSpeedStoptTime)
#endif
                {
                  _GoLine();
                }
            }
          FlagUp=0;
          FlagDistant = 1;
        }
      else
        {
          _GoLine();
          if (_UpHillOrDownHillFeedBack() == UP)
            {
              FlagUp = 1;
            }
          if (FlagUp == 1)
            {
              if (_UpHillOrDownHillFeedBack() == FlatGround)
                {
                  _ArrivePlatform();
                }
            }
        }
    }
}


/*
* 函数名称：SixTOEight()
* 函数作用：控制车辆从六台走到八台
* 函数输入：无
* 函数输出：无
* 注意事项：该函数运行速度由系统节拍决定 目前系统节拍是10ms
* 魔法相关：该函数可能需要调整的值还没有放到magic.h内 当然该函数也可能不用调整
*/
void SixTOEight(void)
{
  const AdcData_t *adcData;
  adcData = UpdateButtom();
  uint32_t _Temp = 0;
  for (int i = 0; i <= 7; i++)
    {
      _Temp += adcData->array[0][i];
    }
  if (UpgradeMotorState() == MOTOR_STOP && FlagStop != 3 && FlagStop != 2)
    {
      //6到8 屏蔽相同点 mDyk3t765vG7Hdxe
      UpdateMotorState(MOTOR_FRONT);
      FlagStop++;
#ifndef Magic
      SetMotorDutyRatio(0.07, 0.07);
      SysTickDelay(500);
#else
      SetMotorDutyRatio(SixTOEight_PointCoverSpeed, SixTOEight_PointCoverSpeed);
      SysTickDelay(SixTOEight_PointCoverTime);
#endif
    }
  else if(UpgradeMotorState() == MOTOR_STOP && FlagStop == 2)
    {
      //无需调整的部分
      SetMotorDutyRatio(0.00, 0.00);
      UpdateMotorState(MOTOR_FRONT);
      FlagStop++;
      // SetMotorDutyRatio(0.00, 0.00);
    }
  else if (UpgradeMotorState() == MOTOR_STOP && FlagStop == 3)
    {
      Stop = 1;
    }
  if (FlagStop == 0)
    {
      if (_FlagBack == 0)
        {
          _GoLineLowSpeed();
          if (_UpHillOrDownHillFeedBack() == DOWN)
            {
              _FlagBack = 1;
            }
        }
      else
        {
          if (_UpHillOrDownHillFeedBack() != FlatGround)
            {
              _GoLineLowSpeed();
            }
          else
            {
              _GoLineLowSpeed();
              //6到8 I点下台临时点检测值总和 gU15crco0soDk31n
#ifndef Magic
              if(_Temp<=13000)
#else
              if(_Temp<=SixTOEight_I_PointDetectGraySum)
#endif
                {
                  UpdateMotorState(MOTOR_STOP);
                }
              if (UpgradeMotorState() == MOTOR_STOP)
                {
                  //printf("1");
                  UpdateMotorState(MOTOR_FRONT);
                  while (GetLLaserStateAntiJitter() != LOW)
                    {
                      _GoLine();
                    }
                  //6到8 I点转弯调整相关 NKYWMYNL8zvBzusl
                  UpdateMotorState(MOTOR_BACK);
#ifndef Magic
                  SetMotorDutyRatio(0.1, 0.1);
                  SysTickDelay(150);
#else
                  SetMotorDutyRatio(SixTOEight_I_PointBackSpeed, SixTOEight_I_PointBackSpeed);
                  SysTickDelay(SixTOEight_I_PointBackTime);
#endif
                  UpdateMotorState(MOTOR_STOP);
#ifndef Magic
                  Motor_TurnLeftBlockingMode(95);
#else
                  Motor_TurnLeftBlockingMode(SixTOEight_I_PointTurnAngle);
#endif
                  UpdateMotorState(MOTOR_STOP);
                  BeginCount();
                  FlagPoint = 0;
                  FlagBalance=0;
                }
            }
        }
    }
  else if (FlagStop == 1)
    {
      if (FlagDistant == 0)
        {
          //6到8 出半圆相关距离调整 DRVj5w26uNu0kQyn
          //6到8 出半圆总距离
#ifndef Magic
          while (Encoder_GetCurrentDistantCount() <= (float)3.2)
            {
              //6到8 圆上从I点到波浪板开始距离 结束距离
              if(Encoder_GetCurrentDistantCount() >= (float)1.2&&Encoder_GetCurrentDistantCount() <= (float)1.5)
#else
          while (Encoder_GetCurrentDistantCount() <= SixTOEight_I_PointOutCircleDistant)
            {
              //6到8 圆上从I点到波浪板开始距离 结束距离
              if(Encoder_GetCurrentDistantCount() >= SixTOEight_barricadeDistantDownLimit&&Encoder_GetCurrentDistantCount() <= SixTOEight_barricadeDistantUpLimit)
#endif
                {
                  //6到8 圆上波浪板 转弯提供误差转弯角度
#ifndef Magic
                  Motor_TurnLeftBlockingMode(10);
#else
                  Motor_TurnLeftBlockingMode(SixTOEight_BeforebarricadeTurnAngle);
#endif
                  UpdateMotorState(MOTOR_FRONT);
                  Crycle=0;
                  //6到8 圆上波浪板 正常速度巡线时间
#ifndef Magic
                  while(Crycle<=1000)
#else
                  while(Crycle<=SixTOEight_barricadeGolineTime)
#endif
                    {
                      _GoLine();
                    }
										Motor_TurnLeftBlockingMode(20);
										UpdateMotorState(MOTOR_FRONT); 
                }
              else
                {
                  _GoCricle();
                }
            }
          FlagBalance=0;
          StopCount();
          BeginCount();
          //走完过波浪板后出圈距离
#ifndef Magic
          while(Encoder_GetCurrentDistantCount() <= (float)1.5)
#else
          while(Encoder_GetCurrentDistantCount() <= SixTOEight_AfterBarricadeOutCircleDistant)
#endif
            {
              //6到8  这里认为有问题 应该采用圆上PID函数 8FLns4yvQsFRendU PCH处理
              _GoLine();
              //_GoCricle();
              if (FlagPoint == 0)
                {
                  _FindPointGo();
                  if (FlagPoint == 2)
                    {
                      // 6到8 出圈转一定角度 防止自己又进圈 okQaI9I7GimDNP7Z
#ifndef Magic
                      Motor_TurnRightBlockingMode(15);
#else
                      Motor_TurnRightBlockingMode(SixTOEight_OutCircle_H1_PiontCoverAngle);
#endif
                      UpdateMotorState(MOTOR_FRONT);
                      //6到8 出圈走一定速度 时间 屏蔽相同点
#ifndef Magic
                      SetMotorDutyRatio(0.1, 0.1);
                      SysTickDelay(200);
#else
                      SetMotorDutyRatio(SixTOEight_OutCircle_H1_PiontCoverSpeed, SixTOEight_OutCircle_H1_PiontCoverSpeed);
                      SysTickDelay(200);
#endif
                      //6到8 屏蔽F点延时巡线
#ifndef Magic
                      _GoLineDelay(500);
#else
                      _GoLineDelay(SixTOEight_OutCircle_F_PiontCoverGolineTime);
#endif
                    }
                }
              //printf("2");
            }
          FlagDistant = 1;
        }
      else
        {
					//_GoLineDelay(100);
          //6到8 需要在J点转弯完成 J到K1点过程中加入距离屏蔽 UWkvBu34fETLOVPl
          _GoLine();
          if (GetRLaserStateAntiJitter() == LOW)
            {
              //printf("1");
              //6到8 J点转弯相关 退后时间 退后速度 X7SWdZUM0Iddt0YY
#ifndef Magic
              UpdateMotorState(MOTOR_BACK);
              SetMotorDutyRatio(0.1, 0.1);
              SysTickDelay(250);
#else
              UpdateMotorState(MOTOR_BACK);
              SetMotorDutyRatio(SixTOEight_J_PointBackSpeed, SixTOEight_J_PointBackSpeed);
              SysTickDelay(SixTOEight_J_PointBackTime);
#endif
              UpdateMotorState(MOTOR_STOP);
              //J点转向T台角度
#ifndef Magic
              Motor_TurnRightBlockingMode(90);
#else
              Motor_TurnRightBlockingMode(SixTOEight_J_PointTurnAngle);
#endif
            }
        }
    }
  else if (FlagStop == 2)
    {
      _GoLineLowSpeed();
      if (_UpHillOrDownHillFeedBack() == UP)
        {
          UpdateMotorState(MOTOR_STOP);
          //6到8 上K1点后车辆稳定时间
          SysTickDelay(500);
          FlagUp = 1;
          BeginCount();
        }
      if (FlagUp == 1)
        {
          //6到8 K1点上台后上开一段距离 防止掉落 速度与距离 gXuvcibGcDIG28Ux
#ifndef Magic
          SetMotorDutyRatio(0.07, 0.07);
          UpdateMotorState(MOTOR_FRONT);
          while(Encoder_GetCurrentDistantCount()<=(float)0.1);
#else
          SetMotorDutyRatio(SixTOEight_K1_PiontUp_Speed, SixTOEight_K1_PiontUp_Speed);
          UpdateMotorState(MOTOR_FRONT);
          while(Encoder_GetCurrentDistantCount()<=SixTOEight_K1_PiontUp_Distant);
#endif

          UpdateMotorState(MOTOR_STOP);
          //6到8 K1点上台后定点转弯角度
#ifndef Magic
          Motor_TurnRightNoInFlatGround(90);
#else
          Motor_TurnRightNoInFlatGround(SixTOEight_K1_PointHalfLockTurnAngle);
#endif
          FlagDistant=0;
        }
    }
  else if (FlagStop == 3)
    {
      if (FlagBridge == 0)
        {
          FlagBridge = 1;
        }
      else
        {
          if (FlagDistant == 0)
            {
              UpdateMotorState(MOTOR_STOP);
              _InitNowYawAngle();
              UpdateMotorState(MOTOR_FRONT);
              Crycle=0;

#ifndef Magic
              //6到8 上8台前开的总时间 5APyS5uke1BFGHZM
              while(Crycle<=2500)
                {
                  //6到8 转完以后惯性导航时间
                  if(Crycle<200)
#else
              while(Crycle<=SixTOEight_K1_8_PointToltalTime)
                {
                  //6到8 转完以后惯性导航时间
                  if(Crycle<SixTOEight_K1_8_PointBridgeTime)
#endif
                    {
                      _GoBridge();

                    }
                  //6到8 惯性导航结束后低速巡线时间
#ifndef Magic
                  else if(Crycle>=200&&Crycle<2500)
#else
                  else if(Crycle>=SixTOEight_K1_8_PointLowSpeedStartTime&&Crycle<SixTOEight_K1_8_PointLowSpeedStopTime)
#endif
                    {
                      _GoLineLowSpeed();
                    }
                }
              FlagDistant = 1;
								FlagBalance=0;
              StopCount();

            }
          else
            {
							if(FlagBalance==0)
							{
								
								Crycle=0;
								//6到8 上8坡屏蔽坡上平地时间 3LgRluUCcYUO2pCA
								#ifndef Magic
								while(Crycle<=2000)
								#else
								while(Crycle<=SixTOEight_UpbridgeTime)
								#endif
								{
									_GoLine();
								}
								FlagBalance=1;
							}
              _GoLine();
              if (_UpHillOrDownHillFeedBack() == UP)
                {
                  FlagUp = 1;
                }
              if (FlagUp == 1)
                {
                  if (_UpHillOrDownHillFeedBack() == FlatGround)
                    {
                      _ArrivePlatform();
                    }
                }
            }
        }
    }
}


/*
* 函数名称：SevenTOOne()
* 函数作用：控制车辆从七台返回一台
* 函数输入：无
* 函数输出：无
* 注意事项：该函数运行速度由系统节拍决定 目前系统节拍是10ms
* 魔法相关：该函数可能需要调整的值还没有放到magic.h内 当然该函数也可能不用调整
*/
void SevenTOOne(void)
{
  const AdcData_t *adcData;
  adcData = UpdateButtom();
  uint32_t _Temp = 0;
  for (int i = 0; i <= 7; i++)
    {
      _Temp += adcData->array[0][i];
    }
  if (UpgradeMotorState() == MOTOR_STOP && FlagStop != 6)
    {
      //停的点不是6点 每次点停转弯后都往前开一定速度 时间屏蔽相同点 rlpyPijdPiAazAfJ
      UpdateMotorState(MOTOR_FRONT);
      FlagStop++;
#ifndef Magic
      SetMotorDutyRatio(0.07, 0.07);
      SysTickDelay(500);
#else
      SetMotorDutyRatio(SevenTOOne_PointCoverSpeed, SevenTOOne_PointCoverSpeed);
      SysTickDelay(SevenTOOne_PointCoverTime);
#endif
    }
  else if (UpgradeMotorState() == MOTOR_STOP && FlagStop == 6)
    {
      //到第六个点就结束脚本
      Stop = 1;
    }
  if(FlagStop == 0)
    {
      if(FlagDown==0)
        {
          //下7台桥 只要没下到就一直下桥刹车巡线
          if(_UpHillOrDownHillFeedBack() == DOWN)
            {
              FlagDown=1;
              _GoLineLowSpeedDownBridge();
							
            }
          else
            {
              _GoLineLowSpeedDownBridge();
							
            }
        }
      else
        {
          if(_UpHillOrDownHillFeedBack()!=FlatGround)
            {
              _GoLineLowSpeedDownBridge();
							
            }
          else
            {
              //7到1 下台后延迟运行时间安排 TCYK5yOLrYnsqIZC
              // W6uo5v4hNcZ4Co7J
              if(_FlagBack==0)
                {
																		
                  Crycle=0;
                  //下完7台坡就低速巡线一定时间到障碍
#ifndef Magic
                  while(Crycle<=300)
#else
                  while(Crycle<=SevenTOOne_Down_7_LowSpeedTime)
#endif
                    {
                      _GoLineLowSpeed();
                    }
                  Crycle=0;
                  //下完7台坡就低速巡线一定时间到障碍
#ifndef Magic
                  while(Crycle<=600)
#else
                  while(Crycle<=SevenTOOne_Down_7_InsameSpeedTime)
#endif
                    {
                      _GoLine(); //可能是魔鬼速度 但是这里没改 暂时按魔鬼速度写
                    }
                  Crycle=0;
                  //然后一定时间的高速PID保证障碍卡不到自己 这里没用魔鬼速度
#ifndef Magic
                  while(Crycle<=1700)
#else
                  while(Crycle<=SevenTOOne_After_M_PiontLowSpeedTime)
#endif
                    {
                      _GoLine();
                    }
                  Crycle=0;
                  //高速PID动作比较剧烈 避免巡飞出去高速后低速
#ifndef Magic
                  while(Crycle<=500)
#else
                  while(Crycle<=SevenTOOne_OverBarricade_LowSpeedTime)
#endif
                    {
                      _GoLineLowSpeed();
                    }
                  _FlagBack=1;
                }
              //等待M点激光反馈
              if(GetLLaserStateAntiJitter()==LOW)
                {
                  Crycle=0;
                  BeginCount();
                  //7到1 在从M点到K2预定转弯位置前 距离确定转弯位置 6vQKK2luVUw2XMCJ
#ifndef Magic
                  while(Encoder_GetCurrentDistantCount()<=(float)0.98)
#else
                  while(Encoder_GetCurrentDistantCount()<=SevenTOOne_M_K1_PiontCoverDistant)
#endif
                    {
                      //低速一定时间保证自己在线上
#ifndef Magic
                      if(Crycle<=1500)
#else
                      if(Crycle<=SevenTOOne_After_M_PiontLowSpeedTime)
#endif
                        {
                          _GoLineLowSpeed();
                        }
                      else
                        {
                          _GoLine();
                        }
                    }
                  StopCount();
                  UpdateMotorState(MOTOR_STOP);
                  //7回家 T形台上半锁定转85度 mYsYAYO8VoGKCrlL
#ifndef Magic
                  Motor_TurnRightNoInFlatGround(85);
#else
                  Motor_TurnRightNoInFlatGround(SevenTOOne_K1_PointTurnAngle);
#endif
                  UpdateMotorState(MOTOR_STOP);
                  _FlagBack=0;
                  BeginCount();
                }
              else
                {
                  //如果没碰到M点激光反馈 就执行巡线函数
                  _GoLineLowSpeed();
									//_GoLine();
                }
            }
        }

    }
  else if(FlagStop == 1)
    {
      if(_FlagBack==0)
        {
          //7回家 距离屏蔽K点 mleCmoIy4Vlzh4uu
#ifndef Magic
          while(Encoder_GetCurrentDistantCount()<=(float)0.6)
#else
          while(Encoder_GetCurrentDistantCount()<=SevenTOOne_K1_J_PointDistantCover)
#endif

            {
              _GoLineLowSpeed();
            }
          StopCount();
          _GoLine();
          // 7回家 临时点检测算法 J点检测激光防抖 灰度总和 DsmmUi4XaLXkwT9C
#ifndef Magic
          if(_Temp<=10000)
#else
          if(_Temp<=SevenTOOne_J_PiontGrayDetectSum)
#endif
            {
              _FlagBack=1;
            }
        }
      else if(_FlagBack==1)
        {

          while(GetRLaserStateAntiJitter()==HIGH)
            {
              _GoLine();
            }
          //找到J点后 一定速度退后一定时间
#ifndef Magic
          SetMotorDutyRatio(0.14,0.14);
          UpdateMotorState(MOTOR_BACK);
          SysTickDelay(200);
#else
          SetMotorDutyRatio(SevenTOOne_J_PiontBackSpeed,SevenTOOne_J_PiontBackSpeed);
          UpdateMotorState(MOTOR_BACK);
          SysTickDelay(SevenTOOne_J_PiontBackTime);
#endif
          UpdateMotorState(MOTOR_STOP);
          //然后转弯转向F点
#ifndef Magic
          Motor_TurnLeftBlockingMode(90);
#else
          Motor_TurnLeftBlockingMode(SevenTOOne_J_PiontTurnAngle);
#endif

        }
    }
  else if(FlagStop == 2)
    {
      _GoLine();
      _FindPointStop();
      //激光寻F点 F点没有加灰度保护 amgRzGhLttHYFbAq
      while(GetLLaserStateAntiJitter()==HIGH)
        {
          _GoLine();
        }
      //F点调整
#ifndef Magic
      SetMotorDutyRatio(0.18,0.18);
      UpdateMotorState(MOTOR_BACK);
      SysTickDelay(200);
#else
      SetMotorDutyRatio(SevenTOOne_F_PiontBackSpeed,SevenTOOne_F_PiontBackSpeed);
      UpdateMotorState(MOTOR_BACK);
      SysTickDelay(SevenTOOne_F_PiontBackTime);
#endif
      UpdateMotorState(MOTOR_STOP);
      //F点右转
#ifndef Magic
      Motor_TurnRightBlockingMode(90);
#else
      Motor_TurnRightBlockingMode(SevenTOOne_F_PiontTurnAngle);
#endif
      BeginCount();
    }
  else if (FlagStop == 3)
    {
      //7回家 距离屏蔽F点到C点 5bGcVxGIHGBTdz7b
#ifndef Magic
      while(Encoder_GetCurrentDistantCount()<=(float)1.5)
#else
      while(Encoder_GetCurrentDistantCount()<=SevenTOOne_F_C_PointDistantCover)
#endif
        {
          _GoLine();
        }
      _GoLine();
      _FindPointStop();
      while(GetLLaserStateAntiJitter()==HIGH)
        {
          _GoLine();
        }
      //7回家 C转向B点转弯前调整与转弯角度
      UpdateMotorState(MOTOR_BACK);
#ifndef Magic
      SetMotorDutyRatio(0.14,0.14);
      SysTickDelay(200);
#else
      SetMotorDutyRatio(SevenTOOne_C_PiontBackSpeed,SevenTOOne_C_PiontBackSpeed);
      SysTickDelay(SevenTOOne_C_PiontBackTime);
#endif
      UpdateMotorState(MOTOR_STOP);
#ifndef Magic
      Motor_TurnRightBlockingMode(90);
#else
      Motor_TurnRightBlockingMode(SevenTOOne_C_PiontTurnAngle);
#endif
    }
  else if(FlagStop == 4)
    {
      _GoLine();
      _FindPointStop();
      while(GetLLaserStateAntiJitter()==HIGH)
        {
          _GoLine();
        }
      TurnFlag=1;
      BeginCount();
      if(TurnFlag==1)
        {
          //激光扫到线段AB后低速寻一玄学距离 BUkrUux94JZQANjj
#ifndef Magic
          while(Encoder_GetCurrentDistantCount()<=(float)0.22)
#else
          while(Encoder_GetCurrentDistantCount()<=SevenTOOne_C_B_PiontLaserTrigDistant)
#endif
            {
              _GoLineLowSpeed();
            }
          UpdateMotorState(MOTOR_STOP);
          //上跷跷板前转弯
#ifndef Magic
          Motor_TurnLeftBlockingMode(25);
#else
          Motor_TurnLeftBlockingMode(SevenTOOne_B_PiontTurnAngle);
#endif
        }
      //while(1);
    }
  else if(FlagStop == 5)
    {
      _GoLineLowSpeed();
      if(_UpHillOrDownHillFeedBack()==UP)
        {
          FlagUp=1;
        }
      if(FlagUp==1)
        {
          _BlockSpeed();
          if(FlagBalance==0)
            {
              _BlockSpeed();
              Crycle=0;
#ifndef Magic
              while(Crycle<3900)
#else
              while(Crycle<SevenTOOne_OnSeesawDelayTime)
#endif
                {
                  _BlockSpeed();
                }
              FlagBalance=1;
              UpdateMotorState(MOTOR_STOP);
              SysTickDelay(500);
              UpdateMotorState(MOTOR_FRONT);
            }
          else
            {
              Crycle=0;
#ifndef Magic
              while(Crycle<=1000)
#else
              while(Crycle<=SevenTOOne_DownSeesawDownBridgePidTime)
#endif
                {
                  _GoLineLowSpeedDownBridge();
                }
              if(_UpHillOrDownHillFeedBack()==FlatGround)
                {
                  FlagUp = 2;
                  FlagBalance=0;
                  UpdateMotorState(MOTOR_FRONT);
                }
            }
        }
      else if(FlagUp==2)
        {

          _GoLineLowSpeed();
          _FindPointStop();
          if(UpgradeMotorState() == MOTOR_STOP )
            {
              UpdateMotorState(MOTOR_FRONT);
#ifndef Magic
              _GoLineDelay(300);
              UpdateMotorState(MOTOR_STOP);
              Motor_TurnRightBlockingMode(35);
#else
              _GoLineDelay(SevenTOOne_Before_0_PiontGolineDelayTime);
              UpdateMotorState(MOTOR_STOP);
              Motor_TurnRightBlockingMode(SevenTOOne_0_PiontTurnAngle);
#endif
              FlagBalance=0;
              FlagUp=0;
            }
        }
    }

  else if(FlagStop==6)
    {
      if(FlagBalance==0)
        {
          Crycle=0;
          while(Crycle<=300)
            {
              _GoLine();
            }
        }
      if (_UpHillOrDownHillFeedBack() == UP)
        {
          FlagUp = 1;
        }
      if (FlagUp == 1)
        {
          if (_UpHillOrDownHillFeedBack() == FlatGround)
            {
							//改成直接停
							UpdateMotorState(MOTOR_STOP);
              //_ArrivePlatformBackUp_Encoder();
              if(UpgradeMotorState() == MOTOR_STOP)
                {
                  while(1);
                }
            }
        }
    }
}

/*
* 函数名称：EightTOOne()
* 函数作用：控制车辆从八台返回一台
* 函数输入：无
* 函数输出：无
* 注意事项：该函数运行速度由系统节拍决定 目前系统节拍是10ms
* 魔法相关：该函数可能需要调整的值还没有放到magic.h内 当然该函数也可能不用调整
*/
void EightTOOne(void)
{
  const AdcData_t *adcData;
  adcData = UpdateButtom();
  uint32_t _Temp = 0;
  for (int i = 0; i <= 7; i++)
    {
      _Temp += adcData->array[0][i];
    }
  if (UpgradeMotorState() == MOTOR_STOP && FlagStop != 6)
    {
      //8回家 每个点点停后屏蔽相同点走的速度与距离 CC1o2X9kkfBLEz4f
      UpdateMotorState(MOTOR_FRONT);
      FlagStop++;
#ifndef Magic
      SetMotorDutyRatio(0.07, 0.07);
      SysTickDelay(500);
#else
      SetMotorDutyRatio(EightTOOne_PointCoverSpeed, EightTOOne_PointCoverSpeed);
      SysTickDelay(EightTOOne_PointCoverTime);
#endif
    }
  else if (UpgradeMotorState() == MOTOR_STOP && FlagStop == 6)
    {
      Stop = 1;
    }
  if(FlagStop == 0)
    {
      if(FlagDown==0)
        {
          //8回家 需要准备两端下坡的方案 GIJyiepWxf8jACE5
					if(FlagBalance==0)
					{
						Crycle=0;
						//8回家 下二段坡屏蔽中间的平台 需要暴露 ASPuxSMhhhzTNjVL
						#ifndef Magic
						while(Crycle<=1000)
						#else
						while(Crycle<=EightTOOne_DownBridgeCoverFlatGroundTime)
						#endif
						{
							
							#ifdef Robot_1
              _GoLineLowSpeedDownBridge();
							#endif
							
							#ifdef Robot_2
							_GoLineLowSpeedDownBridge();
							#endif
						}
						FlagBalance=1;
					}
          //8回家 刚开始下8台就开启下桥巡线
          if(_UpHillOrDownHillFeedBack() == DOWN)
            {
              FlagDown=1;
              _GoLineLowSpeedDownBridge();
            }
          else
            {
              _GoLineLowSpeedDownBridge();
            }
        }
      else
        {
          if(_UpHillOrDownHillFeedBack()!=FlatGround)
            {
              _GoLineLowSpeedDownBridge();
            }
          else
            {
              BeginCount();
              //8回家 完全下台后低速寻一定距离 到台上合适位置执行半锁定转弯 MIML3fpM2TFskpgt

#ifndef Magic
              while(Encoder_GetCurrentDistantCount()<=(float)0.7)
#else
              while(Encoder_GetCurrentDistantCount()<=EightTOOne_DownPlatformTo_K1_PointDistant)
#endif
                {
                  _GoLineLowSpeed();
                }
              StopCount();
              UpdateMotorState(MOTOR_STOP);
              //8回家 T台上执行半锁定转弯
#ifndef Magic
              Motor_TurnLeftNoInFlatGround(85);
#else
              Motor_TurnLeftNoInFlatGround(EightTOOne_K1_PointHalfLockTurnAngle);
#endif
              UpdateMotorState(MOTOR_STOP);
								FlagBalance=0;
              BeginCount();
            }
        }

    }
  else if(FlagStop == 1)
    {
      if(_FlagBack==0)
        {
          //8回家 T台半锁定转完后低速巡线一定距离屏蔽K点 然后正常速度开J点 fNx0SzwwgDXwx2sN
#ifndef Magic
          while(Encoder_GetCurrentDistantCount()<=(float)0.6)
#else
          while(Encoder_GetCurrentDistantCount()<=EightTOOne_K1_K_PointCoverDistant
               )
#endif
            {
              _GoLineLowSpeed();
            }
          StopCount();
          _GoLine();
          //8回家 临时点停算法做J点防抖 nAHyADYWGhy8hiA7
#ifndef Magic
          if(_Temp<=10000)
#else
          if(_Temp<=EightTOOne_J_PointDetectGraySum)
#endif
            {
              _FlagBack=1;
            }
        }
      else if(_FlagBack==1)
        {
          //J点防抖激光停
          while(GetRLaserStateAntiJitter()==HIGH)
            {
              _GoLine();
            }
          UpdateMotorState(MOTOR_STOP);
          //8回家 J点左转到F点角度
#ifndef Magic
          Motor_TurnLeftBlockingMode(90);
#else
          Motor_TurnLeftBlockingMode(EightTOOne_J_PointTurnAngle);
#endif

        }
    }
  else if(FlagStop == 2)
    {
      _GoLine();
      _FindPointStop();
      while(GetLLaserStateAntiJitter()==HIGH)
        {
          _GoLine();
        }
      UpdateMotorState(MOTOR_STOP);
				
      //8回家 F点停转弯角度 AQwmU6tD5kBHvP5a
#ifndef Magic
				SetMotorDutyRatio(0.14,0.14);
				UpdateMotorState(MOTOR_BACK);
				SysTickDelay(200);
				UpdateMotorState(MOTOR_STOP);
      Motor_TurnRightBlockingMode(90);
#else
      Motor_TurnRightBlockingMode(EightTOOne_F_PointTurnAngle);
#endif
      BeginCount();
    }
  else if (FlagStop == 3)
    {
      //8回家 距离屏蔽F到C中间的门 EREQzfgH0O1SRbQm
#ifndef Magic
      while(Encoder_GetCurrentDistantCount()<=(float)1.5)
#else
      while(Encoder_GetCurrentDistantCount()<=EightTOOne_F_C_PointDistantCover)
#endif
        {
          _GoLine();
        }
      _GoLine();
      _FindPointStop();
      while(GetLLaserStateAntiJitter()==HIGH)
        {
          _GoLine();
        }
      //8回家 C点单独转弯前调整
      UpdateMotorState(MOTOR_BACK);
#ifndef Magic
      SetMotorDutyRatio(0.14,0.14);
      SysTickDelay(200);
#else
      SetMotorDutyRatio(EightTOOne_C_PointBackSpeed,EightTOOne_C_PointBackSpeed);
      SysTickDelay(EightTOOne_C_PointBackTime);
#endif
      UpdateMotorState(MOTOR_STOP);
      //8回家 C点转弯
#ifndef Magic
      Motor_TurnRightBlockingMode(90);
#else
      Motor_TurnRightBlockingMode(EightTOOne_C_PointTuenAngle);
#endif
    }
  else if(FlagStop == 4)
    {
      //C开到B点执行部分
      //8回家 C点后动作控制 NpygwfboMl16pN6R
      _GoLine();
      _FindPointStop();
      //8回家 等待激光扫到线AB段
      while(GetLLaserStateAntiJitter()==HIGH)
        {
          _GoLine();
        }
      TurnFlag=1;
      BeginCount();
      if(TurnFlag==1)
        {
          //8回家 激光扫过线AB段后低速巡一魔法距离
#ifndef Magic
          while(Encoder_GetCurrentDistantCount()<=(float)0.22)
#else
          while(Encoder_GetCurrentDistantCount()<=EightTOOne_C_B_PiontLaserTrigDistant)
#endif
            {
              _GoLineLowSpeed();
            }
          UpdateMotorState(MOTOR_STOP);
          //8回家 左转25度准备上跷跷板
#ifndef Magic
          Motor_TurnLeftBlockingMode(25);
#else
					Motor_TurnLeftBlockingMode(EightTOOne_B_PiontTurnAngle);
#endif
        }
      //while(1);
    }
  else if(FlagStop == 5)
    {

      _GoLineLowSpeed();
      if(_UpHillOrDownHillFeedBack()==UP)
        {
          FlagUp=1;
        }
      if(FlagUp==1)
        {
          //检测到上坡就锁速
          _BlockSpeed();
          if(FlagBalance==0)
            {
              _BlockSpeed();
              Crycle=0;
              //8回家 锁速后开一段时间越过跷跷板平衡位置
#ifndef Magic
              while(Crycle<3900)
#else
              while(Crycle<EightTOOne_OnSeesawDelayTime)
#endif
                {
                  _BlockSpeed();
                }
              FlagBalance=1;
              UpdateMotorState(MOTOR_STOP);
              SysTickDelay(500);
              UpdateMotorState(MOTOR_FRONT);
            }
          else
            {
              Crycle=0;
              //8回家 在1000ms内使用下桥PID函数
#ifndef Magic
              while(Crycle<=1000)
#else
              while(Crycle<=EightTOOne_DownSeesawDownBridgePidTime)
#endif
                {
                  _GoLineLowSpeedDownBridge();
                }
              if(_UpHillOrDownHillFeedBack()==FlatGround)
                {
                  FlagUp = 2;
                  FlagBalance=0;
                  UpdateMotorState(MOTOR_FRONT);
                }
            }
        }
      else if(FlagUp==2)
        {
          //下完跷跷板标准找点停
          _GoLineLowSpeed();
          _FindPointStop();
          if(UpgradeMotorState() == MOTOR_STOP )
            {
              UpdateMotorState(MOTOR_FRONT);
#ifndef Magic
              _GoLineDelay(300); //8回家 继续往前巡线延时300ms 因为提前检测到点
              UpdateMotorState(MOTOR_STOP);
              Motor_TurnRightBlockingMode(35); //8回家 0点右转35度准备上1台
#else
              _GoLineDelay(EightTOOne_Before_0_PiontGolineDelayTime); //8回家 继续往前巡线延时300ms 因为提前检测到点
              UpdateMotorState(MOTOR_STOP);
              Motor_TurnRightBlockingMode(EightTOOne_0_PiontTurnAngle); //8回家 0点右转35度准备上1台
#endif
              FlagUp=0;
            }
        }
    }
  else if(FlagStop==6)
    {
      _GoLine();
      if (_UpHillOrDownHillFeedBack() == UP)
        {
          FlagUp = 1;
        }
      if (FlagUp == 1)
        {
          //完全上台了才调用备用上台函数
          if (_UpHillOrDownHillFeedBack() == FlatGround)
            {
              //1台没有挡板供超声波判断距离 采用延时或者编码器备用上台函数
              //这里是上台后到平地才停下 两个轮子肯定上台了 所以不用
              //上台函数可能冲出去 就不用了
              //_ArrivePlatformBackUp_Encoder();
							UpdateMotorState(MOTOR_STOP);
              if(UpgradeMotorState() == MOTOR_STOP)
                {
                  //停住了 根据规则 第二次施放也是我们操作 可按键重启下位机
                  while(1);
                }
            }
        }
    }
}


/*
* 函数名称：_Start()
* 函数作用：控制车辆开始 当拿开禁止行板后下位机给上位机发开始指令 用超声波判断禁行板
* 函数输入：无
* 函数输出：无
* 注意事项：该函数运行较快 运行时间应该小于10ms
* 魔法相关：该函数可能需要调整的值还没有放到magic.h内 当然该函数也可能不用调整
*/
void _Start(void)
{
  if (SuperSonic_GetDistant() >= (float)1.5)
    {
      Flagstart = 1;
      printf("Hello");
    }
  SysTickDelay(100);
}




/*
* 函数名称：TestOnly()
* 函数作用：控制车辆执行一系列测试动作 何种测试动作由magic.h内的#define决定
* 函数输入：无
* 函数输出：无
* 注意事项：该函数运行速度由系统节拍决定 目前系统节拍是10ms
* 魔法相关：他就是魔法s
*/
void TestOnly(void)
{
#ifdef TESTDISTENCE
  _GoLine();
  BeginCount();
  while (Encoder_GetCurrentDistantCount() <= (float)4)
    {
      _GoLine();
      _FindPointGo();
    }
  UpdateMotorState(MOTOR_STOP);
  Stop = 1;
#endif
#ifdef TESTSPEED
		_GoLine();
  printf("lspeed: %f     rspeed: %f\r\n",Encoder_GetLWheelSpeed(),Encoder_GetRWheelSpeed());
#endif
#ifdef TESTBUTTOM
  const AdcData_t *adcData;
  adcData = UpdateButtom();
  uint16_t temp1 = 0, temp2 = 0;
  for (int i = 0; i <= 7; i++)
    {
      printf("%5d , ",adcData->array[0][i]);
    }
  printf("\r\n");
  SysTickDelay(500);
  double Bias;
  for (int i = 0; i <= 3; i++)
    temp1 += adcData->array[0][i];
  for (int i = 4; i <= 7; i++)
    temp2 += adcData->array[0][i];
  Bias = (sqrt(temp1 * 1.0) - sqrt(temp2 * 1.0)) / (temp1 + temp2);
  printf("_TRUE = %f\r\n",Bias);
#endif
#ifdef TESTGYRO
  printf("roll = %f\r\n",Gyro_GetRollAngle());
  printf("pitch = %f\r\n",Gyro_GetPitchAngle());
  printf("yaw = %f\r\n",Gyro_GetYawAngle());
  printf("\r\n");
  SysTickDelay(500);
  if(_UpHillOrDownHillFeedBack()==FlatGround)
    {
      printf("FlatGround\r\n");
    }
  else if(_UpHillOrDownHillFeedBack()==DOWN)
    {
      printf("DOWN\r\n");
    }
  else
    {
      printf("UP\r\n");
    }
  printf("\r\n");
  if(_IsBalanceFeedBack()==BALANCE)
    {
      printf("BALANCE\r\n");
    }
  else
    {
      printf("UNBALANCE\r\n");
    }
  printf("\r\n");
  SysTickDelay(500);
#endif
#ifdef TESTOTHER
  _FindPointStop();
#endif
}


/*
* 函数名称：ThreeTOFour()
* 函数作用：控制车辆从三台走到四台
* 函数输入：无
* 函数输出：无
* 注意事项：该函数运行速度由系统节拍决定 目前系统节拍是10ms
* 魔法相关：该函数可能需要调整的值还没有放到magic.h内 当然该函数也可能不用调整
*/
void ThreeTOFour(void)
{
  if (FlagDown == 0)
    {
      if (FlagUp == 0)
        {
          if (_UpHillOrDownHillFeedBack() != DOWN)
            {
              _GoLine();
              //printf("goline\r\n");
              if (_UpHillOrDownHillFeedBack() == UP)
                {
                  FlagUp = 1;
                  _ArrivePlatform();
                  //printf("arrive\r\n");
                }
              else if (_UpHillOrDownHillFeedBack() == FlatGround)
                {
                  _FindPointGo();
                  //printf("findpoint\r\n");
                }
            }
          else
            {
              FlagDown = 1;
              _GoLineLowSpeed();
              //printf("lowspeed\r\n");
            }
        }
      else
        {
          if (UpgradeMotorState() == MOTOR_STOP)
            {
              //printf("low\r\n");
              Motor_TurnLeftBlockingMode(180);
              UpdateMotorState(MOTOR_FRONT);
              _FlagBack = 1;
              _GoLineLowSpeed();
              //printf("2");
            }
          else if (_FlagBack == 1)
            {
              //printf("flagback\r\n");
              _GoLineLowSpeed();
              if (_UpHillOrDownHillFeedBack() == FlatGround)
                {
                  _FlagBack = 0;
                  FlagUp = 0;
                  //printf("down\r\n");
                }
            }
          else
            {
              _GoLine();
              //_ArrivePlatform();
              if (_UpHillOrDownHillFeedBack() == FlatGround)
                {
                  if (SuperSonic_GetDistant() >= (float)0.13 && SuperSonic_GetDistant() <= (float)0.20)
                    {
                      UpdateMotorState(MOTOR_STOP);
                    }
                  else if (SuperSonic_GetDistant() >= (float)0.20)
                    {
                      UpdateMotorState(MOTOR_FRONT);
                      SetMotorDutyRatio(0.1, 0.1);
                      SysTickDelay(100);
                      SetMotorDutyRatio(0, 0);
                    }
                  else if (SuperSonic_GetDistant() <= (float)0.13)
                    {
                      UpdateMotorState(MOTOR_BACK);
                      SetMotorDutyRatio(0.1, 0.1);
                      SysTickDelay(100);
                      SetMotorDutyRatio(0, 0);
                    }
                }
              //printf("7777777777777\r\n");
            }
        }
    }
  else
    {
      //printf("1\r\n");
      _GoLineLowSpeed();
      if (_UpHillOrDownHillFeedBack() == FlatGround)
        {
          FlagDown = 0;
        }
    }
}

/*
* 函数名称：EightTOSeven()
* 函数作用：控制车辆从八台走到七台
* 函数输入：无
* 函数输出：无
* 注意事项：该函数运行速度由系统节拍决定 目前系统节拍是10ms
* 魔法相关：该函数可能需要调整的值还没有放到magic.h内 当然该函数也可能不用调整
*/
void EightTOSeven(void)
{
  if(_FlagBack==0)
    {
      _GoLineLowSpeedDownBridge();
      if(_UpHillOrDownHillFeedBack()==DOWN)
        {
          _FlagBack=1;
        }
    }
  else
    {
      if(FlagDown==0)
        {
          if(_UpHillOrDownHillFeedBack()!=FlatGround)
            {
              _GoLineLowSpeedDownBridge();
            }
          else
            {
              FlagDown=1;
              BeginCount();
            }
        }
      else
        {
          if(FlagDistant==0)
            {
              Crycle=0;
              while(Crycle<=100)
                {
                  _GoLineLowSpeed();
                }
              _InitNowYawAngle();
              while(Encoder_GetCurrentDistantCount()<=(float)1.8)
                {
                  if(_IsBalanceFeedBack()==UNBALANCE)
                    {
                      _GoBridge();
                    }
                  else
                    {
                      _GoLineLowSpeed();
                    }
                }
              FlagDistant=1;
              StopCount();
            }
          else
            {
              if(FlagBalance==0)
                {
                  if(GetRLaserStateAntiJitter()==LOW)
                    {
                      FlagPoint=1;
                      BeginCount();
                    }
                  if(FlagPoint==1)
                    {
                      while(Encoder_GetCurrentDistantCount()<=(float)1.5)
                        {
                          if(Encoder_GetCurrentDistantCount()<=(float)0.3)
                            {
                              _GoLineLowSpeed();
                            }
                          else if(Encoder_GetCurrentDistantCount()>(float)0.3&&Encoder_GetCurrentDistantCount()<=(float)1.2)
                            {
                              _GoLineHighSpeed();
                            }
                          else
                            {
                              _GoLine();
                            }
                        }
												FlagBalance=1;
                    }
                }
              else
                {
                  _GoLine();
                  if(_UpHillOrDownHillFeedBack()==UP)
                    {
                      FlagUp=1;
                    }
                  if(FlagUp==1)
                    {
                      if(_UpHillOrDownHillFeedBack()==FlatGround)
                        {
                          _ArrivePlatform();
                          if(UpgradeMotorState()==MOTOR_STOP)
                            {
                              Stop=1;
                            }
                        }
                    }
                }
            }
        }
    }

}



/*
* 函数名称：SevenTOEight()
* 函数作用：控制车辆从七台走到八台
* 函数输入：无
* 函数输出：无
* 注意事项：该函数运行速度由系统节拍决定 目前系统节拍是10ms
* 魔法相关：该函数可能需要调整的值还没有放到magic.h内 当然该函数也可能不用调整
*/
void SevenTOEight(void)
{
  if(FlagBridge==0)
    {
      if(FlagDown==0)
        {
          if(_UpHillOrDownHillFeedBack() == DOWN)
            {
              FlagDown=1;
              _GoLineLowSpeedDownBridge();
            }
          else
            {
              _GoLineLowSpeedDownBridge();
            }
        }
      else
        {
          if(_UpHillOrDownHillFeedBack()!=FlatGround)
            {
              _GoLineLowSpeedDownBridge();
            }
          else
            {
              if(_FlagBack==0)
                {
                  Crycle=0;
                  while(Crycle<=200)
                    {
                      _GoLineLowSpeed();
                    }
                  Motor_TurnLeftBlockingMode(1);
                 UpdateMotorState(MOTOR_FRONT);
                  Crycle=0;
                  //_InitNowYawAngle();
                  while(Crycle<=1700)
                    {
                      if(Crycle<=450)
                        {
													_GoLine();
                          
                        }
                      else if(Crycle>450&&Crycle<=900)
                        {
                         
                          _GoLineHighSpeed();
                        }
												else if(Crycle>900&&Crycle<=1200)
												{
													_GoLine();
												}
												else
												{
													
                          _GoLineHighSpeed();
												}
                    }
                  Crycle=0;
                  UpdateMotorState(MOTOR_STOP);
                  SysTickDelay(100);
                  UpdateMotorState(MOTOR_FRONT);
                  while(Crycle<=500)
                    {
                      _GoLineLowSpeed();
                    }
                  _FlagBack=1;
                }
              if(GetLLaserStateAntiJitter()==LOW)
                {
                  Crycle=0;
                  while(Crycle<=200)
                    {
                      _GoLineLowSpeed();
                    }
                  _InitNowYawAngle();
                  BeginCount();
                  FlagBalance=0;
                  while(Encoder_GetCurrentDistantCount()<=(float)2)
                    {
											//printf("%d",_IsBalanceFeedBack());
                      if(_IsBalanceFeedBack()==BALANCE)
                        {
                          _GoLineLowSpeed();
													
                        }
                      else
                        {
                          _GoBridge();
													//printf("1");
                        }
                    }
                  StopCount();
                  FlagBridge=1;
                }
              else
                {
                  _GoLine();
                  _FindPointGo();
                }
            }
        }

    }
  else
    {
      while(_UpHillOrDownHillFeedBack()!=UP)
        {
          _GoLine();
        }
      FlagUp=1;
      while(FlagUp==1)
        {
          _GoLine();
          if(_UpHillOrDownHillFeedBack()==FlatGround)
            {
              _ArrivePlatform();
              if(UpgradeMotorState()==MOTOR_STOP)
                {
                  FlagUp=0;
                  Stop=1;
                }
            }
        }
    }

}


/*
* 函数名称：_Reset()
* 函数作用：重启自己的同时通知上位机也要重启
* 函数输入：无
* 函数输出：无
* 注意事项：无
*/
void _Reset(void)
{

  printf("reset");
  _resetlaser();
  //InitFlag();
}



/*
* 函数名称：UpHand()
* 函数作用：举手函数 控制两只手先后举起来
* 函数输入：无
* 函数输出：无
* 注意事项：无
*/
void UpHand(void)
{
	
  SetServoPWM(SERVO_ARM_LEFT,1400);
  SysTickDelay(500);
  SetServoPWM(SERVO_ARM_RIGHT,1400);
  SysTickDelay(300);
  SetServoPWM(SERVO_ARM_LEFT,2400);
  SetServoPWM(SERVO_ARM_RIGHT,500);
  Stop=1;
}




void ShakeHead(void)
{
  if(FlagDown<2)
    {
      SetServoPWM(SERVO_HEAD,1400);
      SysTickDelay(100);
      SetServoPWM(SERVO_HEAD,500);
      SysTickDelay(100);
      FlagDown++;
    }
  else
    {
      SetServoPWM(SERVO_HEAD,500);
      Stop=1;
    }

}


void Rebot(void)
{
	SysTickDelay(7000);
	NVIC_SystemReset();
}
