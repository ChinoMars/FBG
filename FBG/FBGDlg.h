// FBGDlg.h : 头文件
//

#pragma once

#include "visa.h"
#include "afxwin.h"
#include "CurveLine.h"
#include "SerialPortApi.h"
#include <fstream>
#include <vector>
#include <math.h>
#define BUFFER_SIZE 65535
#define OSA_TIMER_ID 313
#define RECV_TIMER_ID 314
#define SENDTS_TIMER_ID 315
#define SENDTP_TIMER_ID 316
#define MOVETIMER_TIMER_ID 317
#pragma comment(lib,"visa32.lib")

#define NULL_POSITION INT_MIN
#define DIFFPOS_FILENAME "pos.dat"
#define EE 2.718281828459045
using namespace std;

// CFBGDlg 对话框
class CFBGDlg : public CDialog
{
// 构造
public:
	CFBGDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_FBG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
//GPIB
	ViSession defaultRM, vi;
	int GpibIsOpen;
	int GpibIsScaning;
	CString ReadData();
	int hastemplate;

	CCurveLine* TemplateCurve;
	CCurveLine* CurCurve;
	void DrawCurve(int ID,CCurveLine* curve);
//COM
	CSerialPortApi COMPort;
	int COMIsOpen;
	int movecounter;
	int moveidx;
	vector<float> movespan;

	float curPos;
	float oriPos;
	float chirpBeginPos;
	int IschripMode;
	double beginwl,endwl;
	float GetDiffPos();
	void SaveDiffPos(float pos);
	void MoveRelative(float dis);
	void MoveAbsolute(float dis);

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int m_GpibDeviceId;
	int m_GpibPhyAdd;
	afx_msg void OnBnClickedGpibopenButton1();
	CButton m_GPIBbutton;
	CEdit m_GPIBDEVICE;
	afx_msg void OnBnClickedScan();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CButton m_ScanButton;
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedSettemplate();
	CButton m_SetTemplateButton;
	CComboBox m_port;
	afx_msg void OnBnClickedComconnect();
	CButton m_comConnectButton;
	CStatic m_motorstate;
	afx_msg void OnBnClickedOpengz();
	afx_msg void OnBnClickedClosegz();
	//CStatic m_motor_pos;
	int m_MoveDis;
	afx_msg void OnBnClickedMoveleft();
	afx_msg void OnBnClickedMoveright();
	afx_msg void OnBnClickedMotorreset();
	afx_msg void OnEnChangeEdit1();
	CComboBox m_trace;
	int m_TotalTime;
	int m_TimeSpan;
	int m_DisSpan;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedUpdatestate();
	int m_pointnum;
	int m_BeginWL;
	int m_EndWL;
	afx_msg void OnBnClickedSetosabutton();
	afx_msg void OnBnClickedHomebutton1();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnChirpButtonCheck1();
	CButton m_ChirpMode;
	CStatic m_disSpanStatic;
	int m_curtime;
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedSavespectra();
};
