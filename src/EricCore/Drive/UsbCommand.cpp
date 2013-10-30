#include "StdAfx.h"
#include "./UsbCommand.h"

#include "USBDrive.h"
#include "../Utility/UsbCmdSet/UsbCmdStruct.h"
#include "../Utility/UsbCmdSet/USB_3S_VENDOR_DEF.H"


#include <EricCore\Utility\Utility.h>
#include <EricCore\Utility\EricException\MyException.h>

using namespace EricCore;

UsbCommand::UsbCommand(HANDLE deviceHandle)
:m_dvrHandle(deviceHandle)
{
}

UsbCommand::~UsbCommand(void)
{
}

bool UsbCommand::sendCommand(BYTE* cdb, BYTE* buffer, int Length, BYTE direction){
	USBDrive usbDrive;
	bool status = usbDrive.UDISK_SendCommand(m_dvrHandle, cdb, buffer, Length, direction);
	return status;
}

void UsbCommand::getDeviceInfo(BYTE* buf16, int len){
	BYTE cdb[12];
	bool status;
	ZeroMemory(cdb,12);
	cdb[0]=OP_3S_VENDOR_CDB;
	cdb[1]=OP_3S_GET_DEVICE_INFO;

	USBDrive usbDrive;
	status=usbDrive.UDISK_SendCommand(
				m_dvrHandle,
				cdb,
				buf16,
				(len>16)?16:len,
				FLAG_DATA_IN
				);

	if(status == false){
		throw MyException(USBC_VENDOR_CMD_FAIL, "USBC_GETDEVICE_INFO_FAIL");
	}
}

void UsbCommand::getFlashIDs(BYTE* buffer){
	bool status;
	USBDrive usbDrive;
	BYTE cdb[12]={0};
	cdb[0]=0xff;
	cdb[1]=0x03;
	cdb[2]=0x68;
	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cdb, buffer, 512, FLAG_DATA_IN);
	if(status == false){
		throw MyException(USBC_VENDOR_CMD_FAIL, "USBC_GET_FALHIDS_FAIL");
	}
}

void UsbCommand::write8K(ULONG cycleList, BYTE* bufData, BYTE* bufExtraByte) const{
	bool status;
	USBDrive usbDrive;
	BYTE cdb[12]={0};
	cdb[0]=0xff;
	cdb[1]=0x24;
	cdb[2]=0x01;
	cdb[3]= (BYTE)(cycleList>>24)&0xFF;
	cdb[4]= (BYTE)(cycleList>>16)&0xFF;
	cdb[5]= (BYTE)(cycleList>>8)&0xFF;
	cdb[6]= (BYTE)cycleList&0xFF;
	cdb[7]= (BYTE)cycleList&0xFF;

	BYTE buf[9216];
	memcpy(buf     ,bufExtraByte,1024);
	memcpy(buf+1024,bufData,8192);

	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cdb, buf, 9216, FLAG_DATA_OUT);
	if(status == false){
		throw MyException(USBC_VENDOR_CMD_FAIL, "USBC_WRITE_8K_FAIL");
	}
}

void UsbCommand::write_and_verify8K(ULONG cycleList, BYTE* bufData, BYTE* bufExtraByte) const{
	bool status;
	USBDrive usbDrive;
	BYTE cdb[12]={0};
	cdb[0]=0xff;
	cdb[1]=0x24;
	cdb[2]=0x01;
	cdb[3]= (BYTE)(cycleList>>24)&0xFF;
	cdb[4]= (BYTE)(cycleList>>16)&0xFF;
	cdb[5]= (BYTE)(cycleList>>8)&0xFF;
	cdb[6]= (BYTE)cycleList&0xFF;

	BYTE buf[9216];
	memcpy(buf     ,bufExtraByte,1024);
	memcpy(buf+1024,bufData,8192);

	tstring msg;
	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cdb, buf, 9216, FLAG_DATA_OUT);
	if(status == false){
		msg = Utility::strFormat("USBC_WRITE_8K_VERIFY_FAIL at %X", cycleList);
		throw MyException(USBC_WRITE_8K_FAIL, msg);
	}

	this->read8K(cycleList, buf);
	int result = memcmp(buf, bufData, 8192);
	if( result!=0){
		msg = Utility::strFormat("USBC_WRITE_8K_VERIFY_FAIL at %X", cycleList);
		throw MyException(USBC_WRITE_8K_VERIFY_FAIL, msg);
	}

	result = memcmp(buf+8192, bufExtraByte, 1);
	if( result!=0){
		msg = Utility::strFormat("USBC_WRITE_8K_VERIFY_FAIL at %X", cycleList);
		throw MyException(USBC_WRITE_8K_FAIL, msg);
	}
}


void UsbCommand::downLoadISP(BYTE* buffer, BYTE bootCodeCtrl){
	bool status;
	USBDrive usbDrive;
	BYTE cdb[12]={0};
	cdb[0]=0xff;
	//cdb[1]=0x22;
	cdb[1]=0x50;
	cdb[2]=bootCodeCtrl;

	ULONG length = 4096;

	if(bootCodeCtrl!=0){
		length = 2048;
	}

	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cdb, buffer, length, FLAG_DATA_OUT);
	if(status == false){
		throw MyException(USBC_VENDOR_CMD_FAIL, "USBC_DOWNLOAD_ISP_FAIL");
	}
}

// 增加 Driver 能力
void UsbCommand::StrengthenDriver(BYTE TempReg130,BYTE TempReg145,BYTE TempReg20E)
{
	bool status;
	USBDrive usbDrive;
	BYTE buffer[1]={0};
	BYTE cdb[12]={0};
	cdb[0] = 0xFF;	
	cdb[1] = 0x71;
	cdb[2] = TempReg130;
	cdb[3] = TempReg145;
	cdb[4] = TempReg20E;	

	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cdb, buffer, 0x00, FLAG_DATA_OUT);
	if(status == false){
		throw MyException(USBC_VENDOR_CMD_FAIL, "USBC_STRENGTHEN_DRIVE_FAIL");
	}
}


void UsbCommand::eraseBlock(ULONG cycleList) const{
	bool status;
	USBDrive usbDrive;
	BYTE buffer[1];
	BYTE cdb[12]={0};
	cdb[0]=0xff;
	cdb[1]=0x26;
	
	cdb[3]= (BYTE)(cycleList>>24)&0xFF;
	cdb[4]= (BYTE)(cycleList>>16)&0xFF;
	cdb[5]= (BYTE)(cycleList>>8)&0xFF;
	cdb[6]= (BYTE)cycleList&0xFF;
	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cdb, buffer, 0, FLAG_DATA_OUT);
	if(status == false){
		throw MyException(USBC_VENDOR_CMD_FAIL, "USBC_ERASE_FAIL");
	}
}


void UsbCommand::scanErase(BYTE* buffer) const{
	bool status;
	USBDrive usbDrive;
	BYTE cdb[12]={0};
	cdb[0]=0xff;
	cdb[1]=0x28;
	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cdb, buffer, 2048, FLAG_DATA_OUT);
	if(status == false){
		if(buffer[0]==0){
			throw MyException(USBC_ERASE_ALL_FAIL, "USBC_ERASE_ALL_FAIL");
		}
		throw MyException(USBC_VENDOR_CMD_FAIL, "USBC_SCAN_ALL_FAIL");
	}
}


void UsbCommand::readRam(BYTE addr, BYTE* buffer) const{
	bool status;
	USBDrive usbDrive;
	UsbCmdStruct cmd;
	cmd = cmd.readRam();
	cmd.cdb[4]=addr;
	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buffer, cmd.length,  cmd.direction);
	if(status == false){
		tstring msg = cmd.description + " fail";
		throw MyException(USBC_VENDOR_CMD_FAIL, msg);
	}
}

void UsbCommand::writeRam(BYTE addr, BYTE* buffer) const{
	bool status;
	USBDrive usbDrive;
	UsbCmdStruct cmd;
	cmd = cmd.writeRam();
	cmd.cdb[4]=addr;
	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buffer, cmd.length, cmd.direction);
	if(status == false){
		tstring msg = cmd.description + " fail";
		throw MyException(USBC_VENDOR_CMD_FAIL, msg);
	}
}

void UsbCommand::testRam1k(BYTE addr, BYTE* buffer,int inOut, int len) const{
	bool status;
	USBDrive usbDrive;
	UsbCmdStruct cmd;
	cmd = cmd.writeRam();
	cmd.cdb[0]=0xFF;
	cmd.cdb[1]=0x55;
	cmd.cdb[2]=addr;
	cmd.cdb[3]=0;
	cmd.cdb[4]=0;
	cmd.cdb[5]=0;
	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buffer, len, inOut);
	if(status == false){
		tstring msg = cmd.description + " fail";
		throw MyException(USBC_VENDOR_CMD_FAIL, msg);
	}
}

void UsbCommand::read8K(ULONG cycleList, BYTE* buffer) const{
	BOOL status;
	USBDrive usbDrive;
	UsbCmdStruct cmd;
	cmd = cmd.read8k();
	cmd.cdb[3] = (BYTE)(cycleList>>24)&0xFF;
	cmd.cdb[4] = (BYTE)(cycleList>>16)&0xFF;
	cmd.cdb[5] = (BYTE)(cycleList>>8)&0xFF;
	cmd.cdb[6] = (BYTE)cycleList&0xFF;
	cmd.cdb[7] = (BYTE)cycleList&0xFF;

	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buffer, cmd.length,  cmd.direction);
	if(status == FALSE){
		tstring msg = cmd.description + " fail";
		throw MyException(USBC_VENDOR_CMD_FAIL, msg);
	}
}

void UsbCommand::read8K_withoutEcc(ULONG cycleList, BYTE* buffer) const{
	BOOL status;
	USBDrive usbDrive;
	UsbCmdStruct cmd;
	cmd = cmd.read8K_withoutECC();
	cmd.cdb[3] = (BYTE)(cycleList>>24)&0xFF;
	cmd.cdb[4] = (BYTE)(cycleList>>16)&0xFF;
	cmd.cdb[5] = (BYTE)(cycleList>>8)&0xFF;
	cmd.cdb[6] = (BYTE)cycleList&0xFF;

	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buffer, cmd.length,  cmd.direction);
	if(status == FALSE){
		tstring msg = cmd.description + " fail";
		throw MyException(USBC_VENDOR_CMD_FAIL, msg);
	}
}


void UsbCommand::write10(ULONG cycleList, WORD secCnt, BYTE* buffer) const{
	BOOL status;
	USBDrive usbDrive;
	UsbCmdStruct cmd;
	cmd = cmd.write10(cycleList, secCnt);
	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buffer, cmd.length,  cmd.direction);
	if(status == FALSE){
		tstring msg = cmd.description + " fail";
		throw MyException(USBC_SCSI_CMD_FAIL, msg);
	}
}

void UsbCommand::read10(ULONG cycleList, WORD secCnt, BYTE* buffer) const{
	BOOL status;
	USBDrive usbDrive;
	UsbCmdStruct cmd;
	cmd = cmd.read10(cycleList, secCnt);
	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buffer, cmd.length,  cmd.direction);
	if(status == FALSE){
		tstring msg = cmd.description + " fail";
		throw MyException(USBC_SCSI_CMD_FAIL, msg);
	}
}

void UsbCommand::homogeneousEnable(void) const{
	BOOL status;
	BYTE buffer[1];
	USBDrive usbDrive;
	UsbCmdStruct cmd;
	cmd = cmd.homogeneousEnable();
	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buffer, cmd.length,  cmd.direction);
	if(status == FALSE){
		tstring msg = cmd.description + " fail";
		throw MyException(USBC_VENDOR_CMD_FAIL, msg);
	}
}

void UsbCommand::homogeneousDisable(void) const{
	BOOL status;
	BYTE buffer[2];
	USBDrive usbDrive;
	UsbCmdStruct cmd;
	cmd = cmd.homogeneousDisable();
	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buffer, cmd.length,  cmd.direction);
	if(status == FALSE){
		tstring msg = cmd.description + " fail";
		throw MyException(USBC_VENDOR_CMD_FAIL, msg);
	}
}

void UsbCommand::reboot(void) const{
	BOOL status;
	BYTE buffer[1];
	USBDrive usbDrive;
	UsbCmdStruct cmd;
	cmd = cmd.reboot();
	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buffer, cmd.length,  cmd.direction);
	if(status == FALSE){
		tstring msg = cmd.description + " fail";
		throw MyException(USBC_VENDOR_CMD_FAIL, msg);
	}
}

void UsbCommand::reboot_OnCard(void) const{
	BOOL status;
	BYTE buffer[1];
	USBDrive usbDrive;
	UsbCmdStruct cmd;
	cmd = cmd.reboot_OnCardSort();
	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buffer, cmd.length,  cmd.direction);
	if(status == FALSE){
		tstring msg = cmd.description + " fail";
		throw MyException(USBC_VENDOR_CMD_FAIL, msg);
	}
}



void UsbCommand::set7Seg(int first, int second, int third) const{
	bool status;
	BYTE buffer[1];
	USBDrive usbDrive;
	UsbCmdStruct cmd;
	cmd = cmd.set7Segment();

	BYTE DISPLAY[16]={ 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x6F,0,0,0x39,0,0xf9,0xc7};

	cmd.cdb[3] = DISPLAY[first];
	cmd.cdb[4] = DISPLAY[second];
	cmd.cdb[5] = DISPLAY[third];

	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buffer, cmd.length,  cmd.direction);
	if(status == false){
		tstring msg = cmd.description + " fail";
		throw MyException(USBC_VENDOR_CMD_FAIL, msg);
	}
}


void UsbCommand::setFlashTiming() const{
	bool status;
	BYTE buffer[1];
	USBDrive usbDrive;
	UsbCmdStruct cmd;
	cmd = cmd.setFlashTiming();

	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buffer, cmd.length,  cmd.direction);
	if(status == false){
		tstring msg = cmd.description + " fail";
		throw MyException(USBC_VENDOR_CMD_FAIL, msg);
	}
}

void UsbCommand::onCardSort() const{
	bool status;
	BYTE buffer[1];
	USBDrive usbDrive;
	UsbCmdStruct cmd;
	cmd = cmd.onCardSort();

	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buffer, cmd.length,  cmd.direction);
	if(status == false){
		tstring msg = cmd.description + " fail";
		throw MyException(USBC_VENDOR_CMD_FAIL, msg);
	}
}

// for IM
void UsbCommand::write4K(ULONG cycleList, BYTE* aryData, BYTE* aryExtraByte) const{
	bool status;
	USBDrive usbDrive;

	UsbCmdStruct cmd;
	cmd = cmd.write4k();

	cmd.cdb[3]= (BYTE)(cycleList>>24)&0xFF;
	cmd.cdb[4]= (BYTE)(cycleList>>16)&0xFF;
	cmd.cdb[5]= (BYTE)(cycleList>>8)&0xFF;
	cmd.cdb[6]= (BYTE)cycleList&0xFF;

	BYTE buffer[5120];
	memcpy(buffer     , aryExtraByte, 1024);
	memcpy(buffer+1024, aryData, 4096);

	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buffer, cmd.length,  cmd.direction);
	if(status == false){
		tstring msg = cmd.description + " fail";
		throw MyException(USBC_VENDOR_CMD_FAIL, msg);
	}
}

// for IM
void UsbCommand::pollingOnCardSort(BYTE* buf_2Byte) const{
	bool status;
	USBDrive usbDrive;

	UsbCmdStruct cmd;
	cmd = cmd.pollOnCardSort();

	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buf_2Byte, cmd.length,  cmd.direction);
	if(status == false){
		tstring msg = cmd.description + " fail";
		throw MyException(USBC_VENDOR_CMD_FAIL, msg);
	}
}

// for IM
void UsbCommand::setFeature(BYTE ceNo, BYTE cmdBus, BYTE addrBus, BYTE value) const{
	BYTE buffer[2];
	bool status;
	USBDrive usbDrive;

	UsbCmdStruct cmd;
	cmd = cmd.SetFeature_IM();
	cmd.cdb[2] = ceNo;
	cmd.cdb[3] = cmdBus;
	cmd.cdb[4] = addrBus;
	cmd.cdb[5] = value;
	

	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buffer, cmd.length,  cmd.direction);
	if(status == false){
		tstring msg = cmd.description + " fail";
		throw MyException(USBC_VENDOR_CMD_FAIL, msg);
	}
}

// for IM
void UsbCommand::getFeature(BYTE ceNo, BYTE cmdBus, BYTE addrBus) const{
	BYTE buffer[2];
	bool status;
	USBDrive usbDrive;

	UsbCmdStruct cmd;
	cmd = cmd.GetFeature_IM();
	cmd.cdb[2] = ceNo;
	cmd.cdb[3] = cmdBus;
	cmd.cdb[4] = addrBus;
	

	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buffer, cmd.length,  cmd.direction);
	if(status == false){
		tstring msg = cmd.description + " fail";
		throw MyException(USBC_VENDOR_CMD_FAIL, msg);
	}
}

// for CM
void UsbCommand::write8k_RetryWrite(ULONG cycleList, BYTE* bufData, BYTE* bufExtraByte) const{
	bool status;
	USBDrive usbDrive;
	UsbCmdStruct cmd;
	cmd = cmd.write8k_RetryWrite();

	cmd.cdb[3]= (BYTE)(cycleList>>24)&0xFF;
	cmd.cdb[4]= (BYTE)(cycleList>>16)&0xFF;
	cmd.cdb[5]= (BYTE)(cycleList>>8)&0xFF;
	cmd.cdb[6]= (BYTE)cycleList&0xFF;

	BYTE buf[9216];
	memcpy(buf     ,bufExtraByte,1024);
	memcpy(buf+1024,bufData,8192);

	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buf, cmd.length, cmd.direction);
	if(status == false){
		tstring msg = cmd.description + " fail";
		throw MyException(USBC_VENDOR_CMD_FAIL, msg);
	}
}


void UsbCommand::testUnitReady() const{
	bool status;
	BYTE buffer[1];
	USBDrive usbDrive;
	UsbCmdStruct cmd;
	cmd = cmd.testUnitReady();

	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buffer, cmd.length, cmd.direction);
	if(status == false){
		tstring msg = cmd.description + " fail";
		throw MyException(USBC_VENDOR_CMD_FAIL, msg);
	}
}

void UsbCommand::homogeneousEnableShuffle(void) const{
	BOOL status;
	BYTE buffer[1];
	USBDrive usbDrive;
	UsbCmdStruct cmd;
	cmd = cmd.homogeneousEnableShuffle();
	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buffer, cmd.length,  cmd.direction);
	if(status == FALSE){
		tstring msg = cmd.description + " fail";
		throw MyException(USBC_VENDOR_CMD_FAIL, msg);
	}
}

void UsbCommand::homogeneousDisableShuffle(void) const{
	BOOL status;
	BYTE buffer[2];
	USBDrive usbDrive;
	UsbCmdStruct cmd;
	cmd = cmd.homogeneousDisableShuffle();
	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buffer, cmd.length,  cmd.direction);
	if(status == FALSE){
		tstring msg = cmd.description + " fail";
		throw MyException(USBC_VENDOR_CMD_FAIL, msg);
	}
}

void UsbCommand::setSn(BYTE sn1, BYTE sn2) const{
	BOOL status;
	USBDrive usbDrive;
	BYTE buffer[2];
	UsbCmdStruct cmd;
	cmd = cmd.setSN();

	cmd.cdb[3] = sn1;
	cmd.cdb[4] = sn2;

	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buffer, cmd.length,  cmd.direction);
	if(status == FALSE){
		tstring msg = cmd.description + " fail";
		throw MyException(USBC_VENDOR_CMD_FAIL, msg);
	}
}

void UsbCommand::getSn(BYTE* buffer) const{
	BOOL status;
	USBDrive usbDrive;
	UsbCmdStruct cmd;
	cmd = cmd.getSN();

	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buffer, cmd.length,  cmd.direction);
	if(status == FALSE){
		tstring msg = cmd.description + " fail";
		throw MyException(USBC_VENDOR_CMD_FAIL, msg);
	}
}

void UsbCommand::testFPGA(BYTE* buffer) const{
	BOOL status;
	USBDrive usbDrive;
	UsbCmdStruct cmd;
	cmd = cmd.testFPGA();

	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buffer, cmd.length,  cmd.direction);
	if(status == FALSE){
		tstring msg = cmd.description + " fail";
		throw MyException(USBC_VENDOR_CMD_FAIL, msg);
	}
}


void UsbCommand::inquiry(BYTE* buffer) const{
	BOOL status;
	USBDrive usbDrive;
	UsbCmdStruct cmd;
	cmd = cmd.inquiry();

	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buffer, cmd.length,  cmd.direction);
	if(status == FALSE){
		throw MyException(USBC_VENDOR_CMD_FAIL, cmd.description + " fail");
	}
}

void UsbCommand::readCapacity(BYTE* buffer) const{
	BOOL status;
	USBDrive usbDrive;
	UsbCmdStruct cmd;
	cmd = cmd.readCapacity();

	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buffer, cmd.length,  cmd.direction);
	if(status == FALSE){
		throw MyException(USBC_VENDOR_CMD_FAIL, cmd.description + " fail");
	}
}

void UsbCommand::readRom(BYTE addr, BYTE* buffer) const{
	bool status;
	USBDrive usbDrive;
	UsbCmdStruct cmd;
	cmd = cmd.readRom();
	cmd.cdb[3]=addr;

	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buffer, cmd.length,  cmd.direction);
	if(status == false){
		tstring msg = cmd.description + " fail";
		msg +=  Utility::strFormat(", fail ROM addr=%X", addr);

		throw MyException(USBC_VENDOR_CMD_FAIL, msg);
	}
}

void UsbCommand::scanRdy(BYTE* buffer) const {
	BOOL status;
	USBDrive usbDrive;
	UsbCmdStruct cmd;
	cmd = cmd.scanRdy();

	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buffer, cmd.length,  cmd.direction);
	if (status == FALSE) {
		tstring msg = cmd.description + " fail";
		throw MyException(USBC_VENDOR_CMD_FAIL, msg);
	}
}

void UsbCommand::switchECC(BYTE eccMode) const {
	BOOL status;
	USBDrive usbDrive;
	UsbCmdStruct cmd;
	BYTE buffer[1];

	cmd = cmd.switchECC();
	cmd.cdb[2] = eccMode;//change to 40 bit ecc

	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buffer, cmd.length,  cmd.direction);
	if (status == FALSE) {
		tstring msg = cmd.description + " fail";
		throw MyException(USBC_VENDOR_CMD_FAIL, msg);
	}
}


void UsbCommand::identify3sKey(BYTE* buffer) const {
	BOOL status;
	USBDrive usbDrive;
	UsbCmdStruct cmd;
	cmd = cmd.identify3sKey();

	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buffer, cmd.length,  cmd.direction);
	if (status == FALSE) {
		tstring msg = cmd.description + " fail";
		throw MyException(USBC_VENDOR_CMD_FAIL, msg);
	}
}

void UsbCommand::key_write10(ULONG cycleList, WORD secCnt, BYTE* buffer) const{
	BOOL status;
	USBDrive usbDrive;
	UsbCmdStruct cmd;
	cmd = cmd.write10(cycleList, secCnt);
	cmd.cdb[0] = 0xFC;
	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buffer, cmd.length,  cmd.direction);
	if(status == FALSE){
		tstring msg = cmd.description + " fail";
		throw MyException(USBC_SCSI_CMD_FAIL, msg);
	}
}

void UsbCommand::key_read10(ULONG cycleList, WORD secCnt, BYTE* buffer) const{
	BOOL status;
	USBDrive usbDrive;
	UsbCmdStruct cmd;
	cmd = cmd.read10(cycleList, secCnt);
	cmd.cdb[0] = 0xFD;
	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buffer, cmd.length,  cmd.direction);
	if(status == FALSE){
		tstring msg = cmd.description + " fail";
		throw MyException(USBC_SCSI_CMD_FAIL, msg);
	}
}

void UsbCommand::read_16K(ULONG cycleList, BYTE* buffer) const{
	BOOL status;
	USBDrive usbDrive;
	UsbCmdStruct cmd;
	cmd = cmd.read8k();
	cmd.cdb[3] = (BYTE)(cycleList>>24)&0xFF;
	cmd.cdb[4] = (BYTE)(cycleList>>16)&0xFF;
	cmd.cdb[5] = (BYTE)(cycleList>>8)&0xFF;
	cmd.cdb[6] = (BYTE)cycleList&0xFF;
	cmd.cdb[7] = (BYTE)cycleList&0xFF;

	status = usbDrive.UDISK_SendCommand(m_dvrHandle, cmd.cdb, buffer, cmd.length,  cmd.direction);
	if(status == FALSE){
		tstring msg = cmd.description + " fail";
		throw MyException(USBC_VENDOR_CMD_FAIL, msg);
	}
}
