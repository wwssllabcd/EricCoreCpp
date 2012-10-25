#pragma once

class RelinkL1
{
public:
	RelinkL1(void);
	~RelinkL1(void);

	void toArray_2048(BYTE* aryRamC000);
	void cycleListToTypeC(ULONG cycleList, BYTE* ary) const;

	//wi-block
	BYTE ary_WiBlock[6];
	// spare Table
	BYTE arySpareTable[0x80];

	// Reserve Spare Table
	BYTE RST[4096];


	ULONG addr_No0;
	ULONG addr_No1;

	int ceNo;
	int maxWGroup;
	WORD eraseSkip;
	WORD relinkEnd;
	BYTE retryReadCnt;
	WORD maxLB_win;
	bool setF0_AllZero;

};
