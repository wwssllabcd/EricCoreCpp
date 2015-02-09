#pragma once

#include "..\Drive\UsbCommand.h"
#include "..\Project\2090\Flash.h"
#include "..\Utility\Utility.h"
#include "SortBase.h"

class SortingBoardUtility
{
public:
	ULONG sortRecordAddr[16];

	SortingBoardUtility(void);
	~SortingBoardUtility(void);

	void setDefect(int ceNo, BYTE* aryDefect);
	void getDefect(int ceNo, BYTE* aryDefect);

	void loadDefectFromSortingBlock(const UsbCommand& usbCmd, const Flash& f, SortBase& sb);
	void loadDefectFromSortingBlock_V1V2(const UsbCommand& usbCmd, const Flash& f, SortBase& sb);
	void loadDefectFromSortingBlock_V3(const UsbCommand& usbCmd, const Flash& f, SortBase& sb);
	
	void getPartial_Retry(BYTE* arySbCfg, BYTE& enablePartial, BYTE& enableRetry);
	
	void loadMultiRecord(const UsbCommand& usbCmd, const Flash& f);
	void setUse(ULONG blockNo, BYTE* sortTb);

	int getPagePerBlockPartialPage(BYTE partialPage, bool is56D3);
	
private:	
	BYTE _tempDT[8192];
	BYTE _defectTBs[MAX_CE_NO][DEFECT_TABLE_SIZE];


	void _combineDefectTb(BYTE* bitMap, BYTE* recordBuf, int startAddr, int endAddr);
	BYTE _adjustValue(int endPtr);
	void _loadRecord_IM34_over8192(const UsbCommand& usbCmd, const Flash& f, ULONG defectPageAddr);
	void _loadRecord_IM34_less8192(const UsbCommand& usbCmd, const Flash& f, ULONG defectPageAddr, int recordStartPtr, int recordLen);

	void _loadRecord(const UsbCommand& usbCmd, const Flash& f, ULONG defectPageAddr, int recordStartPtr, int recordLen);

	void _loadRecord_OneCe(const UsbCommand& usbCmd, const Flash& f, ULONG defectPageAddr);
	int _getCeCntPerPackage(BYTE idCheckFlag);
	void _makeOneCeTabele(BYTE* bitMap, BYTE* recordTb, const Flash& f);
	int _getRecordOffset(const Flash& f);
};
