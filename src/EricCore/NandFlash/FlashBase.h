#pragma once
#include  <iostream> 
using namespace std;

#include "Chip.h"
#include "..\Utility\Utility.h"


class FlashBase
{
public:
	FlashBase(void);
	~FlashBase(void);

	typedef Chip::Chips Chips;
	typedef Chip::LevelCellType LevelCellType;

	// ============= property =======================
	int	totalSize;// MB
	int	totalBlock;
	int suggesTiming;

	WORD cylinder;
	BYTE header;
	BYTE sector;
	ULONG maxLBA;

	Chips chips;
	ModeType modeType;

	// ============= function =======================
	int blockPerChip(void) const;
	int diePerChip(void) const;
	int totalCE(void) const;
	int getMakerCode(void) const;
	int pagePerBlock(void) const;
	int secPerPage(void) const;
	int secPerBlock(void) const;
	bool hasCacheProgram(void) const;
	bool hasInterleave(void) const;

	tstring toString(void)const;
	BlockType blockType(void) const;
	LevelCellType levelCellType(void) const;

protected:
	ModeType _getModeType(BYTE* flashIdTb);
	Chip::Chips _getChips(const BYTE* flashIdTb);
	void _checkChips(const Chip::Chips& chips);
	int _getTotalBlock(void);
	int _getTotalSize(void);
	void _loadDefaultCHS(int sizeMB);
};
