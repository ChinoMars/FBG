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
	, m_GpibDeviceId(1)
	, m_GpibPhyAdd(1)
	, m_MoveDis(0)
	, m_TotalTime(60000)
	, m_TimeSpan(600)
	, m_DisSpan(15000)
	, m_pointnum(3001)
	, m_BeginWL(1540)
	, m_EndWL(1545)
	, m_curtime(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	GpibIsOpen = 0;
	GpibIsScaning = 0;
	hastemplate = 0;
	COMIsOpen = 0;
	TemplateCurve = NULL;
	CurCurve = NULL ;
	curPos = NULL_POSITION;
	beginwl = NULL_POSITION;
	endwl = NULL_POSITION;
	IschripMode = 0;
}

void CFBGDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_GPIBDEVICE, m_GpibDeviceId);
	DDV_MinMaxInt(pDX, m_GpibDeviceId, 0, 1024);
	DDX_Text(pDX, IDC_GPIBPHYADDRE, m_GpibPhyAdd);
	DDV_MinMaxInt(pDX, m_GpibPhyAdd, 0, 1024);
	DDX_Control(pDX, IDC_GPIBOPEN_BUTTON1, m_GPIBbutton);
	DDX_Control(pDX, IDC_OSA_TYPE, m_GPIBDEVICE);
	DDX_Control(pDX, IDC_SCAN, m_ScanButton);
	DDX_Control(pDX, IDC_SETTEMPLATE, m_SetTemplateButton);
	DDX_Control(pDX, IDC_COMBO1, m_port);
	DDX_Control(pDX, IDC_COMCONNECT, m_comConnectButton);
	DDX_Control(pDX, IDC_MOTOR_STATE, m_motorstate);
//	DDX_Control(pDX, IDC_MOTOR_POS, m_motor_pos);
	DDX_Text(pDX, IDC_EDIT1, m_MoveDis);
	DDV_MinMaxInt(pDX, m_MoveDis, -50001, 50001);
	DDX_Control(pDX, IDC_COMBO2, m_trace);
	DDX_Text(pDX, IDC_EDIT5, m_TotalTime);
	DDX_Text(pDX, IDC_EDIT6, m_TimeSpan);
	DDX_Text(pDX, IDC_EDIT7, m_DisSpan);
	DDX_Text(pDX, IDC_EDIT2, m_pointnum);
	DDV_MinMaxInt(pDX, m_pointnum, 1, 65535);
	DDX_Text(pDX, IDC_EDIT3, m_BeginWL);
	DDX_Text(pDX, IDC_EDIT4, m_EndWL);
	DDX_Control(pDX, IDC_CHECK1, m_ChirpMode);
	DDX_Control(pDX, IDC_DISSPAN_STATIC, m_disSpanStatic);
	DDX_Text(pDX, IDC_STATICCUR, m_curtime);
}

BEGIN_MESSAGE_MAP(CFBGDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_GPIBOPEN_BUTTON1, &CFBGDlg::OnBnClickedGpibopenButton1)
	ON_BN_CLICKED(IDC_SCAN, &CFBGDlg::OnBnClickedScan)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO2, &CFBGDlg::OnCbnSelchangeCombo2)
	ON_EN_CHANGE(IDC_EDIT2, &CFBGDlg::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_SETTEMPLATE, &CFBGDlg::OnBnClickedSettemplate)
	ON_BN_CLICKED(IDC_COMCONNECT, &CFBGDlg::OnBnClickedComconnect)
	ON_BN_CLICKED(IDC_OPENGZ, &CFBGDlg::OnBnClickedOpengz)
	ON_BN_CLICKED(IDC_CLOSEGZ, &CFBGDlg::OnBnClickedClosegz)
	ON_BN_CLICKED(IDC_MOVELEFT, &CFBGDlg::OnBnClickedMoveleft)
	ON_BN_CLICKED(IDC_MOVERIGHT, &CFBGDlg::OnBnClickedMoveright)
	ON_BN_CLICKED(IDC_MOTORRESET, &CFBGDlg::OnBnClickedMotorreset)
	ON_EN_CHANGE(IDC_EDIT1, &CFBGDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON2, &CFBGDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CFBGDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_UPDATESTATE, &CFBGDlg::OnBnClickedUpdatestate)
	ON_BN_CLICKED(IDC_SETOSABUTTON, &CFBGDlg::OnBnClickedSetosabutton)
	ON_BN_CLICKED(IDC_HOMEBUTTON1, &CFBGDlg::OnBnClickedHomebutton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CFBGDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CFBGDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_CHECK1, &CFBGDlg::OnBnChirpButtonCheck1)
	ON_BN_CLICKED(IDC_BUTTON6, &CFBGDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_SAVESPECTRA, &CFBGDlg::OnBnClickedSavespectra)
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
	m_port.ResetContent();

	//Set Port
	for(size_t i = 0;i < COMPort.GetPortNum();i++)
	{
		m_port.AddString(COMPort.PortList[i]);
	}
	if(COMPort.GetPortNum() > 2)
		m_port.SetCurSel(2);
	else{
		m_port.SetCurSel(2);
	}
	m_trace.SetCurSel(0);
	oriPos = NULL_POSITION;
	CComboBox* RBW = (CComboBox*)GetDlgItem(IDC_RBW_COMBO3);
	RBW->SetCurSel(0);
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

	if(CurCurve)
	{
		DrawCurve(IDC_DIFFER,CurCurve);
	}
	if(TemplateCurve)
	{
		DrawCurve(IDC_TEMPLATE,TemplateCurve);
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFBGDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CFBGDlg::OnBnClickedGpibopenButton1()
{
	if(!GpibIsOpen){
		UpdateData(TRUE);
		viOpenDefaultRM (&defaultRM);
		CString GPIBADDR;
		GPIBADDR.Format("GPIB%d::%d::INSTR",m_GpibDeviceId,m_GpibPhyAdd);
		ViStatus GPIBState = viOpen (defaultRM, GPIBADDR.GetBuffer(), VI_NULL,VI_NULL, &vi);
		while(GPIBState != 0 && m_GpibPhyAdd < 32){
			m_GpibPhyAdd++;
			UpdateData(FALSE);
			GPIBADDR.Format("GPIB%d::%d::INSTR",m_GpibDeviceId,m_GpibPhyAdd);
			GPIBState = viOpen (defaultRM, GPIBADDR.GetBuffer(), VI_NULL,VI_NULL, &vi);
		}//Error
		if(GPIBState != 0){
			char ErrorDesc[BUFFER_SIZE];
			viStatusDesc(vi,GPIBState,ErrorDesc);
			m_GPIBDEVICE.SetWindowText(ErrorDesc);
		}
		else{
			char Device[200];
			viPrintf (vi, "*IDN?\n");
			viScanf (vi, "%s", &Device);
			m_GPIBDEVICE.SetWindowText(Device);
			GpibIsOpen = 1;
			m_GPIBbutton.SetWindowText("断开");
			viClear(vi);
		}
	}
	else
	{
		viClose (vi);
		viClose (defaultRM);
		GpibIsOpen = 0;
		m_GPIBbutton.SetWindowText("连接");
		if(GpibIsScaning){
			GpibIsScaning = 0;
			KillTimer(OSA_TIMER_ID);
			m_ScanButton.SetWindowText("扫描");

		}
	}
	
}

void CFBGDlg::OnBnClickedScan()
{
	if(!GpibIsOpen)
	{
		AfxMessageBox("请先打开GPIB!!");
		return;
	}
	if(GpibIsScaning == 0){
		char str[200];
		viPrintf (vi, "sens:wav:star?\n");
		viScanf (vi, "%s", &str);
		beginwl = StringDecode(str,9);
		viPrintf (vi, "sens:wav:stop?\n");
		viScanf (vi, "%s", &str);
		endwl = StringDecode(str,9);
		//CString mstr;
		//mstr.Format("%f %f",beginwl,endwl);
		//AfxMessageBox(mstr);
		
		SetTimer(OSA_TIMER_ID,200,NULL);
		GpibIsScaning = 1;
		m_ScanButton.SetWindowText("停止");
	}
	else{
		GpibIsScaning = 0;
		KillTimer(OSA_TIMER_ID);
		m_ScanButton.SetWindowText("扫描");
	}
	
}

void CFBGDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent == OSA_TIMER_ID){
		if(GpibIsOpen && GpibIsScaning){
			CString Buffer = ReadData();
			if(!hastemplate){
				if(TemplateCurve)
					delete TemplateCurve;
				TemplateCurve = new CCurveLine(Buffer.GetBuffer(),beginwl,endwl);
			}
			else
			{
				if (CurCurve)
					delete CurCurve;
				CurCurve = new CCurveLine(Buffer.GetBuffer(),beginwl,endwl);
				if(CurCurve->PointNum == TemplateCurve->PointNum)
				{
					for(int i = 0;i < CurCurve->PointNum;i++)
					{
						CurCurve->PointData[i] -= TemplateCurve->PointData[i];
						//CurCurve->PointData[i] *= -1;
					}
					DrawCurve(IDC_DIFFER,CurCurve);
				}

			}
			DrawCurve(IDC_TEMPLATE,TemplateCurve);

		}
	}
	else if(nIDEvent == RECV_TIMER_ID)
	{
		if(COMPort.ReceiveFlag)
		{
			vector<CString> strlist = COMPort.ReadRecvByteSplite('\n');
			for(size_t i = 0;i < strlist.size();i++)
			{
				if(0 == strlist[i].Left(3).Compare("1TS"))
				{
					//m_motorstate.SetWindowText(strlist[i]);
					CString state_flag = (strlist[i].Right(strlist[i].GetLength()-7)).Left(2);
					if(state_flag == "OA")
						m_motorstate.SetWindowText("NOT REFERENCED from reset");
					else if(state_flag == "0B")
						m_motorstate.SetWindowText("NOT REFERENCED from HOMING");
					else if(state_flag == "0C")
						m_motorstate.SetWindowText("NOT REFERENCED from CONFIGURATION");
					else if(state_flag == "OD")
						m_motorstate.SetWindowText("NOT REFERENCED from DISABLE");
					else if(state_flag == "0E")
						m_motorstate.SetWindowText("NOT REFERENCED from READY");
					else if(state_flag == "0F")
						m_motorstate.SetWindowText("NOT REFERENCED from MOVING");
					else if(state_flag == "10")
						m_motorstate.SetWindowText("NOT REFERENCED ESP stage error");
					else if(state_flag == "14")
						m_motorstate.SetWindowText("CONFIGURATION");
					else if(state_flag == "1E")
						m_motorstate.SetWindowText("HOMING");
					else if(state_flag == "28")
						m_motorstate.SetWindowText("MOVING");
					else if(state_flag == "32")
						m_motorstate.SetWindowText("READY from HOMING");
					else if(state_flag == "33")
						m_motorstate.SetWindowText("READY from MOVING");
					else if(state_flag == "34")
						m_motorstate.SetWindowText("READY from DISABLE");
					else if(state_flag == "3C")
						m_motorstate.SetWindowText("DISABLE from READY");
					else if(state_flag == "3D")
						m_motorstate.SetWindowText("DISABLE from MOVING");
					else
						m_motorstate.SetWindowText("解析失败"+state_flag);
				}
				else if(0 == strlist[i].Left(3).Compare("1TP"))
				{
					CString posStr = strlist[i].Right(strlist[i].GetLength()-3);
//					m_motor_pos.SetWindowText(posStr);
					curPos = float(atof(posStr));
					if(oriPos == NULL_POSITION)
					{

						float temp = GetDiffPos();
						oriPos = curPos - temp;
					}
					SaveDiffPos(curPos-oriPos);
				}
			}
		}
	}
	else if(nIDEvent == SENDTP_TIMER_ID)
	{
		if(COMIsOpen)
		{
			COMPort.Send("1TP\r\n");
		}
	}
	else if(nIDEvent == SENDTS_TIMER_ID)
	{
		if(COMIsOpen)
		{
			COMPort.Send("1TS\r\n");
		}
	}
	else if(nIDEvent == MOVETIMER_TIMER_ID )
	{
		if(moveidx < movecounter && moveidx >= 0)
		{
			MoveAbsolute(movespan[moveidx]);
			moveidx++;
			UpdateData(TRUE);
			m_curtime = moveidx*100/movecounter;
			UpdateData(FALSE);
		}
		else
		{
			KillTimer(MOVETIMER_TIMER_ID);
			COMPort.Send("gzg\r\n");
		}
	}
	CDialog::OnTimer(nIDEvent);
}

CString CFBGDlg::ReadData()
{
	UpdateData(TRUE);
	char Buffer[BUFFER_SIZE];
	viClear(vi);
	CString str;
	str.Format("trac:data:y? tr%c\n",'a'+m_trace.GetCurSel());
	ViStatus GPIBState = viPrintf (vi, str.GetBuffer());
	if(GPIBState != 0){//Error
		char ErrorDesc[BUFFER_SIZE];
		viStatusDesc(vi,GPIBState,ErrorDesc);
		m_GPIBDEVICE.SetWindowText(ErrorDesc);
		return (CString)Buffer;
	}
	GPIBState = viScanf (vi, "%s", &Buffer);
	if(GPIBState != 0){//Error
		char ErrorDesc[BUFFER_SIZE];
		viStatusDesc(vi,GPIBState,ErrorDesc);
		m_GPIBDEVICE.SetWindowText(ErrorDesc);
		return (CString)Buffer;
	}
	return (CString)Buffer;
}

void CFBGDlg::DrawCurve( int ID,CCurveLine* curve)
{
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

	if(curve != NULL){
		double DataMax = -999999999;
		double DataMin = 999999999;
		for(int i = 0;i < curve->PointNum;i++)
		{
			if(DataMax < curve->PointData[i])
				DataMax = curve->PointData[i];
			if(DataMin > curve->PointData[i])
				DataMin = curve->PointData[i];
		}
		DataMax = int(DataMax)+1;
		DataMin = int(DataMin)-1;
		int Left = 1;
		double ReferenceLevel = DataMax + Left;
		double PlotRange = DataMax-DataMin + Left;
		if(PlotRange < 5){
			PlotRange = 5;
		}

		CPoint LastPoint,CurPoint;
		LastPoint.x = 0;
		LastPoint.y = long((ReferenceLevel - curve->PointData[0])/PlotRange*rect.Height());
		memDC.MoveTo(LastPoint);
		for(int i = 1;i < curve->PointNum;i++)
		{
			CurPoint.x = i*rect.Width()/(curve->PointNum-1);
			CurPoint.y = long((ReferenceLevel - curve->PointData[i])/PlotRange*rect.Height());
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
		str.Format("%.2f db",ReferenceLevel);
		memDC.TextOut(0,0,str);
		str.Format("%.2f db",ReferenceLevel-PlotRange/2);
		memDC.TextOut(0,rect.Height()/2,str);
		str.Format("%.2f db",ReferenceLevel-PlotRange);
		CSize text_size = memDC.GetTextExtent(str);
		memDC.TextOut(0,rect.Height()-text_size.cy,str);

		str.Format("%.1f",beginwl);
		memDC.TextOut(0,rect.Height()/2-text_size.cy,str);

		str.Format("%.1f",endwl);
		memDC.TextOut(rect.Width()-text_size.cx+text_size.cy,rect.Height()/2-text_size.cy,str);

		pen_line.DeleteObject();
		pen_line.CreatePen(PS_DASHDOT,1,RGB(0,255,0));
		memDC.SelectObject(&pen_line);
		for(int i = 1;i < 10;i++)
		{
			memDC.MoveTo(rect.Width()*i/10,0);
			memDC.LineTo(rect.Width()*i/10,rect.Height());
		}
	}
	//////////////////////////////////////////////////////////////
	pDc->BitBlt(rect.left,rect.top,rect.Width(),rect.Height(),&memDC,0,0,SRCCOPY);
	this->ReleaseDC(pDc);
	memDC.DeleteDC();
	memBitmap.DeleteObject();
}


void CFBGDlg::OnCbnSelchangeCombo2()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CFBGDlg::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CFBGDlg::OnBnClickedSettemplate()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!GpibIsOpen){
		AfxMessageBox("请先连接光谱仪");
		return;
	}
	if(!hastemplate)
	{
		hastemplate = 1;
		m_SetTemplateButton.SetWindowText("删除模板");
	}
	else
	{
		hastemplate = 0;
		m_SetTemplateButton.SetWindowText("设置模板");
	}
	
}

void CFBGDlg::OnBnClickedComconnect()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!COMIsOpen)
	{
		if(OPEN_PORT_SUCCESS  != COMPort.OpenPort(COMPort.PortList[m_port.GetCurSel()],CBR_57600,8,NOPARITY,ONESTOPBIT))
		{
			AfxMessageBox("电机连接失败");
		}
		else
		{
			m_comConnectButton.SetWindowText("断开");
			COMIsOpen = 1;
			
			SetTimer(RECV_TIMER_ID,50,NULL);
			SetTimer(SENDTS_TIMER_ID,400,NULL);
			SetTimer(SENDTP_TIMER_ID,500,NULL);
			COMPort.Send("gzg\r\n");
		}
	}
	else
	{
		if(CLOSE_PORT_SUCCESS != COMPort.ClosePort())
		{
			AfxMessageBox("关闭串口失败");
		}
		else
		{
			m_comConnectButton.SetWindowText("连接");
			COMIsOpen = 0;
			KillTimer(RECV_TIMER_ID);
			KillTimer(SENDTS_TIMER_ID);
			KillTimer(SENDTP_TIMER_ID);
		}
	}
	
}

void CFBGDlg::OnBnClickedOpengz()
{
	// TODO: 在此添加控件通知处理程序代码
	if(COMIsOpen)
	{
		COMPort.Send("gzk\r\n");
	}
	else{
		AfxMessageBox("请先打开串口");
	}
	
}

void CFBGDlg::OnBnClickedClosegz()
{
	// TODO: 在此添加控件通知处理程序代码
	if(COMIsOpen)
	{
		COMPort.Send("gzg\r\n");
	}
	else{
		AfxMessageBox("请先打开串口");
	}
}

void CFBGDlg::OnBnClickedMoveleft()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	MoveRelative(float(m_MoveDis)/1000);
}

void CFBGDlg::OnBnClickedMoveright()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	MoveRelative(float(-m_MoveDis)/1000);
}

void CFBGDlg::OnBnClickedMotorreset()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(SENDTS_TIMER_ID);
	COMPort.Send("1or\r\n");
	//COMPort.Send("1PR3\r\n");
	//SetTimer(SENDTS_TIMER_ID,500,NULL);
	//SetTimer(SENDTS_TIMER_ID,400,NULL);
}

void CFBGDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CFBGDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(m_TotalTime <= 0 || m_TimeSpan <= 0 || m_DisSpan <= 0)
	{
		AfxMessageBox("参数不可为负!!");
		return;
	}
	if(COMIsOpen)
	{
		movecounter = int(double(m_TotalTime)/m_TimeSpan);
		moveidx = 0;
		chirpBeginPos = curPos;
		if(!IschripMode)
		{
			movespan.assign(movecounter,float(m_DisSpan)/1000);
			for(size_t i = 1;i < movespan.size();i++)
			{
				movespan[i] += movespan[i-1];
			}
		}
		else
		{
			movespan.assign(movecounter,0);
			for(size_t i = 0;i < movespan.size();i++)
			{
				double idx = i;
				// 修改切枝波形1！！[还有一个地方要修改]
				movespan[i] = 1-pow(EE,-(6*(idx-movecounter/2)/movecounter)*(6*(idx-movecounter/2)/movecounter));
				if(i != 0)
				{
					movespan[i] += movespan[i-1];
				}
			}
			for(size_t i = 0;i < movespan.size();i++)
			{
				movespan[i] *= m_DisSpan/movespan[movespan.size()-1]/1000;
			}
			
			//CString strtemp;
			//strtemp.Format("%f",movespan[movespan.size()-1]);
			//AfxMessageBox(strtemp);
		}
		for(size_t i = 0;i < movespan.size();i++)
		{
			movespan[i] += chirpBeginPos;
		}
		//CString ttt = "";
		//for(int i = 0;i < movespan.size();i++)
		//{
		//	ttt.Format("%s   %f",ttt,movespan[i]);
		//}
		//AfxMessageBox(ttt);
		UpdateData(TRUE);
		m_curtime = 0;
		UpdateData(FALSE);
		COMPort.Send("gzk\r\n");
		SetTimer(MOVETIMER_TIMER_ID,m_TimeSpan,NULL);
	}
	else
	{
		AfxMessageBox("请先打开串口");
	}
}

void CFBGDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(m_TotalTime <= 0 || m_TimeSpan <= 0 || m_DisSpan <= 0)
	{
		AfxMessageBox("参数不可为负!!");
		return;
	}
	if(COMIsOpen)
	{
		movecounter = int(double(m_TotalTime)/m_TimeSpan);
		moveidx = 0;
		chirpBeginPos = curPos;
		if(!IschripMode)
		{
			movespan.assign(movecounter,float(-m_DisSpan)/1000);
			for(size_t i = 1;i < movespan.size();i++)
			{
				movespan[i] += movespan[i-1];
			}
		}
		else
		{
			movespan.assign(movecounter,0);
			for(size_t i = 0;i < movespan.size();i++)
			{
				double idx = i;
				// 修改切枝波形1！！[还有一个地方要修改]
				movespan[i] = 1-pow(EE,-(6*(idx-movecounter/2)/movecounter)*(6*(idx-movecounter/2)/movecounter));
				if(i != 0)
				{
					movespan[i] += movespan[i-1];
				}
			}
			for(size_t i = 0;i < movespan.size();i++)
			{
				movespan[i] *= -m_DisSpan/movespan[movespan.size()-1]/1000;
			}
		}
		for(size_t i = 0;i < movespan.size();i++)
		{
			movespan[i] += chirpBeginPos;
		}
		UpdateData(TRUE);
		m_curtime = 0;
		UpdateData(FALSE);
		COMPort.Send("gzk\r\n");
		SetTimer(MOVETIMER_TIMER_ID,m_TimeSpan,NULL);
	}
	else
	{
		AfxMessageBox("请先打开串口");
	}
}

void CFBGDlg::OnBnClickedUpdatestate()
{
	// TODO: 在此添加控件通知处理程序代码
	SetTimer(SENDTS_TIMER_ID,400,NULL);
}

void CFBGDlg::OnBnClickedSetosabutton()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!GpibIsOpen)
	{
		AfxMessageBox("请先连接GPIB");
	}
	UpdateData(TRUE);
	CString str;
	str.Format("sens:swe:poin %d\n",m_pointnum);
	viPrintf (vi, str.GetBuffer());
	str.Format("sens:wav:star %dnm\n",m_BeginWL);
	viPrintf (vi, str.GetBuffer());
	str.Format("sens:wav:stop %dnm\n",m_EndWL);
	viPrintf (vi, str.GetBuffer());
	//RBW
	//
	int RBW[] = {20,50,100,200,500,1000,2000};
	CComboBox* RBWCOM = (CComboBox*)GetDlgItem(IDC_RBW_COMBO3);
	str.Format("SENSE:BANDWIDTH:RESOLUTION %dPM\n",RBW[RBWCOM->GetCurSel()]);
	viPrintf (vi, str.GetBuffer());
}

void CFBGDlg::OnBnClickedHomebutton1()
{
	// TODO: 在此添加控件通知处理程序代码
	COMPort.Send("1or\r\n");
}

float CFBGDlg::GetDiffPos()
{
	fstream  m_posfile;
	m_posfile.open(DIFFPOS_FILENAME,ios::in);
	if(!m_posfile)
	{
		return NULL_POSITION;
	}
	else
	{
		float pos;
		m_posfile>>pos;
		m_posfile.close();
		return pos;
	}
	
}

void CFBGDlg::SaveDiffPos( float pos )
{
	fstream  m_posfile;
	m_posfile.open(DIFFPOS_FILENAME,ios::out|ios::_Noreplace);
	m_posfile<<pos<<endl;
}

void CFBGDlg::MoveRelative( float dis )
{
	if(!COMIsOpen)
		return;
	CString str;
	str.Format("1PR%f\r\n",dis);
	COMPort.Send(str);
}

void CFBGDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	oriPos = curPos;
}

void CFBGDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	if(oriPos != NULL_POSITION && curPos != NULL_POSITION)
	{
		MoveRelative(oriPos-curPos);
	}
	else
	{
		AfxMessageBox("无法读取初始位置或读取当前位置，请重新设置");
	}
}


void CFBGDlg::OnBnChirpButtonCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_ChirpMode.GetCheck())
	{
		m_disSpanStatic.SetWindowText("总距离：");
		IschripMode = 1;
	}
	else
	{
		m_disSpanStatic.SetWindowText("距离步进：");
		IschripMode = 0;
	}
}

void CFBGDlg::MoveAbsolute( float dis )
{
	if(!COMIsOpen)
		return;
	CString str;
	str.Format("1PA%f\r\n",dis);
	COMPort.Send(str);
}


//Stop moving
void CFBGDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(MOVETIMER_TIMER_ID);
	COMPort.Send("gzg\r\n");
}

void CFBGDlg::OnBnClickedSavespectra()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!CurCurve)
	{
		AfxMessageBox("没有频谱数据可以保持!!");
		return;
	}
	CString   FilePathName;//文件名参数定义
	CFileDialog  Dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"TXT Files(*.txt)|*.txt|All Files(*.*)|*.*");
	//打开文件
	if(Dlg.DoModal() == IDOK)//是否打开成功
	{
		FilePathName =  Dlg.GetPathName();//取得文件路径及文件名
		if(FilePathName.Right(4).Compare(".txt"))
		{
			FilePathName += ".txt";
		}
		CFileStatus status;
		if (!CFile::GetStatus(FilePathName,status))
		{
			CFile fid(FilePathName, CFile::modeCreate|CFile:: modeReadWrite);
			CString strbuff = "";
			strbuff.Format("%sBegin WL:%.1f\n",strbuff,beginwl);
			strbuff.Format("%sStop WL:%.1f\n",strbuff,endwl);
			for(int i = 0;i < CurCurve->PointNum;i++)
			{
				strbuff.Format("%s%f\n",strbuff,CurCurve->PointData[i]);
			}
			fid.Write(strbuff,strbuff.GetLength());
			fid.Close();
		}
		else
		{
			AfxMessageBox("文件已存在，请选择一个新的路径！");
		}
	}
	else//打开失败处理
	{
		//打开失败处理
		MessageBox("文件保存失败!",NULL,MB_OK);
	}
}
