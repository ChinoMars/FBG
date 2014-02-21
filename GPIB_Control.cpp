#include "StdAfx.h"
#include "GPIB_Control.h"

CGPIB_Control::CGPIB_Control(void)
{
	Device = 0;
	PrimaryAddress = 23;
}

CGPIB_Control::~CGPIB_Control(void)
{
}

void CGPIB_Control::GpibError( char *msg )
{
	CString str;
	str = msg;
	str.Format("%sibsta = &H%x  <",str,ibsta);
	if (ibsta & ERR )  str.Append(" ERR");
	if (ibsta & TIMO)  str.Append (" TIMO");
	if (ibsta & END )  str.Append (" END");
	if (ibsta & SRQI)  str.Append (" SRQI");
	if (ibsta & RQS )  str.Append (" RQS");
	if (ibsta & CMPL)  str.Append (" CMPL");
	if (ibsta & LOK )  str.Append (" LOK");
	if (ibsta & REM )  str.Append (" REM");
	if (ibsta & CIC )  str.Append (" CIC");
	if (ibsta & ATN )  str.Append (" ATN");
	if (ibsta & TACS)  str.Append (" TACS");
	if (ibsta & LACS)  str.Append (" LACS");
	if (ibsta & DTAS)  str.Append (" DTAS");
	if (ibsta & DCAS)  str.Append (" DCAS");


	str.Format("%s>\niberr = %d",str, iberr);
	if (iberr == EDVR) str.Append (" EDVR <DOS Error>\n");
	if (iberr == ECIC) str.Append (" ECIC <Not Controller-In-Charge>\n");
	if (iberr == ENOL) str.Append (" ENOL <No Listener>\n");
	if (iberr == EADR) str.Append (" EADR <Address error>\n");
	if (iberr == EARG) str.Append (" EARG <Invalid argument>\n");
	if (iberr == ESAC) str.Append (" ESAC <Not System Controller>\n");
	if (iberr == EABO) str.Append (" EABO <Operation aborted>\n");
	if (iberr == ENEB) str.Append (" ENEB <No GPIB board>\n");
	if (iberr == EOIP) str.Append (" EOIP <Async I/O in progress>\n");
	if (iberr == ECAP) str.Append (" ECAP <No capability>\n");
	if (iberr == EFSO) str.Append (" EFSO <File system error>\n");
	if (iberr == EBUS) str.Append (" EBUS <Command error>\n");
	if (iberr == ESTB) str.Append (" ESTB <Status byte lost>\n");
	if (iberr == ESRQ) str.Append (" ESRQ <SRQ stuck on>\n");
	if (iberr == ETAB) str.Append (" ETAB <Table Overflow>\n");

	str.Format("%sibcntl = %ld\n",str, ibcntl);
	AfxMessageBox(str);

	ibonl (Device,0);
}

int CGPIB_Control::GpibConnect(int mBoardIndex,int mPrimaryAddress,int mSecondaryAddress)
{
	Device = ibdev(                /* Create a unit descriptor handle         */
		mBoardIndex,              /* Board Index (GPIB0 = 0, GPIB1 = 1, ...) */
		mPrimaryAddress,          /* Device primary address                  */
		mSecondaryAddress,        /* Device secondary address                */
		T10s,                    /* Timeout setting (T10s = 10 seconds)     */
		1,                       /* Assert EOI line at end of write         */
		0);                      /* EOS termination mode                    */
	if (ibsta & ERR) 
	{   
		GpibError("ibdev Error");
		return IBDEV_ERROR;
	}
	ibclr(Device);                 /* Clear the device                        */
	if (ibsta & ERR) 
	{
		GpibError("ibclr Error");
		return IBCLR_ERROR;
	}
	PrimaryAddress = mPrimaryAddress;
	return GPIB_CONNECT_SUCCESS;
}

int CGPIB_Control::Gpid_Disconnect()
{
	ibonl(Device, 0);
	if (ibsta & ERR) 
	{
		GpibError("ibonl Error");	
		return IBONL_ERROR;
	}
	return GPIB_DISCONNECT_SUCCESS;
}

void CGPIB_Control::Gpib_Write_Command( char* str )
{
	if(!str)
		return;
	ibwrt(Device, str, strlen(str));
	if (ibsta & ERR) 
	{
		GpibError("ibwrt Error");
	}
}

CString CGPIB_Control::Gpib_Read_Command( char* str )
{
	Gpib_Write_Command(str);
	char* Buffer = new char[BUFFER_SIZE];             /* Read buffer                             */
	ibrd(Device, Buffer, BUFFER_SIZE);     /* Read up to 100 bytes from the device    */
	if (ibsta & ERR) 
	{
		GpibError("ibrd Error");	
	}
	Buffer[ibcntl-1] = '\0';         /* Null terminate the ASCII string         */
	return Buffer;
}
