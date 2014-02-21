#include "StdAfx.h"
#include "OSA.h"

COSA::COSA(void)
{
}

COSA::~COSA(void)
{
}

int COSA::Connect( int mBoardIndex,int mPrimaryAddress,int mSecondaryAddress )
{
	return OSA_Gpib.GpibConnect(mBoardIndex,mPrimaryAddress,mSecondaryAddress);
}

int COSA::Disconnect()
{
	return OSA_Gpib.Gpid_Disconnect();
}

CString COSA::Read_Command( char* str )
{
	return OSA_Gpib.Gpib_Read_Command(str);
}

void COSA::Write_Command( char* str )
{
	OSA_Gpib.Gpib_Write_Command(str);
}

int COSA::GetPrimaryAddress()
{
	return OSA_Gpib.PrimaryAddress;
}

void COSA::SetPrimaryAddress( int mPrimaryAddress )
{
	OSA_Gpib.PrimaryAddress = mPrimaryAddress;
}

char* COSA::GetCurv( int PointNum,int Begin_WaveLength,int End_WaveLength )
{
	CString str;
	int SweepPoint = GetWaveLengthSweepPoint();
	if(SweepPoint != PointNum)
	{
		SetWaveLengthSweepPoint( PointNum );
	}

	int CurBeginWaveLength = int(GetWaveLengthInfo(1));
	if(CurBeginWaveLength != int(Begin_WaveLength))
	{
		SetWaveLengthInfo(1,Begin_WaveLength);
	}

	int CurStopWaveLength = int(GetWaveLengthInfo(0));
	if(CurStopWaveLength != int(End_WaveLength))
	{
		SetWaveLengthInfo(0,End_WaveLength);
	}

	OSA_Gpib.Gpib_Write_Command("init:imm;*opc?");
	str = OSA_Gpib.Gpib_Read_Command("trac:data:y? tra");
	char* ctemp = str.GetBuffer();

	return ctemp;
}

int COSA::GetWaveLengthSweepPoint()
{
	CString str;
	str = OSA_Gpib.Gpib_Read_Command("sens:swe:poin?");
	char* ctemp = str.GetBuffer();
	int SweepPointNum = int(StringDecode(ctemp));
	return SweepPointNum;
}



double COSA::GetWaveLengthInfo(int IsBegin)
{
	CString str;
	if(IsBegin)
		str = OSA_Gpib.Gpib_Read_Command("sens:wav:star?");
	else
		str = OSA_Gpib.Gpib_Read_Command("sens:wav:stop?");

	char* ctemp = str.GetBuffer();
	double WvaLength = StringDecode(ctemp,9);
	return WvaLength;
}

void COSA::SetWaveLengthSweepPoint( int PointNum )
{
	CString str;
	str.Format("sens:swe:poin %d",PointNum);
	char* command = str.GetBuffer();
	OSA_Gpib.Gpib_Write_Command(command);
}

void COSA::SetWaveLengthInfo( int IsBegin,int WaveLength )
{
	CString str;
	if(IsBegin)
		str.Format("sens:wav:star %dnm",int(WaveLength));
	else
		str.Format("sens:wav:stop %dnm",int(WaveLength));

	char* command = str.GetBuffer();
	OSA_Gpib.Gpib_Write_Command(command);
}

