/*++

Copyright (c) 1997-1998  Microsoft Corporation

Module Name:

    GUID829.h

Abstract:

 The below GUID is used to generate symbolic links to
  driver instances created from user mode

Environment:

    Kernel & user mode

Notes:

  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
  PURPOSE.

  Copyright (c) 1997-1998 Microsoft Corporation.  All Rights Reserved.

Revision History:

    11/18/97 : created

--*/
#ifndef GUID829H_INC
#define GUID829H_INC

#include <initguid.h>

// {00873FDF-61A8-11d1-AA5E-00C04FB1728B}  for BULKUSB.SYS
//for MP test
DEFINE_GUID(GUID_CLASS_U3S_TEST, 
0xf0b82a92, 0x32a1, 0x4a73, 0xb0, 0xef, 0x1a, 0x91, 0x4a, 0x50, 0x6e, 0x23);

//for TGE driver
DEFINE_GUID(GUID_CLASS_U3S_TGE, 
0x18c87713, 0x167e, 0x4e5c, 0xaf, 0x4e, 0xdd, 0xc6, 0x94, 0x76, 0x68, 0xd0);

DEFINE_GUID(GUID_CLASS_U3S_NORMAL, 
0xd0dbc300, 0xd03e, 0x11d4, 0x92, 0x39, 0xef, 0x2, 0x1d, 0x8a, 0x9e, 0x4e);

#define U3S_IOCTL_INDEX  0x0000

#define IOCTL_U3S_GET_CONFIG_DESCRIPTOR     CTL_CODE(FILE_DEVICE_UNKNOWN,  \
                                                   U3S_IOCTL_INDEX,\
                                                   METHOD_BUFFERED,  \
                                                   FILE_ANY_ACCESS)
                                                   
#define IOCTL_U3S_RESET_DEVICE				CTL_CODE(FILE_DEVICE_UNKNOWN,  \
                                                   U3S_IOCTL_INDEX+1,\
                                                   METHOD_BUFFERED,  \
                                                   FILE_ANY_ACCESS)                                                              
                                                   
#define IOCTL_U3S_RESET_PIPE				CTL_CODE(FILE_DEVICE_UNKNOWN,  \
                                                   U3S_IOCTL_INDEX+2,\
                                                   METHOD_BUFFERED,  \
                                                   FILE_ANY_ACCESS)                                                           

#define IOCTL_U3S_SEND_IDE_COMMAND			CTL_CODE(FILE_DEVICE_UNKNOWN,  \
                                                   U3S_IOCTL_INDEX+3,\
                                                   METHOD_BUFFERED,  \
                                                   FILE_ANY_ACCESS)                                                           

#define IOCTL_U3S_ON_REGISTER				CTL_CODE(FILE_DEVICE_UNKNOWN,  \
													U3S_IOCTL_INDEX+4,\
													METHOD_BUFFERED,  \
													FILE_ANY_ACCESS)
 
#define IOCTL_U3S_RESET_DEV				CTL_CODE(FILE_DEVICE_UNKNOWN,  \
                                                   U3S_IOCTL_INDEX+5,\
                                                   METHOD_BUFFERED,  \
                                                   FILE_ANY_ACCESS)

#define IOCTL_U3S_GET_MAX_LUN			CTL_CODE(FILE_DEVICE_UNKNOWN,  \
                                                   U3S_IOCTL_INDEX+6,\
                                                   METHOD_BUFFERED,  \
                                                   FILE_ANY_ACCESS)

#define IOCTL_U3S_GET_CAPACITY			CTL_CODE(FILE_DEVICE_UNKNOWN,  \
                                                   U3S_IOCTL_INDEX+7,\
                                                   METHOD_OUT_DIRECT,  \
                                                   FILE_ANY_ACCESS)

#define IOCTL_U3S_WRITE_LBA0				CTL_CODE(FILE_DEVICE_UNKNOWN,  \
                                                   U3S_IOCTL_INDEX+8,\
                                                   METHOD_OUT_DIRECT,  \
                                                   FILE_ANY_ACCESS)

#define IOCTL_U3S_FLASH_INIT			CTL_CODE(FILE_DEVICE_UNKNOWN,  \
                                                   U3S_IOCTL_INDEX+9,\
                                                   METHOD_OUT_DIRECT,  \
                                                   FILE_ANY_ACCESS)

#define IOCTL_U3S_LUN_MAP				CTL_CODE(FILE_DEVICE_UNKNOWN,  \
                                                   U3S_IOCTL_INDEX+13,\
                                                   METHOD_OUT_DIRECT,  \
                                                   FILE_ANY_ACCESS)

#define IOCTL_U3S_PROCESS_CDB_TASK		CTL_CODE(FILE_DEVICE_UNKNOWN,  \
                                                   U3S_IOCTL_INDEX+20,\
                                                   METHOD_OUT_DIRECT,  \
                                                   FILE_ANY_ACCESS)

#define IOCTL_U3S_GET_LAST_ERROR		CTL_CODE(FILE_DEVICE_UNKNOWN,  \
                                                   U3S_IOCTL_INDEX+21,\
                                                   METHOD_OUT_DIRECT,  \
                                                   FILE_ANY_ACCESS)

#define IOCTL_U3S_CHANGE_LUN			CTL_CODE(FILE_DEVICE_UNKNOWN,  \
                                                   U3S_IOCTL_INDEX+22,\
                                                   METHOD_OUT_DIRECT,  \
                                                   FILE_ANY_ACCESS)

#define IOCTL_U3S_GET_DEVICE_OBJECT		CTL_CODE(FILE_DEVICE_UNKNOWN,  \
                                                   U3S_IOCTL_INDEX+30,\
                                                   METHOD_OUT_DIRECT,  \
                                                   FILE_ANY_ACCESS)

#define IOCTL_U3S_ENABLE_BLINKING		CTL_CODE(FILE_DEVICE_UNKNOWN,  \
                                                   U3S_IOCTL_INDEX+31,\
                                                   METHOD_OUT_DIRECT,  \
                                                   FILE_ANY_ACCESS)

#define IOCTL_U3S_DISABLE_BLINKING		CTL_CODE(FILE_DEVICE_UNKNOWN,  \
                                                   U3S_IOCTL_INDEX+32,\
                                                   METHOD_OUT_DIRECT,  \
                                                   FILE_ANY_ACCESS)

#define IOCTL_U3S_SET_VAR				CTL_CODE(FILE_DEVICE_UNKNOWN,  \
                                                   U3S_IOCTL_INDEX+33,\
                                                   METHOD_OUT_DIRECT,  \
                                                   FILE_ANY_ACCESS)

#define IOCTL_U3S_GET_VAR				CTL_CODE(FILE_DEVICE_UNKNOWN,  \
                                                   U3S_IOCTL_INDEX+34,\
                                                   METHOD_OUT_DIRECT,  \
                                                   FILE_ANY_ACCESS)

#define DEVICE_CF	0x01
#define	DEVICE_SM	0x02
#define DEVICE_MMC	0x04
#define DEVICE_DISK	0x08


typedef struct _CDB_TASK
{
	ULONG	flags;
	ULONG	DataTransferLen;
	UCHAR	CDB[12];
} CDB_TASK,*PCDB_TASK;

//return status
#define STATUS_CMD_FAIL			0x01
#define STATUS_CMD_PHASE_ERROR  0x02
#define STATUS_CMD_MEDIA_WP		0x40
#define STATUS_CMD_HW_ERROR		0x80
#define STATUS_CMD_LUN_NOT_READY 0x03

// SCSI OP code
#define OP_FORMAT					0x04
#define OP_INQUIRY					0x12
#define OP_START_STOP				0x1B
#define OP_MODE_SENSE				0x5A
#define OP_MODE_SELECT				0x55
#define OP_MEDIUM_REMOVAL			0x1E
#define OP_READ_10					0x28
#define OP_READ_CAPACITY			0x25
#define OP_READ_FORMAT_CAPACITY		0x23
#define OP_REQUEST_SENSE			0x03
#define OP_REZERO_UNIT				0x01
#define OP_SEEK_10					0x2B
#define OP_SEND_DIAG				0x1D
#define OP_TEST_UNIT_READY			0x00
#define OP_VERIFY					0x2F
#define OP_WRITE_10					0x2A
#define OP_WRITE_VERIFY				0x2E

#define OP_SM_READ					0xf5
#define OP_SM_WRITE_BAD				0xf1 //write bad block
#define OP_SM_ERASE					0xf2
#define OP_SM_PAGE_READ_EX			0xf3
#define OP_SM_BLOCK_READ_EX			0xf4
#define OP_SM_GET_DEVICE_ID 		0xf7
#define OP_SM_READ_ONLY				0xfa
#define OP_SM_SELECT_BANK			0xfb
#define OP_SM_FORMAT				0xfc

#define OP_SM_READ_NO_ECC			0xe8
#define OP_SM_WRITE_NO_ECC			0xe9

#define OP_SM_QUICK_FORMAT			0xed
#define OP_SM_FORMAT_TEST			0xef
#define OP_SM_WRITE_PATTERN			0xc1

#define OP_SM_GET_BANKS				0xc2
#define OP_SM_GET_CIS_ADDR			0xc3

#define OP_SM_REFRESH_CIS			0xc4	

#endif // end, #ifndef GUID829H_INC
