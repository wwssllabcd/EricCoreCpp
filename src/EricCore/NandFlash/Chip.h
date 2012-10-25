#pragma once

#include "../TypeClass/TypeDefine.h"


#include <vector>

using namespace std;

class Chip
{
public:
	Chip(void);
	~Chip(void);

	//type define
	typedef std::vector<Chip> Chips;

	//Type Class
	typedef BlockType BlockType;
	typedef LevelCellType LevelCellType;

	BlockType blockType;
	LevelCellType levelCellType;

	BYTE makerCode;
	BYTE deviceCode;
	BYTE idData3;
	BYTE idData4;
	BYTE idData5;

	int blockPerChip;
	int pagePerBlock;
	int secPerBlock;
	int secPerPage;
	int size;
	int blockSize; //kb
	int dieNum;

	//1=1K, 2=2K, 4=4K, 8=8K
	int pageSize;		
	int nanometer;

	bool hasInterleave;	  
	bool hasCacheProgram;
	bool has2Plane;		
	bool has2PlaneErase;  //support 2 plane Erase (0x60,0x60,0xD0)
	
	WORD maxPage;		 //For J version
	BYTE max_C_endPtr;	 //For J version
	BYTE flashAddr5Mask; //For J version


	int dieStart_1st;
	int dieStart_2st;
	int dieStart_3st;
	int dieStart_4st;

	int dieEnd_1st;
	int dieEnd_2st;
	int dieEnd_3st;
	int dieEnd_4st;
	int defectMarkColumnOffset;


	//function-----------------------------------------------
	Chip getChipById(const BYTE *buf);
	string toString(void) const;
	
	BYTE orgLowPage[64];
	BYTE lowPageTable[64];
	BYTE mlcPageTable[0x100];

	BYTE twoPlaneCmd[6];
	BYTE twoPlaneEraseCmd[3];

	void load(const BYTE *buf);
	
private:
	LevelCellType  _getLevelCellType(const UCHAR *buf, const BlockType& blockType);
	
	void _loadBlockTypeAndSize(BYTE deviceCode, Chip& chip);
	
	bool _has2Plane(const BYTE& makerCode, const BYTE& id3, const BYTE& id5);
	bool _getInterLeave(const BYTE& id3);
	bool _getCacheProgram(const BYTE& id3);
	
	int _getNanometer(void);
	int _getDieNum(const UCHAR *buf);
	int _getPageSize(const BYTE* buf);
	int _getPagePerBlock(const LevelCellType& levelType, const BlockType& blockType);
	int _getBlockSize(const BYTE* buf, const BlockType& blockType, const LevelCellType& lct, int pageSize ,int pagePerBlock);
	
	WORD _getMaxPage(const bool is2PlaneMode, int pagePerBlock);
	BYTE _getMax_C_endPtr(const bool is2Plane);
	BYTE _getFlashAddr5Mask(void);

	void _getLowPageTable(BYTE* ary64byte, const BYTE* buf, LevelCellType& lct, int nanometer);
	void _get2PlaneWriteCmd(BYTE* twoPlaneCmd, BYTE makerCode, bool has2Plane, bool hasWriteCache);
	void _set2PlaneErase(Chip& chip);
	void _setSpecialFlash(Chip& chip);

	void _lookup_intel(void);
	void _lookup_Hynix(void);
	void _lookup_micron(void);

	int _getSecPerPage(int pageSize);
	int _getSecPerBlock(int secPerPage, int pagePerBlock);
	int _getBlockSize(int pageSize, int pagePerBlock);

};
