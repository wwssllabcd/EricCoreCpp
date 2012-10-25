#pragma once
#include "../../NandFlash/FlashBase.h"

#define NO_INTERLEAVE 0
#define INTERNAL_INTERLEAVE 1
#define EXTERNAL_INTERLEAVE 2

class Flash : public FlashBase
{
public:
	Flash(void);
	~Flash(void);

	Flash getFlash(BYTE* flashIdTb);
	Chip::Chips getChips_8ByteFormat(const BYTE* flashIdTb);
	ModeType getModeType_8ByteFormat(BYTE* flashIdTb);
	void loadAge(BYTE* flashIdTb);
	void setProperty(Flash& f);
	void getLowPageCheckTable(BYTE* ary);
	BYTE getPhyPage(BYTE logicPage) const;
	bool getCETab(BYTE* ceTab, BYTE* rdyTab, BYTE* buf);
	

	BYTE maxPlane;
	WORD maxLBperWin;
	BYTE maxSLCpage;
	BYTE maxMLCpage;

	bool is_TSB_43nmD2;
	bool is_TSB_43nmD2_LGA;
	bool is_TSB_43nmD2_8Gb;

	bool is_TSB_43nmD3;
	bool is_TSB_43nmD3_Special;

	bool is_TSB_56nmD3;
	bool is_TSB_32nmD2;
	bool is_TSB_32nmD2_LGA;
	bool is_TSB_24nmD2;
	bool is_TSB_24nmD2_128Page;

	bool is_SanDisk_43nmD2;
	bool is_SanDisk_43nmD3;
	bool is_SanDisk_56nmD3;
	bool is_SanDisk_32nmD2;
	bool is_SanDisk_24nmD2;
	
	
	bool is_IM_50nmD2;
	bool is_IM_34nmD2;
	bool is_IM_34nmD2_256Page;
	bool is_IM_28nmD2_256Page;


	

	//bool is_Hynix_32nmD2;
	bool is_Hynix_32nmD2_256Page;

	bool is_Samsuang_32nmD2;
	bool is_Samsuang_32nmD3;
	bool is_Samsuang_35nmD2_LGA;


	bool is16_IO;
	bool isCM;
	bool is2kDefect;
	bool is4ce;

	bool is2PageMode;

	int winCntPerPlane;

	int defectMarkColumnOffset;
	int pageSize;


	int interExterInterleave;

	int evenDieStart;
	int evenDieEnd;

	int oddDieStart;
	int oddDieEnd;

	int blockLen;     // 128
	ULONG defectBlockAddr;

private:
	void _judge_IM_Age(BYTE maker, BYTE device, BYTE id3, BYTE id4, BYTE id5, BYTE id6);
	void _judge_sandisk_Age(BYTE maker, BYTE device, BYTE id3, BYTE id4, BYTE id5, BYTE id6);
	void _judge_TSB_Age(BYTE maker, BYTE device, BYTE id3, BYTE id4, BYTE id5, BYTE id6);
	void _judge_Hynix_Age(BYTE maker, BYTE device, BYTE id3, BYTE id4, BYTE id5, BYTE id6);
	void _judge_Samsung_Age(BYTE maker, BYTE device, BYTE id3, BYTE id4, BYTE id5, BYTE id6);
	void _showFlashTypeAndAge(void);
};
