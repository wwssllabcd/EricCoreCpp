#pragma once

#include "../Utility/DefineFiles/Definefile.h"

#include <string>
using namespace std;

#define	FLAG_DATA_IN			0x00000002
#define	FLAG_DATA_OUT			0x00000004

#define OS_NOT_SUPPORT			0
#define OS_WIN_98				1
#define OS_WIN_2K				2
#define OS_WIN_ME				3
#define OS_WIN_XP				4
#define OS_WIN_VISTA			6
#define OS_WIN_7	   		    6


class USBDrive
{
public:

	unsigned char TesterID;
	UCHAR INQUIRY_DATA[41] ;

	USBDrive(void);
	~USBDrive(void);
	void UDISK_OpenDevice( HANDLE * devhandle, int deviceType);
	void UDISK_CloseDevice(  HANDLE * devhandle );
	bool UDISK_SendCommand(HANDLE hUsb, PUCHAR cdb, PUCHAR DataBuffer, ULONG  DataTransferLen, BYTE fDirection);
	void UDISK_getDeviceName(tstring* DevHandleName );

	BOOL UDISK_Check3STester( HANDLE drvhandle );
	UCHAR UDISK_GetTesterID( HANDLE drvhandle );
	UCHAR UDISK_GetWinVersion();
	DWORD UDISK_InitializeAspi();
	DWORD UDISK_InitializeSpti();
	void UDISK_ScanDevice(int deviceType);
	bool getCF_testerSuccess( HANDLE handle, CHAR name, HANDLE* aryHandle, char* aryDriveName);

	DWORD SendAspiCommand(HANDLE drive_handle,BYTE *cdb,BYTE *iobuf,DWORD buflen,BYTE flags,WORD timeout);
	ULONG SendSptiCommand(HANDLE drive_handle,BYTE *cdb,BYTE *iobuf,DWORD buflen,BYTE flags,WORD timeout);


	//===================== USB ============================
	BOOL USB_InquiryData(HANDLE drive_handle, BYTE* buf_56);
	BOOL USB_ChkInquiryData(HANDLE drive_handle);
	BYTE USB_udisk_GetFwVersion(HANDLE hUsb);
	BOOL USB_udisk_GetDeviceInfo(HANDLE hUsb,PUCHAR buffer,ULONG len);



};
