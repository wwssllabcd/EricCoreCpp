#include "StdAfx.h"
#include "./UsbCmdStruct.h"
#include "USB_3S_VENDOR_DEF.H"


#define	FLAG_DATA_IN			0x00000002
#define	FLAG_DATA_OUT			0x00000004

UsbCmdStruct::UsbCmdStruct(void)
{
	memset(this->cdb,00, 12);
	this->length = 0;
	this->direction = FLAG_DATA_IN;
}

UsbCmdStruct::~UsbCmdStruct(void)
{
}

bool UsbCmdStruct::operator ==(const UsbCmdStruct& rhs) const{
	
	int result = memcmp(this->cdb, rhs.cdb, sizeof(rhs.cdb) );
	if( result!=0 ){return false;}
	if( this->length != rhs.length ){return false;}
	if( this->direction != rhs.direction ){return false;}
	if( this->description.compare(rhs.description)!=0 ){
		return false;
	}

	return true;
}


UsbCmdStruct::UsbCmdSet UsbCmdStruct::getAllCommandSet(void){
	UsbCmdSet cmdSet;
	cmdSet.push_back( this->vendor_GetFlashId() );
	cmdSet.push_back( this->vendor_GetFlashId_2091() );

	cmdSet.push_back( this->getDeviceInfo() );
	cmdSet.push_back( this->getCisData() );
	cmdSet.push_back( this->writeRam() );
	cmdSet.push_back( this->readRam() );
	cmdSet.push_back( this->write8k() );
	cmdSet.push_back( this->read8k() );
	cmdSet.push_back( this->read8K_withoutECC() );
	cmdSet.push_back( this->downLoadISP() );
	cmdSet.push_back( this->getSN() );
	cmdSet.push_back( this->setSN() );

	cmdSet.push_back( this->readRom() );

	cmdSet.push_back( this->erase() );
	cmdSet.push_back( this->testRam() );
	cmdSet.push_back( this->homogeneousEnable() );
	cmdSet.push_back( this->homogeneousDisable() );
	
	cmdSet.push_back( this->homogeneousEnableShuffle() );
	cmdSet.push_back( this->homogeneousDisableShuffle() );

	cmdSet.push_back( this->reboot() );
	cmdSet.push_back( this->reboot_OnCardSort() );

	cmdSet.push_back( this->reSize() );
	cmdSet.push_back( this->setFlashTiming() );
	cmdSet.push_back( this->getIspVersion() );

	
	cmdSet.push_back( this->getColumSkipTab() );
	cmdSet.push_back( this->columnSkipOn() );
	cmdSet.push_back( this->columnSkipOff() );

	cmdSet.push_back( this->requestSense() );
	cmdSet.push_back( this->inquiry() );
	cmdSet.push_back( this->readCapacity() );
	cmdSet.push_back( this->testUnitReady() );
	cmdSet.push_back( this->readFormatCapacity() );
	
	cmdSet.push_back( this->preventAllowMediumRemoval() );

	cmdSet.push_back( this->read10() );
	cmdSet.push_back( this->write10() );

	cmdSet.push_back( this->set7Segment() );
	cmdSet.push_back( this->onCardSort() );
	cmdSet.push_back( this->pollOnCardSort() );

	cmdSet.push_back( this->pollOnCardSort() );
	cmdSet.push_back( this->SetFeature_IM() );
	cmdSet.push_back( this->GetFeature_IM() );

	cmdSet.push_back( this->write8k_RetryWrite() );
	cmdSet.push_back( this->switchECC() );

	cmdSet.push_back( this->testFPGA() );
	return cmdSet;
}

UsbCmdStruct UsbCmdStruct::vendor_GetFlashId(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0x03;
	usbCmdSet.cdb[2] = 0x68;
	usbCmdSet.length = 512;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "Vendor: Get Flash Id";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::vendor_GetFlashId_2091(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0x07;
	usbCmdSet.cdb[2] = 0x68;
	usbCmdSet.length = 512;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "Vendor: Get Flash Id-2091";
	return usbCmdSet;
}


UsbCmdStruct UsbCmdStruct::getDeviceInfo(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = OP_3S_GET_DEVICE_INFO;
	usbCmdSet.length = 16;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "Vendor: Get Device infomation";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::getCisData(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0x1B;
	usbCmdSet.length = 0x800;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "Vendor: Get CIS DATA";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::read8K_withoutECC(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0x27;
	usbCmdSet.cdb[2] = 0x00;
	usbCmdSet.length = 9216;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "Vendor: read8K_withoutECC";
	return usbCmdSet;
}


UsbCmdStruct UsbCmdStruct::writeRam(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0x12;
	usbCmdSet.cdb[2] = 0x01;
	usbCmdSet.length = 512;
	usbCmdSet.direction = FLAG_DATA_OUT;
	usbCmdSet.description = "Vendor: Write RAM";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::readRam(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0x11;
	usbCmdSet.cdb[2] = 0x01;
	usbCmdSet.length = 512;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "Vendor: Read RAM";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::write8k(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0x24;
	usbCmdSet.cdb[2] = 0x01;
	usbCmdSet.length = 9216;
	usbCmdSet.direction = FLAG_DATA_OUT;
	usbCmdSet.description = "Vendor: write 8k";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::read8k(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0x25;
	usbCmdSet.cdb[2] = 0x01;
	usbCmdSet.length = 9216;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "Vendor: read 8k";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::read8k(ULONG cycleList){
	UsbCmdStruct usbCmdSet;
	usbCmdSet = usbCmdSet.read8k();
	usbCmdSet.cdb[3] = (BYTE)((cycleList >> 0x18) & 0xFF);
	usbCmdSet.cdb[4] = (BYTE)((cycleList >> 0x10) & 0xFF);
	usbCmdSet.cdb[5] = (BYTE)((cycleList >> 0x08) & 0xFF);
	usbCmdSet.cdb[6] = (BYTE)((cycleList >> 0x00) & 0xFF);
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::downLoadISP(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0x50;
	usbCmdSet.length = 4096;
	usbCmdSet.direction = FLAG_DATA_OUT;
	usbCmdSet.description = "Vendor: downLoadISP to 8000H";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::getSN(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0x51;
	usbCmdSet.cdb[2] = 0x00;
	usbCmdSet.length = 2;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "Vendor: get SN";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::setSN(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0x51;
	usbCmdSet.cdb[2] = 0x02;
	usbCmdSet.cdb[3] = 0x41;
	usbCmdSet.cdb[4] = 0x31;

	usbCmdSet.length = 2;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "Vendor: set SN(By Byte3,4)";
	return usbCmdSet;
}


UsbCmdStruct UsbCmdStruct::erase(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0x26;
	usbCmdSet.length = 0;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "Vendor: erase";
	return usbCmdSet;
}



UsbCmdStruct UsbCmdStruct::requestSense(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = 0x03;
	usbCmdSet.cdb[1] = 0x00;
	usbCmdSet.cdb[2] = 0x00;
	usbCmdSet.cdb[3] = 0x00;
	usbCmdSet.cdb[4] = 0x12;
	usbCmdSet.length = 18;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "UFI: Request Sense";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::inquiry(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = 0x12;
	usbCmdSet.cdb[1] = 0x00;
	usbCmdSet.cdb[2] = 0x00;
	usbCmdSet.cdb[3] = 0x00;
	usbCmdSet.cdb[4] = 0x24;
	usbCmdSet.length = 0x24;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "UFI: Inquiry";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::readCapacity(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = 0x25;
	usbCmdSet.cdb[1] = 0x00;
	usbCmdSet.length = 8;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "UFI: Read Capacity";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::testUnitReady(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = 0x00;
	usbCmdSet.length = 0;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "UFI: Test UnitReady";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::readFormatCapacity(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = 0x23;
	usbCmdSet.length = 12;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "UFI: Read Format Capacity";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::preventAllowMediumRemoval(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = 0x1e;
	usbCmdSet.length = 0;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "UFI: Prevent-Allow Medium Removal";
	return usbCmdSet;
}


UsbCmdStruct UsbCmdStruct::read10(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = 0x28;
	usbCmdSet.cdb[8] = 1;
	usbCmdSet.length = 512;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "UFI: Read(10)";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::read10(ULONG lba, WORD secCnt){
	UsbCmdStruct usbCmdSet;
	usbCmdSet = usbCmdSet.read10();
	usbCmdSet.cdb[2] = (BYTE)((lba >> 0x18) & 0xFF);
	usbCmdSet.cdb[3] = (BYTE)((lba >> 0x10) & 0xFF);
	usbCmdSet.cdb[4] = (BYTE)((lba >> 0x08) & 0xFF);
	usbCmdSet.cdb[5] = (BYTE)((lba >> 0x00) & 0xFF);
	usbCmdSet.cdb[7] = (secCnt >> 0x08) & 0xFF;
	usbCmdSet.cdb[8] = (secCnt >> 0x00) & 0xFF;
	usbCmdSet.length = secCnt*0x200;
	return usbCmdSet;
}


UsbCmdStruct UsbCmdStruct::write10(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = 0x2a;
	usbCmdSet.cdb[8] = 1;
	usbCmdSet.length = 512;
	usbCmdSet.direction = FLAG_DATA_OUT;
	usbCmdSet.description = "UFI: Write(10)";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::write10(ULONG lba, WORD secCnt){
	UsbCmdStruct usbCmdSet;
	usbCmdSet = usbCmdSet.write10();
	usbCmdSet.cdb[2] = (BYTE)((lba >> 0x18) & 0xFF);
	usbCmdSet.cdb[3] = (BYTE)((lba >> 0x10) & 0xFF);
	usbCmdSet.cdb[4] = (BYTE)((lba >> 0x08) & 0xFF);
	usbCmdSet.cdb[5] = (BYTE)((lba >> 0x00) & 0xFF);
	usbCmdSet.cdb[7] = (secCnt >> 0x08) & 0xFF;
	usbCmdSet.cdb[8] = (secCnt >> 0x00) & 0xFF;
	usbCmdSet.length = secCnt*0x200;
	return usbCmdSet;
}


UsbCmdStruct UsbCmdStruct::testRam(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0]=0xFF;
	usbCmdSet.cdb[1]=0x55;
	usbCmdSet.length = 512;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "Vendor: testRam";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::homogeneousEnable(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0x2C;
	usbCmdSet.length = 0;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "Vendor: Homogeneous Enable";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::homogeneousDisable(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0x2B;
	usbCmdSet.length = 0;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "Vendor: Homogeneous Disable";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::reboot(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0x1D;
	usbCmdSet.length = 0;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "Vendor: reboot";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::reboot_OnCardSort(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0x7D;
	usbCmdSet.length = 0;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "Vendor: reboot_OnCardSort";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::reSize(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0x56;
	usbCmdSet.length = 0;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "Vendor: reSize";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::set7Segment(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0xFE;
	usbCmdSet.cdb[2] = 0x01;
	usbCmdSet.length = 0;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "SortBoard: Set Seven-Segment";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::setFlashTiming(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0x1E;
	usbCmdSet.cdb[2] = 0x11;
	usbCmdSet.length = 0;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "Vendor: Set Flash Timing";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::onCardSort(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0x7B;
	usbCmdSet.length = 0;
	usbCmdSet.direction = FLAG_DATA_OUT;
	usbCmdSet.description = "Vendor: On Card Sort";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::write4k(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0x29;
	usbCmdSet.cdb[2] = 0x01;
	
	//for 2092 Write 4k
	usbCmdSet.cdb[10] = 0x04;

	usbCmdSet.length = 5120;
	usbCmdSet.direction = FLAG_DATA_OUT;
	usbCmdSet.description = "Vendor: write 4k";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::pollOnCardSort(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0x7C;
	usbCmdSet.length = 31;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "Vendor: Polling OnCard Sort";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::getIspVersion(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0x62;
	usbCmdSet.length = 5;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "Vendor: GetIspVersion";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::SetFeature_IM(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0x40;
	usbCmdSet.cdb[3] = 0xEF;

	usbCmdSet.length = 0;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "Vendor: SetFeature_IM";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::GetFeature_IM(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0x41;
	usbCmdSet.cdb[3] = 0xEE;
	usbCmdSet.length = 512;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "Vendor: GetFeature_IM";
	return usbCmdSet;
}


UsbCmdStruct UsbCmdStruct::write8k_RetryWrite(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0x34;
	usbCmdSet.cdb[2] = 0x01;
	usbCmdSet.length = 9216;
	usbCmdSet.direction = FLAG_DATA_OUT;
	usbCmdSet.description = "Vendor_CM: write8k_RetryWrite 8k";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::homogeneousEnableShuffle(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0x2F;
	usbCmdSet.length = 0;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "Vendor: Homogeneous Enable Shuffle";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::homogeneousDisableShuffle(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0x2E;
	usbCmdSet.length = 0;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "Vendor: Homogeneous Disable Shuffle";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::readRom(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0x06;
	usbCmdSet.length = 512;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "Vendor: Read Rom";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::testFPGA(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0xFF;

	usbCmdSet.length = 0;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "FPGA: FPGA Test Start";
	return usbCmdSet;
}

// Auto-CE
UsbCmdStruct UsbCmdStruct::scanRdy(void) {
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0x30;	//OP_Vdr_SCAN_RDY_BSY
	usbCmdSet.length = 512;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "Vendor: Scan RDY BSY";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::columnSkipOn(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0x0A;
	usbCmdSet.length = 0;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "Vendor: column Skip On";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::columnSkipOff(void){
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0x09;
	usbCmdSet.length = 0;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "Vendor: column Skip Off";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::getColumSkipTab(void)
{
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0x99;
	usbCmdSet.length = 9216;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "Vendor: Get ColumSkip Table";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::switchECC(void)
{
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0x97;
	usbCmdSet.length = 0;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "Vendor: Switch ECC";
	return usbCmdSet;
}

UsbCmdStruct UsbCmdStruct::identify3sKey(void)
{
	UsbCmdStruct usbCmdSet;
	usbCmdSet.cdb[0] = OP_3S_VENDOR_CDB;
	usbCmdSet.cdb[1] = 0xE0;
	usbCmdSet.length = 512;
	usbCmdSet.direction = FLAG_DATA_IN;
	usbCmdSet.description = "Vendor: identify3sKey";
	return usbCmdSet;
}
