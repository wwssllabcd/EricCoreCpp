#include "StdAfx.h"
#include ".\projectutility.h"

#include "..\..\Utility\Utility.h"

using namespace EricCore;

ProjectUtility::ProjectUtility(void)
:defectTbAddr(0)
{
}

ProjectUtility::~ProjectUtility(void)
{
}

void ProjectUtility::checkNeedFile(const Option& opt){

	BYTE ary8K[8192];
	Utility::getFileData("ISP_7.bin", 0,4096, ary8K);
	Utility::getFileData("ISP_8.bin", 0,4096, ary8K);
	Utility::getFileData("ISP_A.bin", 0,4096, ary8K);
	if(opt.secureBank ==true){
		Utility::getFileData("vdr_Secure.bin", 0,4096, ary8K);
	}
	if(opt.gf_hid_En==true){
		Utility::getFileData("Vdr_HID.BIN", 0,4096, ary8K);
	}
}


void ProjectUtility::checkAndChangeHomogeneous(const Flash& f, Option& opt){

	if( f.is_TSB_43nmD3 || f.is_TSB_43nmD3_Special || f.is_TSB_32nmD2 ){
		if(opt.homogeneous==0){
			string msg;
			msg+= "It's TSB 43nm D3/32nm D2 Flash" + Utility::CrLf();
			msg+= "You should turn on Homogeneous function¡APlease Check INI File" + Utility::CrLf();
			msg+= "If you want to turn on Homogeneous, Pass OK" + Utility::CrLf();

			int userSelect = AfxMessageBox(msg.c_str(), MB_OKCANCEL|MB_ICONINFORMATION|MB_DEFBUTTON2);
			if(userSelect == IDOK){
				opt.homogeneous=1;
			}


		}
	}
}

void ProjectUtility::downloadVendor(UsbCommand& usbCmd){
	BYTE vendorCode[4096];
	Utility::getFileData("vendor.bin", 0,4096, vendorCode);
	usbCmd.downLoadISP(vendorCode);
}

bool ProjectUtility::findDefectPage_ByPageNo(const UsbCommand& usbCmd, const Flash& f, bool& isNewDefectFormat, int BlockStart, int BlockEnd, BYTE pageOffset){
	BYTE buf[9216];
	ULONG addr=0;
	for(int i=BlockStart; i<BlockEnd; i++){
		addr = i*f.blockLen + pageOffset;
		usbCmd.read8K(addr, buf);
		isNewDefectFormat = _checkDefectSignature(buf);
		if(buf[8192]==0xFC && isNewDefectFormat){
			this->defectTbAddr = addr;
			return true;
		}
	}
	return false;
}

bool ProjectUtility::searchBootBlock(const UsbCommand& usbCmd, int blockGap, int BlockStart, int BlockEnd, BYTE pageOffset, ULONG& addr){
	BYTE buf[9216];
	usbCmd.homogeneousDisable();

	for(int i=BlockStart; i<BlockEnd; i++){
		addr = i* blockGap + pageOffset;
		usbCmd.read8K(addr, buf);
		if(buf[8192]==0xFC){
			return true;
		}
	}
	return false;
}

bool ProjectUtility::findDefectPage(const UsbCommand& usbCmd, const Flash& f, bool& isNewDefectFormat){

	if( findDefectPage_ByPageNo(usbCmd, f, isNewDefectFormat, 0, 0x40, f.getPhyPage(30) ) ){
		return true;
	}

	//==============================================
	if( findDefectPage_ByPageNo(usbCmd, f, isNewDefectFormat, 0x80, 0xC0, f.getPhyPage(30) ) ){
		return true;
	}

	//==============================================
	if( findDefectPage_ByPageNo(usbCmd, f, isNewDefectFormat, 0x100, 0x140, f.getPhyPage(30)) ){
		return true;
	}

	//==============================================
	if( findDefectPage_ByPageNo(usbCmd, f, isNewDefectFormat, 0x200, 0x240, f.getPhyPage(30)) ){
		return true;
	}

	//==============================================3
	if( findDefectPage_ByPageNo(usbCmd, f, isNewDefectFormat, 0x400, 0x440, f.getPhyPage(30)) ){
		return true;
	}
	return false;
}


bool ProjectUtility::_checkDefectSignature(BYTE* ary){
	BYTE sig[16]={'3', 'S', ' ', '2', '0', '9', '1', ' ', 'D', 'E', 'F', 'E', 'C', 'T', '!', '!'};
	int result = memcmp(ary, sig, 16);
	if(result!=0){
		return false;
	}else{
		return true;
	}
}

void ProjectUtility::checkFwData(const Flash& f, ULONG totalLBA, ULONG totalLB, int secPerBlock){
	
	// check Capacity
	ULONG totalSec = totalLB * secPerBlock;
	if( totalLBA>totalSec){
		throw MyException(PU_CHECK_CAPACITY_FAIL, "totalLBA(0x%X) > totalSec(0x%X)", totalLBA, totalSec );
	}
}


void ProjectUtility::testOneBlock(const UsbCommand& usbCmd, ULONG addr ){
	BYTE buf[8192];
	BYTE extra[1024];

	memset(buf, 0, sizeof(buf) );
	memset(extra, 0, sizeof(extra) );
	usbCmd.eraseBlock(addr);
	for(int pageNo=0; pageNo<0x80; pageNo++){
		usbCmd.write8K(addr + pageNo, buf, extra);
	}
}

void ProjectUtility::testOneBlock_beErase(const UsbCommand& usbCmd, ULONG targetAddr,ULONG endAddr, int eraseGap){
	BYTE buf_write[8192];
	BYTE buf_read[9216];
	BYTE extra[1024];

	memset(buf_write, 0, sizeof(buf_write) );
	memset(extra, 0, sizeof(extra) );

	usbCmd.eraseBlock(targetAddr);
	usbCmd.write8K(targetAddr, buf_write, extra);

	for(ULONG addr=eraseGap; addr<endAddr; addr+=eraseGap){
		if( addr==targetAddr ){
			continue;
		}
		usbCmd.eraseBlock(addr);
		usbCmd.read8K(targetAddr, buf_read);
		if( memcmp(buf_write, buf_read, 8192) !=0 ){
			throw MyException(PU_TEST_FAIL, "PU_TEST_FAIL");
		}
	}
}

ULONG ProjectUtility::typeC_ToCycleList(BYTE *ary){
	ULONG value;
	value =	 ((ULONG)(ary[0]&0x7F))<<24;
	value |= ((ULONG)ary[1])<<16;
	value |= ((ULONG)ary[2])<<8;	
	value |= ((ULONG)ary[0])&0x80;
	return value;
}

void ProjectUtility::cycleListToTypeC(ULONG cycleList, BYTE* ary){
	ary[0] = (BYTE) (cycleList>>24)&0xFF;
	ary[1] = (BYTE) (cycleList>>16)&0xFF;
	ary[2] = (BYTE) (cycleList>> 8)&0xFF;

	if( (cycleList&0xFF)==0x80){
		ary[0] |= BIT7;
	}
}

bool ProjectUtility::_writeReadCmp(const UsbCommand& usbCmd, ULONG blockAddr,BYTE isReWrite, bool is4kWrite){
	int result = 0;

	BYTE data[8192];
	BYTE extraByte[1024];
	BYTE dt[9216];	
	
	memset(data,0xff, sizeof(data));
	memset(extraByte,0xff,sizeof(extraByte));

	if( is4kWrite ){
		if( isReWrite ){
			usbCmd.write4K( blockAddr, data, extraByte );
			usbCmd.write4K( blockAddr, data, extraByte );
			usbCmd.write4K( blockAddr, data, extraByte );
			usbCmd.write4K( blockAddr, data, extraByte );
			usbCmd.write4K( blockAddr, data, extraByte );
			usbCmd.write4K( blockAddr, data, extraByte );
		}else{
			usbCmd.write8K( blockAddr, data, extraByte );
		}
	}else{
		if( isReWrite ){
			usbCmd.write8K( blockAddr, data, extraByte );
			usbCmd.write8K( blockAddr, data, extraByte );
			usbCmd.write8K( blockAddr, data, extraByte );
			usbCmd.write8K( blockAddr, data, extraByte );
			usbCmd.write8K( blockAddr, data, extraByte );
			usbCmd.write8K( blockAddr, data, extraByte );
		}else{
			usbCmd.write8K( blockAddr, data, extraByte );
		}
	}

	usbCmd.read8K(blockAddr, dt);
	if( is4kWrite ){
		result = memcmp(dt, data, 4096);
	}else{
		result = memcmp(dt, data, 8192);
	}

	if(result !=0 ){
		tstring msg = Utility::getDiffStringInTwoBuf(blockAddr, 8192, dt, data);
		Utility::sendMsg("Fail addr =0x%x", blockAddr);
		Utility::sendMsg(msg);
	}


	if(result==0){
		return true;
	}
	return false;
}

bool ProjectUtility::checkBlock(const UsbCommand& usbCmd, const Flash& f, ULONG addr, int length, BYTE isPartial, BYTE isReWrite, bool is4kWrite){
	bool result = false;
	usbCmd.eraseBlock(addr);

	ULONG blockAddr=0;
	for(int i=0; i<length; i++){
		if( isPartial ){
			blockAddr = addr | f.getPhyPage(i);
		}else{
			blockAddr = addr | i;
		}

		result = _writeReadCmp(usbCmd, blockAddr, isReWrite, is4kWrite);
		if(result==false ){
			break;
		}
	}	
	usbCmd.eraseBlock(addr);
	return result;
}

// ¼W±j driver
void ProjectUtility::Strengthen_Driver(UsbCommand& usbCmd, const Option& opt){	
	Utility::sendMsg("set Strengthen_Driver, Reg130=%X, Reg145=%X, Reg20E=%X", opt.reg130, opt.reg145, opt.reg20E);
	usbCmd.StrengthenDriver(opt.reg130, opt.reg145, opt.reg20E);
}


ULONG ProjectUtility::getCheckSum(const UsbCommand& usbCmd, int icNo){
	BYTE buf[512];
	ULONG ckSum=0;
	for(int i=0; i<64; i+=2){
		usbCmd.readRom(i, buf);
		for(int j=0; j<512; j++){
			ckSum+= buf[j];
		}
	}


	ULONG icCkSum;

	if( icNo==2092) {
		icCkSum = 0x252F34;
	}

	if( ckSum != icCkSum){
		throw MyException(PU_ROM_CHECKSUM_FAIL, "CheckSum Fail, is it not %d IC, or ROM code error", icNo);
	}

	return ckSum;
}

