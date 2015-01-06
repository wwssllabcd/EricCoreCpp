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

	//res += "=== Fast Test ===" + Utility::CrLf();

	vector< pair<BYTE, string> > debugInfos;
	
	for( int i=1; i<0x200; i++){

		ULONG offset = i*0x10;
		BYTE tmp = buffer[offset];

		if(tmp == 0xFF){
			continue;
		}

		 pair<BYTE, string> info;

		BYTE loop_i = tmp >>4;
		ULONG addr = Utility::arrayToUlong(buffer+offset);
		addr &= 0x0FFFFFFF;

		BYTE ceNo = tmp&0x0F;
		info.first = ceNo;

		BYTE err_state_0 = (buffer[offset+7]>>6)&0x03;
		BYTE err_state_1 = (buffer[offset+7]>>4)&0x03;
		BYTE err_state_2 = (buffer[offset+7]>>2)&0x03;
		BYTE err_state_3 = (buffer[offset+7]>>0)&0x03;

		info.second += "("+ Utility::toHexString(i, "%03X") + ")" 
			+ "CE=" + Utility::toHexString(ceNo,"%02X") + ", Ptn=" + Utility::toHexString(loop_i) 
			+ ", Addr = " + Utility::toHexString(addr, "%08X") 
			+ ", Err_Bit = " + Utility::toHexString(buffer[offset+6]) 
			+ ", ECC_Sts = " + Utility::toHexString(err_state_0) + "," + Utility::toHexString(err_state_1) + "," + Utility::toHexString(err_state_2) + "," + Utility::toHexString(err_state_3)
			+ ", Fail 4k No= " + Utility::toHexString(buffer[offset+5]) 
			+ ", ECC_BitCnt = " + Utility::toHexString(buffer[offset+8]) + "," + Utility::toHexString(buffer[offset+9]) + "," + Utility::toHexString(buffer[offset+0x0A]) + "," + Utility::toHexString(buffer[offset+0x0B])
			+ Utility::CrLf();

		debugInfos.push_back(info);
	}

	string res_0, res_1, res_2, res_3;
	for(size_t i=0; i<debugInfos.size(); i++){
		 pair<BYTE, string> info = debugInfos[i];
		 
		 switch(info.first)
		 {
		 case 0:
			 res_0+=info.second;
			 continue;
		 case 1:
			 res_1+=info.second;
			 continue;
		 case 2:
			 res_2+=info.second;
			 continue;
		 case 3:
			 res_3+=info.second;
			 continue;
		 default:
			 res+=info.second;
			 continue;
			
		 }
	}

	res = res + res_0 + res_1 + res_2 + res_3;
	res+=Utility::CrLf();
	return res;
}

string SortParser::getSortingToolInfo(BYTE* buffer)
{
	string res;
	BYTE sortTool = buffer[0x10 + 14];
	string strSTool;
	if( sortTool == 0x04){
		strSTool = "4 Port";
	}
	if( sortTool == 0x16){
		strSTool = "16 Port";
	}
	return strSTool;
}

void SortParser::getBadBlock(vector<ULONG>& badBlock, BYTE* buffer )
{
	ULONG bb_addr;
	for( int i =0x800; i<0xB30; i++){
		BYTE tmp = buffer[i];
		for(int j=0; j<8; j++){
			if( (tmp&BIT0) == 0){
				bb_addr =  (i-0x800)*8 + j;
				badBlock.push_back(bb_addr);
			}
			tmp = tmp>>1;
		}
	}
}

string SortParser::parsertSortTable(BYTE* buffer)
{
	string res;
	SortBase sb;

	if( sb.checkSignature_98(buffer) == false ){
		return "";
	}

	res += "=== 3S 2098 SORTING === " + Utility::CrLf() ;
	WORD totalGoodCnt = Utility::arrayToWord( buffer+0x10);
	WORD totalbadCnt = Utility::arrayToWord( buffer+0x1C);

	res += "Total Good Block Cnt = 0x" + Utility::toHexString(totalGoodCnt) + "(" + Utility::toString(totalGoodCnt) + ")" + Utility::CrLf() ;
	res += "Total Bad  Block Cnt = 0x" + Utility::toHexString(totalbadCnt) + "(" + Utility::toString(totalbadCnt) + ")" + Utility::CrLf() ;

	res += "FW version = 0x" + Utility::arrayToHexString( buffer+0x12, 1, "") + Utility::CrLf();
	res += "Bin Grade = 0x" + Utility::arrayToHexString( buffer+0x13, 1, "") + Utility::CrLf();
	res += "Sorting Tool = " + getSortingToolInfo(buffer) + Utility::CrLf();

	res +=  Utility::CrLf();
	res += "===== Sort Param ====="  + Utility::CrLf();
	res += "TestFlag = 0x" + Utility::toHexString( buffer[0xB40 + 4]) + Utility::CrLf();
	res += Utility::CrLf();

	vector<ULONG> badBlock;
	getBadBlock(badBlock, buffer);
	res += "===== Bad Block cnt = 0x" + Utility::toHexString( (ULONG)badBlock.size()) + Utility::CrLf();

	for(size_t i=0; i<badBlock.size(); i++){
				
		int offset = 0x800 + badBlock[i]/8;
		int bit = badBlock[i]%8;

		res += "BadBlk = 0x" + Utility::toHexString( badBlock[i] ) 
			+ ", offset = 0x" + Utility::toHexString(offset ) 
			+ ", BIT = " + Utility::toHexString(bit ) 

			+ Utility::CrLf();
	}

	return res;
}
