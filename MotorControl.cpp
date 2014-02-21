
#include "stdafx.h"
#include "MoterControl.h"

int MotorStart()
{
	if ( CH375OpenDevice( 0 ) != INVALID_HANDLE_VALUE )
		return 1;
	return 0;
}


void MotorMove( unsigned char Dir,UINT step,float speed )
{
	if(speed > 40 || speed < 0.001)
	{
		AfxMessageBox("步进机移动时速度初始化有误！！");
		return;
	}
	unsigned long Length;
	unsigned char Buffer[5];
	step = (step / MOTOR_RATE);
	Buffer[0] = Dir;
	Buffer[1] = char(int(40/speed)/256);
	Buffer[2] = char(int(40/speed)%256);
	Buffer[3] = step/256;
	Buffer[4] = step%256;
	Length = 5;
	CH375WriteData( 0, Buffer, &Length ); 
}

void MotorStop()
{
	unsigned long Length;
	unsigned char Buffer[4];
	Buffer[0] = 4;
	Length = 1;
	CH375WriteData( 0, Buffer, &Length ); 
}

void MotorContinue()
{
	unsigned long Length;
	unsigned char Buffer[4];
	Buffer[0] = 3;
	Length = 1;
	CH375WriteData( 0, Buffer, &Length ); 
}

void LightSwitch()
{
	unsigned long Length;
	unsigned char Buffer[4];
	Buffer[0] = 5;
	Length = 1;
	CH375WriteData( 0, Buffer, &Length ); 
}
