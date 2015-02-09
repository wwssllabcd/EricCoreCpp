#pragma once

#include "Fat16.h"

class Fat32: public Fat16
{
public:
	Fat32(void);
	~Fat32(void);

	ULONG fatSz32;
	WORD extFlags;
	WORD fsVer;
	ULONG rootClus;
	WORD FSInfo;
	WORD BkBootSec;

	void load(ULONG totalSec, int sizeMB,int hiddenSec);
	void toAry(BYTE* array32);

	int getSecPerFat(int TS);

private:
	int _getBoundaryUnit(int fat32_SizeMB);
	int _getSecPerCluster(int fat32_SizeMB);
	void _getHS( int& h, int& s, int sizeMB);

};
