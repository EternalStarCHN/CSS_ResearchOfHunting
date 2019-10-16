#include "SHELL.h"
int Start=1;
int Stop=0;
uint8_t Command;
uint8_t motion;
uint8_t rho_ss;
uint8_t rho;
uint8_t theta_ss;
uint8_t theta;
int i=0;
int Crycle=0;
extern volatile uint8_t _Command[5];
void _Timer(void)
{
		_resetlaser();
		i++;
		if(i==5)
		{
			PointFlag=0;
			i=0;
		}
}

void Match(void)
{
	//printf("%d",Is_Command_Updated());
	if(Is_Command_Updated()==1&&Start==1)
	{
		rho_ss = _Command[0];
		rho = _Command[1];
		theta_ss= _Command[2];
		theta = _Command[3];		
		Start=0;
		UpdateMotorState(MOTOR_FRONT);
		SetMotorDutyRatio(0.01,0);
		Clear_CommandUpdate_PendingBit();
		InitFlag();
		Encoder_StopDistantCount();
	}
	if(Start==0&&Stop==0)
	{
//		switch(Command)
//		{
//			case 0x00 :TestOnly();break;
//			case 0x01 :OneTOTwo();break;
//			case 0x02 :TwoTOThree();break;
//			case 0x03 :TwoTOFour();break;
//			case 0x04 :ThreeTOFive();break;
//			case 0x05 :ThreeTOSix();break;
//			case 0x06 :FourTOFive();break;
//			case 0x07 :FourTOSix();break;
//			case 0x08 :FiveTOSeven();break;
//			case 0x09 :FiveTOEight();break;
//			case 0x0A :SixTOSeven();break;
//			case 0x0B :SixTOEight();break;
//			case 0x0C :SevenTOEight();break;
//			case 0x0D :EightTOSeven();break;
//			case 0x0E :SevenTOOne();break;
//			case 0x0F :EightTOOne();break;
//		}
//		switch(motion)
//		{
//			case 0x00 :break;
//			case 0x0F :TurnBack();break; 
//			case 0x0E :UpHand();break; 
//			case 0x0A :ShakeHead();break; //摇头；
//		}	
	}
	if(Stop==1)
	{
//		UpdateMotorState(MOTOR_STOP);
//		printf("0xAB");
//		Start=1;
//		Stop=0;
	}
}
