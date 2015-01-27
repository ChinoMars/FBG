#pragma once

#include "SerialPort.h"
#include <afxmt.h>
#include <deque>
#include <vector>
#define PORT_DEBUG_MODE 1

#define MAX_PORT_NUM 256

#define OPEN_PORT_SUCCESS 1
#define OPEN_PORT_FAIL -1

#define CLOSE_PORT_SUCCESS 1
#define CLOSR_PORT_FAIL 0

#define PORT_NUM_INVALID -2
#define BAUDRATE_INVALID -3
#define DATABITS_INVALID -4
#define PARITY_INVALID -5
#define STOPBIT_INVALID -6

#define BUFSIZE 65536	

static const DWORD BAUDRATE[] = 
{CBR_110,CBR_300,CBR_600,CBR_1200,CBR_2400,CBR_4800,
CBR_9600,CBR_14400,CBR_19200,CBR_38400,CBR_56000 ,
CBR_57600,CBR_115200 ,CBR_128000 ,CBR_256000 
};

static const DWORD PARITY[] = {NOPARITY,ODDPARITY,EVENPARITY,MARKPARITY,SPACEPARITY};

static const DWORD STOPBITS[]={ONESTOPBIT,ONE5STOPBITS,TWOSTOPBITS};


class CSerialPortApi
{
public:
	CSerialPortApi(void);
	virtual ~CSerialPortApi(void);

	
	//��Ч�˿�
 	CString PortList[MAX_PORT_NUM];
	//��ȡ��Ч�˿���Ŀ
 	size_t GetPortNum();

	//�򿪴���
	INT OpenPort(CString sPort,DWORD dwBaudRate,BYTE byDataBits,BYTE byParity,BYTE byStopBits);
	//�رմ���
	INT ClosePort();
	//��������
	void Send(CString str);
	//�Ƿ��н�������δ��ȡ
	BOOL ReceiveFlag;
	//��ȡ���ܵ�����
	CString ReadRecv();
	vector<CString> ReadRecvByteSplite(char ch);
	CString ErrorMsg();
private:
	//************************************
	// Method:    getExistPort
	// FullName:  CSerialPortApi::getExistPort
	// Access:    public 
	// Returns:   void
	// ��ȡ���еĶ˿ںţ�����PortList��;
	// ����������Ч�˿���ĿPortNum
	//************************************
 	void getExistPort();

	HANDLE m_hCom;
 	size_t PortNum;
	CSerialPort m_port;
	BOOL b_portIsOpen;
	HANDLE m_hSendEvent;

	static UINT SendThreadProc(LPVOID pParam);
	static UINT RevThreadProc(LPVOID pParam);
	CString Str4Send;

	CCriticalSection m_bRevCS;
	deque<BYTE> m_dequeRevData;

	BOOL IsPortValid(CString sCom);
	BOOL IsBaudrateValid(DWORD dwBaudRate);
	BOOL IsDataBitValid(BYTE byDataBits);
	BOOL IsParityValid(BYTE byParity);
	BOOL IsStopbitValid(BYTE byStopBits);
};


typedef struct _SEND
{
	BOOL bFlag;
	DOUBLE value;
}SEND;