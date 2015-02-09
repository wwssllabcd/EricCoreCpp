#pragma once

#include "..\..\Utility\Utility.h"

class DataParser
{
public:
	DataParser(void);
	~DataParser(void);

	tstring makeRam0String_2090(BYTE* ram0);
	tstring makeC000_info(BYTE* ramC000, bool isIM_CM);
	tstring checkC000(BYTE* bufC000);
	tstring check_duplicateBlock(BYTE* ramC000, BYTE* ramA000);


private:

};
