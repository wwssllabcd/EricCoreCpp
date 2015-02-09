#include "StdAfx.h"
#include ".\dataparser.h"

using namespace EricCore;

DataParser::DataParser(void)
{
}

DataParser::~DataParser(void)
{
}

tstring DataParser::makeRam0String_2090(BYTE* ram0){
	tstring result;
	result += "CycleList = " + Utility::arrayToHexString(ram0+0x48, 4) + Utility::CrLf();

	result += "now_window = " + Utility::toHexString(ram0[0x3a]) + Utility::CrLf();
	result += "Current Window = " + Utility::toHexString(ram0[0x3B]) + Utility::CrLf();
	result += "CAreaP = " + Utility::arrayToHexString(ram0+0x3C,2) + Utility::CrLf();
	result += "CSegSP = " + Utility::toHexString(ram0[0x3e]) + Utility::CrLf();
	result += "CSegSP_Plane = " + Utility::toHexString(ram0[0x3f]) + Utility::CrLf();
	result += "CSegSP_Sec = " + Utility::toHexString(ram0[0x40]) + Utility::CrLf();

	result += "swap_endptr = " + Utility::toHexString(ram0[0x34]) + Utility::CrLf();
	result += "max_SLC_page = " + Utility::toHexString(ram0[0x37]) + Utility::CrLf();
	result += "max_plane = " + Utility::toHexString(ram0[0x38]) + Utility::CrLf();
	result += "max_MLC_page = " + Utility::toHexString(ram0[0x39]) + Utility::CrLf();


	result += "total_W = " + Utility::toHexString(ram0[0x44]) + Utility::CrLf();
	result += "error_code = " + Utility::toHexString(ram0[0x6A]) + Utility::CrLf();
	return result;
}


tstring DataParser::makeC000_info(BYTE* ramC000, bool isIM_CM){
	tstring msg,temp;
	WORD wcStartAddr = 0x100;
	for(int wcNo=0; wcNo<8; wcNo++){
		temp = Utility::strFormat("(%d)", wcNo) ;
		msg+= temp ;
		msg+= "Param = " +  Utility::arrayToHexString(ramC000+wcStartAddr+wcNo*0x20+0x00,13) + Utility::CrLf();
		msg+= " W-Block = " +  Utility::arrayToHexString(ramC000+wcStartAddr+wcNo*0x20+0x0E,0x6) + Utility::CrLf();
		msg+= " D-Block = " +  Utility::arrayToHexString(ramC000+wcStartAddr+wcNo*0x20+0x14,0x6) + Utility::CrLf();
		msg+= " C-Block = " +  Utility::arrayToHexString(ramC000+wcStartAddr+wcNo*0x20+0x1A,0x6) + Utility::CrLf();
	}


	msg+= Utility::CrLf();


	msg+= "Wi-Block(TypeA,B) = " +  Utility::arrayToHexString(ramC000+0xB7, 0x6) + Utility::CrLf();
	msg+= "Wi-Block-pageEnd = " +  Utility::toHexString(ramC000[0xA5]) + Utility::CrLf();


	WORD baseBlockNo = (WORD)(ramC000[0xA0]<<8) + ramC000[0xA1];
	msg+= "Base Block = " + Utility::toHexString(baseBlockNo) + Utility::CrLf();



	WORD rlnkStart = (WORD)(ramC000[0xA6]<<8) + ramC000[0xA7];
	WORD rlnkEnd = (WORD)(ramC000[0xA8]<<8) + ramC000[0xA9];
	msg+= "Relink Start = " + Utility::toHexString(rlnkStart) + Utility::CrLf();
	msg+= "Relink End   = " + Utility::toHexString(rlnkEnd) + Utility::CrLf();
	
	WORD eraseSkip = (WORD)(ramC000[0xAA]<<8) + ramC000[0xAB];
	msg+= "Erase Skip = " + Utility::toHexString(eraseSkip) + Utility::CrLf();

	if( rlnkStart>= rlnkEnd){	
		msg = "RlnkStart>RlnkEnd，Attention Please !!!!" + Utility::CrLf() + Utility::CrLf()  + msg;
	}

	if( rlnkStart >= rlnkEnd){	
		if( eraseSkip ){
			msg = "RlnkStart>=RlnkEnd，and Erase!=0 Attention Please !!!!" + Utility::CrLf() + Utility::CrLf() + msg;
		}
	}


	BYTE maxWin = ramC000[0x404+1];
	msg+= "Max Window = " + Utility::toHexString(maxWin) + Utility::CrLf();

	WORD MaxLB = (WORD)(ramC000[0x404+17]<<8) + ramC000[0x404+18];
	msg+= "Max LB = " + Utility::toHexString(MaxLB) + Utility::CrLf();
	msg+= "Max SLC Page = " + Utility::toHexString(ramC000[0x404+19]) + Utility::CrLf();
	msg+= "Max Plane = " + Utility::toHexString(ramC000[0x404+20]) + Utility::CrLf();
	msg+= "Max MLC Page = " + Utility::toHexString(ramC000[0x404+21]) + Utility::CrLf();

	msg+= "Relink L0#0(Type C) = " +  Utility::arrayToHexString(ramC000+0xBD, 6) + Utility::CrLf();
	msg+= "Relink L0#1(Type C) = " +  Utility::arrayToHexString(ramC000+0xBD+6, 6) + Utility::CrLf();
	msg+= "Relink endPtr = " +  Utility::toHexString(ramC000[0xBD+12]) + Utility::CrLf();


	msg+= Utility::CrLf();
	msg+= "=== FinalWi Table ==="+ Utility::CrLf();
	int wiOffset = 8;
	int relnkOffset = 6;

	if( isIM_CM ){
		wiOffset = 4;
		relnkOffset = 3;
	}

	for(int i=0; i<maxWin; i++){
		msg+= "win-("+ Utility::toHexString(i)  + ") = " + Utility::arrayToHexString(ramC000+0x480+i*wiOffset, wiOffset) + Utility::CrLf();
	}

	msg+= Utility::CrLf();
	msg+= "=== Relink L1#0 Table(Type C) ==="+ Utility::CrLf();
	for(int i=0; i<maxWin; i++){
		msg+= "Relink L1#0-("+ Utility::toHexString(i)  + ") = " + Utility::arrayToHexString(ramC000+0x500+i*relnkOffset, relnkOffset) + Utility::CrLf();
	}

	msg+= Utility::CrLf();
	msg+= "=== Relink L1#1 Table(Type C) ==="+ Utility::CrLf();
	for(int i=0; i<maxWin; i++){
		msg+= "Relink L1#1-("+ Utility::toHexString(i)  + ") = " + Utility::arrayToHexString(ramC000+0x560+i*relnkOffset, relnkOffset) + Utility::CrLf();
	}
	return msg;
}

tstring DataParser::checkC000(BYTE* bufC000){
	tstring result;
	tstring msg;
	for(int wcNo=0; wcNo<8; wcNo++){
		//int wcOffset = 0x100+wcNo*0x20;
		// check W-Flag has Ecc-error ?
		BYTE w_flag = bufC000[0x100+wcNo*12];
		if(w_flag&1){
			msg ="W has ECC fail Flag";
		}
	}


	//檢查SpareEnd是否與SpareSorting相同
	BYTE spareEnd = bufC000[0xA2];
	if( spareEnd<0x10 ){
		for(int i=0;i<spareEnd; i++){
			if( bufC000[0x80+i]==0xFF ){
				msg = "spare sorting`s end != spareEnd ";
				result += msg + Utility::CrLf();
			}
		}

		//檢查SpareSorting記入的內容直，是否在spareTable中合法
		for(int i=0;i<spareEnd; i++){
			BYTE offset = bufC000[0x80+i];
			if( bufC000[offset*8]  ==0xFF){
				msg = "spare sorting`s record != sparTable, block is invaild ";
				result += msg + Utility::CrLf();
			}
		}
	}else{
		msg = "spare end > 0x10";
		result += msg + Utility::CrLf();
	}

	return result;
}



