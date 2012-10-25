#include "stdafx.h"
#include ".\testutility.h"
#include "..\..\src\Utility\Utility.h"

#include <math.h>
#include <string>


TestUtility::TestUtility(void)
{

}

TestUtility::~TestUtility(void)
{
}
void TestUtility::start(void){
	Utility::sendMsg( _T("Test <Utility> Start ..."));
	Utility::sendMsg( _T(""));

	test_GetBit();
	test_ToBool();
	test_ToWordPtr();
	test_ToBytePtr();
	test_SetBitWord();
	test_SetBitByte();
	test_hexStringToULONG();
	
}

void TestUtility::test_GetBit(void){
	Utility::sendMsg(_T("     test GetBit() ..... "));
	
	//測試 Word 的Get Bit
	WORD pattern1 = 0xAAAA;
	//int pattern2 = 0xFF00;
	//int pattern3 = 0x00FF;
	int wordResult(2);

	wordResult = Utility::getBit(pattern1,0);
	ASSERT(wordResult == 0);
	wordResult = Utility::getBit(pattern1,1);
	ASSERT(wordResult == 1);
	wordResult = Utility::getBit(pattern1,2);
	ASSERT(wordResult == 0);
	wordResult = Utility::getBit(pattern1,3);
	ASSERT(wordResult == 1);
	wordResult = Utility::getBit(pattern1,4);
	ASSERT(wordResult == 0);
	wordResult = Utility::getBit(pattern1,5);
	ASSERT(wordResult == 1);
	wordResult = Utility::getBit(pattern1,6);
	ASSERT(wordResult == 0);
	wordResult = Utility::getBit(pattern1,7);
	ASSERT(wordResult == 1);
	wordResult = Utility::getBit(pattern1,8);
	ASSERT(wordResult == 0);
	wordResult = Utility::getBit(pattern1,9);
	ASSERT(wordResult == 1);
	wordResult = Utility::getBit(pattern1,10);
	ASSERT(wordResult == 0);
	wordResult = Utility::getBit(pattern1,11);
	ASSERT(wordResult == 1);
	wordResult = Utility::getBit(pattern1,12);
	ASSERT(wordResult == 0);
	wordResult = Utility::getBit(pattern1,13);
	ASSERT(wordResult == 1);
	wordResult = Utility::getBit(pattern1,14);
	ASSERT(wordResult == 0);
	wordResult = Utility::getBit(pattern1,15);
	ASSERT(wordResult == 1);


	//測試 Byte 的Get Bit
	BYTE bytePattern1 = 0xAA;
	int byteResult(2);

	byteResult = Utility::getBit(bytePattern1,0);
	ASSERT(byteResult == 0);
	byteResult = Utility::getBit(bytePattern1,1);
	ASSERT(byteResult == 1);
	byteResult = Utility::getBit(bytePattern1,2);
	ASSERT(byteResult == 0);
	byteResult = Utility::getBit(bytePattern1,3);
	ASSERT(byteResult == 1);
	byteResult = Utility::getBit(bytePattern1,4);
	ASSERT(byteResult == 0);
	byteResult = Utility::getBit(bytePattern1,5);
	ASSERT(byteResult == 1);
	byteResult = Utility::getBit(bytePattern1,6);
	ASSERT(byteResult == 0);
	byteResult = Utility::getBit(bytePattern1,7);
	ASSERT(byteResult == 1);

	Utility::sendMsg(false, false, _T("PASS"));
}



void TestUtility::test_SetBitByte(void){
	Utility::sendMsg(_T("     test SetBitByte() ..... "));

	BYTE target;
	BYTE i(0);

	//測試設為1
	for(i=0; i<8; i++){
		target = 0;
		Utility::setBit(target,i,1);
		//pow為次方函數
		ASSERT(target == pow(2,i));
	}

	//測試設為0
	for(i=0; i<8; i++){
		target = 0xFF;
		Utility::setBit(target,i,0);
		//pow為次方函數
		ASSERT(target == (0xFF-pow(2,i)) );
	}

	//測試除錯能力，故意設為8
	try{
		target = 0;
		Utility::setBit(target,8,1);
		ASSERT(false);
	}catch(...){
	}

	//測試除錯能力，故意把bit0設為3
	try{
		target = 0;
		Utility::setBit(target,8,3);
		ASSERT(false);
	}catch(...){
	}

	Utility::sendMsg(false, false, _T("PASS"));
}

void TestUtility::test_SetBitWord(void){
	Utility::sendMsg(_T("     test SetBitWord() ..... "));

	WORD target;
	BYTE i(0);

	//測試設為1
	for(i=0; i<16; i++){
		target = 0;
		Utility::setBit(target,i,1);
		//pow為次方函數
		ASSERT(target == pow(2,i));
	}

	//測試設為0
	for(i=0; i<16; i++){
		target = 0xFFFF;
		Utility::setBit(target,i,0);
		//pow為次方函數
		ASSERT(target == (0xFFFF-pow(2,i)) );
	}

	//測試除錯能力，故意設為16
	try{
		target = 0;
		Utility::setBit(target,16,1);
		ASSERT(false);
	}catch(...){
	}

	//測試除錯能力，故意把bit0設為3
	try{
		target = 0;
		Utility::setBit(target,0,3);
		ASSERT(false);
	}catch(...){
	}

	Utility::sendMsg(false, false, _T("PASS"));
}

void TestUtility::test_ToWordPtr(void){
	Utility::sendMsg(_T("     test ToWordPtr() ..... "));

	BYTE bArray[100];
	int arraySize = sizeof(bArray)/sizeof(BYTE);
	BYTE i(0);

	//下隨機種
	srand((unsigned int)time(NULL));

	//建立隨機陣列
	for(i=0; i<arraySize; i++){
		bArray[i] = static_cast<BYTE>(rand());
	}

	//轉換指標
	WORD* wArray;
	wArray = Utility::toWordPtr(bArray);

	//開始測試 Read Data
	for(i=0; i<arraySize; i++){
		ASSERT(wArray[i] == (bArray[i*2+1]*256 + bArray[i*2]) );
	}

	//建立另一個隨機陣列，看wArray是否會隨著改變？
	//下隨機種
	srand((unsigned int)time(NULL));
	for(i=0; i<arraySize; i++){
		bArray[i] = static_cast<BYTE>(rand());
	}

	//開始測試 Read Data
	for(i=0; i<arraySize; i++){
		ASSERT(wArray[i] == (bArray[i*2+1]*256 + bArray[i*2]) );
	}

	Utility::sendMsg(false, false, _T("PASS"));
}


void TestUtility::test_ToBytePtr(void){
	Utility::sendMsg(_T("     test ToBytePtr() ..... "));

	WORD wArray[100];
	int arraySize = sizeof(wArray)/sizeof(WORD);
	WORD i(0);

	//下隨機種
	srand((unsigned int)time(NULL));

	//建立隨機陣列
	for(i=0; i<arraySize; i++){
		wArray[i] = rand();
	}

	//轉換指標
	BYTE* bArray;
	bArray = Utility::toBytePtr(wArray);

	//開始測試 Read Data
	for(i=0; i<arraySize; i++){
		ASSERT(wArray[i] == (bArray[i*2+1]*256 + bArray[i*2]) );
	}

	//建立另一個隨機陣列，看wArray是否會隨著改變？
	//下隨機種
	srand((unsigned int)time(NULL));
	for(i=0; i<arraySize; i++){
		bArray[i] = static_cast<BYTE>( rand() );
	}

	//開始測試 Read Data
	for(i=0; i<arraySize; i++){
		ASSERT(wArray[i] == (bArray[i*2+1]*256 + bArray[i*2]) );
	}

	Utility::sendMsg(false, false, _T("PASS"));
}

void TestUtility::test_ToBool(void){
	Utility::sendMsg(_T("     test ToBool() ..... "));

	int zero = 0;
	int one = 1;
	bool result;

	result = Utility::toBool(zero);
	ASSERT(result == false);

	result = Utility::toBool(one);
	ASSERT(result == true);

	result = Utility::toBool(-1);
	ASSERT(result == true);

	result = Utility::toBool(2);
	ASSERT(result == true);

	Utility::sendMsg(false, false, _T("PASS"));

}

void TestUtility::test_hexStringToULONG(void){
	Utility::sendMsg(_T("     test hexStringToInt() ..... "));
	int res=0;
	tstring str;

	str = "0x1234";
	res = Utility::hexStringToULONG(str);
	ASSERT(res == 0x1234);

	str = "1234h";
	res = Utility::hexStringToULONG(str);
	ASSERT(res == 0x1234);

	//it should be 0x1234
	str = "1234";
	res = Utility::hexStringToULONG(str);
	ASSERT(res == 0x1234);

	str = "0x12345678";
	res = Utility::hexStringToULONG(str);
	ASSERT(res == 0x12345678);

	str = "12345678h";
	res = Utility::hexStringToULONG(str);
	ASSERT(res == 0x12345678);

	//it should be 12345678
	str = "12345678";
	res = Utility::hexStringToULONG(str);
	ASSERT(res == 0x12345678);


	// =========  hex test =============
	str = "0x12ab";
	res = Utility::hexStringToULONG(str);
	ASSERT(res == 0x12ab);

	str = "ab12h";
	res = Utility::hexStringToULONG(str);
	ASSERT(res == 0xab12);


	str = "ab12";
	res = Utility::hexStringToULONG(str);
	ASSERT(res == 0xab12);

	str = "0xabcdef12";
	res = Utility::hexStringToULONG(str);
	ASSERT(res == 0xabcdef12);

	Utility::sendMsg(false, false, _T("PASS"));
}

void TestUtility::test_toInt(void){
	Utility::sendMsg(_T("     test toInt() ..... "));

	Utility::sendMsg(false, false, _T("PASS"));
}