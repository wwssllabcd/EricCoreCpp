#pragma once
#include <string>
using namespace std;

class ModeType
{
public:
	~ModeType(void);

	static ModeType null(void);
	static ModeType byteMode(void);
	static ModeType wordMode(void);

	bool operator ==(const ModeType& rhs) const;
	string toString(void) const;


private:
	ModeType(int value);
	int _value;

};
