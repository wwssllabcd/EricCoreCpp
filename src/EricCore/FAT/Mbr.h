#include "PartitionTable.h"

#pragma once
//MBR(Master Boot Record)位於第一個Sector，又稱LBA0，共1個Sector(512 byte)，
//前 446 個位元組 ( 0～445 ) 用來存放啟動資料與程式
//而接下來的 64 個位元組就是分割表(Partition Table)
//最後一個字組 ( 2 個位元組 ) 是 AA55H，作為識別之用。
class Mbr
{
public:
	Mbr(void);
	Mbr(int sizeMB);
	~Mbr(void);

	UCHAR _bootDataAndProgram[446];
	const UINT partitionTableOffset;	

	PartitionTable partitionTable1;
	PartitionTable partitionTable2;
	PartitionTable partitionTable3;
	PartitionTable partitionTable4;

	void load(const UCHAR* bufMbr);
	void toArray(BYTE* array);

	void getCHS(int& c, int& h, int& s, ULONG flashTotalSec, bool isFAT32);
};



