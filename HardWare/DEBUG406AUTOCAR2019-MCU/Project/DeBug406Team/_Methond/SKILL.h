#ifndef SKILL_H
#define SKILL_H
#include "cpu_test.h"
#include "motor.h"
#include "servo.h"
#include "encoder.h"
#include "buttom.h"
#include "gyro.h"
#include "laser.h"
#include "line.h"
#include "point.h"
#include "UpHillOrDownHill.h"
#include "motor_turn.h"
#include "magic.h"
void InitFlag(void);
void ThreeTOFour(void);
void OneTOTwo(void);
void TwoTOThree(void);
void _resetlaser(void);
void TwoTOFour(void);
void FourTOFive(void);
void ThreeTOFive(void);
void FiveTOSeven(void);
void ThreeTOSix(void);
void FourTOSix(void);
void TestOnly(void);
void TurnBack(void);
void FiveTOEight(void);
void SixTOSeven(void);
void SixTOEight(void);
void SevenTOOne(void);
void EightTOOne(void);
void EightTOSeven(void);
void SevenTOEight(void);

void _Start(void);
void _Reset(void);

extern int FlagUp;
extern int FlagDown;
extern int _FlagBack;
extern int FlagPoint;
extern int TurnFlag;
extern int FlagStop;
extern int FlagBalance;
extern int FlagBridge;
extern int FlagDistant;
extern int Flagstart;   //此变量只有在回到1台后才可初始化。
#endif
