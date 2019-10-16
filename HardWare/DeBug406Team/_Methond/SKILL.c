#ifdef USE_Magic
#include "SKILL.h"
#define TEST2
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
            if (FlagUp == 0)
            {
                _GoLine();
                _FindPointGo();
                if (_UpHillOrDownHillFeedBack() == UP)
                {
                    UpdateMotorState(MOTOR_STOP);
                    BeginCount();
                    SysTickDelay(500);
                    FlagUp = 1;
                }
            }
            else if (FlagUp == 1)
            {
//                UpdateMotorState(MOTOR_FRONT);
//                // _BlockSpeed();
//                _GoLine();
//                if (_UpHillOrDownHillFeedBack() == FlatGround)
//                {

//                    FlagUp = 2;
//                    FlagBridge = 1;
//                    BeginCount();
//                }
                _InitNowYawAngle();
                while (Encoder_GetCurrentDistantCount() <= (float)5)
                {
                    _GoBridge();
                    if (_UpHillOrDownHillFeedBack() == DOWN)
                    {
                        break;
                    }
                }
                UpdateMotorState(MOTOR_FRONT);
                FlagBridge = 0;
                _FlagBack = 1;
                FlagUp = 0;
            }
//            if (FlagBridge == 1)
//            {
//                _InitNowYawAngle();
//                while (Encoder_GetCurrentDistantCount() <= (float)5)
//                {
//                    _GoBridge();
//                    if (_UpHillOrDownHillFeedBack() == DOWN)
//                    {
//                        break;
//                    }
//                }
//                UpdateMotorState(MOTOR_FRONT);
//                FlagBridge = 0;
//                _FlagBack = 1;
//                FlagUp = 0;
//            }
        }
    }
    else
    {
        _GoLine();
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
void TwoTOThree(void)
{
    if (UpgradeMotorState() == MOTOR_STOP && FlagStop != 2)
    {
        UpdateMotorState(MOTOR_FRONT);
        FlagStop++;
        SetMotorDutyRatio(Angle_GoSpeed, Angle_GoSpeed);
        SysTickDelay(Angle_GoTime);
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

                UpdateMotorState(MOTOR_FRONT);
                SetMotorDutyRatio(0.05, 0.05);
                ClearLLaserChangePendingBit();
                while (UpgradeMotorState() == MOTOR_FRONT)
                {
                    if (IsLLaserChange() == Changed)
                    {
                        UpdateMotorState(MOTOR_BACK);
                        SysTickDelay(200);
                        Motor_TurnRightBlockingMode(140);
                        FlagDown = 0;
                    }
                }
            }
        }
        else
        {
            _GoLineDelay(3000);
            UpdateMotorState(MOTOR_FRONT);
            ClearLLaserChangePendingBit();
            FlagDown = 1;
        }
    }
    else if (FlagStop == 2)
    {
        _GoLine();
        //_FindPointGo();
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
                FlagDown = 1;
            }
        }
        if (FlagDown == 1)
        {
            if (_UpHillOrDownHillFeedBack() == FlatGround)
            {
                _GoLine();
                _FindPointStop();
                if (UpgradeMotorState() == MOTOR_STOP)
                {
                    Motor_TurnRightBlockingMode(35);
                    FlagDown = 0;
                    //SetMotorDutyRatio(0.04,0.04);
                    //SysTickDelay(100);
                }
            }
            else if (_UpHillOrDownHillFeedBack() == DOWN)
            {
                _GoLineLowSpeed();
            }
        }
    }
}

void TwoTOFour()
{
    if (UpgradeMotorState() == MOTOR_STOP && FlagStop != 2)
    {
        UpdateMotorState(MOTOR_FRONT);
        FlagStop++;
        SetMotorDutyRatio(Angle_GoSpeed, Angle_GoSpeed);
        SysTickDelay(Angle_GoTime);
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

                UpdateMotorState(MOTOR_FRONT);
                SetMotorDutyRatio(0.05, 0.05);
                ClearLLaserChangePendingBit();
                while (UpgradeMotorState() == MOTOR_FRONT)
                {
                    if (IsLLaserChange() == Changed)
                    {
                        UpdateMotorState(MOTOR_BACK);
                        SysTickDelay(200);
                        Motor_TurnLeftBlockingMode(33);
                        FlagDown = 0;
                    }
                }
            }
        }
        else
        {
            _GoLineDelay(3000);
            UpdateMotorState(MOTOR_FRONT);
            ClearLLaserChangePendingBit();
            FlagDown = 1;
        }
    }
    else if (FlagStop == 2)
    {
        _GoLine();
        //_FindPointGo();
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
                _GoLine();
                _FindPointStop();
                if (UpgradeMotorState() == MOTOR_STOP)
                {
                    SetMotorDutyRatio(0.14,0.14);
                    UpdateMotorState(MOTOR_STOP);
                    SysTickDelay(200);
                    UpdateMotorState(MOTOR_BACK);
                    SysTickDelay(100);
                    UpdateMotorState(MOTOR_STOP);
                    Motor_TurnRightBlockingMode(37);
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
        UpdateMotorState(MOTOR_FRONT);
        FlagStop++;
        SetMotorDutyRatio(Angle_GoSpeed, Angle_GoSpeed);
        SysTickDelay(300);
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
                    }
                }
            }
            else if (FlagDown == 1)
            {
                if (TurnFlag == 0)
                {
                    _GoLine();
                    _FindPointStop();
                    if (FlagPoint == 1)
                    {
                        UpdateMotorState(MOTOR_FRONT);
                        SetMotorDutyRatio(0.05, 0.05);
                        ClearLLaserChangePendingBit();
                        while (UpgradeMotorState() == MOTOR_FRONT)
                        {
                            _GoLine();
                            if (IsLLaserChange() == Changed)
                            {
                                UpdateMotorState(MOTOR_BACK);
                                SysTickDelay(100);
                                UpdateMotorState(MOTOR_STOP);
                                Motor_TurnLeftBlockingMode(90);
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
                ClearLLaserChangePendingBit();
                ClearRLaserChangePendingBit();
                UpdateMotorState(MOTOR_FRONT);
                SetMotorDutyRatio(0.04, 0.04);
                ClearLLaserChangePendingBit();
                while (UpgradeMotorState() == MOTOR_FRONT)
                {
                    if (IsLLaserChange() == Changed)
                    {
                        UpdateMotorState(MOTOR_STOP);
                        Motor_TurnLeftBlockingMode(90);
                        FlagDown = 0;
                    }
                }
            }
        }
        else if (FlagStop == 2)
        {
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
        UpdateMotorState(MOTOR_FRONT);
        FlagStop++;
        SetMotorDutyRatio(Angle_GoSpeed,Angle_GoSpeed);
        SysTickDelay(Angle_GoTime);
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
                    }
                }
            }
            else if (FlagDown == 1)
            {
                _GoLine();
                if (TurnFlag == 0)
                {
                    _FindPointStop();
                    if(UpgradeMotorState() == MOTOR_STOP)
                    {
                        UpdateMotorState(MOTOR_FRONT);
                        SetMotorDutyRatio(0.1, 0.1);
                        if(GetRLaserStateAntiJitter()==LOW)
                        {
                            UpdateMotorState(MOTOR_BACK);
                            SetMotorDutyRatio(0.1, 0.1);
                            SysTickDelay(200);
                            UpdateMotorState(MOTOR_STOP);
                            Motor_TurnRightBlockingMode(45);
                            BeginCount();
                        }
                    }
                    //同4到5需测试
                }
            }
        }
        else if (FlagStop == 1)
        {
            while(Encoder_GetCurrentDistantCount()<=(float)3)
            {
                _GoLine();
            }
            StopCount();
            _FindPointStop();
            if(UpgradeMotorState() == MOTOR_STOP)
            {
                UpdateMotorState(MOTOR_FRONT);
                SetMotorDutyRatio(0.1, 0.1);
                if(GetRLaserStateAntiJitter()==LOW)
                {
                    UpdateMotorState(MOTOR_BACK);
                    SetMotorDutyRatio(0.1, 0.1);
                    SysTickDelay(200);
                    UpdateMotorState(MOTOR_STOP);
                    Motor_TurnLeftBlockingMode(45);
                    BeginCount();
                }
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
void FiveTOSeven(void)
{
    const AdcData_t *adcData;
    adcData = UpdateButtom();
    uint32_t _Temp = 0;
    //  float _TempAngleLow=0;
    //  float _TempAngleHigh=0;
    for (int i = 0; i <= 7; i++)
    {
        _Temp += adcData->array[0][i];
    }
    if (UpgradeMotorState() == MOTOR_STOP && FlagStop != 2)
    {
        UpdateMotorState(MOTOR_FRONT);
        FlagStop++;
        SetMotorDutyRatio(Angle_GoSpeed,Angle_GoSpeed);
        SysTickDelay(Angle_GoTime);
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
                if (Encoder_GetCurrentDistantCount() > (float)2.7)
                {
                    //printf("1");
                    //printf("%f\r\n",Encoder_GetCurrentDistantCount());
                    ClearLLaserChangePendingBit();
                    FlagDistant = 1;
                    Encoder_StopDistantCount();
                    Encoder_ResetDistant();
                }
                if (FlagDistant == 1)
                {
                    _GoLine();
                    //printf("1");
                    if (GetLLaserStateAntiJitter() == LOW)
                    {
                        //printf("1");
                        UpdateMotorState(MOTOR_BACK);
                        SetMotorDutyRatio(0.15, 0.15);
                        SysTickDelay(200);
                        UpdateMotorState(MOTOR_STOP);

                        Motor_TurnLeftBlockingMode(90);
                        BeginCount();
                    }
                }
            }
        }
        else if (FlagStop == 1)
        {
            _GoLineLowSpeed();
            if(Encoder_GetCurrentDistantCount()>=(float)1)
            {
                if (_UpHillOrDownHillFeedBack() == UP)
                {
                    UpdateMotorState(MOTOR_STOP);
                    SysTickDelay(500);
                    FlagUp = 1;
                }
                if (FlagUp == 1)

                {
                    SetMotorDutyRatio(0.23, 0.23);
                    UpdateMotorState(MOTOR_FRONT);
                    SysTickDelay(245);
                    UpdateMotorState(MOTOR_STOP);
                    Motor_TurnLeftNoInFlatGround(75);
                    StopCount();
                    //_InitNowYawAngle();
                }
            }
        }
        else if (FlagStop == 2)
        {
            if (FlagBridge == 0)
            {
                _GoLine();

                if (_UpHillOrDownHillFeedBack() == FlatGround)
                {
                    FlagBridge = 1;
                    BeginCount();
                }
            }
            else
            {
                if (FlagDistant == 0)
                {

                    _GoLineMSpeed();
                    //printf("%f",Encoder_GetCurrentDistantCount());
                    //                  Encoder_ResetDistant();
                    //                  Encoder_StartDistantCount();
                    if (Encoder_GetCurrentDistantCount() >= (float)2)
                    {
                        FlagDistant = 1;
                        //                      Encoder_StopDistantCount();
                        //                      Encoder_ResetDistant();
                        StopCount();
                    }
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
    }
    else
    {
        //      Motor_TurnLeftBlockingMode(180);
        //      UpdateMotorState(MOTOR_FRONT);
        _FlagBack = 1;
    }
}
void TurnBack(void)
{
    Motor_TurnLeftBlockingMode(180);
    if (UpgradeMotorState() == MOTOR_STOP)
    {
        Stop = 1;
    }
}

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
        UpdateMotorState(MOTOR_FRONT);
        FlagStop++;
        SetMotorDutyRatio(Angle_GoSpeed,Angle_GoSpeed);
        SysTickDelay(Angle_GoTime);
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
                        UpdateMotorState(MOTOR_BACK);
                        SetMotorDutyRatio(0.1, 0.1);
                        SysTickDelay(100);
                        UpdateMotorState(MOTOR_STOP);
                        //找到后立刻停车，并转弯，并且将motor_stop状态传达至下一轮询
                        Motor_TurnRightBlockingMode(85);
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
            if (_Temp <= 10000)
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

                UpdateMotorState(MOTOR_STOP);
                UpdateMotorState(MOTOR_BACK);
                SetMotorDutyRatio(0.1, 0.1);
                SysTickDelay(150);
                UpdateMotorState(MOTOR_STOP);
                Motor_TurnRightBlockingMode(90);
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
            //当右侧激光传感器被触发时，右转145°
            UpdateMotorState(MOTOR_STOP);
            Motor_TurnRightBlockingMode(135);
        }
        else
        {
            UpdateMotorState(MOTOR_FRONT);
        }
    }
    //第三点停到第四点停时
    else if (FlagStop == 3)
    {
        //当未检测上坡时，低速寻线
        if (FlagUp == 0)
        {
            _GoCricle();
            //当检测到上坡时，左转26矫正，保证上跷跷板时两前轮均能不悬空，并将flag置为1
            if (_UpHillOrDownHillFeedBack() == UP)
            {
                FlagUp = 1;
                Motor_TurnLeftBlockingMode(20);
                UpdateMotorState(MOTOR_FRONT);
            }
        }
        //当flagup为1时，实行轮速锁定，保证速度不会过低
        else if (FlagUp == 1)
        {
            _BlockSpeed();
            //一直保持转台直至回到平地。因为我们的陀螺仪因为防抖需要所以在有一部分还在跷跷板时就会认为在平地上，所以先需要向前行进一段距离做矫正
            //左转40°因为圆的线方向与上桥时相比已经有了变化。还需要将flagup置为2
            if (_UpHillOrDownHillFeedBack() == FlatGround)
            {
                FlagUp = 2;
                Encoder_ResetDistant();
                Encoder_StartDistantCount();
                UpdateMotorState(MOTOR_FRONT);
                SetMotorDutyRatio(0.14, 0.14);
                SysTickDelay(200);
                Motor_TurnLeftBlockingMode(40);
                SysTickDelay(100);
                UpdateMotorState(MOTOR_FRONT);
            }
        }

        //当flagup为2时执行低速寻线0.5，这里的执行距离是防止刚下跷跷板后的车处于非稳定状态误判入点
        else if (FlagUp == 2)
        {
            if (FlagBalance == 0)
            {
                //printf("%f\r\n",Encoder_GetCurrentDistantCount());
                _GoLineLowSpeed();
                if (Encoder_GetCurrentDistantCount() >= (float)0.5)
                {
                    //printf("1\r\n");
                    //此处的入点方式为临时入点，原因为此处边缘仅为1，与点模式的通用描述不同
                    if (_Temp <= 20000)
                    {
                        UpdateMotorState(MOTOR_STOP);
                        ClearLLaserChangePendingBit();
                    }
                }
                //由临时点模式造成的电机停有时，继续前进，直至左侧激光管检测到线，并将flagbalance置为1
                if (UpgradeMotorState() == MOTOR_STOP)
                {
                    //printf("2\r\n");
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
                UpdateMotorState(MOTOR_STOP);
                Motor_TurnLeftBlockingMode(90);
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
            _GoLine();
            if (_UpHillOrDownHillFeedBack() == UP)
            {
                FlagUp = 1;
            }
        }
        //上坡后执行到台停止函数
        else
        {
            _ArrivePlatform();
        }
    }
}
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
        UpdateMotorState(MOTOR_FRONT);
        FlagStop++;
        SetMotorDutyRatio(Angle_GoSpeed,Angle_GoSpeed);
        SysTickDelay(Angle_GoTime);
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
            _GoLine();
            _FindPointStop();
            if (UpgradeMotorState() == MOTOR_STOP)
            {
                UpdateMotorState(MOTOR_FRONT);
                while (IsLLaserChange() == UnChange)
                {
                    _GoLine();
                }
                UpdateMotorState(MOTOR_STOP);
                Motor_TurnLeftBlockingMode(45);
                BeginCount();
            }
        }
    }
    else if (FlagStop == 1)
    {
        if (FlagDistant == 0)
        {

            if (Encoder_GetCurrentDistantCount() <= (float)3)
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
            //          while(GetLLaserStateAntiJitter()==HIGH)
            //            {
            //							Encoder_ResetDistant();
            //            }
            //          UpdateMotorState(MOTOR_STOP);
            //          Encoder_StartDistantCount();
            //          UpdateMotorState(MOTOR_BACK);
            //          SetMotorDutyRatio(0.1,0.1);
            //					SysTickDelay(150);
            //					if(GetLLaserStateAntiJitter()==HIGH)
            //					{
            //						FlagPoint=1;
            //          }
            //					if(FlagPoint==1)
            //					{
            //					if(GetLLaserStateAntiJitter()==LOW)
            //            {
            //
            //              UpdateMotorState(MOTOR_STOP);
            //              TempDistant=Encoder_GetCurrentDistantCount();
            //              UpdateMotorState(MOTOR_FRONT);
            //              SetMotorDutyRatio(0.1,0.1);
            //							printf("%f\r\n",TempDistant);
            //              StopCount();
            //              BeginCount();
            //            }
            //          if(Encoder_GetCurrentDistantCount()<=(TempDistant/(float)2.0+(float)0.02)&&Encoder_GetCurrentDistantCount()>(TempDistant/(float)2.0-(float)0.02))
            //            {
            //              UpdateMotorState(MOTOR_STOP);
            //              Motor_TurnRightBlockingMode(45);
            //							StopCount();
            //            }
            //					}
            UpdateMotorState(MOTOR_FRONT);
            SysTickDelay(150);
            UpdateMotorState(MOTOR_STOP);
            Motor_TurnRightBlockingMode(37);
            BeginCount();
        }
    }
    else if (FlagStop == 2)
    {
        _GoLine();
        if (Encoder_GetCurrentDistantCount() >= (float)1)
        {
            UpdateMotorState(MOTOR_STOP);
            StopCount();
        }
        //			if(GetRLaserStateAntiJitter()==LOW)
        //			{
        //				UpdateMotorState(MOTOR_STOP);
        //			}

        if (UpgradeMotorState() == MOTOR_STOP)
        {
            //_GoLineDelay(200);
            SetMotorDutyRatio(0.18, 0.15);
            UpdateMotorState(MOTOR_FRONT);
            SysTickDelay(150);
            UpdateMotorState(MOTOR_STOP);
            Motor_TurnRightBlockingMode(145);
            UpdateMotorState(MOTOR_FRONT);
            SetMotorDutyRatio(0.1, 0.1);
            SysTickDelay(200);
            UpdateMotorState(MOTOR_STOP);
        }
        else
        {
            UpdateMotorState(MOTOR_FRONT);
        }
    }
    else if (FlagStop == 3)
    {
        if (FlagUp == 0)
        {
            _GoLineLowSpeed();
            if (_UpHillOrDownHillFeedBack() == UP)
            {
                FlagUp = 1;
                Motor_TurnLeftBlockingMode(20);
                SysTickDelay(100);
                UpdateMotorState(MOTOR_FRONT);
                Encoder_ResetDistant();
                Encoder_StartDistantCount();
            }
        }
        else if (FlagUp == 1)
        {
            _BlockSpeed();
            if (_UpHillOrDownHillFeedBack() == FlatGround)
            {
                FlagUp = 2;
                Encoder_ResetDistant();
                Encoder_StartDistantCount();
                UpdateMotorState(MOTOR_FRONT);
                SetMotorDutyRatio(0.17, 0.17);
                SysTickDelay(250);
                UpdateMotorState(MOTOR_BACK);
                SetMotorDutyRatio(0.1, 0.1);
                Motor_TurnLeftBlockingMode(45);
                SysTickDelay(100);
                UpdateMotorState(MOTOR_FRONT);
            }
        }
        else if (FlagUp == 2)
        {
            if (FlagBalance == 0)
            {
                //printf("%f\r\n",Encoder_GetCurrentDistantCount());
                _GoLineLowSpeed();
                if (Encoder_GetCurrentDistantCount() >= (float)0.5)
                {
                    //printf("1\r\n");
                    if (_Temp <= 20000)
                    {
                        UpdateMotorState(MOTOR_STOP);
                        ClearLLaserChangePendingBit();
                    }
                }
                if (UpgradeMotorState() == MOTOR_STOP)
                {
                    //printf("2\r\n");
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
            else
            {
                UpdateMotorState(MOTOR_STOP);
                Motor_TurnLeftBlockingMode(90);
                FlagUp = 0;
            }
        }
    }
    else if (FlagStop == 4)
    {
        if (FlagUp == 0)
        {
            _GoLine();
            if (_UpHillOrDownHillFeedBack() == UP)
            {
                FlagUp = 1;
            }
        }
        else
        {
            _ArrivePlatform();
        }
    }
}
void FiveTOEight(void)
{
    const AdcData_t *adcData;
    adcData = UpdateButtom();
    uint32_t _Temp = 0;
    //  float _TempAngleLow=0;
    //  float _TempAngleHigh=0;
    for (int i = 0; i <= 7; i++)
    {
        _Temp += adcData->array[0][i];
    }
    if (UpgradeMotorState() == MOTOR_STOP && FlagStop != 2)
    {
        UpdateMotorState(MOTOR_FRONT);
        FlagStop++;
        SetMotorDutyRatio(Angle_GoSpeed,Angle_GoSpeed);
        SysTickDelay(Angle_GoTime);
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
                if (Encoder_GetCurrentDistantCount() > (float)2.7)
                {
                    //printf("1");
                    //printf("%f\r\n",Encoder_GetCurrentDistantCount());
                    ClearLLaserChangePendingBit();
                    FlagDistant = 1;
                    Encoder_StopDistantCount();
                    Encoder_ResetDistant();
                }
                if (FlagDistant == 1)
                {
                    _GoLine();
                    //printf("%d\r\n",LeftCountTimes)
                    //printf("1");
                    if (GetLLaserStateAntiJitter() == LOW)
                    {
                        //printf("1");
                        UpdateMotorState(MOTOR_STOP);
                        Motor_TurnLeftBlockingMode(90);
                        BeginCount();
                    }
                }
            }
        }
        else if (FlagStop == 1)
        {
            _GoLineLowSpeed();
            if(Encoder_GetCurrentDistantCount()>=(float)1)
            {
                if (_UpHillOrDownHillFeedBack() == UP)
                {
                    UpdateMotorState(MOTOR_STOP);
                    SysTickDelay(Angle_GoTime);
                    FlagUp = 1;
                }
            }
            if (FlagUp == 1)

            {
                SetMotorDutyRatio(0.18, 0.18);
                UpdateMotorState(MOTOR_FRONT);
                SysTickDelay(245);
                UpdateMotorState(MOTOR_STOP);
                Motor_TurnRightNoInFlatGround(75);

            }
        }
        else if (FlagStop == 2)
        {
            if (FlagBridge == 0)
            {
                _GoLine();
                if (_UpHillOrDownHillFeedBack() == FlatGround)
                {
                    FlagBridge = 1;
                }
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
    else
    {
        //      Motor_TurnLeftBlockingMode(180);
        //      UpdateMotorState(MOTOR_FRONT);
        _FlagBack = 1;
    }
}
void SixTOSeven(void)
{
    const AdcData_t *adcData;
    adcData = UpdateButtom();
    uint32_t _Temp = 0;
    //  float _TempAngleLow=0;
    //  float _TempAngleHigh=0;
    for (int i = 0; i <= 7; i++)
    {
        _Temp += adcData->array[0][i];
    }
    if (UpgradeMotorState() == MOTOR_STOP && FlagStop != 3)
    {
        UpdateMotorState(MOTOR_FRONT);
        FlagStop++;
        SetMotorDutyRatio(Angle_GoSpeed,Angle_GoSpeed);
        SysTickDelay(Angle_GoTime);
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
                _GoLine();
                _FindPointStop();
                if (UpgradeMotorState() == MOTOR_STOP)
                {
                    //printf("1");
                    UpdateMotorState(MOTOR_FRONT);
                    while (GetLLaserStateAntiJitter() != LOW)
                    {
                        _GoLine();
                    }
                    UpdateMotorState(MOTOR_BACK);
                    SetMotorDutyRatio(0.1, 0.1);
                    SysTickDelay(150);
                    UpdateMotorState(MOTOR_STOP);
                    Motor_TurnLeftBlockingMode(105);
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
            while (Encoder_GetCurrentDistantCount() <= (float)3.2)
            {
                //            if(FlagPoint==2)
                //            {
                //                _GoLineDelay(100);
                //                _FindPointGo();
                //								FlagPoint=0;
                //            }
                //            else
                //            {
                //                _GoLineLowSpeed();
                //                _FindPointGo();
                //            }
                //
                //printf("1");
                //_GoCricle();
                _GoCricleSpeedLock();
            }
            StopCount();
            BeginCount();
            while (Encoder_GetCurrentDistantCount() <= (float)1.5)
            {
                _GoLine();
                if (FlagPoint == 0)
                {
                    _FindPointGo();
                    if (FlagPoint == 2)
                    {
                        Motor_TurnRightBlockingMode(10);
                        UpdateMotorState(MOTOR_FRONT);
                        SetMotorDutyRatio(0.1, 0.1);
                        SysTickDelay(200);
                        _GoLineDelay(500);
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
                UpdateMotorState(MOTOR_BACK);
                SetMotorDutyRatio(0.1, 0.1);
                SysTickDelay(250);
                UpdateMotorState(MOTOR_STOP);

                Motor_TurnRightBlockingMode(90);
            }
        }
    }
    else if (FlagStop == 2)
    {
        _GoLineMSpeed();
        if (_UpHillOrDownHillFeedBack() == UP)
        {
            UpdateMotorState(MOTOR_STOP);
            SysTickDelay(500);
            FlagUp = 1;
        }
        if (FlagUp == 1)

        {
            SetMotorDutyRatio(0.18, 0.18);
            UpdateMotorState(MOTOR_FRONT);
            SysTickDelay(225);
            UpdateMotorState(MOTOR_STOP);
            Motor_TurnLeftNoInFlatGround(87);
            FlagUp=0;
        }
    }
    else if (FlagStop == 3)
    {
        if (FlagDistant == 0)
        {

            while (Encoder_GetCurrentDistantCount() <= (float)2.7)
            {
                if(_UpHillOrDownHillFeedBack() == UP)
                {
                    FlagUp=1;
                }
                if(FlagUp==1)
                {
                    _GoLineMSpeed();
                }
                else
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
void SixTOEight(void)
{
    const AdcData_t *adcData;
    adcData = UpdateButtom();
    uint32_t _Temp = 0;
    //  float _TempAngleLow=0;
    //  float _TempAngleHigh=0;
    for (int i = 0; i <= 7; i++)
    {
        _Temp += adcData->array[0][i];
    }
    if (UpgradeMotorState() == MOTOR_STOP && FlagStop != 3)
    {
        UpdateMotorState(MOTOR_FRONT);
        FlagStop++;
        SetMotorDutyRatio(Angle_GoSpeed,Angle_GoSpeed);
        SysTickDelay(Angle_GoTime);
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
                _GoLine();
                _FindPointStop();
                if (UpgradeMotorState() == MOTOR_STOP)
                {
                    //printf("1");
                    UpdateMotorState(MOTOR_FRONT);
                    while (GetLLaserStateAntiJitter() != LOW)
                    {
                        _GoLine();
                    }
                    UpdateMotorState(MOTOR_BACK);
                    SetMotorDutyRatio(0.1, 0.1);
                    SysTickDelay(150);
                    UpdateMotorState(MOTOR_STOP);
                    Motor_TurnLeftBlockingMode(105);
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
            while (Encoder_GetCurrentDistantCount() <= (float)3.2)
            {
                _GoCricle();
            }
            StopCount();
            BeginCount();
            while (Encoder_GetCurrentDistantCount() <= (float)1.5)
            {
                _GoLine();
                if (FlagPoint == 0)
                {
                    _FindPointGo();
                    if (FlagPoint == 2)
                    {
                        Motor_TurnRightBlockingMode(15);
                        UpdateMotorState(MOTOR_FRONT);
                        SetMotorDutyRatio(0.1, 0.1);
                        SysTickDelay(200);
                        _GoLineDelay(500);
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
                UpdateMotorState(MOTOR_BACK);
                SetMotorDutyRatio(0.1, 0.1);
                SysTickDelay(250);
                UpdateMotorState(MOTOR_STOP);

                Motor_TurnRightBlockingMode(90);
            }
        }
    }
    else if (FlagStop == 2)
    {
        _GoLineMSpeed();
        if (_UpHillOrDownHillFeedBack() == UP)
        {
            UpdateMotorState(MOTOR_STOP);
            SysTickDelay(500);
            FlagUp = 1;
        }
        if (FlagUp == 1)

        {
            SetMotorDutyRatio(0.18, 0.18);
            UpdateMotorState(MOTOR_FRONT);
            SysTickDelay(225);
            UpdateMotorState(MOTOR_STOP);
            Motor_TurnRightNoInFlatGround(87);
        }
    }
    else if (FlagStop == 3)
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
void SevenTOOne(void)
{
}
void EightTOOne(void)
{
}

void _Start(void)
{
    if (SuperSonic_GetDistant() >= (float)1)
    {
        Flagstart = 1;
        printf("Hello");
    }
    SysTickDelay(100);
}
void TestOnly(void)
{
    BeginCount();
    while (Encoder_GetCurrentDistantCount() <= (float)4)
    {
        _GoLine();
        _FindPointGo();
    }
    UpdateMotorState(MOTOR_STOP);
    Stop = 1;
}
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

void _Reset(void)
{

    printf("reset");
    _resetlaser();
    //InitFlag();
}

#endif
#ifndef USE_Magic
#include "SKILL.h"
#define TEST2
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
            if (FlagUp == 0)
            {
                _GoLine();
                _FindPointGo();
                if (_UpHillOrDownHillFeedBack() == UP)
                {
                    UpdateMotorState(MOTOR_STOP);
                    BeginCount();
                    SysTickDelay(500);
                    _InitNowYawAngle();
                    FlagUp = 1;
                    UpdateMotorState(MOTOR_FRONT);
                }
            }
            else if (FlagUp == 1)
            {

                while (Encoder_GetCurrentDistantCount() <= (float)5)
                {
                    _GoBridge();
                    if (_UpHillOrDownHillFeedBack() == DOWN)
                    {
                        break;
                    }
                }
                UpdateMotorState(MOTOR_FRONT);
                FlagBridge = 0;
                _FlagBack = 1;
                FlagUp = 0;
            }
        }
    }
    else
    {
        _GoLine();
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
void TwoTOThree(void)
{
    if (UpgradeMotorState() == MOTOR_STOP && FlagStop != 2)
    {
        UpdateMotorState(MOTOR_FRONT);
        FlagStop++;
        SetMotorDutyRatio(0.04, 0.04);
        SysTickDelay(500);
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

                UpdateMotorState(MOTOR_FRONT);
                SetMotorDutyRatio(0.05, 0.05);
                ClearLLaserChangePendingBit();
                while (UpgradeMotorState() == MOTOR_FRONT)
                {
                    if (IsLLaserChange() == Changed)
                    {
                        UpdateMotorState(MOTOR_BACK);
                        SysTickDelay(200);
                        Motor_TurnRightBlockingMode(140);
                        FlagDown = 0;
                    }
                }
            }
        }
        else
        {
            _GoLineDelay(3700);
            UpdateMotorState(MOTOR_FRONT);
            ClearLLaserChangePendingBit();
            FlagDown = 1;
        }
    }
    else if (FlagStop == 2)
    {
        _GoLine();
        //_FindPointGo();
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
                FlagDown = 1;
            }
        }
        if (FlagDown == 1)
        {
            if (_UpHillOrDownHillFeedBack() == FlatGround)
            {
                _GoLine();
                _FindPointStop();
                if (UpgradeMotorState() == MOTOR_STOP)
                {
                    Motor_TurnRightBlockingMode(35);
                    FlagDown = 0;
                    //SetMotorDutyRatio(0.04,0.04);
                    //SysTickDelay(100);
                }
            }
            else if (_UpHillOrDownHillFeedBack() == DOWN)
            {
                _GoLineLowSpeed();
            }
        }
    }
}

void TwoTOFour()
{
    if (UpgradeMotorState() == MOTOR_STOP && FlagStop != 2)
    {
        UpdateMotorState(MOTOR_FRONT);
        FlagStop++;
        SetMotorDutyRatio(0.04, 0.04);
        SysTickDelay(500);
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

                UpdateMotorState(MOTOR_FRONT);
                SetMotorDutyRatio(0.05, 0.05);
                ClearLLaserChangePendingBit();
                while (UpgradeMotorState() == MOTOR_FRONT)
                {
                    if (IsLLaserChange() == Changed)
                    {
                        UpdateMotorState(MOTOR_BACK);
                        SysTickDelay(200);
                        Motor_TurnLeftBlockingMode(33);
                        FlagDown = 0;
                    }
                }
            }
        }
        else
        {
            _GoLineDelay(3700);
            UpdateMotorState(MOTOR_FRONT);
            ClearLLaserChangePendingBit();
            FlagDown = 1;
        }
    }
    else if (FlagStop == 2)
    {
        _GoLine();
        //_FindPointGo();
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
                _GoLine();
                _FindPointStop();
                if (UpgradeMotorState() == MOTOR_STOP)
                {
                    SetMotorDutyRatio(0.14,0.14);
                    UpdateMotorState(MOTOR_STOP);
                    SysTickDelay(200);
                    UpdateMotorState(MOTOR_BACK);
                    SysTickDelay(100);
                    UpdateMotorState(MOTOR_STOP);
                    Motor_TurnRightBlockingMode(37);
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
        UpdateMotorState(MOTOR_FRONT);
        FlagStop++;
        SetMotorDutyRatio(0.1, 0.1);
        SysTickDelay(300);
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
                    }
                }
            }
            else if (FlagDown == 1)
            {
                if (TurnFlag == 0)
                {
                    _GoLine();
                    _FindPointStop();
                    if (FlagPoint == 1)
                    {
                        UpdateMotorState(MOTOR_FRONT);
                        SetMotorDutyRatio(0.05, 0.05);
                        ClearLLaserChangePendingBit();
                        while (UpgradeMotorState() == MOTOR_FRONT)
                        {
                            _GoLine();
                            if (IsLLaserChange() == Changed)
                            {
                                UpdateMotorState(MOTOR_BACK);
                                SysTickDelay(100);
                                UpdateMotorState(MOTOR_STOP);
                                Motor_TurnLeftBlockingMode(90);
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
                ClearLLaserChangePendingBit();
                ClearRLaserChangePendingBit();
                UpdateMotorState(MOTOR_FRONT);
                SetMotorDutyRatio(0.04, 0.04);
                ClearLLaserChangePendingBit();
                while (UpgradeMotorState() == MOTOR_FRONT)
                {
                    if (IsLLaserChange() == Changed)
                    {
                        UpdateMotorState(MOTOR_STOP);
                        Motor_TurnLeftBlockingMode(90);
                        FlagDown = 0;
                    }
                }
            }
        }
        else if (FlagStop == 2)
        {
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
        UpdateMotorState(MOTOR_FRONT);
        FlagStop++;
        SetMotorDutyRatio(0.1, 0.1);
        SysTickDelay(300);
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
                    }
                }
            }
            else if (FlagDown == 1)
            {
                _GoLine();
                if (TurnFlag == 0)
                {
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
                    UpdateMotorState(MOTOR_FRONT);
                    SetMotorDutyRatio(0.1, 0.1);
                    if(GetRLaserStateAntiJitter()==LOW)
                    {
                        UpdateMotorState(MOTOR_BACK);
                        SetMotorDutyRatio(0.1, 0.1);
                        SysTickDelay(200);
                        UpdateMotorState(MOTOR_STOP);
                        Motor_TurnRightBlockingMode(45);
                        TurnFlag=0;
                        BeginCount();
                    }

                }
            }
        }
        else if (FlagStop == 1)
        {
            while(Encoder_GetCurrentDistantCount()<=(float)3)
            {
                _GoLine();
            }
            StopCount();
            _FindPointStop();
            if(UpgradeMotorState() == MOTOR_STOP)
            {
                UpdateMotorState(MOTOR_FRONT);
                SetMotorDutyRatio(0.1, 0.1);
                if(GetRLaserStateAntiJitter()==LOW)
                {
                    UpdateMotorState(MOTOR_BACK);
                    SetMotorDutyRatio(0.1, 0.1);
                    SysTickDelay(200);
                    UpdateMotorState(MOTOR_STOP);
                    Motor_TurnLeftBlockingMode(45);
                    BeginCount();
                }
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
void FiveTOSeven(void)
{
    const AdcData_t *adcData;
    adcData = UpdateButtom();
    uint32_t _Temp = 0;
    //  float _TempAngleLow=0;
    //  float _TempAngleHigh=0;
    for (int i = 0; i <= 7; i++)
    {
        _Temp += adcData->array[0][i];
    }
    if (UpgradeMotorState() == MOTOR_STOP && FlagStop != 2)
    {
        UpdateMotorState(MOTOR_FRONT);
        FlagStop++;
        SetMotorDutyRatio(0.07, 0.07);
        SysTickDelay(500);
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
                if (Encoder_GetCurrentDistantCount() > (float)2.7)
                {
                    //printf("1");
                    //printf("%f\r\n",Encoder_GetCurrentDistantCount());
                    ClearLLaserChangePendingBit();
                    FlagDistant = 1;
                    Encoder_StopDistantCount();
                    Encoder_ResetDistant();
                }
                if (FlagDistant == 1)
                {
                    _GoLine();
                    //printf("1");
                    if (GetLLaserStateAntiJitter() == LOW)
                    {
                        //printf("1");
                        UpdateMotorState(MOTOR_BACK);
                        SetMotorDutyRatio(0.15, 0.15);
                        SysTickDelay(200);
                        UpdateMotorState(MOTOR_STOP);

                        Motor_TurnLeftBlockingMode(90);
                        BeginCount();
                    }
                }
            }
        }
        else if (FlagStop == 1)
        {
            _GoLineLowSpeed();
            if(Encoder_GetCurrentDistantCount()>=(float)1)
            {
                if (_UpHillOrDownHillFeedBack() == UP)
                {
                    UpdateMotorState(MOTOR_STOP);
                    SysTickDelay(500);
                    FlagUp = 1;
                }
                if (FlagUp == 1)

                {
                    SetMotorDutyRatio(0.23, 0.23);
                    UpdateMotorState(MOTOR_FRONT);
                    SysTickDelay(245);
                    UpdateMotorState(MOTOR_STOP);
                    Motor_TurnLeftNoInFlatGround(75);
                    StopCount();
                    //_InitNowYawAngle();
                }
            }
        }
        else if (FlagStop == 2)
        {
            if (FlagBridge == 0)
            {
                _GoLine();

                if (_UpHillOrDownHillFeedBack() == FlatGround)
                {
                    FlagBridge = 1;
                    BeginCount();
                }
            }
            else
            {
                if (FlagDistant == 0)
                {

                    _GoLineMSpeed();
                    //printf("%f",Encoder_GetCurrentDistantCount());
                    //                  Encoder_ResetDistant();
                    //                  Encoder_StartDistantCount();
                    if (Encoder_GetCurrentDistantCount() >= (float)2)
                    {
                        FlagDistant = 1;
                        //                      Encoder_StopDistantCount();
                        //                      Encoder_ResetDistant();
                        StopCount();
                    }
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
    }
    else
    {
        //      Motor_TurnLeftBlockingMode(180);
        //      UpdateMotorState(MOTOR_FRONT);
        _FlagBack = 1;
    }
}
void TurnBack(void)
{
    Motor_TurnLeft180OnPlatform();
    if (UpgradeMotorState() == MOTOR_STOP)
    {
        Stop = 1;
    }
}

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
        UpdateMotorState(MOTOR_FRONT);
        FlagStop++;
        SetMotorDutyRatio(0.07, 0.07);
        SysTickDelay(350);
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
                        UpdateMotorState(MOTOR_BACK);
                        SetMotorDutyRatio(0.1, 0.1);
                        SysTickDelay(100);
                        UpdateMotorState(MOTOR_STOP);
                        //找到后立刻停车，并转弯，并且将motor_stop状态传达至下一轮询
                        Motor_TurnRightBlockingMode(85);
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
            if (_Temp <= 10000)
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

                UpdateMotorState(MOTOR_STOP);
                UpdateMotorState(MOTOR_BACK);
                SetMotorDutyRatio(0.1, 0.1);
                SysTickDelay(150);
                UpdateMotorState(MOTOR_STOP);
                Motor_TurnRightBlockingMode(90);
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
            //当右侧激光传感器被触发时，右转145°
            UpdateMotorState(MOTOR_STOP);
            Motor_TurnRightBlockingMode(135);
        }
        else
        {
            UpdateMotorState(MOTOR_FRONT);
        }
    }
    //第三点停到第四点停时
    else if (FlagStop == 3)
    {
        //当未检测上坡时，低速寻线
        if (FlagUp == 0)
        {
            _GoCricle();
            //当检测到上坡时，左转26矫正，保证上跷跷板时两前轮均能不悬空，并将flag置为1
            if (_UpHillOrDownHillFeedBack() == UP)
            {
                FlagUp = 1;
                Motor_TurnLeftBlockingMode(20);
                UpdateMotorState(MOTOR_FRONT);
            }
        }
        //当flagup为1时，实行轮速锁定，保证速度不会过低
        else if (FlagUp == 1)
        {
            _BlockSpeed();
            //一直保持转台直至回到平地。因为我们的陀螺仪因为防抖需要所以在有一部分还在跷跷板时就会认为在平地上，所以先需要向前行进一段距离做矫正
            //左转40°因为圆的线方向与上桥时相比已经有了变化。还需要将flagup置为2
            if (_UpHillOrDownHillFeedBack() == FlatGround)
            {
                FlagUp = 2;
                Encoder_ResetDistant();
                Encoder_StartDistantCount();
                UpdateMotorState(MOTOR_FRONT);
                SetMotorDutyRatio(0.14, 0.14);
                SysTickDelay(200);
                Motor_TurnLeftBlockingMode(40);
                SysTickDelay(100);
                UpdateMotorState(MOTOR_FRONT);
            }
        }

        //当flagup为2时执行低速寻线0.5，这里的执行距离是防止刚下跷跷板后的车处于非稳定状态误判入点
        else if (FlagUp == 2)
        {
            if (FlagBalance == 0)
            {
                //printf("%f\r\n",Encoder_GetCurrentDistantCount());
                _GoLineLowSpeed();
                if (Encoder_GetCurrentDistantCount() >= (float)0.5)
                {
                    //printf("1\r\n");
                    //此处的入点方式为临时入点，原因为此处边缘仅为1，与点模式的通用描述不同
                    if (_Temp <= 20000)
                    {
                        UpdateMotorState(MOTOR_STOP);
                        ClearLLaserChangePendingBit();
                    }
                }
                //由临时点模式造成的电机停有时，继续前进，直至左侧激光管检测到线，并将flagbalance置为1
                if (UpgradeMotorState() == MOTOR_STOP)
                {
                    //printf("2\r\n");
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
                UpdateMotorState(MOTOR_STOP);
                Motor_TurnLeftBlockingMode(90);
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
            _GoLine();
            if (_UpHillOrDownHillFeedBack() == UP)
            {
                FlagUp = 1;
            }
        }
        //上坡后执行到台停止函数
        else
        {
            _ArrivePlatform();
        }
    }
}
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
        UpdateMotorState(MOTOR_FRONT);
        FlagStop++;
        SetMotorDutyRatio(0.07, 0.07);
        SysTickDelay(350);
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
            _GoLine();
            _FindPointStop();
            if (UpgradeMotorState() == MOTOR_STOP)
            {
                UpdateMotorState(MOTOR_FRONT);
                while (IsLLaserChange() == UnChange)
                {
                    _GoLine();
                }
                UpdateMotorState(MOTOR_STOP);
                Motor_TurnLeftBlockingMode(45);
                BeginCount();
            }
        }
    }
    else if (FlagStop == 1)
    {
        if (FlagDistant == 0)
        {

            if (Encoder_GetCurrentDistantCount() <= (float)3)
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
            //          while(GetLLaserStateAntiJitter()==HIGH)
            //            {
            //							Encoder_ResetDistant();
            //            }
            //          UpdateMotorState(MOTOR_STOP);
            //          Encoder_StartDistantCount();
            //          UpdateMotorState(MOTOR_BACK);
            //          SetMotorDutyRatio(0.1,0.1);
            //					SysTickDelay(150);
            //					if(GetLLaserStateAntiJitter()==HIGH)
            //					{
            //						FlagPoint=1;
            //          }
            //					if(FlagPoint==1)
            //					{
            //					if(GetLLaserStateAntiJitter()==LOW)
            //            {
            //
            //              UpdateMotorState(MOTOR_STOP);
            //              TempDistant=Encoder_GetCurrentDistantCount();
            //              UpdateMotorState(MOTOR_FRONT);
            //              SetMotorDutyRatio(0.1,0.1);
            //							printf("%f\r\n",TempDistant);
            //              StopCount();
            //              BeginCount();
            //            }
            //          if(Encoder_GetCurrentDistantCount()<=(TempDistant/(float)2.0+(float)0.02)&&Encoder_GetCurrentDistantCount()>(TempDistant/(float)2.0-(float)0.02))
            //            {
            //              UpdateMotorState(MOTOR_STOP);
            //              Motor_TurnRightBlockingMode(45);
            //							StopCount();
            //            }
            //					}
            UpdateMotorState(MOTOR_FRONT);
            SysTickDelay(150);
            UpdateMotorState(MOTOR_STOP);
            Motor_TurnRightBlockingMode(37);
            BeginCount();
        }
    }
    else if (FlagStop == 2)
    {
        _GoLine();
        if (Encoder_GetCurrentDistantCount() >= (float)1)
        {
            UpdateMotorState(MOTOR_STOP);
            StopCount();
        }
        //			if(GetRLaserStateAntiJitter()==LOW)
        //			{
        //				UpdateMotorState(MOTOR_STOP);
        //			}

        if (UpgradeMotorState() == MOTOR_STOP)
        {
            //_GoLineDelay(200);
            SetMotorDutyRatio(0.18, 0.15);
            UpdateMotorState(MOTOR_FRONT);
            SysTickDelay(150);
            UpdateMotorState(MOTOR_STOP);
            Motor_TurnRightBlockingMode(145);
            UpdateMotorState(MOTOR_FRONT);
            SetMotorDutyRatio(0.1, 0.1);
            SysTickDelay(200);
            UpdateMotorState(MOTOR_STOP);
        }
        else
        {
            UpdateMotorState(MOTOR_FRONT);
        }
    }
    else if (FlagStop == 3)
    {
        if (FlagUp == 0)
        {
            _GoLineLowSpeed();
            if (_UpHillOrDownHillFeedBack() == UP)
            {
                FlagUp = 1;
                Motor_TurnLeftBlockingMode(20);
                SysTickDelay(100);
                UpdateMotorState(MOTOR_FRONT);
                Encoder_ResetDistant();
                Encoder_StartDistantCount();
            }
        }
        else if (FlagUp == 1)
        {
            _BlockSpeed();
            if (_UpHillOrDownHillFeedBack() == FlatGround)
            {
                FlagUp = 2;
                Encoder_ResetDistant();
                Encoder_StartDistantCount();
                UpdateMotorState(MOTOR_FRONT);
                SetMotorDutyRatio(0.17, 0.17);
                SysTickDelay(250);
                UpdateMotorState(MOTOR_BACK);
                SetMotorDutyRatio(0.1, 0.1);
                Motor_TurnLeftBlockingMode(45);
                SysTickDelay(100);
                UpdateMotorState(MOTOR_FRONT);
            }
        }
        else if (FlagUp == 2)
        {
            if (FlagBalance == 0)
            {
                //printf("%f\r\n",Encoder_GetCurrentDistantCount());
                _GoLineLowSpeed();
                if (Encoder_GetCurrentDistantCount() >= (float)0.5)
                {
                    //printf("1\r\n");
                    if (_Temp <= 20000)
                    {
                        UpdateMotorState(MOTOR_STOP);
                        ClearLLaserChangePendingBit();
                    }
                }
                if (UpgradeMotorState() == MOTOR_STOP)
                {
                    //printf("2\r\n");
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
            else
            {
                UpdateMotorState(MOTOR_STOP);
                Motor_TurnLeftBlockingMode(90);
                FlagUp = 0;
            }
        }
    }
    else if (FlagStop == 4)
    {
        if (FlagUp == 0)
        {
            _GoLine();
            if (_UpHillOrDownHillFeedBack() == UP)
            {
                FlagUp = 1;
            }
        }
        else
        {
            _ArrivePlatform();
        }
    }
}
void FiveTOEight(void)
{
    const AdcData_t *adcData;
    adcData = UpdateButtom();
    uint32_t _Temp = 0;
    //  float _TempAngleLow=0;
    //  float _TempAngleHigh=0;
    for (int i = 0; i <= 7; i++)
    {
        _Temp += adcData->array[0][i];
    }
    if (UpgradeMotorState() == MOTOR_STOP && FlagStop != 2)
    {
        UpdateMotorState(MOTOR_FRONT);
        FlagStop++;
        SetMotorDutyRatio(0.07, 0.07);
        SysTickDelay(500);
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
                if (Encoder_GetCurrentDistantCount() > (float)2.7)
                {
                    //printf("1");
                    //printf("%f\r\n",Encoder_GetCurrentDistantCount());
                    ClearLLaserChangePendingBit();
                    FlagDistant = 1;
                    Encoder_StopDistantCount();
                    Encoder_ResetDistant();
                }
                if (FlagDistant == 1)
                {
                    _GoLine();
                    //printf("%d\r\n",LeftCountTimes)
                    //printf("1");
                    if (GetLLaserStateAntiJitter() == LOW)
                    {
                        //printf("1");
                        UpdateMotorState(MOTOR_STOP);
                        Motor_TurnLeftBlockingMode(90);
                        BeginCount();
                    }
                }
            }
        }
        else if (FlagStop == 1)
        {
            _GoLineLowSpeed();
            if(Encoder_GetCurrentDistantCount()>=(float)1)
            {
                if (_UpHillOrDownHillFeedBack() == UP)
                {
                    UpdateMotorState(MOTOR_STOP);
                    SysTickDelay(500);
                    FlagUp = 1;
                }
            }
            if (FlagUp == 1)

            {
                SetMotorDutyRatio(0.18, 0.18);
                UpdateMotorState(MOTOR_FRONT);
                SysTickDelay(245);
                UpdateMotorState(MOTOR_STOP);
                Motor_TurnRightNoInFlatGround(75);

            }
        }
        else if (FlagStop == 2)
        {
            if (FlagBridge == 0)
            {
                _GoLine();
                if (_UpHillOrDownHillFeedBack() == FlatGround)
                {
                    FlagBridge = 1;
                }
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
    else
    {
        //      Motor_TurnLeftBlockingMode(180);
        //      UpdateMotorState(MOTOR_FRONT);
        _FlagBack = 1;
    }
}
void SixTOSeven(void)
{
    const AdcData_t *adcData;
    adcData = UpdateButtom();
    uint32_t _Temp = 0;
    //  float _TempAngleLow=0;
    //  float _TempAngleHigh=0;
    for (int i = 0; i <= 7; i++)
    {
        _Temp += adcData->array[0][i];
    }
    if (UpgradeMotorState() == MOTOR_STOP && FlagStop != 3)
    {
        UpdateMotorState(MOTOR_FRONT);
        FlagStop++;
        SetMotorDutyRatio(0.07, 0.07);
        SysTickDelay(500);
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
                _GoLine();
                _FindPointStop();
                if (UpgradeMotorState() == MOTOR_STOP)
                {
                    //printf("1");
                    UpdateMotorState(MOTOR_FRONT);
                    while (GetLLaserStateAntiJitter() != LOW)
                    {
                        _GoLine();
                    }
                    UpdateMotorState(MOTOR_BACK);
                    SetMotorDutyRatio(0.1, 0.1);
                    SysTickDelay(150);
                    UpdateMotorState(MOTOR_STOP);
                    Motor_TurnLeftBlockingMode(105);
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
            while (Encoder_GetCurrentDistantCount() <= (float)3.2)
            {
                //            if(FlagPoint==2)
                //            {
                //                _GoLineDelay(100);
                //                _FindPointGo();
                //								FlagPoint=0;
                //            }
                //            else
                //            {
                //                _GoLineLowSpeed();
                //                _FindPointGo();
                //            }
                //
                //printf("1");
                //_GoCricle();
                _GoCricleSpeedLock();
            }
            StopCount();
            BeginCount();
            while (Encoder_GetCurrentDistantCount() <= (float)1.5)
            {
                _GoLine();
                if (FlagPoint == 0)
                {
                    _FindPointGo();
                    if (FlagPoint == 2)
                    {
                        Motor_TurnRightBlockingMode(10);
                        UpdateMotorState(MOTOR_FRONT);
                        SetMotorDutyRatio(0.1, 0.1);
                        SysTickDelay(200);
                        _GoLineDelay(500);
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
                UpdateMotorState(MOTOR_BACK);
                SetMotorDutyRatio(0.1, 0.1);
                SysTickDelay(250);
                UpdateMotorState(MOTOR_STOP);

                Motor_TurnRightBlockingMode(90);
            }
        }
    }
    else if (FlagStop == 2)
    {
        _GoLineMSpeed();
        if (_UpHillOrDownHillFeedBack() == UP)
        {
            UpdateMotorState(MOTOR_STOP);
            SysTickDelay(500);
            FlagUp = 1;
        }
        if (FlagUp == 1)

        {
            SetMotorDutyRatio(0.18, 0.18);
            UpdateMotorState(MOTOR_FRONT);
            SysTickDelay(225);
            UpdateMotorState(MOTOR_STOP);
            Motor_TurnLeftNoInFlatGround(87);
            FlagUp=0;
        }
    }
    else if (FlagStop == 3)
    {
        if (FlagDistant == 0)
        {

            while (Encoder_GetCurrentDistantCount() <= (float)2.7)
            {
                if(_UpHillOrDownHillFeedBack() == UP)
                {
                    FlagUp=1;
                }
                if(FlagUp==1)
                {
                    _GoLineMSpeed();
                }
                else
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
void SixTOEight(void)
{
    const AdcData_t *adcData;
    adcData = UpdateButtom();
    uint32_t _Temp = 0;
    for (int i = 0; i <= 7; i++)
    {
        _Temp += adcData->array[0][i];
    }
    if (UpgradeMotorState() == MOTOR_STOP && FlagStop != 3)
    {
        UpdateMotorState(MOTOR_FRONT);
        FlagStop++;
        SetMotorDutyRatio(0.07, 0.07);
        SysTickDelay(500);
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
                _GoLine();
                _FindPointStop();
                if (UpgradeMotorState() == MOTOR_STOP)
                {
                    //printf("1");
                    UpdateMotorState(MOTOR_FRONT);
                    while (GetLLaserStateAntiJitter() != LOW)
                    {
                        _GoLine();
                    }
                    UpdateMotorState(MOTOR_BACK);
                    SetMotorDutyRatio(0.1, 0.1);
                    SysTickDelay(150);
                    UpdateMotorState(MOTOR_STOP);
                    Motor_TurnLeftBlockingMode(105);
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
            while (Encoder_GetCurrentDistantCount() <= (float)3.2)
            {
                _GoCricle();
            }
            StopCount();
            BeginCount();
            while (Encoder_GetCurrentDistantCount() <= (float)1.5)
            {
                _GoLine();
                if (FlagPoint == 0)
                {
                    _FindPointGo();
                    if (FlagPoint == 2)
                    {
                        Motor_TurnRightBlockingMode(15);
                        UpdateMotorState(MOTOR_FRONT);
                        SetMotorDutyRatio(0.1, 0.1);
                        SysTickDelay(200);
                        _GoLineDelay(500);
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
                UpdateMotorState(MOTOR_BACK);
                SetMotorDutyRatio(0.1, 0.1);
                SysTickDelay(250);
                UpdateMotorState(MOTOR_STOP);

                Motor_TurnRightBlockingMode(90);
            }
        }
    }
    else if (FlagStop == 2)
    {
        _GoLineMSpeed();
        if (_UpHillOrDownHillFeedBack() == UP)
        {
            UpdateMotorState(MOTOR_STOP);
            SysTickDelay(500);
            FlagUp = 1;
        }
        if (FlagUp == 1)

        {
            SetMotorDutyRatio(0.18, 0.18);
            UpdateMotorState(MOTOR_FRONT);
            SysTickDelay(225);
            UpdateMotorState(MOTOR_STOP);
            Motor_TurnRightNoInFlatGround(87);
        }
    }
    else if (FlagStop == 3)
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
void SevenTOOne(void)
{
}
void EightTOOne(void)
{
}

void _Start(void)
{
    if (SuperSonic_GetDistant() >= (float)1)
    {
        Flagstart = 1;
        printf("Hello");
    }
    SysTickDelay(100);
}
void TestOnly(void)
{
	#ifndef TESTBUTTOM
	_GoLine();
	#endif
	#ifdef TESTDISTENCE
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
	printf("lspeed: %f     rspeed: %f\r\n",Encoder_GetLWheelSpeed(),Encoder_GetRWheelSpeed());
	#endif
	#ifdef TESTBUTTOM
	const AdcData_t *adcData;
  adcData = UpdateButtom();
  
    for (int i = 0; i <= 7; i++)
    {
        printf("%5d , ",adcData->array[0][i]);
    }
		printf("\r\n");
		SysTickDelay(500);
	#endif
	#ifdef TESTOTHER
	
	#endif
}
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
                while(Encoder_GetCurrentDistantCount()<=(float)3)
                {
                    _GoLineMSpeed();
                }
                FlagDistant=1;
                StopCount();
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
void SevenTOEight(void)
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
                while(Encoder_GetCurrentDistantCount()<=(float)3)
                {
                    _GoLineMSpeed();
                }
                FlagDistant=1;
                StopCount();
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
void _Reset(void)
{

    printf("reset");
    _resetlaser();
    //InitFlag();
}

#endif
