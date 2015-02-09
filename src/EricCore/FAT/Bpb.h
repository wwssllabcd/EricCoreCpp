#pragma once

class Bpb
{
public:
	Bpb(void);
	~Bpb(void);

	USHORT bytsPerSec;
	UCHAR secPerClu;
	USHORT rsvdSecCnt;
	UCHAR numFATs;
	USHORT rootEntCnt;

	USHORT totSec16;
	UCHAR media;
	USHORT fatSz16;
	USHORT secPerTrk;
	USHORT numHeads;

	ULONG hiddSec;
	ULONG totSec32;

	UCHAR drvNum;
	UCHAR reserved1;
	UCHAR bootSig;
	CString volID;
	CString volLab;
	CString filSysType;


	//FAT32才有的欄位
	//UCHAR fatSz32;
	//UCHAR extFlags;
	//UCHAR fsVer;
	//UCHAR rootClus;
	//UCHAR fsInfo;
	//UCHAR bkBootSec;
	//UCHAR Reserved;



	CString toString();
	void load(const UCHAR* bootSecBuf);

};
