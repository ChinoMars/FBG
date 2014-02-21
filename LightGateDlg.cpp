// LightGateDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FBG.h"
#include "LightGateDlg.h"


// CLightGateDlg 对话框

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


// CLightGateDlg 消息处理程序

void CLightGateDlg::OnBnClickedButton2()
{
	LightSwitch();
	// TODO: 在此添加控件通知处理程序代码
}
