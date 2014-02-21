#pragma once
#include <math.h>
class CCurveLine
{
public:
	CCurveLine(void);
	CCurveLine(char* data,double Begin_WL,double End_WL);
	CCurveLine(CString FileName);
	CCurveLine(CCurveLine* dest,CCurveLine* CurFromOSA);
	double ReadWaveLength(CString str,CString Pattern);
	int InititalCurv(char* data);
	~CCurveLine(void);

	

	double Begin_WaveLength;
	double End_WaveLength;
	int PointNum;
	double* PointData;

	int InitSuccessFlag;
};

char* GetSubString(char* str,int Begin_Idx,char EndFlag);
//×ª»»¡°-3.88518405E+001¡±³ÉÊý×Ö
double StringDecode(char* str,int power_bias = 0);