#pragma once

#include "..\..\Utility\Utility.h"

class DataParser_97
{
public:
	DataParser_97(void);
	~DataParser_97(void);


	tstring makeRam0String(BYTE* ram0);
	tstring makeFwStructInfo(BYTE* ram9000, BYTE* ramA000, BYTE* ramB000, BYTE* ramC000 );
	tstring makeC000_info(BYTE* ram9000, BYTE* ramA000, BYTE* ramB000, BYTE* ramC000 );

		
	tstring checkC000(BYTE* ram9000, BYTE* ramA000, BYTE* ramB000, BYTE* ramC000);
	tstring checkSpareAge_AndSpareTb(BYTE* ramC000);
	tstring check_CTable(BYTE* ramB000, BYTE* ramC000);

	WORD getPageNo(WORD defaultValue, BYTE* bufB000);
	tstring checkMlcTable(BYTE* ramC000);
	tstring check_duplicateBlock(BYTE* ramC000, BYTE* ramA000);

	tstring check_3sBlock_struct(BYTE* ptr3sBlock, WORD offset);

private:
	tstring _checkSpareTableAndCnt(BYTE* ramC000);
	tstring _checkWrongCurC_Ptr(BYTE* ramC000);
	int _getMlcTableLen(BYTE* ramC000);
	int _getSlcTableLen(BYTE* ramC000);
	int _getEntryTableLen(BYTE* ramA000);

};
