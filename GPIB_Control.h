#pragma once

#include "ni488.h"

#define IBDEV_ERROR -1
#define IBCLR_ERROR -2
#define IBONL_ERROR -3
#define GPIB_CONNECT_SUCCESS 1
#define GPIB_DISCONNECT_SUCCESS 1

#define BUFFER_SIZE 65535
class CGPIB_Control
{
public:
	int Device;                   /* Device unit descriptor                  */
	int PrimaryAddress;
	CGPIB_Control(void);
	~CGPIB_Control(void);


	void GpibError(char *msg);        /* Error function declaration              */
	int GpibConnect(int mBoardIndex,int mPrimaryAddress,int mSecondaryAddress);
	int Gpid_Disconnect();

	void Gpib_Write_Command(char* str);
	CString Gpib_Read_Command(char* str);
};
