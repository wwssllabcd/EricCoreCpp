#pragma once

#include <string>
#include <vector>
using namespace std;

namespace EricCore{
	class SortParser
	{
	public:
		SortParser(void);
		~SortParser(void);

		string parsertSortTable(BYTE* buffer);
		string parsertSortDebugMsg(BYTE* buffer);

		string getSortingToolInfo(BYTE* buffer);
		void getBadBlock(vector<ULONG>& badBlock, BYTE* buffer );

	};

}
