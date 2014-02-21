// FBGDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FBG.h"
#include "FBGDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CFBGDlg �Ի���




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


// CFBGDlg ��Ϣ�������

BOOL CFBGDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//Gpib��ʼ��
	
	Gpib_State_Flag = GPIB_CLOSE;

	FBG_StartFlag = 0;
	DiffCurv = NULL;
	DestinateCurv = NULL;
	CurvFromOSA = NULL;

	CEdit *DestinateCurvFileName = (CEdit*)GetDlgItem(IDC_DESTINATE_FILE);
	DestinateCurvFileName->EnableWindow(FALSE);

	CButton *FBGStartButton = (CButton*)GetDlgItem(IDC_STAR_FBG_BUTTON);
	FBGStartButton->EnableWindow(FALSE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFBGDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	
	//��ͼ
	if(CurvFromOSA)
		DrawCurve(*CurvFromOSA,DIFF_CURV_ID);

	if(DestinateCurv)
		DrawCurve(*DestinateCurv,MUBIAO_CURV_ID);
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	CPen pen_line(PS_SOLID,2,RGB(255,0,0));//������ɫ
	memDC.SelectObject(&pen_line);

	//�Զ����ͼ����
	memDC.FillSolidRect(CRect(0,0,rect.Width(),rect.Height()),RGB(180,180,180));//����
	
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

	//��ʾ����
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



//������Ӱ�ť
void CFBGDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CButton *GpibButton = (CButton*)GetDlgItem(GPIB_BUTTON_ID);
	CStatic *StateText = (CStatic*)GetDlgItem(GPIB_STATE_TEXT_ID);
	if (GPIB_CLOSE == Gpib_State_Flag)//��Ҫ����
	{
		if(GPIB_CONNECT_SUCCESS != OSA_Ando6370C.Connect(0,23,0))
		{
			AfxMessageBox("����ʧ�ܣ�");
			return;
		}
		GpibButton->SetWindowText("�Ͽ�");
		StateText->SetWindowText("������");

		CString str;
		CString str1 = OSA_Ando6370C.Read_Command("*IDN?");
		str.Format("GPIB���豸���ӳɹ���\n�豸��Ϣ��%s",str1.GetBuffer());
		AfxMessageBox(str);

		Gpib_State_Flag = GPIB_LINKED;
	}
	else//�Ͽ�
	{
		if(GPIB_DISCONNECT_SUCCESS != OSA_Ando6370C.Disconnect())
		{
			AfxMessageBox("���ӶϿ�ʧ�ܣ���");
			return;
		}
		GpibButton->SetWindowText("����");
		StateText->SetWindowText("�ر�");
		FBG_StartFlag = 0;
		KillTimer(1);
		Gpib_State_Flag = GPIB_CLOSE;
	}
}

//���������ַ
void CFBGDlg::OnBnClickedPhyaddressChange()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CPhyAddress* dial = new CPhyAddress();
	dial->NewPhyAddress = OSA_Ando6370C.GetPrimaryAddress();
	if (IDOK == dial->DoModal())
	{
		CString str;
		str.Format("%d",dial->NewPhyAddress);
		if(dial->NewPhyAddress == OSA_Ando6370C.GetPrimaryAddress())//û���޸�
			return;

		CStatic *PhysicsAddressText = (CStatic*)GetDlgItem(IDC_PHYSICSADDRESS);
		PhysicsAddressText->SetWindowText(str);
		OSA_Ando6370C.SetPrimaryAddress(dial->NewPhyAddress);
		if(Gpib_State_Flag == GPIB_LINKED)
		{
			OSA_Ando6370C.Disconnect();

			CButton *GpibButton = (CButton*)GetDlgItem(GPIB_BUTTON_ID);
			CStatic *StateText = (CStatic*)GetDlgItem(GPIB_STATE_TEXT_ID);
			GpibButton->SetWindowText("����");
			StateText->SetWindowText("�ر�");
			Gpib_State_Flag = GPIB_CLOSE;
		}
	}
}

void CFBGDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(Gpib_State_Flag == GPIB_LINKED)
	{
		CRect rect;
		GetDlgItem(DIFF_CURV_ID)->GetWindowRect(&rect);
		GetDlgItem(DIFF_CURV_ID)->GetParent()->ScreenToClient(rect);
	
		GetCurvFromOSA();

		//if(DiffCurv)
		//	delete DiffCurv;

		/*ͼ����Ŀ����������*/
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog pCFileDialog(TRUE,NULL,NULL,0,"Ŀ�������ļ�(*.txt)|*.txt||"); 
	pCFileDialog.m_ofn.lpstrTitle = "��Ŀ�������ļ� ";
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
	InfoText.Format(_T("Ŀ�����ߣ���ʼ����:%.2f nm,��ֹ����:%.2f nm"),DestinateCurv->Begin_WaveLength,DestinateCurv->End_WaveLength);
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!FBG_StartFlag && Gpib_State_Flag == GPIB_CLOSE)
	{
		AfxMessageBox("����ȷ��PC����ͨ��GPIB���������ͨ!!");
		return;
	}
	
	CButton *FBGStartButton = (CButton*)GetDlgItem(IDC_STAR_FBG_BUTTON);
	if(FBG_StartFlag == 0)
	{
		if(!MotorStart())
		{
			AfxMessageBox("�����������ʧ��!!�������ӣ�");
			return;
		}
		//�л���բ״̬���ر�
		CLightGateDlg *LightGateChangeDlg = new CLightGateDlg;
		LightGateChangeDlg->DoModal();

		FBG_StartFlag = 1;
		FBGStartButton->SetWindowText("ֹͣ������դ");
		SetTimer(1,50,NULL);
	}
	else
	{
		FBG_StartFlag = 0;
		FBGStartButton->SetWindowText("��ʼ������դ");
		KillTimer(1);
	}
}