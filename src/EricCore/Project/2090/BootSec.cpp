#include "StdAfx.h"
#include ".\bootsec.h"

#include "..\..\Utility\Utility.h"
using namespace EricCore;

BootSec::BootSec(void)
:clusterOffset(0)
,maxWindow(0)
,bootVer(0)
,productVer(0)
,ispMainVer(0)
,ispMinorVer(0)
,reg130(0)
,regWordModeCtrl(0)
,bootBlock(0)
,bootBlock_Backup(0)
,ispABSstartPage(0)
,frontEndConfig1(0)
,frontEndConfig2(0)
,frontEndConfig3(0)
,reg145(0)
,reg20E(0)
,reg20F(0)
,HighPowerDevicemA(0)
,maxLbTableABS(0)
,ispSecureABSPage(0)
,ispHIDABSPage(0)
,bhBoundary(0xFFFFFFFF)
,bhLbaMask(0)
,x_isp2ABSPage(0)
,bootPage4_7_PageNo(0)
,max_LB(0)
,max_SLC_page(0)
,max_plane(0)
,max_MLC_page(0)
,ispBackEndInitABSPage(0)
,ispRWModuleABSPage(0)

{
	memset(reserveSpareBlock,0xFF, sizeof(reserveSpareBlock) );
	memset(convertTb,0x00, sizeof(convertTb) );
	memset(info_block, 0x00, sizeof(info_block) );
}

BootSec::~BootSec(void)
{
}

void BootSec::toArray(BYTE* ary64){
	int const bootAddr = 0x04;
	memset(ary64,0x00, 0x40);

	ary64[0] = 0x5A;
	ary64[1] = 0xA5;
	ary64[2] = 0xA5;
	ary64[3] = 0x5A;

	ary64[bootAddr+0x00] = this->clusterOffset;
	ary64[bootAddr+0x01] = this->maxWindow;
	ary64[bootAddr+0x02] = this->bootVer;
	ary64[bootAddr+0x03] = this->productVer;
	ary64[bootAddr+0x04] = this->ispMainVer;
	ary64[bootAddr+0x05] = this->ispMinorVer;
	ary64[bootAddr+0x06] = this->reg130;

	ary64[bootAddr+0x07] = this->regWordModeCtrl;

	Utility::toArray( this->bootBlock,        &ary64[bootAddr+8] );
	Utility::toArray( this->bootBlock_Backup, &ary64[bootAddr+12] );

	ary64[bootAddr+16] = this->ispABSstartPage;

	ary64[bootAddr+17] = this->max_LB>>8;
	ary64[bootAddr+18] = this->max_LB&0xFF;

	ary64[bootAddr+19] = this->max_SLC_page;
	ary64[bootAddr+20] = this->max_plane;
	ary64[bootAddr+21] = this->max_MLC_page;

	ary64[bootAddr+22] = this->ispBackEndInitABSPage;
	ary64[bootAddr+23] = this->ispRWModuleABSPage;

	memcpy(ary64+bootAddr+24, this->info_block, 6);

	ary64[bootAddr+30]  = this->frontEndConfig1;
	ary64[bootAddr+31]  = this->frontEndConfig2;
	ary64[bootAddr+32]  = this->frontEndConfig3;
	ary64[bootAddr+33]  = this->reg145;
	ary64[bootAddr+34]  = this->reg20E;
	ary64[bootAddr+35]  = this->reg20F;
	ary64[bootAddr+36]  = this->HighPowerDevicemA;
	ary64[bootAddr+37]  = this->ispSecureABSPage;

	//move to init code 
	//ary64[bootAddr+38]  = this->maxLbTableABS;
	//_ULONGtoArray( this->bhBoundary, &ary64[bootAddr+39] );
	//ary64[bootAddr+43]  = this->bhLbaMask;

	memcpy(ary64+0x30, reserveSpareBlock, sizeof(reserveSpareBlock) );
}

void BootSec::load(BYTE* array){
	int offset = 4;
	this->reg130 =  array[offset+6];
	this->reg145 =  array[offset+33];
}

string BootSec::toString(){
	string msg;
	msg+= "Reg130=0x" + Utility::toHexString(this->reg130) + Utility::CrLf();
	msg+= "Reg145=0x" + Utility::toHexString(this->reg145) + Utility::CrLf();
	return msg;
}
