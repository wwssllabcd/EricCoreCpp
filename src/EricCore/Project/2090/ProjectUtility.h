#pragma once

#include "..\..\Option\Option.h"
#include "..\..\Drive\UsbCommand.h"

#include "Flash.h"

class ProjectUtility
{
public:
	ProjectUtility(void);
	~ProjectUtility(void);

	void checkNeedFile(const Option& opt);
	void checkAndChangeHomogeneous(const Flash& f, Option& opt);
	void downloadVendor(UsbCommand& usbCmd);
	bool findDefectPage(const UsbCommand& usbCmd, const Flash& f, bool& isNewDefectFormat);
	bool findDefectPage_ByPageNo(const UsbCommand& usbCmd, const Flash& f, bool& isNewDefectFormat, int BlockStart, int BlockEnd, BYTE pageCnt);

	void checkFwData(const Flash& f, ULONG totalLBA, ULONG totalLB, int secPerBlock);

	// for test 
	void testOneBlock(const UsbCommand& usbCmd, ULONG addr );

	void testOneBlock_beErase(const UsbCommand& usbCmd, ULONG targetAddr,ULONG endAddr, int eraseGap);


	ULONG typeC_ToCycleList(BYTE *ary);
	void cycleListToTypeC(ULONG cycleList, BYTE* ary);


	bool checkBlock(const UsbCommand& usbCmd, const Flash& f, ULONG addr, int length, BYTE isPartial, BYTE isReWrite, bool is4kWrite);
	void Strengthen_Driver(UsbCommand& usbCmd, const Option& opt);
	ULONG getCheckSum(const UsbCommand& usbCmd, int icNo);
	bool searchBootBlock(const UsbCommand& usbCmd, int blockGap, int BlockStart, int BlockEnd, BYTE pageOffset, ULONG& addr);



	ULONG defectTbAddr;
	

private:
	bool _checkDefectSignature(BYTE* ary);
	bool _writeReadCmp(const UsbCommand& usbCmd, ULONG blockAddr, BYTE isReWrite, bool is4kWrite);

};
