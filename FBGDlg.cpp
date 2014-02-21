// FBGDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FBG.h"
#include "FBGDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CFBGDlg 对话框




CFBGDlg::CFBGDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFBGDlg::IDD, pParent)
	, ErrorAllowance(0.25)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFBGDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, ErrorAllowance);
	DDV_MinMaxDouble(pDX, ErrorAllowance, 0, 10);
}

BEGIN_MESSAGE_MAP(CFBGDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_GPIB_BUTTON, &CFBGDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_PHYADDRESS_CHANGE, &CFBGDlg::OnBnClickedPhyaddressChange)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_READDESTINATE_LINE, &CFBGDlg::OnBnClickedReaddestinateLine)
	ON_BN_CLICKED(IDC_STAR_FBG_BUTTON, &CFBGDlg::OnBnClickedStarFbgButton)
END_MESSAGE_MAP()


// CFBGDlg 消息处理程序

BOOL CFBGDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//Gpib初始化
	
	Gpib_State_Flag = GPIB_CLOSE;

	FBG_StartFlag = 0;
	DiffCurv = NULL;
	DestinateCurv = NULL;
	CurvFromOSA = NULL;

	CEdit *DestinateCurvFileName = (CEdit*)GetDlgItem(IDC_DESTINATE_FILE);
	DestinateCurvFileName->EnableWindow(FALSE);

	CButton *FBGStartButton = (CButton*)GetDlgItem(IDC_STAR_FBG_BUTTON);
	FBGStartButton->EnableWindow(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFBGDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFBGDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	
	//绘图
	if(CurvFromOSA)
		DrawCurve(*CurvFromOSA,DIFF_CURV_ID);

	if(DestinateCurv)
		DrawCurve(*DestinateCurv,MUBIAO_CURV_ID);
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFBGDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFBGDlg::DrawCurve( CCurveLine& Curve,int ID )
{
	double DataMax = -999999999;
	double DataMin = 999999999;
	for(int i = 0;i < Curve.PointNum;i++)
	{
		if(DataMax < Curve.PointData[i])
			DataMax = Curve.PointData[i];
		if(DataMin > Curve.PointData[i])
			DataMin = Curve.PointData[i];
	}
	int Left = 0;
	double ReferenceLevel = DataMax + Left;
	double PlotRange = DataMax-DataMin + Left*2;

	CRect rect;
	CDC *pDc; 
	CDC memDC; 
	GetDlgItem(ID)->GetWindowRect(&rect);
	GetDlgItem(ID)->GetParent()->ScreenToClient(rect);
	pDc = this->GetDC();

	CBitmap memBitmap;
	memDC.CreateCompatibleDC(NULL);              
	memBitmap.CreateCompatibleBitmap(pDc,rect.Width(),rect.Height());
	memDC.SelectObject(&memBitmap);
	CPen pen_line(PS_SOLID,2,RGB(255,0,0));//画线颜色
	memDC.SelectObject(&pen_line);

	//自定义绘图函数
	memDC.FillSolidRect(CRect(0,0,rect.Width(),rect.Height()),RGB(180,180,180));//背景
	
	CPoint LastPoint,CurPoint;
	LastPoint.x = 0;
	LastPoint.y = long((ReferenceLevel - Curve.PointData[0])/PlotRange*rect.Height());
	memDC.MoveTo(LastPoint);
	for(int i = 1;i < Curve.PointNum;i++)
	{
		CurPoint.x = i*rect.Width()/(Curve.PointNum-1);
		CurPoint.y = long((ReferenceLevel - Curve.PointData[i])/PlotRange*rect.Height());
		memDC.LineTo(CurPoint);
		CurPoint = LastPoint;
	}

	pen_line.DeleteObject();
	pen_line.CreatePen(PS_SOLID,1,RGB(0,0,0));
	memDC.SelectObject(&pen_line);
	memDC.MoveTo(0,rect.Height()/2);
	memDC.LineTo(rect.Width(),rect.Height()/2);

	//显示坐标
	CString str;
	memDC.SetBkMode(TRANSPARENT);
	str.Format("%.2f dbm",ReferenceLevel);
	memDC.TextOut(0,0,str);
	str.Format("%.2f dbm",ReferenceLevel-PlotRange/2);
	memDC.TextOut(0,rect.Height()/2,str);
	str.Format("%.2f dbm",ReferenceLevel-PlotRange);
	CSize text_size = memDC.GetTextExtent(str);
	memDC.TextOut(0,rect.Height()-text_size.cy,str);


	//////////////////////////////////////////////////////////////
	pDc->BitBlt(rect.left,rect.top,rect.Width(),rect.Height(),&memDC,0,0,SRCCOPY);
	this->ReleaseDC(pDc);
	memDC.DeleteDC();
	memBitmap.DeleteObject();
}



//点击连接按钮
void CFBGDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton *GpibButton = (CButton*)GetDlgItem(GPIB_BUTTON_ID);
	CStatic *StateText = (CStatic*)GetDlgItem(GPIB_STATE_TEXT_ID);
	if (GPIB_CLOSE == Gpib_State_Flag)//需要连接
	{
		if(GPIB_CONNECT_SUCCESS != OSA_Ando6370C.Connect(0,23,0))
		{
			AfxMessageBox("连接失败！");
			return;
		}
		GpibButton->SetWindowText("断开");
		StateText->SetWindowText("连接中");

		CString str;
		CString str1 = OSA_Ando6370C.Read_Command("*IDN?");
		str.Format("GPIB与设备连接成功！\n设备信息：%s",str1.GetBuffer());
		AfxMessageBox(str);

		Gpib_State_Flag = GPIB_LINKED;
	}
	else//断开
	{
		if(GPIB_DISCONNECT_SUCCESS != OSA_Ando6370C.Disconnect())
		{
			AfxMessageBox("连接断开失败！！");
			return;
		}
		GpibButton->SetWindowText("连接");
		StateText->SetWindowText("关闭");
		FBG_StartFlag = 0;
		KillTimer(1);
		Gpib_State_Flag = GPIB_CLOSE;
	}
}

//更改物理地址
void CFBGDlg::OnBnClickedPhyaddressChange()
{
	// TODO: 在此添加控件通知处理程序代码
	CPhyAddress* dial = new CPhyAddress();
	dial->NewPhyAddress = OSA_Ando6370C.GetPrimaryAddress();
	if (IDOK == dial->DoModal())
	{
		CString str;
		str.Format("%d",dial->NewPhyAddress);
		if(dial->NewPhyAddress == OSA_Ando6370C.GetPrimaryAddress())//没有修改
			return;

		CStatic *PhysicsAddressText = (CStatic*)GetDlgItem(IDC_PHYSICSADDRESS);
		PhysicsAddressText->SetWindowText(str);
		OSA_Ando6370C.SetPrimaryAddress(dial->NewPhyAddress);
		if(Gpib_State_Flag == GPIB_LINKED)
		{
			OSA_Ando6370C.Disconnect();

			CButton *GpibButton = (CButton*)GetDlgItem(GPIB_BUTTON_ID);
			CStatic *StateText = (CStatic*)GetDlgItem(GPIB_STATE_TEXT_ID);
			GpibButton->SetWindowText("连接");
			StateText->SetWindowText("关闭");
			Gpib_State_Flag = GPIB_CLOSE;
		}
	}
}

void CFBGDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(Gpib_State_Flag == GPIB_LINKED)
	{
		CRect rect;
		GetDlgItem(DIFF_CURV_ID)->GetWindowRect(&rect);
		GetDlgItem(DIFF_CURV_ID)->GetParent()->ScreenToClient(rect);
	
		GetCurvFromOSA();

		//if(DiffCurv)
		//	delete DiffCurv;

		/*图像与目标曲线做差*/
		//DiffCurv = new CCurveLine(DestinateCurv,CurvFromOSA);
		//if(!DiffCurv->InitSuccessFlag)
		//	return;

		InvalidateRect(rect,FALSE);
	}
	CDialog::OnTimer(nIDEvent);
}

void CFBGDlg::GetCurvFromOSA()
{
	char* ctemp = OSA_Ando6370C.GetCurv(DestinateCurv->PointNum,
										DestinateCurv->Begin_WaveLength,
										DestinateCurv->End_WaveLength);

	if (CurvFromOSA)
		delete CurvFromOSA;
	CurvFromOSA = new CCurveLine(ctemp,DestinateCurv->Begin_WaveLength,DestinateCurv->End_WaveLength);
}




void CFBGDlg::OnBnClickedReaddestinateLine()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog pCFileDialog(TRUE,NULL,NULL,0,"目标曲线文件(*.txt)|*.txt||"); 
	pCFileDialog.m_ofn.lpstrTitle = "打开目标曲线文件 ";
	if(pCFileDialog.DoModal() != IDOK) 
		return; 

	CEdit *DestinateCurvFileName = (CEdit*)GetDlgItem(IDC_DESTINATE_FILE);
	DestinateCurvFileName->SetWindowText(pCFileDialog.GetPathName());
	
	if(DestinateCurv)
	{
		delete DestinateCurv;
		DestinateCurv = NULL;
		
	}	
	DestinateCurv = new CCurveLine(pCFileDialog.GetPathName());
	if(!DestinateCurv->InitSuccessFlag)
		return;

	CStatic *DestinateInfo = (CStatic*)GetDlgItem(IDC_MUBIAO_STATIC);
	CString InfoText;
	InfoText.Format(_T("目标曲线：起始波长:%.2f nm,终止波长:%.2f nm"),DestinateCurv->Begin_WaveLength,DestinateCurv->End_WaveLength);
	DestinateInfo->SetWindowText(InfoText);

	CRect rect;
	GetDlgItem(MUBIAO_CURV_ID)->GetWindowRect(&rect);
	GetDlgItem(MUBIAO_CURV_ID)->GetParent()->ScreenToClient(rect);
	InvalidateRect(rect,FALSE);

	CButton *FBGStartButton = (CButton*)GetDlgItem(IDC_STAR_FBG_BUTTON);
	FBGStartButton->EnableWindow(TRUE);
}

void CFBGDlg::OnBnClickedStarFbgButton()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!FBG_StartFlag && Gpib_State_Flag == GPIB_CLOSE)
	{
		AfxMessageBox("请先确认PC机已通过GPIB与光谱仪连通!!");
		return;
	}
	
	CButton *FBGStartButton = (CButton*)GetDlgItem(IDC_STAR_FBG_BUTTON);
	if(FBG_StartFlag == 0)
	{
		if(!MotorStart())
		{
			AfxMessageBox("步进电机启动失败!!请检查连接！");
			return;
		}
		//切换关闸状态至关闭
		CLightGateDlg *LightGateChangeDlg = new CLightGateDlg;
		LightGateChangeDlg->DoModal();

		FBG_StartFlag = 1;
		FBGStartButton->SetWindowText("停止制作光栅");
		SetTimer(1,50,NULL);
	}
	else
	{
		FBG_StartFlag = 0;
		FBGStartButton->SetWindowText("开始制作光栅");
		KillTimer(1);
	}
}