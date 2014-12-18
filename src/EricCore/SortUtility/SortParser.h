#pragma once

#include <string>

using namespace std;

namespace EricCore{
	class SortParser
	{
	public:
		SortParser(void);
		~SortParser(void);

		string parsertSortTable(BYTE* buffer);

	};

}
