/*++

Module Name:

	U3SLIB.h

Abstract:

	Header file for U3SLIB.lib, which exports functions for access to the 
	3S USB mass storage devices.

Author:

    Fly Chen

Environment:

    Win32 static library

Notes:

  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
  PURPOSE.

  Copyright (c) 2000 - 2001 3System Corporation.  All Rights Reserved.

Revision History:

    07/15/01 : created

--*/

#ifndef _U3SLIB_H_
#define _U3SLIB_H_

//Device Map(device type tags); (consistent to 3S USB function driver)
//U3s_GetDeviceMap() fills an array whose members are DEVICE_XXX as the followings.
//And, every U3S_XXXXXXXX() functions described below requires a input parameter which is
//whatever DEVICE_XXX. Please see U3s_GetDeviceMap() below for detail.
/* Formally, we call a 3S USB multi-purpose deivce with CF/SM/MMC card readers built in */
/* as a "DEVICE"; And the CF/SM/MMC card readers or USB Disk on the DEVICE as LUN(s).	*/
/* But sometimes we also call a card-reader or USB Disk as a DEVICE. So LUN_CF/LUN_SM.. */
/* could be more appropriate															*/
#define DEVICE_CF					0x01
#define DEVICE_SM					0x02
#define DEVICE_MMC					0x04
#define DEVICE_DISK					0x08
#define DEVICE_DISK_EX				0x09	//added for TGE "pseudo" LUN 1
#define DEVICE_NULL					0xFF

//max luns 3S's device may support
#define MAX_LUN						3
#define U3S_MAX_LUN					MAX_LUN

//frequently used signature. In fact, it = 'USBC' (from LSB to MSB)
#define TAG_USBC					0x43425355

//The SCSI capacity_data format, which U3s_ReadCapacity() command returns.
typedef struct _READ_CAPACITY_DATA {
    ULONG LogicalBlockAddress;
    ULONG BytesPerBlock;
} READ_CAPACITY_DATA, *PREAD_CAPACITY_DATA;

typedef union _LONG_BYTE 
{
	DWORD LONG;
	struct
	{
		BYTE BYTE0;
		BYTE BYTE1;
		BYTE BYTE2;
		BYTE BYTE3;
	} BYTES;
}LONG_BYTES, *P_LONG_BYTES;

//Smart Media page extension format(redundant area)
typedef struct _SM_PAGE_EX {
	ULONG UserData;
	UCHAR DataStatus;
	UCHAR BlockStatus;
	UCHAR BA11;
	UCHAR BA10;
	UCHAR Ecc20;
	UCHAR Ecc21;
	UCHAR Ecc22;
	UCHAR BA21;
	UCHAR BA20;
	UCHAR Ecc10;
	UCHAR Ecc11;
	UCHAR Ecc12;
} SM_PAGE_EX,*PSM_PAGE_EX;

//Configuration of a Smart Media
typedef struct _SM_ID{
	UCHAR	DeviceCode;
	USHORT	PageDataSize;
	UCHAR	PageExtSize;
	UCHAR	BlockSize;
	UCHAR	Status;
	USHORT	Blocks;
} SM_ID, *PSM_ID;

/* The "static" statement make files that include this file have its */
/* own memory for holding the table below	                         */
//A table to map DeviceCode into PageDataSize/PageExtSize/BlockSize...
//For DEV_CODE_TO_SM_CFG() function to use
static SM_ID SM_TBL[14] = 
{	//DeviceCode	PageDataSize	PageExtSize	BlockSize	Status	Blocks
	{0x6e,			256,			8,			0x10,		0x08,	0x100},	//1M
	{0xe8,			256,			8,			0x10,		0x08,	0x100},	//1M
	{0xec,			256,			8,			0x10,		0x08,	0x100},	//1M
	{0x64,			256,			8,			0x10,		0x08,	0x200},	//2M
	{0xea,			256,			8,			0x10,		0x08,	0x200},	//2M
	{0x6b,			512,			16,			0x10,		0,		0x200},	//4M
	{0xe3,			512,			16,			0x10,		0,		0x200},	//4M
	{0xe5,			512,			16,			0x10,		0,		0x200},	//4M
	{0xe6,			512,			16,			0x10,		0,		0x400},	//8M
	{0x73,			512,			16,			0x20,		0,		0x400},	//16M
	{0x75,			512,			16,			0x20,		0,		0x800},	//32M
	{0x76,			512,			16,			0x20,		0x20,	0x1000},//64M
	{0x79,			512,			16,			0x20,		0x20,	0x2000},//128M
	{0xff,			0xffff,			0xff,		0xff,		0xff,	0xffff}
};

//Given a Smart Media deivce_code, return a pointer with which we can know the 
//device's PageDataSize/PageExtSize... information
PSM_ID DEV_CODE_TO_SM_CFG(BYTE dev_code);

#pragma pack(1) /* disabe the following structures' WORD/DWORD/QUADWORD/ alignment(no padding) */
//3S's USB Disk "private CIS" page; for 3S use only
typedef struct _DISK_CIS_PAGE {
	ULONG	Tag_CIS;			//0-3
	UCHAR	Tbl_GoodBlk[256]; 	//4-259
	ULONG	Tbl_BankPages[8];	//260-291 
	ULONG	Tag_VID;			//292-295
	USHORT	VendorId;			//296-297
	USHORT	ProductId;			//298-299
	ULONG	Tag_LbaOffset;		//300-303 Tag is valid ==> LbaOffset is valid==>flash is split
	                            //        into 2 LUNs; Tag is invalid==>flash is not split(1 LUN)
	ULONG	LbaOffset;			//304-307
	UCHAR	SecureMethod;		//308
	UCHAR	Support1E;			//309 0x1E ==> support, 0 ==> not support
	UCHAR	TGELun0Attrib;		//310 1 ==> secured(need password for access), 0 ==> public
	UCHAR	TGELun1Attrib;		//311 1 ==> secured, 0 ==> public
	ULONG	Tag_Pwd;			//312-315 Tag is valid ==> page (cis+2) contains valid 3S user password
	UCHAR	Floppy;				//316
	UCHAR	WakeupPWDProtect;	//317
	UCHAR	AllZeros[194];		//318-511
	UCHAR	All0xFF6[6];		//512-517
	UCHAR	AllZeros10[10]; 	//518-527
} DISK_CIS_PAGE, *PDISK_CIS_PAGE;

//USB disk vendor page loacated next to disk_cis_page
typedef struct _DISK_VENDOR_PAGE {
	ULONG	Tag_Vendor;			//all the following tags are TAG_USBC				0~3
	/* part of inquiry_data. O.S shows the VendorStr[] ProductStr[] as the device description string */
	UCHAR	DeviceType;			//0 Direct Access Device							~4
	UCHAR	RMB;				//1 RMB bit is set by inquiry data					~5
	UCHAR	AnsiVer;			//2 ANSI ver										~6
	UCHAR	DataFormat;			//3 Data format = 2									~7
	ULONG	AdditionalLength;	//4 Additional length	(changed to 0 from 0x75)	~11
	UCHAR	VendorStr[8];		//8-15 , Manufacturer 8 digits						~19
	UCHAR	ProductStr[16];		//16-31 product name								~35
	UCHAR	RevStr[4];			//32-35 Revision 4 digits							~39
	/* end of inquiry_data part. See SCSI inquiry_data format for detail	*/
	/* the following members are for 3S's private usage						*/
	ULONG	Tag_Boundary;		//TAG_USBC											~43
	USHORT	Boundary;			//													~45
	ULONG	Tag_Banks;			//													~49		
	UCHAR	TotalBanks;			//													~50
	ULONG	Tag_Expandable;		//													~54
	UCHAR	Expandable;			//													~55
	ULONG	Tag_Page1stBank;	//													~59
	ULONG	PagesOf1stBank;		//													~63
	ULONG	Tag_PageTotalBanks;	//													~67
	ULONG	PagesOfTotalBanks;	//													~71
	UCHAR	Rev[456];			//													~527
	/* end of 3S's private usage											*/
} DISK_VENDOR_PAGE, *PDISK_VENDOR_PAGE;

/* for 3S's private usage, and customers */
typedef struct _DISK_VENDOR_REST_BANK_PAGE{
	ULONG	Tag_RestBankPage;
	ULONG	PagesOfRestBanks;
	UCHAR	Rev[512 - 8];
	UCHAR	AllZeros6_[6];
	USHORT	Tag_0;
	UCHAR	ECC_0[3];
	USHORT	Tag_1;
	UCHAR	ECC_1[3];
} DISK_VENDOR_REST_BANK_PAGE, *PDISK_VENDOR_REST_BANK_PAGE;

typedef struct _U3S_MSC_DEVICE_INFO {
	UCHAR	FwVersion;				//0		F/W version
	UCHAR	Rev0;					//1		Reserved
	UCHAR	Rev1;					//2		Reserved
	UCHAR	Rev2;					//3		Reserved
	USHORT	VendorId;				//4-5	Vendor ID
	USHORT	ProductId;				//6-7	Product ID
	UCHAR	DeviceMap;				//8		Device map(see struct DEVICE_MAP)
	UCHAR	MaxLun;					//9		Max Lun(see struct DEVICE_MAP)
	UCHAR	ExternSram;				//10	Is external SRAM existing?
	UCHAR	AutoLun;				//11	If F/W use "AutoLunnig"(FIFS, first in first served)?
	UCHAR	Rev5; 					//12	Reserved 
	UCHAR	Rev6; 					//13	Reserved 
	UCHAR	Rev7; 					//14	Reserved 
	UCHAR	Rev8; 					//15	Reserved 
	//UCHAR	Rev9[512 - 16];
} U3S_MSC_DEVICE_INFO, *PU3S_MSC_DEVICE_INFO;
#pragma pack() //restore the structure alignment method specified in compiler's /Zp[n] argument

//command result status that U3s_GetLastError() function returns
#define STATUS_CMD_NO_ERROR				0x00000000
#define STATUS_CMD_NO_MEDIA				0x00000002
#define STATUS_CMD_MEDIA_CHANGE			0x00000004
#define STATUS_CMD_ERASE_ERROR			0x00000009
#define STATUS_CMD_WRITE_ERROR			0x0000000A
#define STATUS_CMD_MEDIA_WRITE_PROTECT	0x00000005
#define STATUS_CMD_MEDIA_ERROR			0x00000006
#define STATUS_CMD_ILLEGAL_REQUEST		0x00000007
#define STATUS_CMD_INVALID_CDB			0x00000008
#define STATUS_CMD_TIME_OUT				0x000000F0
#define STATUS_CMD_FATAL_ERROR			0x000000FF

//media status 3S's F/W uses internally. 
//U3s_SetMediaStatus() also set these status to LUN for special usage. 
#define STATUS_MEDIA_PRESENT			0x01
#define STATUS_MEDIA_CHANGE				0x02
#define STATUS_MEDIA_PWD_SET			0x04
#define STATUS_MEDIA_SECURED			0x04
#define	STATUS_MEDIA_WRITE_PROTECT		0x08
#define STATUS_MEDIA_PWD_PROTECT		0x10

//What security method(lun attribute) we currently use
#define LUN_ATTRIB_SECURE_3S		0x10
#define LUN_ATTRIB_SECURE_TGE		0x20

extern "C" {


//Conver a DWORD from little-endian to big-endian
DWORD BIG_TO_LITTLE(DWORD x);


} //end extern "C" {

#endif // _U3SLIB_H_