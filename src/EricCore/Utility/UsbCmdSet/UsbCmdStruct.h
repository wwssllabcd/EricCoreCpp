#pragma once
#include "../DefineFiles/DefineFile.h"
#include <vector>

using namespace std;

class UsbCmdStruct
{
public:
	UsbCmdStruct(void);
	~UsbCmdStruct(void);

	BYTE cdb[12];
	int length;
	BYTE direction;
	tstring description;

	typedef std::vector<UsbCmdStruct> UsbCmdSet;

	bool operator ==(const UsbCmdStruct& rhs) const;

	UsbCmdSet getAllCommandSet(void);
	UsbCmdStruct vendor_GetFlashId(void);
	UsbCmdStruct vendor_GetFlashId_2091(void);

	UsbCmdStruct getDeviceInfo(void);
	UsbCmdStruct getCisData(void);
	UsbCmdStruct read8K_withoutECC(void);
	UsbCmdStruct writeRam(void);
	UsbCmdStruct readRam(void);
	
	UsbCmdStruct write8k(void);
	UsbCmdStruct write8k_RetryWrite(void);
	UsbCmdStruct read8k(void);
	UsbCmdStruct read8k(ULONG cycleList);

	UsbCmdStruct downLoadISP(void);
	UsbCmdStruct getSN(void);
	UsbCmdStruct setSN(void);
	UsbCmdStruct erase(void);
	UsbCmdStruct readRom(void);

	UsbCmdStruct requestSense(void);
	UsbCmdStruct inquiry(void);
	UsbCmdStruct readCapacity(void);
	UsbCmdStruct testUnitReady(void);
	UsbCmdStruct readFormatCapacity(void);
	
	UsbCmdStruct preventAllowMediumRemoval(void);

	UsbCmdStruct read10(void);
	UsbCmdStruct read10(ULONG lba, WORD secCnt);
	UsbCmdStruct write10(void);
	UsbCmdStruct write10(ULONG lba, WORD secCnt);

	UsbCmdStruct testRam(void);
	UsbCmdStruct homogeneousEnable(void);
	UsbCmdStruct homogeneousDisable(void);

	UsbCmdStruct homogeneousEnableShuffle(void);
	UsbCmdStruct homogeneousDisableShuffle(void);

	UsbCmdStruct getColumSkipTab(void);
	UsbCmdStruct columnSkipOn(void);
	UsbCmdStruct columnSkipOff(void);


	UsbCmdStruct reboot(void);
	UsbCmdStruct reSize(void);
	UsbCmdStruct set7Segment(void);
	UsbCmdStruct setFlashTiming(void);
	UsbCmdStruct onCardSort(void);

	UsbCmdStruct write4k(void);
	UsbCmdStruct pollOnCardSort(void);
	UsbCmdStruct getIspVersion(void);

	UsbCmdStruct reboot_OnCardSort(void);

	UsbCmdStruct SetFeature_IM(void);
	UsbCmdStruct GetFeature_IM(void);

	UsbCmdStruct getFuture_TSB();
	UsbCmdStruct setFuture_TSB();


	UsbCmdStruct switchECC(void);

	UsbCmdStruct testFPGA(void);

	UsbCmdStruct scanRdy(void);

	//
	UsbCmdStruct identify3sKey(void);

	UsbCmdStruct read_16k(ULONG cycleList);

	UsbCmdStruct toggleEnable();
	UsbCmdStruct toggleDisable();
};
