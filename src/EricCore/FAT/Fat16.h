#pragma once
#include "Bpb.h"

class Fat16
{
public:
	Fat16(void);
	~Fat16(void);

	void load(BYTE* bufMbr);

	//要有一個type
	int type;

	ULONG jmpBoot;
	CString OEMName;

	WORD bpb_BytesPerSec;
	BYTE bpb_SecPerClu;
	WORD bpb_RsvdSecCnt;

	//Fat的份數，通常有兩份，目的是為了以防萬一
	BYTE bpb_NumFats;
	WORD bpb_RootEntCnt;

	//13H～14H 這個字組表示邏輯磁區總數，但最多只能表示 0FFFFH
	//所以若一個磁碟的邏輯磁區大於 0FFFFH ( 或是說磁碟容量大於 32MB，32MB 是 10000H*512D 來的 )
	//則此欄位為 0，邏輯磁區總數記錄在 20H 處開始的雙字組。
	//因為 FAT32 分割必定大於 32MB，故此欄位不使用，均填入 0。
	WORD bpb_TotSec16;

	//位址 15H 這個位元組表示磁碟種類。此欄位和 FAT 的第一個位元組一樣，其意義如下表：
	//0F0H：1.44MB 三吋半軟碟	0F8H：硬碟
	//0F9H：1.2M 5.25 吋軟碟	0FDH：360K 5.25 吋軟碟
	//0FEH：160K 5.25 吋軟碟	0FFH：320K 5.25 吋軟碟
	BYTE bpb_Media;

	//每份 FAT 所佔磁區數；但對 FAT32 而言，此欄位不用，填上 0，
	WORD bpb_FatSz16;

	//代表每一磁軌之磁區數
	WORD bpb_SecPerTrk;

	//代表磁頭數
	WORD bpb_NumHeads;

	//這個雙字組 (共4個位元組) 表示隱藏磁區數，與分割表或擴充分割表中的分割區相對位置的數值一樣
	ULONG bpb_HiddSec;

	//這個雙字組(共4個位元組) 代表邏輯磁區總數。如果 16H～17H 欄為 0，則此欄不可為零。
	ULONG bpb_TotSec32;

	BYTE drvNum;
	BYTE reserved1;
	BYTE bootSig;
	CString volID;
	CString volLab;
	CString filSysType;


	ULONG getTotalSec();
	ULONG getFatSize();
	ULONG getDataSec();
	ULONG getRootDirSectors();
	ULONG getTotalCluster();

	void toAry(BYTE* array16);

	void loadNewFat16(ULONG totalSec, int sizeMB, int hiddenSec);
	int getSecPerFat(int TS);
	//int getDskSize(int sizeMb);
	
	void getHS( int& h, int& s, int sizeMB);

private:
	int _getSecPerCluster(int sizeMB);
	
};
