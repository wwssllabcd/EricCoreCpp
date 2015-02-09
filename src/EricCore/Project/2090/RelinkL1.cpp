#include "StdAfx.h"
#include ".\relinkl1.h"

RelinkL1::RelinkL1(void)
:addr_No0(0)
,addr_No1(0)
,ceNo(0xFF)
,maxWGroup(0)
,eraseSkip(0)
,maxLB_win(0)
,setF0_AllZero(false)
,retryReadCnt(0)
,relinkEnd(0)
{
	memset(ary_WiBlock, 0xFF, sizeof(ary_WiBlock));
	memset(arySpareTable, 0xFF, sizeof(arySpareTable));
	memset(RST, 0xFF, sizeof(RST));
}

RelinkL1::~RelinkL1(void)
{
}


void RelinkL1::toArray_2048(BYTE* aryRamC000){
	BYTE ramC000[2048];
	memset(ramC000, 0xFF, sizeof(ramC000));

	//wi-block
	memcpy(ramC000, this->arySpareTable, sizeof(this->arySpareTable));

	//====== spare sorting ========
	ramC000[0x80]=0;
	ramC000[0x81]=1;
	ramC000[0x82]=2;
	ramC000[0x83]=3;
	ramC000[0x84]=4;
	ramC000[0x85]=5;
	ramC000[0x86]=6;
	ramC000[0x87]=7;


	// for UNC use
	ramC000[0xF0]=0;
	ramC000[0xF1]=0;

	int const WI_SETTING = 0xA0;

	//=======base block
	ramC000[WI_SETTING+0] = this->ceNo;
	ramC000[WI_SETTING+1] = 0x00;

	//spare end
	ramC000[WI_SETTING+2]=8;

	//max W group
	ramC000[WI_SETTING+3] = this->maxWGroup;

	//W replace Token
	ramC000[WI_SETTING+4]=0;

	// wi-page-End
	ramC000[WI_SETTING+5]=0;

	//relink start
	ramC000[WI_SETTING+6]=0;
	ramC000[WI_SETTING+7]=0;

	//relink end
	ramC000[WI_SETTING+8] = (BYTE)((this->relinkEnd>>8)&0xFF);
	ramC000[WI_SETTING+9] = (BYTE)((this->relinkEnd&0xFF));

	//erase skip
	ramC000[WI_SETTING+10] = (BYTE)((this->eraseSkip>>8)&0xFF);
	ramC000[WI_SETTING+11] = (BYTE)((this->eraseSkip&0xFF));

	//next wi block
	ramC000[WI_SETTING+12]=8;

	//dirty D,W,C, new D, new D LB
	ramC000[WI_SETTING+13]=0xFF;
	ramC000[WI_SETTING+14]=0xFF;
	ramC000[WI_SETTING+15]=0xFF;
	ramC000[WI_SETTING+16]=0xFF;
	ramC000[WI_SETTING+17]=0xFF;
	ramC000[WI_SETTING+18]=0xFF;

	//mini age count
	ramC000[WI_SETTING+19]=0xFF;
	ramC000[WI_SETTING+20]=0xFF;
	ramC000[WI_SETTING+21]=0xFF;
	ramC000[WI_SETTING+22]=0xFF;

	//wi-block
	memcpy(ramC000+WI_SETTING+23, this->ary_WiBlock, sizeof(this->ary_WiBlock));

	int const RELINK_L1_ADDR = 0xBD;

	this->cycleListToTypeC(this->addr_No0, ramC000+RELINK_L1_ADDR);
	this->cycleListToTypeC(this->addr_No1, ramC000+RELINK_L1_ADDR+6);

	//relink L1 endPtr
	ramC000[RELINK_L1_ADDR+12]=0;

	//relink L1 ring
	ramC000[RELINK_L1_ADDR+13]=0;

	//retry read Cnt
	ramC000[ 0xD7 ]= this->retryReadCnt;

	// is set 0xF0 all zero
	if( setF0_AllZero ){
		memset( ramC000+0xF0, 0 , 0x10);
	}

	ramC000[0x415]= (BYTE)((maxLB_win>>8)&0xFF);
	ramC000[0x416]= (BYTE)((maxLB_win&0xFF));


	//copy array
	memcpy(aryRamC000, ramC000, sizeof(ramC000));
}

void RelinkL1::cycleListToTypeC(ULONG cycleList, BYTE* ary) const{
	ary[0] = (BYTE) (cycleList>>24)&0xFF;
	ary[1] = (BYTE) (cycleList>>16)&0xFF;
	ary[2] = (BYTE) (cycleList>> 8)&0xFF;

	if( (cycleList&0xFF)==0x80){
		ary[0] |= 0x80;
	}
}
