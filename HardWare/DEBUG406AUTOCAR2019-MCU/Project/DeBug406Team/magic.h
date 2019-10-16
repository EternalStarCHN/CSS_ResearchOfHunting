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


/* 5.20有问题的部分 */
//6到8 需要在J点转弯完成 J到K1点过程中加入距离屏蔽 UWkvBu34fETLOVPl 待处理
//8回家 需要准备两段下坡的方案 GIJyiepWxf8jACE5
//7回家 目前7回家的脚本没有在过波浪板的时候采用魔鬼速度  W6uo5v4hNcZ4Co7J
//36 46 到6下跷跷板以后没有加距离保护？


/* 5.21凌晨修改 */
/*

1.完成8回家
2.完善了昨晚新加的桥上巡线部分注释
3.将认为禁行板移开的距离从1改到了1.5m _Start() 函数内
4.8回家函数最后上台不执行台停函数 改为直接停 理论上认为自己在平地上时已经有两个轮子在1台上
5.新版本1到2更新 需要确认 PSoVUrROsjoqsYpD 处作用
6.重新审核了23 24的魔法调整部分 需要再审核
7.按照robot1的参数根据经验脑补了第二辆车桥上的PID参数 需要测试 需要调整真值
8.完成了7回家脚本 需要审核
9.修正了4到6进入H1点前没有向前调整的bug
*/
//5.22 
// 增加灰度 amgRzGhLttHYFbAq
//          UWkvBu34fETLOVPl
/* 本人要改的地方 */

/*

1. 机械结构紧跟一号车

*/


/* ------------------------------ magic.h文件目录 -------------------------------- */

//文件目录使用方法 每一个块都有自己唯一的 ID 用MDK的搜索功能搜这个ID就能快速跳转到相关位置

/*
** 0.系统节拍        kgQspdAefszmz5HV
** 1.电机左右转相关  2hby9sdnBTYZwuMg
** 6.超声波上台距离  xT5LF0x6Tuy3ituI
** 9.重复点屏蔽      EMm8KScNSf9QYWIe
**10.到点停灵敏度    n2W5G6bSa8Qr0uqj
**11.到点走灵敏度    2sqxbGSHEwcFX7U8
**12.上下坡角度判断  LV7yRKlQDzAIU4zi
*/

/* 巡线相关调整 直接放在line.c的头部 位置在 jcbCKhyF58RkX0sc */

/*
** 0.一到二脚本      sJpPTeUZVDXseiAm ok
** 1.二到三脚本      azV0GQIuZzDBKbMv ok 
** 2.二到四脚本      CAAbcy2WG5JoxkiK ok 
** 3.三到五脚本      nyENPn28cKQryLOa ok
** 4.三到六脚本      fjM6u9QfFW8tyxXe ok
** 5.四到五脚本      u4Gx8UHr7cLNm9vx ok
** 6.四到六脚本      phSQ5lmIP3yHcL3d ok
** 7.五到七脚本      XArl9R4c24mpOdV7 修改完成 待验证 明天必须验证
** 8.五到八脚本			 U3G5wElpXhQEFEko 修改完成 待验证 明天必须验证
** 9.六到七脚本      mxwQ7fKNWuzmJxIr 修改完成 待验证 明天必须验证
**10.六到八脚本      FenDbtKY0Nm1cczd 写完 等地审核
**11.七到八脚本      xZkAW9WcmNnjXPMZ 初赛不使用
**12.八到七脚本      ItZJR51hV6Zh2u74 初赛不使用
**13.七回家脚本      SiSEGyvDbjUX3EpF 写完 等待审核
**14.八回家脚本      8s3Y6D3c51OSbk87 写完 等待审核
*/
#ifndef MAGIC_H_
#define MAGIC_H_

/*各个传感器测试*/

//#define TESTDISTENCE  //编码器距离反馈
//7#define TESTSPEED			//左右编码器速度反馈
//#define TESTBUTTOM		//灰度传感器反馈
//#define TESTGYRO                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   //陀螺仪测试
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
#define MOTOR_TurnLeft_LeftPWM (float)0.22
#define MOTOR_TurnLeft_RightPWM (float)0.22

/* 左转时粗调到微调延迟 和每一辆车的陀螺仪稳定性有关 单位ms */
#define MOTOR_TurnLeft_WaitTime 200

/* 左转时最后微调的PWM决定 */
#define MOTOR_TurnLeft_LeftPWM_ADJ (float)0.18
#define MOTOR_TurnLeft_RightPWM_ADJ (float)0.18

/* ------------------------------------------------------------------------------- */

/* 左转时进入函数等待陀螺仪稳定的时间 */
#define MOTOR_TurnRight_StableTime 100

/* 右转时左右轮PWM决定 */
#define MOTOR_TurnRight_LeftPWM (float)0.22
#define MOTOR_TurnRight_RightPWM (float)0.22

/* 右转时粗调到微调延迟 和每一辆车的陀螺仪稳定性有关 单位ms */
#define MOTOR_TurnRight_WaitTime 200

/* 右转时最后微调的PWM决定 */
#define MOTOR_TurnRight_LeftPWM_ADJ (float)0.18
#define MOTOR_TurnRight_RightPWM_ADJ (float)0.18




/* ------------------------------------------------------------------------------- */




/* 在台上原地左转掉头180°相关魔法 */

/* 台上原地掉头的角度 更改该值会影响车辆在台上掉头后的角度 */
#define MOTOR_TurnAroundOnPlatformAngle 180u

/* 台上原地掉头左边 右边电机的PWM值 更改这两个值将影响台上掉头的转弯中心以及角度 */
#ifdef Robot_1
#define MOTOR_TurnAroundOnPlatformLPWM (float)0.20

#define MOTOR_TurnAroundOnPlatformRPWM (float)0.17
#endif

#ifdef Robot_2
#define MOTOR_TurnAroundOnPlatformLPWM (float)0.20

#define MOTOR_TurnAroundOnPlatformRPWM (float)0.35
#endif


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
#define SuperSonic_StepTime 35

/* 超声波采样一次后的等待时间 越长越慢 但是被干扰的可能性越小 */
#define SuperSonic_WaitTime 65

/* 超声波获取距离时用于计算的声速 我们的超声波模块不带温度补偿 所以要设置 */
/* 注意 该值应 / 1000000  例如340m/s 填 0.000340 */
#define SuperSonic_AcousticVelocity (float)0.000341
/*
c＝（331.45＋0.61t／℃）m•s-1
（其中331.45是在0度时声音在空气中的速度）
*/
/* ------------------------------------------------------------------------------- */

/* 重复点屏蔽  EMm8KScNSf9QYWIe */

/*因为我们的寻线算法在点周围会受到附近线干扰，所以每一次停下转弯后都需要向前走一段距离*/
/*以下两个参数为向前走的距离和速度*/
#define Angle_GoSpeed 0.07
#define Angle_GoTime 350


/* ------------------------------------------------------------------------------- */

/* 上下坡临界角度判断 LV7yRKlQDzAIU4zi */

/* 上坡判断 */
//陀螺仪上仰是从360减到180 因此Min值会比Max值低
//因此min是最低上坡判断角度 Max是最高上坡判断角度
#define UpHillOrDownHill_UpHill_Max_PitchAngle (float)300
#define UpHillOrDownHill_UpHill_Min_PitchAngle (float)358


/* 下坡判断 */
//陀螺仪探头是从0增加到360 因此min是最低下坡判断角度 Max是最高上坡判断角度
#define UpHillOrDownHill_DownHill_Max_PitchAngle (float)100
#define UpHillOrDownHill_DownHill_Min_PitchAngle (float)6


//Roll角稳定判断 基本不用
//#define UpHillOrDownHill_Balance_Max_RollAngle (float)
//#define UpHillOrDownHill_Balance_Min_RollAngle (float)


/* ------------------------------------------------------------------------------- */

/* n2W5G6bSa8Qr0uqj */
/* 到点停灵敏度调节 快速定位相关代码 jNFDd3lSDTRLlU04 */
#define FindPointStopSensitivity 3


/* 2sqxbGSHEwcFX7U8 */
/* 到点走灵敏度调节 快速定位相关代码 qR9Ifrq4I9Jmbv5b */
#define FindPointGoSensitivity   4


/* ----------------------------------往下为脚本设置------------------------------- */





/* ------------------------------------------------------------------------------- */


/*脚本一台到二台   sJpPTeUZVDXseiAm*/
/*如果在桥面上冲出去，请修改过桥PID*/
/*如果采用灰度在桥面上飞出去 关注桥上灰度PID 以及冲出去的位置 可能提示距离屏蔽原因*/


/* **********************************陀螺仪方案参数******************************* */

/* 一到二上桥前获取参考角度等待陀螺仪稳定的时间 单位ms  NFYMCQgYukXJtPmr */
#define OneToTwo_GyroStableTime 500

/* 一到二上桥后距离屏蔽下坡 单位m 定位 eEYIjRWy5DlDoIvl*/
#define OneToTwo_OnBridgeDistant (float)2


/* **********************************灰度方案参数********************************* */

/* 1到2 灰度方案 桥上行驶的总距离 过大过小都将跑飞 al4oxNYUmsKd23qX */
#define OneToTwo_GraySolution_OnBridgeDistant (float) 1.275

/* 1到2 灰度方案 刚上桥采用正常巡线过度时间 */
#define OneToTwo_GraySolution_UpBridgeGolineTime 400

/* ------------------------------------------------------------------------------- */

/*脚本二台到三台 azV0GQIuZzDBKbMv */

/* 巡点转弯后屏蔽同一个点直行时间 快速定位代码 rG2wkjTY1yylRc5z */
#define TwoTOThree_PointCoverTime  500

/* 巡点转弯后屏蔽同一个点直行速度 与上一个设置位置一样 */
#define TwoTOThree_PointCoverSpeed (float)0.1



/* 二台到三台A点转弯相关  osNlUTo9kCsr9Ldm 该ID可以快速链接到相关代码 */

/* 2到3 A点寻到后前进时间 */
#define TwoTOThree_A_PointBackTime 50

/* 2到3 A点寻到后前进速度 */
#define TwoTOThree_A_PointBackSpeed (float)0.14

/* 2到3 A点寻到后转弯角度 */
#define TwoTOThree_A_PointTurnAngle (float)27




/* 脚本二台到三台 A点到B点转移过程中过障碍延时巡线时间 oS7ogBfylETMiD18 该ID可以快速链接到相关代码 */
#define TwoTOThree_PointAtoB_GolineDelayTime 3500




/* 二台到三台B点转弯相关 lGcWKjU4fM0l8XG9 该ID可以快速链接到相关代码 */

/* 2到3 B点寻到后后退时间 */
#define TwoTOThree_B_PointBackTime 310

/* 2到3 B点寻到后后退速度 */
#define TwoTOThree_B_PointBackSpeed (float)0.18

/* 2到3 B点寻到后转弯角度 */
#define TwoTOThree_B_PointTurnAngle (float)150


/* ------------------------------------------------------------------------------- */

/* 脚本二台到四台 CAAbcy2WG5JoxkiK */

//二到四 点停转弯后点屏蔽相关 1Xe4BV3ApFnHYPXt 该ID可以快速链接到相关代码
/* 巡点转弯后屏蔽同一个点直行时间 */
#define TwoTOFour_PointCoverTime 500

/* 巡点转弯后屏蔽同一个点直行速度 */
#define TwoTOFour_PointCoverSpeed (float)0.1




/* 2到4 A点寻到后调整 快速定位代码 Ebabpx9IvphwcCYo */
/* 2到4 A点寻到后前进时间 */
#define TwoTOFour_A_PointBackTime 100

/* 2到4 A点寻到后前进速度 */
#define TwoTOFour_A_PointBackSpeed (float)0.14

/* 2到4 A点寻到后转弯角度 */
#define TwoTOFour_A_PointTurnAngle (float)30


/* 脚本二台到四台 A点到B点转移过程中过障碍延时巡线时间 1jF57tb2cnPVO5UJ 该ID可以快速链接到相关代码 */
#define TwoTOFour_PointAtoB_GolineDelayTime 3500



/* 2到4 B点寻到后调整 快速定位代码 LN997ancgNnSZiNA */
/* 2到3 B点寻到后后退时间 */
#define TwoTOFour_B_PointBackTime 300

/* 2到3 B点寻到后后退速度 */
#define TwoTOFour_B_PointBackSpeed (float)0.15

/* 2到3 B点寻到后转弯角度 */
#define TwoTOFour_B_PointTurnAngle (float)25





/* ------------------------------------------------------------------------------- */

/* 脚本四到五可能需要调整的地方 u4Gx8UHr7cLNm9vx */


/* 4到5点停转弯后屏蔽相同点相关 快速定位 A8gFEeGJO9W46kmZ */
/* 寻点转弯后屏蔽同一个点直行时间 */
#define FourTOFive_PointCoverTime  300

/* 巡点转弯后屏蔽同一个点直行速度 */
#define FourTOFive_PointCoverSpeed  (float)0.1
	



/* 4到5 D点转弯相关 快速定位 8TYvIapnyyipa3Mz */
/* D点退后速度 */
#define FourTOFive_D_PointBackSpeed (float)0.14
/* D点退后时间 */
#define FourTOFive_D_PointBackTime        150
/* D点转弯角度 */
#define FourTOFive_D_PointTurnAngle (float)90
	


/* 4到5 G点转弯相关 快速定位 OKEwq2AMELMr0XUD */
/* G点退后速度 */
#define FourTOFive_G_PointBackSpeed (float)0.14
/* G点退后时间 */
#define FourTOFive_G_PointBackTime        0
/* G点转弯角度 */
#define FourTOFive_G_PointTurnAngle (float)90



/* ------------------------------------------------------------------------------- */

/* 脚本三到五可能需要调整的地方 nyENPn28cKQryLOa */

/* 3到5点停转弯后屏蔽相同点相关 快速定位 IxU2oCKayK8p5D6z */
/* 寻点转弯后屏蔽同一个点直行时间 */
#define ThreeTOFive_PointCoverTime  300

/* 巡点转弯后屏蔽同一个点直行速度 */
#define ThreeTOFive_PointCoverSpeed  (float)0.1




/* 3到5 C点转弯相关 快速定位 PRF9Cbzp8Sy82CQk */
/* C点退后速度 */
#define ThreeTOFive_C_PointBackSpeed (float)0.1
/* C点退后时间 */
#define ThreeTOFive_C_PointBackTime        200
/* C点转弯角度 */
#define ThreeTOFive_C_PointTurnAngle (float)45




/* 3到5 G点转弯相关 快速定位 e0lVNGajHjzbHyvY */
/* G点前进时间 G点没有前进速度调节 */
#define ThreeTOFive_G_PointFrontTime        150
/* G点转弯角度 */
#define ThreeTOFive_G_PointTurnAngle (float)45


/* ------------------------------------------------------------------------------- */

/* 脚本三到六可能需要调整的地方 fjM6u9QfFW8tyxXe */


/* 3到6点停转弯后屏蔽相同点相关 快速定位 ttWvbxCzV2krZACv */
/* 寻点转弯后屏蔽同一个点直行时间 */
#define ThreeTOSix_PointCoverTime  350

/* 巡点转弯后屏蔽同一个点直行速度 */
#define ThreeTOSix_PointCoverSpeed  (float)0.07



/* 3到6 C点转弯相关 快速定位 ZMsAcwvH6qKcK9f0 */
/* C点退后速度 */
#define ThreeTOSix_C_PointBackSpeed (float)0.14
/* C点退后时间 */
#define ThreeTOSix_C_PointBackTime        200
/* C点转弯角度 */
#define ThreeTOSix_C_PointTurnAngle (float)85
	

/* 3到6 F点临时点检测 快速定位 Hh1wKW8dOMyAgvMa */
//根据场地光环境和灰度特征调整 
#define ThreeTOSix_F_PointDetectGraySum 10000



/* 3到6 F点转弯相关 快速定位  y2FINQVL4sfx8rYq */
/* F点退后速度 */
#define ThreeTOSix_F_PointBackSpeed (float)0.1
/* F点退后时间 */
#define ThreeTOSix_F_PointBackTime        250
/* F点转弯角度 */
#define ThreeTOSix_F_PointTurnAngle (float)90
	


/* 3到6 H1点圆上转弯后单独后退相关 快速定位 MLp88MY89ZtaRsp7 */
/* H1点退后速度 */
#define ThreeTOSix_H1_PointBackSpeed (float)0.07
/* H1点退后时间 */
#define ThreeTOSix_H1_PointBackTime        350


//5.23新加
/* 3到6 H1点圆上转弯前前进一段距离调整位置相关 FnVLyW1QWghfZRMN */
#define ThreeTOSix_H1_PointBeforeTurnGoTime 400
//5.23新加
#define ThreeTOSix_H1_PointBeforeTurnGoSpeed (float)0.14



/* 3到6 H1点圆上转弯相关 快速定位  gGFhBKxUKnFykquW */
#define ThreeTOSix_H1_PointTurnAngle (float)140
	
/* 3到6 H1到H3点1/4圆距离屏蔽 快速定位 EpJIv8k6ENnykNan */
#define ThreeTOSix_H1_H3_DistantCover (float)0.4
	
/* 3到6 H3点上跷跷板转弯调整 BOYZVLk4YfmJ5olf */
#define ThreeTOSix_H3_PointTurnAngle (float)27.5

/* 3到6 H3点上跷跷板 过跷跷板轮速锁延迟 太低将过不了跷跷板 */
/* 快速定位 IYyvbgcEpFIllIWk */
#define ThreeTOSix_OnSeesawDelayTime 4500

/* 3到6 H4点下跷跷板 从跷跷板下来下桥巡线时间 定位 kXRGlmj3GMjfXGOH */
#define ThreeTOSix_DownSeesawDownBridgeTime 500

/* 3到6 H4点下跷跷板 下跷跷板后转弯 快速定位 6EUdPkXo1Qfth9rx */
#define ThreeTOSix_H4_PointTurnAngle (float)30

/* 3到6 I点临时点检测算法阈值 快速定位 UMZMT9OGErQStomk */
/* 越高越迟钝 理论上 */
#define ThreeTOSix_I_PointDetectGraySum 25000




/* 3到6 I点转弯相关 快速定位  YysIWU1XhGBpnxB2 */
/* I点退后速度 */
#define ThreeTOSix_I_PointBackSpeed (float)0.14
/* I点退后时间 */
#define ThreeTOSix_I_PointBackTime        150
/* I点转弯角度 */
#define ThreeTOSix_I_PointTurnAngle (float)90



/* ------------------------------------------------------------------------------- */

/* 脚本四到六可能需要调整的地方  phSQ5lmIP3yHcL3d */


/* 4到6点停转弯后屏蔽相同点相关 快速定位 Pjp9eQs1l8618kJH */
/* 寻点转弯后屏蔽同一个点直行时间 */
#define FourTOSix_PointCoverTime  350

/* 巡点转弯后屏蔽同一个点直行速度 */
#define FourTOSix_PointCoverSpeed  (float)0.07
	

/* 4到6 D点转弯相关 快速定位 p9ZvVRZ1nL2REfKG */
/* D点退后速度 */
#define FourTOSix_D_PointBackSpeed (float)0.14
/* D点退后时间 */
#define FourTOSix_D_PointBackTime        0
/* D点转弯角度 */
#define FourTOSix_D_PointTurnAngle (float)45
	


/* 4到6 F点转弯相关 快速定位 MgW1qp1zrjTQjVtN */
/* F点前进时间 F点前进没有速度调节 只有时间 */
#define FourTOSix_F_PointFrontTime        150
/* F点转弯角度 */
#define FourTOSix_F_PointTurnAngle (float)35



/* 4到6 H1点圆上单独后退相关 快速定位 IzZmLTHP95lhuhGY */
/* H1点退后速度 */
#define FourTOSix_H1_PointBackSpeed (float)0.07
/* H1点退后时间 */
#define FourTOSix_H1_PointBackTime        350


/* 4到6 H1点圆上转弯前前进速度时间调整 Lt6SeMwok8WmSZEw */
//5.23新加
#define FourTOSix_H1_PointBeforeTurnGoTime 200
//5.23新加
#define FourTOSix_H1_PointBeforeTurnGoSpeed (float)0.14



/* 4到6 H1点圆上转弯相关 快速定位  Xp4o4PxUggn8kfWn */
#define FourTOSix_H1_PointTurnAngle (float)135

/* 4到6 H1到H3点距离屏蔽 快速定位  9J0IPkCrRRcaVrng */
#define FourTOSix_H1_H3_DistantCover (float)0.7

/* 4到6 H3点上跷跷板转弯调整 qoh2fNPtd8JknFX4 */
#define FourTOSix_H3_PointTurnAngle (float)27.5
	
/* 4到6 H3点上跷跷板 过跷跷板轮速锁延迟 太低将过不了跷跷板 */
/* 快速定位 qF8jkqxf7Qm0yAie */
#define FourTOSix_OnSeesawDelayTime 4500

/* 4到6 H4点下跷跷板 从跷跷板下来下桥巡线时间 定位 tHNWddLXqeeEPm2e  */
#define FourTOSix_DownSeesawDownBridgeTime 500

/* 4到6 H4点下跷跷板 下跷跷板后转弯 快速定位 p0KIeBItkUzrl5Y2 */
#define FourTOSix_H4_PointTurnAngle (float)40

/* 4到6 I点临时点检测算法阈值 快速定位 kKO5DeZMVCLcFA4U */
/* 越高越迟钝 理论上 */
#define FourTOSix_I_PointDetectGraySum 25000



/* 4到6 I点转弯相关 快速定位  qNr40qZ62LSkf5Gs */
/* I点退后速度 */
#define FourTOSix_I_PointBackSpeed (float)0.14
/* I点退后时间 */
#define FourTOSix_I_PointBackTime        150
/* I点转弯角度 */
#define FourTOSix_I_PointTurnAngle (float)90



/* ------------------------------------------------------------------------------- */

/* 脚本五到七可能需要调整的地方  XArl9R4c24mpOdV7  */


/* 5到7点停转弯后屏蔽相同点相关 快速定位 A94xE4UEGtN3JV0v */
/* 寻点转弯后屏蔽同一个点直行时间 */
#define FiveTOSeven_PointCoverTime  500

/* 巡点转弯后屏蔽同一个点直行速度 */
#define FiveTOSeven_PointCoverSpeed  (float)0.07
	
/* 5到7 从5台下来以后距离屏蔽G点 定位 2NHQPJvSayey56T9 */
#define FiveTOSeven_5_G_DistantCover (float) 2.7




/* 5到7 J点转弯相关 快速定位 Wt7p07dgHaM70lSb */
/* J点退后速度 */
#define FiveTOSeven_J_PointBackSpeed (float)0.15
/* J点退后时间 */
#define FiveTOSeven_J_PointBackTime        200
/* J点转弯角度 */
#define FiveTOSeven_J_PointTurnAngle (float)90


/* 5到7 从J到K点的距离屏蔽 定位 M6BhHpTalxMjxt4V */
#define FiveTOSeven_J_K_DistantCover (float) 1
	

/* 5到7 K1点上斜坡车往上开速度与距离 定位在 rP8iDAFMCIuVqy48  */ 
#define FiveTOSeven_K1_PiontUp_Speed (float)0.07

#define FiveTOSeven_K1_PiontUp_Distant  (float)0.05

/* 5到7 K1点斜坡上半锁定转弯角度 定位在上一条调整值下面 */
#define FiveTOSeven_K1_PointHalfLockTurnAngle (float)85
	


/* 5到7 K1点到L1点惯性导航 低 魔鬼 正常速巡线 过障碍 防寻飞微调 */
// 9YTn95VTOsmEJNIo
// 5到7 K1转完开到N2的总时间
#define FiveTOSeven_K1_N2_PointTotalTime 5000
// 5到7 K1点转弯惯性导航时间
#define FiveTOSeven_K1_N2_PointGyroTime  200


/* 5到7 下T形状板后低速巡线找线 开始时间 */
#define FiveTOSeven_K1_N2_PointLowSpeedStartTime 200
/* 5到7 下T形状板后低速巡线找线 结束时间 */
#define FiveTOSeven_K1_N2_PointLowSpeedStopTime  3000


// 5到7 过障碍刚开始采用魔鬼速度冲上去 防止卡住 开始时间
#define FiveTOSeven_K1_N2_PointInsameStartTime 3000

// 5到7 过障碍刚开始采用魔鬼速度冲上去 防止卡住 结束时间
#define FiveTOSeven_K1_N2_PointInsameStopTime  3700


/* 5到7 过障碍途中改回正常速度巡线 开始时间 */
#define FiveTOSeven_K1_N2_PointNormalSpeedStartTime  3700

/* 5到7 过障碍途中改回正常速度巡线 结束时间 */
#define FiveTOSeven_K1_N2_PointNormalSpeedStoptTime  5000


/* ------------------------------------------------------------------------------- */

/* 脚本五到八可能需要修改的地方 U3G5wElpXhQEFEko */

/* 5到8点停转弯后屏蔽相同点相关 快速定位 Icth53GNxEDlaipP */
/* 寻点转弯后屏蔽同一个点直行时间 */
#define FiveTOEight_PointCoverTime  500

/* 巡点转弯后屏蔽同一个点直行速度 */
#define FiveTOEight_PointCoverSpeed  (float)0.07

/* 5到8 从5台下来以后距离屏蔽G点 定位 7BPA6eRugWZXB5TG */
#define FiveTOEight_5_G_DistantCover (float) 2.7
	

/* 5到8 J点灰度激光联合检测 灰度阈值 LvZ9m40cag2urfKA */
#define FourTOSix_J_PointDetectGraySum (float)20000

/* 5到8 J点转弯相关 快速定位 1ppOxBugIh7xd7K0 */
/* J点退后速度 */
#define FiveTOEight_J_PointBackSpeed (float)0.1
/* J点退后时间 */
#define FiveTOEight_J_PointBackTime        200
/* J点转弯角度 */
#define FiveTOEight_J_PointTurnAngle (float)90
	

/* 5到8 从J到K点的距离屏蔽 定位 8kDsAhFv72AOhfvK */
#define FiveTOEight_J_K_DistantCover (float) 1


/* 5到8 K1点上斜坡车往上开速度距离 定位在 OER0kJlKC3AdrONB */
#define FiveTOEight_K1_PiontUp_Speed (float)0.07

#define FiveTOEight_K1_PiontUp_Distant  (float)0.05


/* 5到8 K1点斜坡上半锁定转弯角度 定位在上一条调整值下面 */
#define FiveTOEight_K1_PointHalfLockTurnAngle (float)85


/* 5到8 转向L1点后 上长坡前运行时间相关 oH85arhctcvIWnah */
/* 5到8 转向L1点后 上长坡前运行总时间 */
#define FiveTOEight_K1_8_PointToltalTime 2500
//转弯后惯性导航时间
#define FiveTOEight_K1_8_PointBridgeTime 200
//开启低速巡线时间
#define FiveTOEight_K1_8_PointLowSpeedStartTime 200
//关闭低速巡线时间
#define FiveTOEight_K1_8_PointLowSpeedStopTime  2500

/* 5到8 上桥屏蔽一段时间 屏蔽桥上非坡道时间 fw3Ijj3Uf1CF6dwu */
#define FiveTOEight_BridgeUpTime 1000

/* ------------------------------------------------------------------------------- */

/* 六到七脚本可能需要调整的地方 mxwQ7fKNWuzmJxIr */


/* 6到7点停转弯后屏蔽相同点相关 快速定位 YicJdmzfGpeHYoTV */
/* 寻点转弯后屏蔽同一个点直行时间 */
#define SixTOSeven_PointCoverTime  500

/* 巡点转弯后屏蔽同一个点直行速度 */
#define SixTOSeven_PointCoverSpeed  (float)0.07


/* 6到7 下台后I点临时检测算法调整 定位 ZtcHAYg7aQ0gYoOw */
#define SixTOSeven_I_PointDetectGraySum 60000

/* 6到7 下台后I点转弯相关 定位  */
/* I点退后速度 */
#define SixTOSeven_I_PointBackSpeed (float)0.1
/* I点退后时间 */
#define SixTOSeven_I_PointBackTime        150
/* I点转弯角度 */
#define SixTOSeven_I_PointTurnAngle (float)90


/* 6到7 从I点开始到出圆的距离 m08NntLbZ1X01Oxe */
#define SixTOSeven_I_PointOutCircleDistant (float)3.2

/* 6到7 从I点开始到波浪板的距离下限 W1nxYty6wpCWi0O4 */
#define SixTOSeven_BarricadeDistantDownLimit (float)1.15
	
/* 6到7 在波浪板前微调角度防止自己卡住玄学代码 和上一条位置一样 */
#define SixTOSeven_BeforebarricadeTurnAngle (float)10

/* 6到7 从I点开始到波浪板的距离上限 和上一条位置一样 */
#define SixTOSeven_BarricadeDistantUpLimit (float)1.50

/* 6到7 波浪板上的延时巡线时间 RUWU0UgaxWsdbA9M */
#define SixTOSeven_BarricadeGolineTime 1000

/* 6到7 剩余的出圈距离 */
#define SixTOSeven_AfterBarricadeOutCircleDistant (float) 1.8



/* 6到7 出圈以后屏蔽H1点相关 PkOku0t3KBaGjme9 */
/* 6到7 出圈以后转一玄学角度防止被巡线待会圈内 */
#define SixTOSeven_OutCircle_H1_PiontCoverAngle (float)10
	
/* 6到7 出圈以后直行屏蔽圈时间 */
#define SixTOSeven_OutCircle_H1_PiontCoverTime 200

/* 6到7 出圈以后直行屏蔽圈速度 */
#define SixTOSeven_OutCircle_H1_PiontCoverSpeed (float)0.1

/* 6到7 出圈以后直行屏蔽F点巡线延迟 */
#define SixTOSeven_OutCircle_F_PiontCoverGolineTime 500



/* 6到7 J点转弯相关 快速定位 oiWGjxaYnanbmE2J */
/* J点退后速度 */
#define SixTOSeven_J_PointBackSpeed (float)0.1
/* J点退后时间 */
#define SixTOSeven_J_PointBackTime        250
/* J点转弯角度 */
#define SixTOSeven_J_PointTurnAngle (float)90



/* 6到7 从J到K点的距离屏蔽 定位 SwOylLiHqPI8ZDAR */
#define SixTOSeven_J_K_DistantCover (float) 1


/* 6到7 K1点上斜坡车往上开速度距离 定位在 CKjXE5IJ3tl4yjby */
#define SixTOSeven_K1_PiontUp_Speed (float)0.07

#define SixTOSeven_K1_PiontUp_Distant (float)0.1

/* 6到7 K1点斜坡上半锁定转弯角度 定位在上一条调整值下面 */
#define SixTOSeven_K1_PointHalfLockTurnAngle (float)85
	

/* 6到7 K1点到L1点惯性导航 低 魔鬼 正常速巡线 过障碍 防寻飞微调 */
// JEReGrTupcAJz4gG
// 6到7 K1转完开到N2的总时间
#define SixTOSeven_K1_N2_PointTotalTime 5000
// 6到7 K1点转弯惯性导航时间
#define SixTOSeven_K1_N2_PointGyroTime  200


/* 6到7 下T形状板后低速巡线找线 开始时间 */
#define SixTOSeven_K1_N2_PointLowSpeedStartTime 200
/* 6到7 下T形状板后低速巡线找线 结束时间 */
#define SixTOSeven_K1_N2_PointLowSpeedStopTime  3000


// 6到7 过障碍刚开始采用魔鬼速度冲上去 防止卡住 开始时间
#define SixTOSeven_K1_N2_PointInsameStartTime 3000

// 6到7 过障碍刚开始采用魔鬼速度冲上去 防止卡住 结束时间
#define SixTOSeven_K1_N2_PointInsameStopTime  3700


/* 6到7 过障碍途中改回正常速度巡线 开始时间 */
#define SixTOSeven_K1_N2_PointNormalSpeedStartTime  3700

/* 6到7 过障碍途中改回正常速度巡线 结束时间 */
#define SixTOSeven_K1_N2_PointNormalSpeedStoptTime  5000

	


/* ------------------------------------------------------------------------------- */

/* 六到八脚本可能需要调整的地方 FenDbtKY0Nm1cczd */


/* 6到8点停转弯后屏蔽相同点相关 快速定位 mDyk3t765vG7Hdxe */
/* 寻点转弯后屏蔽同一个点直行时间 */
#define SixTOEight_PointCoverTime  500

/* 巡点转弯后屏蔽同一个点直行速度 */
#define SixTOEight_PointCoverSpeed  (float)0.07
	

/* 6到8 I点下台临时点检测值总和 gU15crco0soDk31n */
#define SixTOEight_I_PointDetectGraySum 13000


/* 6到8 I点转弯相关 快速定位 NKYWMYNL8zvBzusl */
/* I点退后速度 */
#define SixTOEight_I_PointBackSpeed (float)0.1
/* I点退后时间 */
#define SixTOEight_I_PointBackTime        150
/* I点转弯角度 */
#define SixTOEight_I_PointTurnAngle (float)92.5



/* 6到8 出半圆相关距离调整  DRVj5w26uNu0kQyn */
/* 6到8 从I点开始到出圆的距离  */
#define SixTOEight_I_PointOutCircleDistant (float)3.2

/* 6到8 从I点开始到波浪板的距离下限  */
#define SixTOEight_barricadeDistantDownLimit (float)1.2
	
/* 6到8 在波浪板前微调角度防止自己卡住玄学代码 和上一条位置一样 */
#define SixTOEight_BeforebarricadeTurnAngle (float)10

/* 6到8 从I点开始到波浪板的距离上限 和上一条位置一样 */
#define SixTOEight_barricadeDistantUpLimit (float)1.5

/* 6到8 波浪板上的延时巡线时间  */
#define SixTOEight_barricadeGolineTime 1000

/* 6到8 剩余的出圈距离 */
#define SixTOEight_AfterBarricadeOutCircleDistant (float) 1.5
	


/* 6到8 出圈以后屏蔽H1点相关 okQaI9I7GimDNP7Z */
/* 6到8 出圈以后转一玄学角度防止被巡线待会圈内 */
#define SixTOEight_OutCircle_H1_PiontCoverAngle (float)15
	
/* 6到8 出圈以后直行屏蔽圈时间 */
#define SixTOEight_OutCircle_H1_PiontCoverTime 200

/* 6到8 出圈以后直行屏蔽圈速度 */
#define SixTOEight_OutCircle_H1_PiontCoverSpeed (float)0.1

/* 6到8 出圈以后直行屏蔽F点巡线延迟 */
#define SixTOEight_OutCircle_F_PiontCoverGolineTime 500



/* 6到8 J点转弯相关 快速定位 X7SWdZUM0Iddt0YY */
/* J点退后速度 */
#define SixTOEight_J_PointBackSpeed (float)0.1
/* J点退后时间 */
#define SixTOEight_J_PointBackTime        250
/* J点转弯角度 */
#define SixTOEight_J_PointTurnAngle (float)90



/* 6到8 K1点上斜坡车往上开速度距离 定位在 gXuvcibGcDIG28Ux */
#define SixTOEight_K1_PiontUp_Speed (float)0.07

#define SixTOEight_K1_PiontUp_Distant (float)0.1

/* 6到8 K1点斜坡上半锁定转弯角度 定位在上一条调整值下面 */
#define SixTOEight_K1_PointHalfLockTurnAngle (float)85
	

/* 6到8 转向L1点后 上长坡前运行时间相关 5APyS5uke1BFGHZM */
/* 6到8 转向L1点后 上长坡前运行总时间 */
#define SixTOEight_K1_8_PointToltalTime 2500
//转弯后惯性导航时间
#define SixTOEight_K1_8_PointBridgeTime 200
//开启低速巡线时间
#define SixTOEight_K1_8_PointLowSpeedStartTime 200
//关闭低速巡线时间
#define SixTOEight_K1_8_PointLowSpeedStopTime  2500

/* 6到8 上8坡屏蔽坡上平地时间 3LgRluUCcYUO2pCA */
#define SixTOEight_UpbridgeTime 2000


/* ------------------------------------------------------------------------------- */

/* 八回家脚本可能需要调整的地方    8s3Y6D3c51OSbk87 */


/* 8回家 点停转弯后屏蔽相同点相关 快速定位 CC1o2X9kkfBLEz4f */
/* 寻点转弯�7�屏蔽同一个点直行时间 */
#define EightTOOne_PointCoverTime  500

/* 8回家 巡点转弯后屏蔽同一个点直行速度 */
#define EightTOOne_PointCoverSpeed  (float)0.07


/* 8回家 针对两段下桥的时间屏蔽 ASPuxSMhhhzTNjVL */
#define EightTOOne_DownBridgeCoverFlatGroundTime 6500

	
/* 8回家 下8台后巡一定距离到预定半锁定转弯地点 MIML3fpM2TFskpgt */
#define EightTOOne_DownPlatformTo_K1_PointDistant (float)0.90
	
/* 8回家 K1点下斜坡半锁定转弯到J点角度 定位在上一条调整值下面 */
#define EightTOOne_K1_PointHalfLockTurnAngle (float)85

/* 8回家 下斜坡后低速行进距离屏蔽K点 fNx0SzwwgDXwx2sN */
#define EightTOOne_K1_K_PointCoverDistant (float)1.0
	
/* 8回家 J点灰度激光联合检测 灰度阈值 nAHyADYWGhy8hiA7  */
#define EightTOOne_J_PointDetectGraySum 10000

/* 8回家 J点左转到F点角度 上一个下面 */
#define EightTOOne_J_PointTurnAngle (float)90
	
/* 8回家 F点右转角度 AQwmU6tD5kBHvP5a */
#define EightTOOne_F_PointTurnAngle (float)90
	
/* 8回家 距离屏蔽F到C点中间的门 EREQzfgH0O1SRbQm */
#define EightTOOne_F_C_PointDistantCover (float)1.5




/* 8回家 C点转弯单独调整相关 上一条下面 */
/* 8回家 C点单独转弯后退速度  */
#define EightTOOne_C_PointBackSpeed (float)0.14
	
/* 8回家 C点单独转弯后退时间 */
#define EightTOOne_C_PointBackTime 150

/* 8回家 C点单独转弯角度 */
#define EightTOOne_C_PointTuenAngle (float)85



/* 8回家 C点后动作控制 NpygwfboMl16pN6R */
/* 8回家 C到B激光扫到AB线段后寻的距离 */
#define EightTOOne_C_B_PiontLaserTrigDistant (float)0.22

/* 8回家 B点准备上跷跷板转弯的角度 */
#define EightTOOne_B_PiontTurnAngle (float)25

/* 8回家 速度锁上跷跷板越过中线的时间 */
#define EightTOOne_OnSeesawDelayTime 4000

/* 8回家 跷跷板越过中线后下跷跷板下桥PID时间 */
#define EightTOOne_DownSeesawDownBridgePidTime 1000

/* 8回家 下跷跷板提前点停后往前延时巡线到0点上的时间 */
#define EightTOOne_Before_0_PiontGolineDelayTime 300

/* 8回家 在0点上的转弯角度  */
#define EightTOOne_0_PiontTurnAngle (float)35




/* ------------------------------------------------------------------------------- */


/* 7回家脚本可能需要调整的地方 SiSEGyvDbjUX3EpF */

/* 7回家 点停转弯后屏蔽相同点相关 快速定位 rlpyPijdPiAazAfJ */
/* 寻点转弯后屏蔽同一个点直行时间 */
#define SevenTOOne_PointCoverTime  500

/* 7回家 巡点转弯后屏蔽同一个点直行速度 */
#define SevenTOOne_PointCoverSpeed  (float)0.07



/* 7回家 下7台坡后过波浪板一系列定时调整 TCYK5yOLrYnsqIZC */
/* 7回家 下完7台低速巡线到波浪板N2点时间  */
#define SevenTOOne_Down_7_LowSpeedTime  300
  
/* 7回家 下完7台到波浪板魔鬼速度时间  */
#define SevenTOOne_Down_7_InsameSpeedTime  600

/* 7回家 下完7台过波浪板中间高速时间  */
#define SevenTOOne_Down_7_HighSpeedTime 1700

/* 7回家 过完波浪板采用低速巡线找线时间  */
#define SevenTOOne_OverBarricade_LowSpeedTime 500



/* 7回家 从M点到预定半锁定转弯距离 影响车在T台上的转弯位置 6vQKK2luVUw2XMCJ */
#define SevenTOOne_M_K1_PiontCoverDistant (float)0.80

/* 7回家 扫过M点上方白线后低速时间 保证自己在线上 */
#define SevenTOOne_After_M_PiontLowSpeedTime 1500

/* 7回家 根据距离到达指定T台转弯位置转向J点角度 mYsYAYO8VoGKCrlL */
#define SevenTOOne_K1_PointTurnAngle (float) 85
	
/* 7回家 从K1到J过程中 距离屏蔽K点 mleCmoIy4Vlzh4uu */
#define SevenTOOne_K1_J_PointDistantCover (float)1.5
	


/* 7回家 J点 寻找 转弯调整相关 DsmmUi4XaLXkwT9C */
/* 7回家 J点 临时点检测灰度阈值 */
#define SevenTOOne_J_PiontGrayDetectSum 10000

/* 7回家 激光找到J点后的后退速度和时间 */
#define SevenTOOne_J_PiontBackSpeed (float) 0.18
#define SevenTOOne_J_PiontBackTime 100

/* 7回家 调整完毕后J点转点转弯角度 */
#define SevenTOOne_J_PiontTurnAngle (float)90




/* 7回家 J点转弯后F点调整转弯相关 amgRzGhLttHYFbAq */
/* F点后退速度 时间 */
#define SevenTOOne_F_PiontBackSpeed (float) 0.14
#define SevenTOOne_F_PiontBackTime 100

/* F转向C点角度 */
#define SevenTOOne_F_PiontTurnAngle (float)90
	


/* 7回家 距离屏蔽 F C之间的门 5bGcVxGIHGBTdz7b */
#define SevenTOOne_F_C_PointDistantCover (float)1.5
	


/* 7回家 C点转向B点转弯调整相关 在上一条下面 */
/* 7回家 激光找到C点后的后退速度和时间 */
#define SevenTOOne_C_PiontBackSpeed (float) 0.14
#define SevenTOOne_C_PiontBackTime 200

/* 7回家 调整完毕后C点转点转弯角度 */
#define SevenTOOne_C_PiontTurnAngle (float)87.5




/* 7回家 激光扫到AB线段后巡一玄学距离 	BUkrUux94JZQANjj */
#define SevenTOOne_C_B_PiontLaserTrigDistant (float)0.22
	
/* 7回家 AB段激光触发到距离后转一角度准备上跷跷板 */
#define SevenTOOne_B_PiontTurnAngle (float)25
	
/* 7回家 转弯后上跷跷板速度锁时间 无比保证该时间足够越过跷跷板中心 */
#define SevenTOOne_OnSeesawDelayTime 4250

/* 7回家 跷跷板越过中线后下跷跷板下桥PID时间 */
#define SevenTOOne_DownSeesawDownBridgePidTime 1000

/* 7回家 下跷跷板提前点停后往前延时巡线到0点上的时间 */
#define SevenTOOne_Before_0_PiontGolineDelayTime 300

/* 7回家 在0点上的转弯角度  */
#define SevenTOOne_0_PiontTurnAngle (float)35
	


/* ------------------------------------------------------------------------------- */



#endif
