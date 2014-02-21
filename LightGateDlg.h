#pragma once
#include "MoterControl.h"

// CLightGateDlg 对话框

class CLightGateDlg : public CDialog
{
	DECLARE_DYNAMIC(CLightGateDlg)

public:
	CLightGateDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLightGateDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
};
