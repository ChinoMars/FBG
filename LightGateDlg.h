#pragma once
#include "MoterControl.h"

// CLightGateDlg �Ի���

class CLightGateDlg : public CDialog
{
	DECLARE_DYNAMIC(CLightGateDlg)

public:
	CLightGateDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLightGateDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
};
