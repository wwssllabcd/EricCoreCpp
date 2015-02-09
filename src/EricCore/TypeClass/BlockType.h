#pragma once
#include <string>
using namespace std;

class BlockType
{
public:
	
	~BlockType(void);

	static BlockType null(void);
	static BlockType smallBlock(void);
	static BlockType largeBlock(void);

	bool operator ==(const BlockType& rhs) const;
	string toString(void) const;

private:
	BlockType(int value);
	int _value;

};
