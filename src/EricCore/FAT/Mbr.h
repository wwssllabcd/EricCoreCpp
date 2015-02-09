#include "PartitionTable.h"

#pragma once
//MBR(Master Boot Record)���Ĥ@��Sector�A�S��LBA0�A�@1��Sector(512 byte)�A
//�e 446 �Ӧ줸�� ( 0��445 ) �ΨӦs��Ұʸ�ƻP�{��
//�ӱ��U�Ӫ� 64 �Ӧ줸�մN�O���Ϊ�(Partition Table)
//�̫�@�Ӧr�� ( 2 �Ӧ줸�� ) �O AA55H�A�@���ѧO���ΡC
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



