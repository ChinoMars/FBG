#pragma once


// CPhyAddress �Ի���

class CPhyAddress : public CDialog
{
	DECLARE_DYNAMIC(CPhyAddress)

public:
	CPhyAddress(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPhyAddress();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// �µ������ַ
	int NewPhyAddress;
};
