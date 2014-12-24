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

	res += "=== Fast Test ===" + Utility::CrLf();

	for( int i=1; i<0x200; i++){

		ULONG offset = i*0x10;
		BYTE tmp = buffer[offset];

		if(tmp == 0xFF){
			continue;
		}

		BYTE loop_i = tmp >>4;
		ULONG addr = Utility::arrayToUlong(buffer+offset);
		addr &= 0x0FFFFFFF;

		BYTE err_state_0 = (buffer[offset+7]>>6)&0x03;
		BYTE err_state_1 = (buffer[offset+7]>>4)&0x03;
		BYTE err_state_2 = (buffer[offset+7]>>2)&0x03;
		BYTE err_state_3 = (buffer[offset+7]>>0)&0x03;

		res += "("+ Utility::toHexString(i, "%03X") + ")" 
			+ "Pattern=" + Utility::toHexString(loop_i) + ",CE=" + Utility::toHexString(tmp&0x0F,"%02X") 
			+ ", Addr = " + Utility::toHexString(addr, "%08X") 
			+ ", Err_Bit = " + Utility::toHexString(buffer[offset+6]) 
			+ ", ECC_Sts = " + Utility::toHexString(err_state_0) + "," + Utility::toHexString(err_state_1) + "," + Utility::toHexString(err_state_2) + "," + Utility::toHexString(err_state_3)
			+ ", ECC_BitCnt = " + Utility::toHexString(buffer[offset+8]) + "," + Utility::toHexString(buffer[offset+9]) + "," + Utility::toHexString(buffer[offset+0x0A]) + "," + Utility::toHexString(buffer[offset+0x0B])
			+ Utility::CrLf();
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
