#include "StdAfx.h"
#include ".\sortparser.h"


#include ".\sortbase.h"
#include "EricCore\Utility\Utility.h"


using namespace EricCore;
using namespace std;



SortParser::SortParser(void)
{
}

SortParser::~SortParser(void)
{
}

string SortParser::parsertSortDebugMsg(BYTE* buffer)
{
	string res;
	SortBase sb;
	if( sb.checkSignature_98_debugMsg(buffer) == false ){
		return "";
	}

	return res;
}

string SortParser::parsertSortTable(BYTE* buffer)
{
	string res;
	SortBase sb;

	if( sb.checkSignature_98(buffer) == false ){
		return "";
	}


	res += "Total Good Block Cnt = 0x" + Utility::arrayToHexString( buffer+0x10, 2, "") + Utility::CrLf();
	res += "FW version  = 0x" + Utility::arrayToHexString( buffer+0x12, 1, "") + Utility::CrLf();
	res += "Bin Grade = 0x" + Utility::arrayToHexString( buffer+0x13, 1, "") + Utility::CrLf();
	res += "Sorting Board Sig = 0x" + Utility::arrayToHexString( buffer+0x10 + 14, 1, "") + Utility::CrLf();
	int bb_cnt = 0;

	res += "===== Bad Block ====="  + Utility::CrLf();
	for( int i =0x800; i<0xB30; i++){
		BYTE tmp = buffer[i];

		for(int j=0; j<8; j++){
			if( (tmp&BIT0) == 0){
				int bb_addr =  (i-0x800)*8 + j;
				res += "bad Block = 0x" +  Utility::toHexString(bb_addr) + Utility::CrLf();
				bb_cnt++;
			}
			tmp = tmp>>1;
		}
	}
	res += "Bad Block Cnt = 0x" + Utility::toHexString(bb_cnt) + Utility::CrLf();

	res += "===== Sort Param ====="  + Utility::CrLf();

	res += "Test Param = 0x" + Utility::toHexString( buffer[0xB40 + 4]) + Utility::CrLf();

	

	return res;
}
