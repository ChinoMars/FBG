// LightGateDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FBG.h"
#include "LightGateDlg.h"


// CLightGateDlg �Ի���

IMPLEMENT_DYNAMIC(CLightGateDlg, CDialog)

CLightGateDlg::CLightGateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLightGateDlg::IDD, pParent)
{

}

CLightGateDlg::~CLightGateDlg()
{
}

void CLightGateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLightGateDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CLightGateDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CLightGateDlg ��Ϣ�������

void CLightGateDlg::OnBnClickedButton2()
{
	LightSwitch();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
