#pragma once

#include "../Utility/DefineFiles/Definefile.h"

#define	FLAG_DATA_IN			0x00000002
#define	FLAG_DATA_OUT			0x00000004

class UsbCommand
{
public:
	UsbCommand(HANDLE deviceHandle);
	~UsbCommand(void);


	void getDeviceInfo(BYTE* buf16, int len);
	void getFlashIDs(BYTE* buffer);


	void write8K(ULONG cycleList, BYTE* bufData, BYTE* bufExtraByte) const;
	void read8K(ULONG cycleList, BYTE* buffer) const;

	void write8k_RetryWrite(ULONG cycleList, BYTE* bufData, BYTE* bufExtraByte) const;
	void write_and_verify8K(ULONG cycleList, BYTE* bufData, BYTE* bufExtraByte) const;

	void downLoadISP(BYTE* buffer, BYTE bootCodeCtrl=0);
	void eraseBlock(ULONG cycleList) const;
	bool sendCommand(BYTE* cdb, BYTE* buffer, int Length, BYTE direction);
	void scanErase(BYTE* buffer) const;
	void readRam(BYTE addr, BYTE* buffer) const;
	void writeRam(BYTE addr, BYTE* buffer) const;

	void read8K_withoutEcc(ULONG cycleList, BYTE* buffer) const;

	void testRam1k(BYTE addr, BYTE* buffer,int inOut, int len) const;

	void homogeneousEnable(void) const;
	void homogeneousDisable(void) const;

	void homogeneousEnableShuffle(void) const;
	void homogeneousDisableShuffle(void) const;


	void reboot(void) const;
	void reboot_OnCard(void) const;

	void write10(ULONG cycleList, WORD secCnt, BYTE* buffer) const;
	void read10(ULONG cycleList, WORD secCnt, BYTE* buffer) const;
	void setFlashTiming() const;

	void set7Seg(int first, int second, int third) const;
	void onCardSort() const;
	void testUnitReady() const;

	void setSn(BYTE sn1, BYTE sn2) const;
	void getSn(BYTE* buffer) const;
	void readRom(BYTE addr, BYTE* buffer) const;
	void scanRdy(BYTE* buffer) const;


	void read_16K(ULONG cycleList, BYTE* buffer) const;



	//UFI
	void inquiry(BYTE* buffer) const;
	void readCapacity(BYTE* buffer) const;

	void testFPGA(BYTE* buffer) const;

	// for IM
	void StrengthenDriver(BYTE TempReg130,BYTE TempReg145,BYTE TempReg20E);
	void write4K(ULONG cycleList, BYTE* aryData, BYTE* aryExtraByte) const;
	void pollingOnCardSort(BYTE* buf_2Byte) const;
	void setFeature(BYTE ceNo, BYTE cmdBus, BYTE addrBus, BYTE value) const;
	void getFeature(BYTE ceNo, BYTE cmdBus, BYTE addrBus) const;
	void switchECC(BYTE eccMode) const;


	//for OCS
	void identify3sKey(BYTE* buffer) const;
	void key_read10(ULONG cycleList, WORD secCnt, BYTE* buffer) const;
	void key_write10(ULONG cycleList, WORD secCnt, BYTE* buffer) const;

	//property
	HANDLE m_dvrHandle;

private:
	


	

};
