#pragma once


// CPhyAddress 对话框

class CPhyAddress : public CDialog
{
	DECLARE_DYNAMIC(CPhyAddress)

public:
	CPhyAddress(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPhyAddress();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 新的物理地址
	int NewPhyAddress;
};
