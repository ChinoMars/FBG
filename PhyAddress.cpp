// PhyAddress.cpp : 实现文件
//

#include "stdafx.h"
#include "FBG.h"
#include "PhyAddress.h"


// CPhyAddress 对话框

IMPLEMENT_DYNAMIC(CPhyAddress, CDialog)

CPhyAddress::CPhyAddress(CWnd* pParent /*=NULL*/)
	: CDialog(CPhyAddress::IDD, pParent)
	, NewPhyAddress(0)
{

}

CPhyAddress::~CPhyAddress()
{
}

void CPhyAddress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, NewPhyAddress);
	DDV_MinMaxInt(pDX, NewPhyAddress, 0, 99);
}


BEGIN_MESSAGE_MAP(CPhyAddress, CDialog)
END_MESSAGE_MAP()


// CPhyAddress 消息处理程序
