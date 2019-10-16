/*
** 文件名称：magic.h
** 文件作用：存放魔法值
** 文件说明：首先对不起大家 我们的算法鲁棒性还不够强大 由于每辆车
** 文件说明：机械结构与电路结构上都有着微小的差距 这些差距在运行
** 文件说明：某些代码的时候将会出现不可预期的问题 因此我们单独把
** 文件说明：需要调整的地方以宏定义的形式暴露在这个文件中
** 文件说明：最后每辆车调试出问题的时候大家直接改这个文件内的宏
** 文件说明：就好 就不需要去动源码了
*/

/* ------------------------------ magic.h文件目录 -------------------------------- */

//文件目录使用方法 每一个块都有自己唯一的 ID 用MDK的搜索功能搜这个ID就能快速跳转到相关位置

/*
** 0.系统节拍        kgQspdAefszmz5HV
** 1.电机左右转相关  2hby9sdnBTYZwuMg
** 2.正常速度巡线    wzuwRXAlVO639rUJ
** 3.低速巡线 下坡   McEPX5An7HQha6cD
** 4.过桥惯性PID     avomfHCHqNaH3Uiq
** 5.轮速锁PID       fwSF4xfWhlejft0W
** 6.超声波上台距离  xT5LF0x6Tuy3ituI
** 7.圆内平地PID
** 8.圆内障碍PID
*/

/*
** 0.一到二脚本      sJpPTeUZVDXseiAm
** 1.电机左右转相关  2hby9sdnBTYZwuMg
** 2.正常速度巡线    wzuwRXAlVO639rUJ
** 3.低速巡线 下坡   McEPX5An7HQha6cD
** 4.过桥惯性PID     avomfHCHqNaH3Uiq
** 5.轮速锁PID       fwSF4xfWhlejft0W
** 6.超声波上台距离  xT5LF0x6Tuy3ituI
** 7.圆内平地PID
** 8.圆内障碍PID
*/

#ifndef MAGIC_H_
#define MAGIC_H_


/*各个传感器测试*/

//#define TESTDISTENCE  //编码器距离反馈
//#define TESTSPEED			//左右编码器速度反馈
//#define TESTBUTTOM		//灰度传感器反馈
//#define TESTOTHER			//有关传感器测试
/*测试完成后将define注释掉*/
/* ------------------------------------------------------------------------------- */

/* kgQspdAefszmz5HV */

/*
** 系统节拍调整 这里可以调整系统的节拍 系统节拍是指系统多久轮询一次主循环
**
** 注意                    ****该项一般不用调整****
** 系统的节拍默认为10 单位ms
*/
#define State_MachineTickTime 10



/* ------------------------------------------------------------------------------- */
/* 2hby9sdnBTYZwuMg */

/*
** 电机转向相关魔法 更改下面两个值将改变原地向左向右转时
** 左右电机的PWM 最终反应在车辆的转弯角度 以及车辆转弯的
** 位置精确度上(也就是车辆转弯完后相对地面位置是否改变)
**
** 调整方法：观察车在场地上的转弯情况 在排除外部摩擦力
** 等场地因素以后 确认是程序原因以后 观察车辆的转弯情况
**
** 如果在左转的时候车辆相对转向原来的位置左偏 则提示
** 右边轮子过快 左边轮子过慢 需要调整相对速度到实际测试
** 不太偏移为止
**
** 右转一样 不再论述
*/

/* 左转时进入函数等待陀螺仪稳定的时间 */
#define MOTOR_TurnLeft_StableTime 100

/* 左转时左右轮PWM决定 */
#define MOTOR_TurnLeft_LeftPWM  (float)0.14
#define MOTOR_TurnLeft_RightPWM (float)0.14

/* 左转时粗调到微调延迟 和每一辆车的陀螺仪稳定性有关 单位ms */
#define MOTOR_TurnLeft_WaitTime 200

/* 左转时最后微调的PWM决定 */
#define MOTOR_TurnLeft_LeftPWM_ADJ  (float) 0.12
#define MOTOR_TurnLeft_RightPWM_ADJ (float) 0.12

/* ------------------------------------------------------------------------------- */

/* 左转时进入函数等待陀螺仪稳定的时间 */
#define MOTOR_TurnRight_StableTime 100

/* 右转时左右轮PWM决定 */
#define MOTOR_TurnRight_LeftPWM  (float)0.14
#define MOTOR_TurnRight_RightPWM (float)0.14

/* 右转时粗调到微调延迟 和每一辆车的陀螺仪稳定性有关 单位ms */
#define MOTOR_TurnRight_WaitTime 200

/* 右转时最后微调的PWM决定 */
#define MOTOR_TurnRight_LeftPWM_ADJ  (float) 0.12 
#define MOTOR_TurnRight_RightPWM_ADJ (float) 0.12

/* ------------------------------------------------------------------------------- */



/*
** 机器人PID巡线相关魔法 改变以下魔法值将改变车辆的巡线行为
** 具体PID调整请咨询首席魔法师PCH同学以及首席PID参数整定工程
** 师HYT队长
*/

/* ------------------------------------------------------------------------------- */

/* wzuwRXAlVO639rUJ */
/* 正常速度PID的各项参数 */
#define KpHightSpeed      (float)137.763
#define KiHightSpeed      (float)10
#define KdHightSpeed      (float)22.5

/* 正常巡线左右PWM值 每辆车的电机有差异 会导致左偏或者右偏 根据实际情况调整 */
/*左边*/
#define HightSpeedPID_LPWM (float)0.21
/*右边*/
#define HightSpeedPID_RPWM (float)0.21

/* ------------------------------------------------------------------------------- */

/* McEPX5An7HQha6cD */

/* 低速巡线PID的各项参数  下长坡的PID也由以下值控制 */
#define KpLowSpeed        (float)109.763
#define KiLowSpeed        (float)0.789
#define KdLowSpeed        (float)9.6
/* 低速巡线PWM值 */
#define LowSpeedPID_PWM   (float)0.06

/* 下长坡应该刹车的速度 单位m/s 每辆车调整能力不一样 要单独调 */
#define DownBridgeBrakeSpeed (float)0.4
/* 下长坡每次刹车的刹车时间 影响下桥速度 */
#define DownBridgeBrakeTime  (float)20
/* 下长坡车辆巡线用的PWM值 影响下桥速度与车辆自然下桥时是否偏移中线 */
/*左边*/
#define DownBridge_LPWM        (float)0.06
/*右边*/
#define DownBridge_RPWM        (float)0.06
	
/* ------------------------------------------------------------------------------- */

/* avomfHCHqNaH3Uiq */

/* 过桥惯性导航PID的各项参数 */
#define KpBridge          (float)1.53
#define KiBridge          (float)0.9
#define KdBridge          (float)0.045
/* 过桥惯性导航PWM值 影响过桥效果与过桥速度 */
#define BridgePID_PWM     (float)0.14

/* ------------------------------------------------------------------------------- */

/*
** 机器人PID速度锁相关魔法 改变以下魔法值将改变车辆的速度锁
** 具体PID调整请咨询首席魔法师PCH同学以及首席PID参数整定工程
** 师HYT队长
*/
/* ------------------------------------------------------------------------------- */

/* fwSF4xfWhlejft0W */

/* 轮速锁目标速度设定 单位m/s */
#define BlockSpeed_TargetSpeed (float)0.4

/* 轮速锁PID值 */
#define BlockSpeed_Kp (float)0.475
#define BlockSpeed_Ki (float)0.05
#define BlockSpeed_Kd (float)0.60

/* 轮速锁刹车时间决定 影响轮速锁刹车效果 */
#define BlockSpeed_BrakeTime 75

/* ------------------------------------------------------------------------------- */

/*
** 到台超声波停距离控制魔法 和车上台后距离图像的位置有关系 此此魔法值应该与上位机连调
** 以确定最佳图像识别距离
*/

/* xT5LF0x6Tuy3ituI */

/* 距离要识别图像的最远距离 超声波决定 */
#define SuperSonic_MaxDistant (float)0.24

/* 距离要识别图像的最近距离 超声波决定 */
#define SuperSonic_MinDistant (float)0.22

/* 调整车辆位置的时间步长 越小越精确 但是越慢 越大越迅速越粗暴 */
#define SuperSonic_StepTime   35

/* 超声波采样一次后的等待时间 越长越慢 但是被干扰的可能性越小 */
#define SuperSonic_WaitTime   65

/* 超声波获取距离时用于计算的声速 我们的超声波模块不带温度补偿 所以要设置 */
/* 注意 该值应 / 1000000  例如340m/s 填 0.000340 */
#define SuperSonic_AcousticVelocity (float)0.000341
/*
c＝（331.45＋0.61t／℃）m•s-1
（其中331.45是在0度时声音在空气中的速度）
*/
/* ------------------------------------------------------------------------------- */
/*因为我们的寻线算法在点周围会受到附近线干扰，所以每一次停下转弯后都需要向前走一段距离*/
/*以下两个参数为向前走的距离和速度*/
#define Angle_GoSpeed 0.07
#define Angle_GoTime 350

/*脚本一台到二台   sJpPTeUZVDXseiAm*/
/*如果在桥面上冲出去，请修改过桥PID*/
/* ------------------------------------------------------------------------------- */

/*脚本二台到三台   sJpPTeUZVDXseiAm*/

#endif
