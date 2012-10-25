//
// This file is to describe the details of private commands. For the stardand
// command, please refer UFI spec.
//
// command format : total length is 12 BYTE 
//	BYTE 0 : operation code
//  BYTE 1 to BYTE 11 : depends on operation code.
//

#define OP_SM_WRITE_BAD				0xf1 
// Write bad mark (all 0x00)
// COMMAND {
//		UCHAR	opCode;
//		UCHAR   pageCount;
//		UCHAR	pageNo1L;
//		UCHAR	pageNo1M;
//		UCHAR	pageNo1H;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR   reserve;
//} COMMAND;

#define OP_SM_ERASE_BLOCK			0xf2
// Erase block
//	COMMAND {
//		UCHAR	opCode;
//		UCHAR   reserve;
//		UCHAR	pageNo1L;
//		UCHAR	pageNo1M;
//		UCHAR	pageNo1H;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR   reserve;
//} COMMAND;

#define OP_SM_PAGE_READ_EX			0xf3
// Get page_extended_part (16-byte) in contiguous pages
//	COMMAND {
//		UCHAR	opCode;
//		UCHAR   BlockSize;
//		UCHAR	pageNo1L;
//		UCHAR	pageNo1M;
//		UCHAR	pageNo1H;
//		UCHAR	pageCount;
//		UCHAR   reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR   reserve;
//} COMMAND;

#define OP_SM_BLOCK_READ_EX			0xf4
// Get page_extended_part in contiguous blocks. For each block, 
// it will return the extended part of first page and last page.
//	COMMAND {
//		UCHAR	opCode;
//		UCHAR   BlockSize;
//		UCHAR	pageNo1L;
//		UCHAR	pageNo1M;
//		UCHAR	pageNo1H;
//		UCHAR	pageCount;
//		UCHAR   reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR   reserve;
//} COMMAND;

#define OP_SM_READ					0xf5
// Get page_data_part (512 bytes)
//	COMMAND {
//		UCHAR	opCode;
//		UCHAR   BlockSize;
//		UCHAR	pageNo1L;
//		UCHAR	pageNo1M;
//		UCHAR	pageNo1H;
//		UCHAR	pageCount;
//		UCHAR   reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR   reserve;
//} COMMAND;

#define OP_SM_WRITE_ERASE			0xf6
//  Write page_data_part (512 bytes) to new block, and erase the old block.
//	COMMAND {
//		UCHAR	opCode;
//		UCHAR   BlockSize;
//		UCHAR	pageNo1L; //page for old block
//		UCHAR	pageNo1M;
//		UCHAR	pageNo1H;
//		UCHAR	pageCount;
//		UCHAR   pageOffset;
//		UCHAR	pageNo2L; //page for new block
//		UCHAR	pageNo2M;
//		UCHAR	pageNo2H;
//		UCHAR   BA1; // block address field
//		UCHAR   BA2;
//} COMMAND;

#define OP_SM_GET_DEVID				0xf7
// Get device id (1 byte)
//	COMMAND {
//		UCHAR	opCode;
//		UCHAR   reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR   reserve;
//} COMMAND;

#define OP_SM_WRITE					0xf8
//  Write page_data_part (512 bytes) to new block.
//	COMMAND {
//		UCHAR	opCode;
//		UCHAR   BlockSize;
//		UCHAR	pageNo1L;// page for old block
//		UCHAR	pageNo1M;
//		UCHAR	pageNo1H;
//		UCHAR	pageCount;
//		UCHAR   pageOffset;
//		UCHAR	pageNo2L;// page for new block
//		UCHAR	pageNo2M;
//		UCHAR	pageNo2H;
//		UCHAR   BA1;
//		UCHAR   BA2;
//} COMMAND;

#define OP_SM_SELECT_BANK			0xfb
// Change flash bank.
//	COMMAND {
//		UCHAR	opCode;
//		UCHAR   bank_no;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR   reserve;
//} COMMAND;

#define OP_SM_WRITE_PATTERN			0xc1
// Write pattern
//	COMMAND {
//		UCHAR	opCode;
//		UCHAR   pattern0;  // If pattern0 is non-zero, it will make a 528-byte pattern with pattern0, and 
//		UCHAR	pattern1;  // and write to desired pages. If pattern0 is 0, then it will make a 528-byte 
//		UCHAR	reserve;   // pattern with pattern1,pattern1+1,pattern1+2,...., and write to desired pages.	
//		UCHAR	reserve;
//		UCHAR	pageCount;
//		UCHAR   reserve;
//		UCHAR	pageNo2L;
//		UCHAR	pageNo2M;
//		UCHAR	pageNo2H;
//		UCHAR   reserve;
//		UCHAR   reserve;
//} COMMAND;
// Note that for 6633Bx, it will only make a 528-byte pattern with pattern0, and write to desired
// pages.

#define OP_SM_GET_BANKS				0xc2
// Get the total number of flash banks.
//	COMMAND {
//		UCHAR	opCode;
//		UCHAR   reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR   reserve;
//} COMMAND;

#define OP_SM_READ_NO_ECC			0xe8
// Read the whole pages (528 bytes) without ECC.
//	COMMAND {
//		UCHAR	opCode;
//		UCHAR   BlockSize;
//		UCHAR	pageNo1L;
//		UCHAR	pageNo1M;
//		UCHAR	pageNo1H;
//		UCHAR	pageCount;
//		UCHAR   reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR   reserve;
//} COMMAND;

#define OP_SM_WRITE_NO_ECC			0xe9
// Write the whole pages (528 bytes) without ECC.
//	COMMAND {
//		UCHAR	opCode;
//		UCHAR   BlockSize;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	pageCount;
//		UCHAR   pageOffset;
//		UCHAR	pageNo2L;
//		UCHAR	pageNo2M;
//		UCHAR	pageNo2H;
//		UCHAR   reserve;
//		UCHAR   reserve;
//} COMMAND;

#define OP_SM_REFRESH_CIS			0xc4
// Device will re-initialize again with reading the setting from CIS page.
//	COMMAND {
//		UCHAR	opCode;
//		UCHAR   reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR   reserve;
//} COMMAND;

#define OP_SM_GET_CIS_ADDR			0xc3
// Get the page address (4 bytes) of CIS block
//	COMMAND {
//		UCHAR	opCode;
//		UCHAR   reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR   reserve;
//} COMMAND;

#define OP_SET_MEDIA_STATUS			0xFE
// Set the current media status.
//	COMMAND {
//		UCHAR	opCode;
//		UCHAR   reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR   reserve;
//} COMMAND;
// Note that 6633B1 does not support it.

//The followings are vendor commands for 6633B2, 6655B1,...
#define OP_3S_VENDOR_CDB			0xFF
	#define OP_GET_MEDIA_STATUS			0x01
// Get the current media status
//	COMMAND {
//		UCHAR	opCode;
//		UCHAR   SubOpCode;
//		UCHAR	ReturnBytes; //1 or 2
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR   reserve;
//} COMMAND;
	#define OP_GET_DEVICE_INFO			0x02
// Get device information(16 bytes)
//	COMMAND {
//		UCHAR	opCode;
//		UCHAR   SubOpCode;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR   reserve;
//} COMMAND;
#define OP_GET_MAX_LUN				0x03
// Return Lun information (4 bytes)
//	COMMAND {
//		UCHAR	opCode;
//		UCHAR   SubOpCode;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR   reserve;
//} COMMAND;
//#define OP_GET_LUN_ATTRIB			0x04
#define OP_GET_CURRENT_BANK_ZONE	0x04
// Return the information (2 bytes) of current flash bank, and current zone, 
//	COMMAND {
//		UCHAR	opCode;
//		UCHAR   SubOpCode;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR   reserve;
//} COMMAND;

#define OP_TEST_PASSWORD			0x05
// Input password (20 bytes), please consult Fly for details.
//	COMMAND {
//		UCHAR	opCode;
//		UCHAR   SubOpCode;
//		UCHAR	reserve;
//		UCHAR	password_set;//0 or 1
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR   reserve;
//} COMMAND;
#define OP_TEST_KEY1_KEY2			0x06
// One of the method for Lock/Unlock the secured area.
//	COMMAND {
//		UCHAR	opCode;
//		UCHAR   SubOpCode;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR   reserve;
//} COMMAND;
#define OP_GET_RW_COUNT				0x07
// Return 1-byte read/write count
//	COMMAND {
//		UCHAR	opCode;
//		UCHAR   SubOpCode;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR   reserve;
//} COMMAND;

#define OP_LED_ON_OFF				0x08
// LED control
//	COMMAND {
//		UCHAR	opCode;
//		UCHAR   SubOpCode;
//		UCHAR	LedType;		//0,8  : for flash power Led
//		UCHAR	reserve;		//0xff : for access Led (but not support in 6633B2,6655B1E1)
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR	reserve;
//		UCHAR   reserve;
//		UCHAR   reserve;
//} COMMAND;
#define OP_USB_DISCONNECT			0x09

//
// The following vendor commands are available for 6655B1E2/6655B1E3
// For details, I need to check firmware.
//
	#define OP_READ_ZONE_IMAGE			0x0A
	#define OP_GET_LUN0_SEC				0x0B
	#define OP_SET_LUN0_SEC				0x0C
	#define OP_GET_LBA_OFFSET			0x0D
	#define OP_SET_LBA_OFFSET			0x0E
	#define OP_GET_TOTAL_BANK_PAGES		0x0F
	#define OP_SET_POWER_DOWN_MODE		0x10
	#define OP_GET_CURRENT_LUN			0x11
	#define OP_SET_CURRENT_LUN			0x12
	#define OP_GET_REFERENCE_COUNT		0x13
	#define OP_SET_REFERENCE_COUNT		0x14
	#define OP_GET_ALLOWED_PWD_TEST_COUNT 0x15
