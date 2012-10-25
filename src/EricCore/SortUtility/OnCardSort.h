#pragma once

class SortBase;
class SbOption;

#include <string>
using namespace std;

class OnCardSort
{
public:
	OnCardSort(void);
	~OnCardSort(void);

	bool doOnCardSort(HANDLE handle, string iniPathOfSorting);
	void laod_7800_8000(HANDLE handle,  string iniPathOfSorting);
	void ispRamTest(HANDLE handle, BYTE* source, int ram_h, int sec);
	bool showOnCardSortStatus(HANDLE hansle);
	void laod_0_7FF(HANDLE handle, string iniPathOfSorting);
};
