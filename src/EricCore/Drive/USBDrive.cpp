#include "StdAfx.h"
#include ".\usbdrive.h"

//#include "..\stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <winioctl.h>

//#include "..\DefineFile\USB_VC.h"//定義檔

#include "WinInterface\usbdesc.h"
#include "WinInterface\devioctl.h" //copied from win98 DDK
#include "WinInterface\usbdrive.h" //usb driver interface definition
#include "WinInterface\usbdi.h"    //copied from win98 DDK
#include "WinInterface\usb_firm.h"
#include "WinInterface\scsidefs.h"
#include "WinInterface\wnaspi32.h"
#include "WinInterface\ntddscsi.h"
#include "WinInterface\spti.h"
#include "WinInterface\u3slib.h"


#define MAX_DEVICES 32

static	HANDLE			UsbDevice[MAX_DEVICES];
static	char			UsbDriver[MAX_DEVICES];
static UCHAR gfWinNT;
static BOOL gfAdministorAccount;
static BOOL fDosMode;
static UCHAR WinOS;

//------------ Definitions used internally only -----------------
#define INIT_OK			0x80000000
#define INIT_NO_ASPI	0x80000001
#define INIT_NO_SPTI	0x80000002
#define INIT_NO_MEM		0x80000003
#define INIT_NO_DEV		0x80000004
#define INIT_NO_PROC	0x80000005
#define INIT_NO_SCSI	0x80000006

#define SEND_ASPI_SPTI_CMD(hHandle, cdb, buf, len, flag, sec) \
	(gDllHandle) ? \
	(SendAspiCommand(hHandle, cdb, buf, len, flag, sec) == SS_COMP) : \
	(SendSptiCommand(hHandle, cdb, buf, len, flag, sec) == SCSISTAT_GOOD)

BOOL TestAspiDirect(HANDLE drive_handle);

//prototype of functions provided by WNASPI32.DLL
typedef DWORD (*GET_ASPI32_SUPPORT_INFO_PROC)();
typedef DWORD (*SEND_ASPI32_COMMAND_PROC)(LPSRB);

static volatile USHORT gAllocateCount;
//---------------- Functions used internally only ----------------------
static DWORD	SendAspiCommand(HANDLE drvhandle,BYTE *cdb,BYTE *iobuf,DWORD buflen,BYTE flags,WORD timeout);
static ULONG	SendSptiCommand(HANDLE drvhandle,BYTE *cdb,BYTE *iobuf,DWORD buflen,BYTE flags,WORD timeout);

//prototype of ASPI callback routine; we do not use it currently
static void		ASPI32Post(PSRB_ExecSCSICmd srb);
static DWORD	InitializeAspi();
static DWORD	InitializeSpti();
static BOOL		DeviceExist();

//---------------- Global variable used internally only ----------------
//ID of "logical" SCSI adapter/target, which our device attached to
static BYTE gAdapterCount = 0;

//handle to WNASPI32.DLL
static HINSTANCE gDllHandle = NULL;

//if the SCSI port or miniport driver supports SPTI_DIRECT
static BOOL gSptiDirect = FALSE;
static BOOL gSptiDirectTested = FALSE;

//address holders for ASPI32 provided functions
static GET_ASPI32_SUPPORT_INFO_PROC GetASPI32SupportInfoProc;
static SEND_ASPI32_COMMAND_PROC SendASPI32CommandProc;



//UCHAR INQUIRY_DATA[41] = {
//	0x00,	//0 Direct Access Device
//	0x80,	//1 RMB bit is set by inquiry data
//	0x02,	//2 ANSI ver
//	0x02,	//3 Data format = 2
//	0x00,	//4 Additional length	(changed to 0 from 0x75)
//	0,0,0,	//5-7
//	//8-15 , Manufacturer 8 digits
//	'G','E','N','E','R','I','C',' ',
//	//16-31 product name
//	'U','S','B',' ','M','a','s','s',' ','S','t','o','r','a','g','e',
//	//32-35 Revision 4 digits
//	'1','.','0','0',
//	//36-40-50 VendorSpecific: "3SDSK"3SCF "/"3SSM "/"3SMMC"
//	'3','S',' ',' ',' '
//};
//unsigned char TesterID;


USBDrive::USBDrive(void)
:TesterID(0)
{
	//UCHAR INQUIRY_DATA[41] = {
	//0x00,	//0 Direct Access Device
	//0x80,	//1 RMB bit is set by inquiry data
	//0x02,	//2 ANSI ver
	//0x02,	//3 Data format = 2
	//0x00,	//4 Additional length	(changed to 0 from 0x75)
	//0,0,0,	//5-7
	////8-15 , Manufacturer 8 digits
	//'G','E','N','E','R','I','C',' ',
	////16-31 product name
	//'U','S','B',' ','M','a','s','s',' ','S','t','o','r','a','g','e',
	////32-35 Revision 4 digits
	//'1','.','0','0',
	////36-40-50 VendorSpecific: "3SDSK"3SCF "/"3SSM "/"3SMMC"
	//'3','S',' ',' ',' '
	//};
	
	memset(INQUIRY_DATA, 0, sizeof(INQUIRY_DATA));
}

USBDrive::~USBDrive(void)
{
}


//---------------------------------------------------------------------------------------
bool USBDrive::UDISK_SendCommand(HANDLE hUsb,PUCHAR cdb,PUCHAR DataBuffer,ULONG  DataTransferLen,UCHAR  fDirection)
{
	UCHAR flags=0;
	if(DataTransferLen==0)			fDirection=0;

	if (fDirection & FLAG_DATA_IN)	{
		flags |= ((gDllHandle) ? (SRB_DIR_IN) : (SCSI_IOCTL_DATA_IN));
	}
	
	if (fDirection & FLAG_DATA_OUT) {
		flags |= ((gDllHandle) ? (SRB_DIR_OUT) : (SCSI_IOCTL_DATA_OUT));
	}

	BOOL result = SEND_ASPI_SPTI_CMD(hUsb,cdb,DataBuffer,DataTransferLen,flags, 30);
	if(result==TRUE){
		return true;
	}
	return false;
}

//---------------------------------------------------------------------------------------
UCHAR USBDrive::UDISK_GetWinVersion()
{
   OSVERSIONINFOEX osvi;
   BOOL bOsVersionInfoEx;

   // Try calling GetVersionEx using the OSVERSIONINFOEX structure.
   // If that fails, try using the OSVERSIONINFO structure.
   ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
   osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

   if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) ) 
   {
      // If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.
      osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
      if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) )	return FALSE;
   }

   switch (osvi.dwPlatformId) 
   {
      // Tests for Windows NT product family.
      case VER_PLATFORM_WIN32_NT:
			if ( (osvi.dwMajorVersion==5)&&(osvi.dwMinorVersion==0) )	return OS_WIN_2K;
			if ( (osvi.dwMajorVersion==5)&&(osvi.dwMinorVersion==1) )	return OS_WIN_XP;
			if ( (osvi.dwMajorVersion==6)&&(osvi.dwMinorVersion==0) )	return OS_WIN_VISTA;
			if ( (osvi.dwMajorVersion==6)&&(osvi.dwMinorVersion==1) )	return OS_WIN_7;
			break;

      // Test for the Windows 95 product family.
      case VER_PLATFORM_WIN32_WINDOWS:
			if ( (osvi.dwMajorVersion==4)&&(osvi.dwMinorVersion==10) )	return OS_WIN_98;
			if ( (osvi.dwMajorVersion==4)&&(osvi.dwMinorVersion==90) )	return OS_WIN_ME;
			break;
   }
   return OS_NOT_SUPPORT; 
}

/*---------------------------------------------------------------------------------------
	Search device through SPTI interface
---------------------------------------------------------------------------------------*/
DWORD USBDrive::UDISK_InitializeSpti()
{
	//-- Assume it is Win2000 with our driver so SPTI Direct is not supported --
	gSptiDirect = FALSE;

	//-- Test if SPTI Direct supported in getdevicemap() --
	gSptiDirectTested = FALSE;

	CHAR 	string[25];
	HANDLE	drive_handle;

	STRCPY(string, "\\\\.\\C:");
	drive_handle = CreateFile( string,
							   GENERIC_WRITE | GENERIC_READ,
							   FILE_SHARE_READ | FILE_SHARE_WRITE,
							   NULL,
							   OPEN_EXISTING,
							   0,
							   NULL);

	gfAdministorAccount=0;
	if (drive_handle != INVALID_HANDLE_VALUE) 
	{
		gfAdministorAccount=1;
		CloseHandle(drive_handle);
	}

	return INIT_OK;
}

/*---------------------------------------------------------------------------------------
	See if ASPI manager exists;	
	Return initialization status
	A DLL handle is opened to ASPI manager if exists
---------------------------------------------------------------------------------------*/
DWORD USBDrive::UDISK_InitializeAspi()
{
	DWORD	aspi_status;
    //BOOL	found = FALSE;

	//--See if win32 aspi supported by O.S(Win95/98/ME)
	if( (gDllHandle = LoadLibrary("WNASPI32.DLL")) == NULL )
	{		
		//printf("LoadLibrary for WNASPI32.DLL fails\n");
		return INIT_NO_ASPI;
	}

	GetASPI32SupportInfoProc = 
		(GET_ASPI32_SUPPORT_INFO_PROC) GetProcAddress(gDllHandle, "GetASPI32SupportInfo");

	if(GetASPI32SupportInfoProc == NULL) 
	{
		FreeLibrary(gDllHandle);
		//printf("GetProcAddress() for GetASPI32SupportInfo() fails\n");
		return INIT_NO_PROC;
	}

	SendASPI32CommandProc = 
		(SEND_ASPI32_COMMAND_PROC) GetProcAddress(gDllHandle, "SendASPI32Command");

	if(SendASPI32CommandProc == NULL) 
	{
		FreeLibrary(gDllHandle);
		//printf("GetProcAddress() for SendASPI32Command() fails\n");
		return INIT_NO_PROC;
	}

	aspi_status = (*GetASPI32SupportInfoProc)();
	gAdapterCount = (LOBYTE(LOWORD(aspi_status)));

	if (   (gAdapterCount == 0) 
		|| (HIBYTE(LOWORD(aspi_status)) != SS_COMP)) 
	{
		FreeLibrary(gDllHandle);
  		return INIT_NO_SCSI;
	}

	if( gDllHandle == NULL)
	{
		FreeLibrary(gDllHandle);
		return INIT_NO_MEM;
	}

	return INIT_OK;
}

//---------------------------------------------------------------------------------------
BOOL USBDrive::UDISK_Check3STester( HANDLE drvhandle )
{
	UCHAR	cdb[12];
	char	buf[56];

	ZeroMemory(cdb,12);
	ZeroMemory(buf,56);

	cdb[0] = SCSIOP_INQUIRY;	
	cdb[4] = 56;

	if (UDISK_SendCommand(drvhandle,cdb,(UCHAR*)&buf[0],56,FLAG_DATA_IN)!=FALSE)
	{
		if (memcmp(&buf[36],"3STSR",5)==0)	return( true );
	}
	return( false );
}

//---------------------------------------------------------------------------------------
UCHAR USBDrive::UDISK_GetTesterID( HANDLE drvhandle )
{
	UCHAR	cdb[12];
	UCHAR	id;

	ZeroMemory(cdb,12);

	cdb[0] = 0xFF;	
	cdb[1] = 0x03;	
	cdb[2] = 0xC0;

	if (UDISK_SendCommand(drvhandle,cdb,&id,1,FLAG_DATA_IN)!=FALSE)		return( id );
	return( 0xEF );
}

//---------------------------------------------------------------------------------------
void USBDrive::UDISK_ScanDevice(int deviceType)
{
	
	CHAR	szDriveName[3]="A:";
	HANDLE	drvhandle;
	ULONG	mask;
	tstring	DeviceChain;
	

	if (gfWinNT==0) 
	{
		// not support
		throw exception("not support");
	}
	else 
	{
		ULONG	ulLogicalDrives;
		int usbCount=0;
		ulLogicalDrives=GetLogicalDrives();
		for(CHAR i=2;i<32;i++)			// do not access Disk-A & B
		{
			mask = (ULONG)(1<<i);
			if (!(ulLogicalDrives & mask))	continue;

			szDriveName[0]='A'+i;
 			if (GetDriveType(szDriveName)!=DRIVE_REMOVABLE) 		continue;

			//Open the Drive's handle
			CHAR	szDrive[25] = "\\\\.\\";
			strcat(szDrive,szDriveName);
			drvhandle = CreateFile( szDrive,
							   GENERIC_WRITE | GENERIC_READ,
							   FILE_SHARE_READ | FILE_SHARE_WRITE,
							   NULL,
							   OPEN_EXISTING,
							   0,
							   NULL);
			if (drvhandle==INVALID_HANDLE_VALUE)	continue;

			bool isCloseHandle = true;
			if( deviceType==0){
				//-- check this handle is 3S MP Tester or not --
				if ( getCF_testerSuccess(drvhandle, szDriveName[0], UsbDevice, UsbDriver) == true){
					isCloseHandle = false;
				}
			}else if (deviceType==1){//usb
				BOOL status= TRUE;
				if ( status == TRUE){
					//BYTE bFwVersion = USB_udisk_GetFwVersion(drvhandle);
					UsbDevice[usbCount] = drvhandle;
					UsbDriver[usbCount] = szDriveName[0];
					isCloseHandle = false;
					usbCount++;
				}
			}

			if (isCloseHandle ==true){
				CloseHandle(drvhandle);
			}
		}
	}
}

bool USBDrive::getCF_testerSuccess( HANDLE handle, CHAR name, HANDLE* aryHandle, char* aryDriveName){

	if (UDISK_Check3STester(handle))
	{
		BYTE TesterID = UDISK_GetTesterID(handle);
		if (TesterID!=0xEF)
		{
			aryHandle[(TesterID&0x0F)] = handle;
			aryDriveName[(TesterID&0x0F)] = name;
			return true;
		}
	}
	return false;
}

//---------------------------------------------------------------------------------------
void USBDrive::UDISK_OpenDevice( HANDLE *DevHandle, int deviceType)
{
	int		i;
	UCHAR	WinVersion;
	gAllocateCount=0;
	for( i=0;i<MAX_DEVICES;i++ )	UsbDevice[i] = INVALID_HANDLE_VALUE;
	for( i=0;i<MAX_DEVICES;i++ )	UsbDriver[i] = 'A';					// Jorry 2006.04.14

	WinVersion = UDISK_GetWinVersion();
	gfWinNT = (WinVersion==OS_WIN_2K || WinVersion==OS_WIN_XP||WinVersion==OS_WIN_VISTA || WinVersion==OS_WIN_7 )? 1:0;

	//--None ASPI O.S, such as WinNT/2K/XP --
	if (gfWinNT)	UDISK_InitializeSpti();	
	else			UDISK_InitializeAspi();

	//-- scan SCSI bus --
	UDISK_ScanDevice(deviceType);

	for( i=0;i<MAX_DEVICES;i++ )
	{
		DevHandle[i] = UsbDevice[i];
	/*	DevDriver[i] = UsbDriver[i];		*/							// Jorry 2006.04.14
	}
}


//---------------------------------------------------------------------------------------
void USBDrive::UDISK_CloseDevice(  HANDLE *DevHandle )
{


	if (gfWinNT)	//-- only Spti need to close handle --
	{
		for(int i=0;i<MAX_DEVICES;i++ )
			if(DevHandle[i]!=INVALID_HANDLE_VALUE)
			{
				CloseHandle(DevHandle[i]);
				DevHandle[i]=INVALID_HANDLE_VALUE;
			}
	}

	//-- free DLL --
	if (gDllHandle!=NULL)
	{
		FreeLibrary(gDllHandle);
		gDllHandle = NULL;
	}

	//U3SLIB is not necessarily unloaded even if U3s_Close() is called.
	//Maybe U3s_Initialize(), U3s_GetDeviceMap() will get called againg.
	//So reset the following global variable.
}


//------------------------------- Functions used internally only -------------------------------
DWORD USBDrive::SendAspiCommand(HANDLE drive_handle,BYTE *cdb,BYTE *iobuf,DWORD buflen,
		BYTE flags,WORD timeout)
{

	PSRB_ExecSCSICmd srb;
	srb = (PSRB_ExecSCSICmd)malloc(ASPI_SRB_LEN);
	if (srb==NULL)		return( SS_ABORT_FAIL );

	memset(srb, 0, ASPI_SRB_LEN);

	ULONG ul_handle = ULONG(drive_handle);
	srb->SRB_Cmd		= SC_EXEC_SCSI_CMD;
	srb->SRB_Flags		= flags;

	srb->SRB_Target	    = (UCHAR)((ul_handle    )&0xff);
	srb->SRB_HaId		= (UCHAR)((ul_handle>>8 )&0xff);
	srb->SRB_Lun		= (UCHAR)((ul_handle>>16)&0xff);


	srb->SRB_BufPointer= iobuf;
	srb->SRB_BufLen	= buflen;
	srb->SRB_SenseLen	= 0;
	srb->SRB_CDBLen	= 12;
	memcpy(srb->CDBByte, cdb, 12);

	DWORD SrbStatus = (*SendASPI32CommandProc)((LPSRB)srb);

	if(timeout) {
		for(WORD i = 0; i < timeout*1000; i++) {
			if(srb->SRB_Status != SS_PENDING) {
				SrbStatus = srb->SRB_Status;
				free(srb);
				return SrbStatus;
			} else
				Sleep(1);
		}
		free(srb);
		return (SrbStatus = SS_ABORTED); //times out
	} else {
		free(srb);
		return SrbStatus;
	}
}

ULONG USBDrive::SendSptiCommand(HANDLE	drive_handle,BYTE	*cdb,BYTE	*iobuf,DWORD	buflen,
		BYTE	flags,WORD	timeout)
{
    BOOL status = 0;
    SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER sptdwb;
	SCSI_PASS_THROUGH_WITH_BUFFERS *Sptwb;
    ULONG length = 0, bytes_returned = 0;
	ULONG input_buffer_length, output_buffer_length;

	//Allocate SPTI none-direct structure buffer plus 64K data buffer
	Sptwb = (SCSI_PASS_THROUGH_WITH_BUFFERS *)malloc( sizeof(SCSI_PASS_THROUGH_WITH_BUFFERS) + 1024*64 );
	if (Sptwb==NULL)	return( SS_ABORT_FAIL );

	if(!gSptiDirect)	goto Use_Spti_None_Direct;

    ZeroMemory(&sptdwb, sizeof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER));

    sptdwb.sptd.Length = sizeof(SCSI_PASS_THROUGH_DIRECT);
	//PathId, TargetId, Lun is not important since we use drive handle
    sptdwb.sptd.PathId = 0;
    sptdwb.sptd.TargetId = 0;
    sptdwb.sptd.Lun = 0;
    sptdwb.sptd.CdbLength = 12;
    sptdwb.sptd.SenseInfoLength = 0;
    sptdwb.sptd.DataIn = flags;

	//Flyc:
	//Set the DataTransferLength to 1 fail SCSI_PASS_THROUGH DeviceIoControl(), this is 
	//MicroSoft bug(see 2000 DDK); As a temporary workaround, set DataTransferLength 
	//to a value other than 1
	if(buflen == 1) {
		sptdwb.sptd.DataTransferLength = 2;
		//[0524] Borrow this buffer for temporary use
		sptdwb.sptd.DataBuffer = Sptwb->ucDataBuf;
		if( !(flags & SCSI_IOCTL_DATA_IN) ) {//Data out
			Sptwb->ucDataBuf[0] = *iobuf;
		}
	} else {
		sptdwb.sptd.DataTransferLength = buflen;
		sptdwb.sptd.DataBuffer = iobuf;
	}
    sptdwb.sptd.TimeOutValue = timeout;
    sptdwb.sptd.SenseInfoOffset = NULL;
	memcpy(sptdwb.sptd.Cdb, cdb, 12);

    length = sizeof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER);
    status = DeviceIoControl(drive_handle,
                             IOCTL_SCSI_PASS_THROUGH_DIRECT,
                             &sptdwb,
                             length,
                             &sptdwb,
                             length,
                             &bytes_returned,
                             FALSE);

    if ((sptdwb.sptd.ScsiStatus == 0) && (status != 0)) {
		if(buflen == 1 && flags & SCSI_IOCTL_DATA_IN) {
			//[0524] Copy data from the temporary borrowed buffer
			*iobuf = Sptwb->ucDataBuf[0];
		}
		free(Sptwb);
		return SCSISTAT_GOOD;
    } else {
		//printf("SendSptiCommand() fail with ScsiStatus = %x, IoCtl Status = %x\n", 
		//	sptdwb.sptd.ScsiStatus, status);
		//PrintError(GetLastError());
		free(Sptwb);
		return SCSISTAT_CHECK_CONDITION; //Return an whatever error status;
	}

Use_Spti_None_Direct:

	ZeroMemory(Sptwb, sizeof(SCSI_PASS_THROUGH_WITH_BUFFERS) );

	if( buflen ) 
	{ 
		//-- data out --
		if( !(flags&SCSI_IOCTL_DATA_IN) ) 	memcpy((PUCHAR)&Sptwb->ucDataBuf[0],(PUCHAR)iobuf,buflen);
		//-- data in --
		else if(buflen<512)					ZeroMemory(Sptwb->ucDataBuf, buflen);
	}

    Sptwb->spt.Length = sizeof(SCSI_PASS_THROUGH);
	//PathId, TargetId, Lun is not important since we use drive handle
    Sptwb->spt.PathId = 0;
    Sptwb->spt.TargetId = 0;
    Sptwb->spt.Lun = 0;
    Sptwb->spt.CdbLength = 12;
    Sptwb->spt.SenseInfoLength = 0;
    Sptwb->spt.DataIn = flags;

	//Flyc:
	//Set the DataTransferLength to 1 fail SCSI_PASS_THROUGH DeviceIoControl(), this is 
	//MicroSoft bug(see 2000 DDK); As a temporary workaround, set DataTransferLength 
	//to a value other than 1
    Sptwb->spt.DataTransferLength = (buflen==1) ? 2 : buflen;
    Sptwb->spt.TimeOutValue = timeout;
    Sptwb->spt.DataBufferOffset = offsetof(SCSI_PASS_THROUGH_WITH_BUFFERS, ucDataBuf);
    Sptwb->spt.SenseInfoOffset = NULL;
	memcpy(Sptwb->spt.Cdb, cdb, 12);

	if(buflen) {
 		if( flags & SCSI_IOCTL_DATA_IN ) { //data in

			input_buffer_length = sizeof(SCSI_PASS_THROUGH);
			output_buffer_length = offsetof(SCSI_PASS_THROUGH_WITH_BUFFERS, ucDataBuf) +
				Sptwb->spt.DataTransferLength;
		} else { //data out
			input_buffer_length = offsetof(SCSI_PASS_THROUGH_WITH_BUFFERS, ucDataBuf) +
				Sptwb->spt.DataTransferLength;
			output_buffer_length = sizeof(SCSI_PASS_THROUGH);
		}
	} else {
		input_buffer_length = output_buffer_length = sizeof(SCSI_PASS_THROUGH);
	}

    status = DeviceIoControl(drive_handle, //    HANDLE  設備句柄：是從CreateFile返回的VxD句柄。
                             IOCTL_SCSI_PASS_THROUGH, //DWORD  控制碼：是用來制定VxD將要進行的操作。你應該在你要選用那種操作之前得到可能的dwIoControlCode值得列表。
                             Sptwb,// 輸入數據緩衝區指針：是包含了VxD完成dwIoControlCode所制定操作的數據的緩衝區地址。如果這個操作不需要數據，你可以傳為NULL。
                             input_buffer_length,// 輸入數據緩衝區長度：是由lpInBuffer所指向的緩衝區的地址的大小（byte）。
                             Sptwb,// 輸出數據緩衝區指針：是VxD程序在操作成功之後要將輸出數據輸出到的緩衝區。如果這個操作沒有任何返回值，這個值可以為NULL。
                             output_buffer_length,// 是lpOutBuffer所指向的緩衝區的大小（byte）。
                             &bytes_returned,// 是一個dword型變量的地址。這個變量用來接收VxD在lpOutBuffer中寫入數據的大小。
                             FALSE);// 重疊操作結構指針：lpOverlapped是一個OVERLAPPED結構的指針。如果你要一直等直到操作完成，這個值為NULL。

    if ((Sptwb->spt.ScsiStatus == 0) && (status != 0)) {

		if( buflen && (flags & SCSI_IOCTL_DATA_IN) ) { //data in
			//copy ucDataBuf to client's input buffer
			memcpy((PUCHAR)iobuf,(PUCHAR)&Sptwb->ucDataBuf[0],buflen);
		}
		free(Sptwb);
		return SCSISTAT_GOOD;

    } else {
		//printf("SendSptiCommand() fail with ScsiStatus = %x, IoCtl Status = %x\n", 
		//	gSptwb->sptd.ScsiStatus, status);
		//PrintError(GetLastError());
		free(Sptwb);
		return SCSISTAT_CHECK_CONDITION; //Return an whatever error status;
	}

}

void USBDrive::UDISK_getDeviceName(string* DevHandleName )
{
	int i=0;
	for(i=0;i<MAX_DEVICES;i++ ){
		char a= UsbDriver[i];
		DevHandleName[i] = a;
	}
}


BOOL USBDrive::USB_InquiryData(HANDLE drive_handle, BYTE* buf_56){
	UCHAR cdb[12];
	BOOL status;
	ZeroMemory(cdb,12);
	ZeroMemory(buf_56,56);

	cdb[0] = SCSIOP_INQUIRY;	
	cdb[4] = 56;

	status=UDISK_SendCommand(
		drive_handle,
		cdb,
		&buf_56[0],
		56,
		FLAG_DATA_IN
		);

	return status;
}



BOOL USBDrive::USB_ChkInquiryData(HANDLE drive_handle){
	UCHAR cdb[12];
	BOOL status;
	UCHAR temp_buf[56];

	ZeroMemory(cdb,12);
	ZeroMemory(temp_buf,56);

	cdb[0] = SCSIOP_INQUIRY;	
	cdb[4] = 56;

	status=UDISK_SendCommand(
		drive_handle,
		cdb,
		&temp_buf[0],
		56,
		FLAG_DATA_IN
		);

	BOOL result=FALSE;
	if(status == TRUE)
	{
		result=FALSE;
		if ((!memcmp((char *)&temp_buf[36],"3SDSK",5))		// for 663X
		 || (!memcmp((char *)&temp_buf[36],"HAGAK",4))
		 || (!memcmp((char *)&temp_buf[36],"5SDSK",5))		//for 665X
		 || (!memcmp((char *)&temp_buf[36],"6SDSK",5))		//for 667X
		 || (!memcmp((char *)&temp_buf[36],"6SDS",4))		//for all			// Jorry 2006.08.03
		 || (!memcmp((char *)&temp_buf[36],"FSDSK",5))
		 || (!memcmp((char *)&temp_buf[36],"TREKDSK",7))	// for TREK(0x83)	// Jorry 2006.01.03
		 || (!memcmp((char *)&temp_buf[36],"TESTK",5)))		//for finger print
			result=TRUE;

		if (!memcmp((char *)&temp_buf[36],"HSDSK",5))		result=TRUE;	// for UDRW2	// Jorry 2005.09.20
		if (memcmp((char *)&temp_buf[8],"HAGIWARA",8)==0)	result=TRUE;	// for UDRW2	// Jorry 2005.11.02
		
	}
	return result;
}


BYTE USBDrive::USB_udisk_GetFwVersion(HANDLE hUsb){
	BOOL status;

	UCHAR buffer[56];
	ZeroMemory(buffer,16);
	status=USB_udisk_GetDeviceInfo(hUsb,buffer,16);

	//-- check F/W is 0x72 or UDRW2(0x77) --						// Jorry 2005.09.20
	if (buffer[0]==0x72)											// Jorry 2005.09.20
	{																// Jorry 2005.09.20
		UCHAR cdb[12];												// Jorry 2005.09.20
		ZeroMemory(cdb,12);											// Jorry 2005.09.20
																	// Jorry 2005.09.20
		cdb[0] = SCSIOP_INQUIRY;									// Jorry 2005.09.20
		cdb[4] = 56;												// Jorry 2005.09.20
																	// Jorry 2005.09.20
		if (UDISK_SendCommand(hUsb,cdb,&buffer[0],56,FLAG_DATA_IN))	// Jorry 2005.09.20
		{
			//if (!memcmp((char *)&buffer[36],"HSDSK",5))			// Jorry 2005.09.20
			if (memcmp((char *)&buffer[36],"HSDSK",5)==0)					return(0x77);	// Jorry 2005.11.02
			if (memcmp((char *)&buffer[8],"HAGIWARA",8)==0)					return(0x77);	// Jorry 2005.11.02
		}
		buffer[0]=0x72;												// Jorry 2005.09.20
	}																// Jorry 2005.09.20
	return (status)?buffer[0]:0;
}


BOOL USBDrive::USB_udisk_GetDeviceInfo(HANDLE hUsb,PUCHAR buffer,ULONG len){
	UCHAR cdb[12];
	BOOL status;

	ZeroMemory(cdb,12);
	cdb[0]=OP_3S_VENDOR_CDB;
	cdb[1]=OP_GET_DEVICE_INFO;

	status=UDISK_SendCommand(hUsb,
				cdb,
				buffer,
				(len>16)?16:len,
				FLAG_DATA_IN
				);

	return status;

}