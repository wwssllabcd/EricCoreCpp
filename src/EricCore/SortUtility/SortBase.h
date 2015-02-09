#pragma once
#include "SbOption.h"
#include "..\Drive\UsbCommand.h"

#define SORTBASE_READ_8k 9216

class SortBase
{
public:
	SortBase(void);
	~SortBase(void);

	void modify8000(BYTE* ary8k, const SbOption& opt, BYTE sn, BYTE version);
	bool findBootBlock(ULONG& addr, UsbCommand& usbCmd);
	bool findSortingPage(const UsbCommand& usbCmd, int ceNo);
	//bool findSortingPage(const UsbCommand& usbCmd, int ceNo, BYTE* aryCfg9216 );
	bool checkRepeat64(BYTE* ary);
	bool checkSignature(BYTE* ary);
	bool checkSignature_98(BYTE* ary);
	bool checkSignature_98_debugMsg(BYTE* ary);
	

	ULONG sortTbAddr;
	BYTE sortTbVer;
	BYTE sortingTb[SORTBASE_READ_8k];

private:

	void _loadSortingTbParam(BYTE* arySortTb);
	void _setBinValue(BYTE* ary8k, int binValue, int totalBlock);
	void _setCustomBinValue(BYTE* ary8k, WORD binValue);

	int _setPattern(BYTE* ary8k, const SbOption& opt);
	bool _setPattern_1(int patternNo, int ptnOffset, bool isLong, bool isShort, BYTE* ary8k);


	//2097 sorting Board
	ULONG blkToCycleList(int win, const int blk, int page);
	bool searchBootBlk(const UsbCommand& usbCmd, ULONG& bootblk,BYTE maxSLCpage);

};
