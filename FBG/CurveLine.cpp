#include "StdAfx.h"
#include "CurveLine.h"

CCurveLine::CCurveLine(void)
{
	PointData = NULL;
}

CCurveLine::CCurveLine( char* data,double Begin_WL,double End_WL )
{
	PointData = NULL;
	InitSuccessFlag = 0;
	Begin_WaveLength = Begin_WL;
	End_WaveLength = End_WL;
	InititalCurv(data);
	InitSuccessFlag = 1;
}

CCurveLine::CCurveLine( CString FileName )
{
	PointData = NULL;
	InitSuccessFlag = 0;
	CStdioFile DataFile;
	int hResult = DataFile.Open(FileName,CFile::modeRead);
	if(hResult == 0) 
	{ 
		AfxMessageBox("打开文件失败!"); 
		return; 
	}
	CString str;
	DataFile.ReadString(str);
	Begin_WaveLength = ReadWaveLength(str,"BeginWaveLength:");
	if(Begin_WaveLength < 0)
	{
		AfxMessageBox("起始波长格式有误!"); 
		return;
	}
	DataFile.ReadString(str);
	End_WaveLength = ReadWaveLength(str,"StopWaveLength:");
	if(End_WaveLength < 0)
	{
		AfxMessageBox("结束波长格式有误!"); 
		return;
	}	
	if(End_WaveLength <= Begin_WaveLength)
	{
		AfxMessageBox("结束波长应该大于起始波长!!");
		return;
	}

	DataFile.ReadString(str);
	if(str != "DATA:")
	{
		AfxMessageBox("数据应以   DATA:   为开头，并空一行"); 
		return;
	}

	DataFile.ReadString(str);
	char* data = str.GetBuffer();
	InititalCurv(data);
	InitSuccessFlag = 1;
}

CCurveLine::CCurveLine( CCurveLine* dest,CCurveLine* CurFromOSA )
{
	PointData = NULL;
	InitSuccessFlag = 0;
	if(dest->PointNum != CurFromOSA->PointNum)
		return;
	if(dest->Begin_WaveLength != CurFromOSA->Begin_WaveLength)
		return;
	if(dest->End_WaveLength != CurFromOSA->End_WaveLength)
		return;

	PointNum = dest->PointNum;
	Begin_WaveLength = dest->Begin_WaveLength;
	End_WaveLength = dest->End_WaveLength;

	PointData = new double[PointNum];
	for(int i = 0;i < PointNum;i++)
	{
		PointData[i] = dest->PointData[i] - CurFromOSA->PointData[i];
	}
	InitSuccessFlag = 1;
}

CCurveLine::~CCurveLine(void)
{
	if(PointData && this)
	{
		delete []PointData;
		PointData = NULL;
	}

}

double CCurveLine::ReadWaveLength(CString str,CString Pattern)
{
	int PatternLength = Pattern.GetLength();
	if(str.GetLength() >= PatternLength && str.Left(PatternLength) == Pattern && str.Right(2) == "nm")
	{
		return atof((str.Right(str.GetLength()-PatternLength)).Left(str.GetLength()-PatternLength-2));
	}
	else
		return -1;
}

int CCurveLine::InititalCurv( char* data )
{
	PointNum = 0;
	for(int i = 0;data[i] != '\0' ;i++)
	{
		if(data[i] == ',')
			PointNum++;
	}
	PointNum++;

	PointData = new double[PointNum];
	for(int i = 0;i < PointNum;i++)
	{
		PointData[i] = 0;
	}

	int str_idx = 0;
	for(int i = 0;i < PointNum;i++)
	{
		char* SubStr = (i==(PointNum-1))?GetSubString(data,str_idx,'\0'):GetSubString(data,str_idx,',');
		PointData[i] = StringDecode(SubStr);
		str_idx += strlen(SubStr)+1;	
	}
	return 1;
}


char* GetSubString( char* str,int Begin_Idx,char EndFlag )
{
	int End_Idx = Begin_Idx + 1;
	while(str[End_Idx] != EndFlag)
	{
		End_Idx++;
	}

	char* substr = new char[End_Idx-Begin_Idx+1];
	for(int i = Begin_Idx;i < End_Idx;i++)
	{
		substr[i-Begin_Idx] = str[i];
	}
	substr[End_Idx-Begin_Idx] = '\0';
	return substr;
}

//-4.02115616E+001
double StringDecode( char* str,int power_bias )
{
	int IntegerPart = 0;
	double DecimalPart = 0;
	int PowerPart = 0;
	//1:整数  2:小数   3:幂
	int PartIdx = 1;
	int IsPositive = 0;
	int IsPowerPartPositive = 0;

	int DecimalCounter = 0;
	for(unsigned int i = 0;i < strlen(str);i++)
	{
		if(PartIdx == 1 && str[i] == '-')
			IsPositive = 1;
		else if(PartIdx == 1 && str[i] == '+')
			IsPositive = 0;
		else if(PartIdx == 3 && str[i] == '-')
			IsPowerPartPositive = 1;
		else if(PartIdx == 3 && str[i] == '+')
			IsPowerPartPositive = 0;
		else if(str[i] == '.')
		{
			PartIdx = 2;
			DecimalCounter = 1;
		}
		else if(str[i] == 'E')
			PartIdx = 3;
		else
		{
			if (PartIdx == 1)
			{
				IntegerPart = IntegerPart*10 + str[i] - '0';
			}
			else if(PartIdx == 2)
			{
				DecimalPart += double(str[i]-'0')/pow(10.0,DecimalCounter);
				DecimalCounter++;
			}
			else
			{
				PowerPart = PowerPart*10 + str[i] - '0';
			}
		}
	}
	PowerPart = IsPowerPartPositive?-PowerPart:PowerPart;
	double result = (IntegerPart + DecimalPart)*pow(10.0,PowerPart+power_bias);
	return IsPositive?-result:result;
}
