#pragma once
#include <string>
using namespace std;

class LevelCellType
{
public:
	
	~LevelCellType(void);

	static LevelCellType null(void);
	static LevelCellType SLC(void);
	static LevelCellType MLC(void);
	static LevelCellType TLC(void);

	bool operator ==(const LevelCellType& rhs) const;
	bool operator !=(const LevelCellType& rhs) const;

	string toString(void) const;

private:
	LevelCellType(int value);
	int _value;

};
