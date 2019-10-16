#include "SHELL.h"
int Start=1;
int Stop=0;
uint8_t Command;
uint8_t motion;
int i=0;
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
		Command=_Command[1];
		motion=_Command[0];
		Start=0;
		UpdateMotorState(MOTOR_FRONT);
		SetMotorDutyRatio(0,0);
		Clear_CommandUpdate_PendingBit();
		InitFlag();
		Encoder_StopDistantCount();
	}
	if(Start==0&&Stop==0)
	{
		switch(Command)
		{
			case 0x00 :TestOnly();break;
			case 0x01 :OneTOTwo();break;
			case 0x02 :TwoTOThree();break;
			case 0x03 :TwoTOFour();break;
			case 0x04 :ThreeTOFive();break;
			case 0x05 :ThreeTOSix();break;
			case 0x06 :FourTOFive();break;
			case 0x07 :FourTOSix();break;
			case 0x08 :FiveTOSeven();break;
			case 0x09 :FiveTOEight();break;
			case 0x0A :SixTOSeven();break;
			case 0x0B :SixTOEight();break;
			case 0x0C :SevenTOOne();break;
			case 0x0D :EightTOOne();break;
		}
		switch(motion)
		{
			case 0x00 :break;
			case 0x0F :TurnBack();break; 
			case 0x0E :{printf("0xab");
									Stop=1;								}break; 
		}
		
	}
	if(Stop==1)
	{
		UpdateMotorState(MOTOR_STOP);
		printf("0xAB");
		Start=1;
		Stop=0;
	}
}
