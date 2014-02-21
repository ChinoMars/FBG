#pragma once

#pragma comment (lib,"CH375DLL")
#include "CH375DLL.H"

#define MOTOR_LEFT 1
#define MOTOR_RIGHT 2


#define MOTOR_RATE 1.244528

// �����ڼ�����͵�Ƭ����Ӧ�ó���֮��Լ���´����ݵ����ֽ���������
#define		TEST_DATA			0x21	// ����������ȷ��

int MotorStart();
void MotorMove(unsigned char Dir,UINT step,float speed);
void MotorStop();
void MotorContinue();
void LightSwitch();