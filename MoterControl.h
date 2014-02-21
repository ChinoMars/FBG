#pragma once

#pragma comment (lib,"CH375DLL")
#include "CH375DLL.H"

#define MOTOR_LEFT 1
#define MOTOR_RIGHT 2


#define MOTOR_RATE 1.244528

// 我们在计算机和单片机的应用程序之间约定下传数据的首字节是命令码
#define		TEST_DATA			0x21	// 测试数据正确性

int MotorStart();
void MotorMove(unsigned char Dir,UINT step,float speed);
void MotorStop();
void MotorContinue();
void LightSwitch();