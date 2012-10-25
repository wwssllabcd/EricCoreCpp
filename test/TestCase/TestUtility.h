#pragma once
#include "..\..\src\Utility\Utility.h"

class TestUtility
{
public:
	TestUtility(void);
	~TestUtility(void);
	void start(void);
	void test_GetBit(void);


	void test_SetBitWord(void);
	void test_SetBitByte(void);
	
	void test_ToWordPtr(void);
	void test_ToBytePtr(void);
	void test_ToBool(void);
	void test_hexStringToULONG(void);
	void test_toInt(void);

private:

};
