#pragma once

#include "GPIB_Control.h"
#include "CurveLine.h"

class COSA
{
public:
	CGPIB_Control OSA_Gpib;
	int Connect(int mBoardIndex,int mPrimaryAddress,int mSecondaryAddress);
	int Disconnect();

	CString Read_Command(char* str);
	void Write_Command(char* str);

	int GetPrimaryAddress();
	void SetPrimaryAddress(int mPrimaryAddress);

	int GetWaveLengthSweepPoint();
	void SetWaveLengthSweepPoint(int PointNum);
	double GetWaveLengthInfo(int IsBegin);
	void SetWaveLengthInfo(int IsBegin,int WaveLength);

	char* GetCurv(int PointNum,int Begin_WaveLength,int End_WaveLength);
	COSA(void);
	~COSA(void);
};
