#include "StdAfx.h"
#include ".\bpb.h"

#include "..\Utility\Utility.h"

Bpb::Bpb(void)
:bytsPerSec(0)
,secPerClu(0)
,rsvdSecCnt(0)
,numFATs(0)
,rootEntCnt(0)
,totSec16(0)
,media(0)
,fatSz16(0)
,secPerTrk(0)
,numHeads(0)
,hiddSec(0)
,totSec32(0)
,drvNum(0)
,reserved1(0)
,bootSig(0)
{
}

Bpb::~Bpb(void)
{
}

void Bpb::load(const UCHAR* bootSecBuf){

	this->bytsPerSec = *(USHORT*)(bootSecBuf+11);
	this->secPerClu = *(bootSecBuf+13);
	this->rsvdSecCnt = *(USHORT*)(bootSecBuf+14);
	this->numFATs =  *(bootSecBuf+16);
	this->rootEntCnt = *(USHORT*)(bootSecBuf+17);
	this->totSec16 = *(USHORT*)(bootSecBuf+19);
	this->media = *(bootSecBuf+21);
	this->fatSz16 = *(USHORT*)(bootSecBuf+22);
	this->secPerTrk = *(USHORT*)(bootSecBuf+24);
	this->numHeads = *(USHORT*)(bootSecBuf+26);
	this->hiddSec = *(ULONG*)(bootSecBuf+28);
	this->totSec32 = *(ULONG*)(bootSecBuf+32);
	
	//this->fatSz32 = *(USHORT*)(bootSecBuf+11);
	//this->extFlags = *(USHORT*)(bootSecBuf+11); 
	//this->fsVer = *(USHORT*)(bootSecBuf+11);
	//this->rootClus = *(USHORT*)(bootSecBuf+11);
	//this->fsInfo = *(USHORT*)(bootSecBuf+11);
	//this->bkBootSec = *(USHORT*)(bootSecBuf+11);
	
	this->drvNum = *(bootSecBuf+36);
	this->bootSig = *(bootSecBuf+38);
	this->volID = Utility::arrayToCString(bootSecBuf+39,4);
	this->volLab = Utility::arrayToCString(bootSecBuf+43,11);
	this->filSysType = Utility::arrayToCString(bootSecBuf+54,8);
}

CString Bpb::toString(){
	CString msg;
	msg+= "bpb.bootSig =" + Utility::toString(this->bootSig) + Utility::CrLf();
	msg+= "bpb.bytsPerSec =" + Utility::toString(this->bytsPerSec) + Utility::CrLf();
	msg+= "bpb.drvNum =" + Utility::toString(this->drvNum) + Utility::CrLf();
	msg+= "bpb.fatSz16 =" + Utility::toString(this->fatSz16) + Utility::CrLf();
	msg+= "bpb.filSysType =" + Utility::toString(this->filSysType) + Utility::CrLf();
	msg+= "bpb.hiddSec =" + Utility::toString(this->hiddSec) + Utility::CrLf();
	msg+= "bpb.media =" + Utility::toString(this->media) + Utility::CrLf();
	msg+= "bpb.numFATs =" + Utility::toString(this->numFATs) + Utility::CrLf();
	msg+= "bpb.numHeads =" + Utility::toString(this->numHeads) + Utility::CrLf();
	msg+= "bpb.reserved1 =" + Utility::toString(this->reserved1) + Utility::CrLf();

	msg+= "bpb.rootEntCnt =" + Utility::toString(this->rootEntCnt) + Utility::CrLf();
	msg+= "bpb.rsvdSecCnt =" + Utility::toString(this->rsvdSecCnt) + Utility::CrLf();
	msg+= "bpb.secPerClu =" + Utility::toString(this->secPerClu) + Utility::CrLf();
	msg+= "bpb.secPerTrk =" + Utility::toString(this->secPerTrk) + Utility::CrLf();
	msg+= "bpb.totSec16 =" + Utility::toString(this->totSec16) + Utility::CrLf();
	msg+= "bpb.totSec32 =" + Utility::toString(this->totSec32)+ Utility::CrLf();
	msg+= "bpb.volID =" + Utility::toString(this->volID) + Utility::CrLf();
	msg+= "bpb.volLab " + Utility::toString(this->volLab) + Utility::CrLf();
	return msg;
	
}


