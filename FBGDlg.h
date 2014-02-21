// FBGDlg.h : ͷ�ļ�
//


#include "CurveLine.h"

#include "PhyAddress.h"
#include "MoterControl.h"
#include "LightGateDlg.h"
#include "OSA.h"

#define MUBIAO_CURV_ID IDC_STATIC
#define DIFF_CURV_ID IDC_STATIC2

#define GPIB_CLOSE 0
#define GPIB_LINKED 1

#define GPIB_BUTTON_ID IDC_GPIB_BUTTON
#define GPIB_STATE_TEXT_ID IDC_GPIB_STATE



// CFBGDlg �Ի���
class CFBGDlg : public CDialog
{
// ����
public:
	CFBGDlg(CWnd* pParent = NULL);	// ��׼���캯��
	
	void DrawCurve(CCurveLine& Curve,int ID);
	
	COSA OSA_Ando6370C;
	int Gpib_State_Flag;
	int FBG_StartFlag;
	CCurveLine* DiffCurv;
	CCurveLine* DestinateCurv;
	CCurveLine* CurvFromOSA;

	
	void GetCurvFromOSA();
	double GetWaveLengthInfo(int IsBegin);
	int GetWaveLengthSweepPoint();

	
// �Ի�������
	enum { IDD = IDD_FBG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedPhyaddressChange();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedReaddestinateLine();
	afx_msg void OnBnClickedStarFbgButton();
	// ����
	double ErrorAllowance;
};
