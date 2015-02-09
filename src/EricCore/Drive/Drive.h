#pragma once
#include <string>
using namespace std;

#include "windows.h"

#define	 BUSY			 0
#define	 IDEL			 1

class Drive
{
public:
	//--------------- property ------------
	HANDLE address;
	string name;
	int testNo;
	int status;

	// -------------- function ------------
	Drive(void);
	~Drive(void);

	bool operator ==(const Drive& rhs) const;
};
