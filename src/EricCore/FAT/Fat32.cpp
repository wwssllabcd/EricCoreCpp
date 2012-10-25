#include "StdAfx.h"
#include ".\fat32.h"

#include <exception>
#include "..\Utility\Utility.h"

Fat32::Fat32(void)
:fatSz32(0)
,extFlags(0)
,fsVer(0)
,rootClus(0)
,FSInfo(0)
,BkBootSec(0)
{
}

Fat32::~Fat32(void)
{
}


void Fat32::toAry(BYTE* array){
	unsigned char data[512] = {
		0xEB, 0x58, 0x90, 0x4D, 0x53, 0x57, 0x49, 0x4E, 0x34, 0x2E, 0x31, 0x00, 0x02, 0x08, 0x20, 0x00, 
			0x02, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x3F, 0x00, 0xFF, 0x00, 0x3F, 0x00, 0x00, 0x00, 
			0x5E, 0x6F, 0xF2, 0x00, 0x8D, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 
			0x01, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x80, 0x00, 0x29, 0xF5, 0x17, 0x1C, 0x1E, 0x4E, 0x4F, 0x20, 0x4E, 0x41, 0x4D, 0x45, 0x20, 0x20, 
			0x20, 0x20, 0x46, 0x41, 0x54, 0x33, 0x32, 0x20, 0x20, 0x20, 0xFA, 0x33, 0xC9, 0x8E, 0xD1, 0xBC, 
			0xF8, 0x7B, 0x8E, 0xC1, 0xBD, 0x78, 0x00, 0xC5, 0x76, 0x00, 0x1E, 0x56, 0x16, 0x55, 0xBF, 0x22, 
			0x05, 0x89, 0x7E, 0x00, 0x89, 0x4E, 0x02, 0xB1, 0x0B, 0xFC, 0xF3, 0xA4, 0x8E, 0xD9, 0xBD, 0x00, 
			0x7C, 0xC6, 0x45, 0xFE, 0x0F, 0x8B, 0x46, 0x18, 0x88, 0x45, 0xF9, 0x38, 0x4E, 0x40, 0x7D, 0x25, 
			0x8B, 0xC1, 0x99, 0xBB, 0x00, 0x07, 0xE8, 0x97, 0x00, 0x72, 0x1A, 0x83, 0xEB, 0x3A, 0x66, 0xA1, 
			0x1C, 0x7C, 0x66, 0x3B, 0x07, 0x8A, 0x57, 0xFC, 0x75, 0x06, 0x80, 0xCA, 0x02, 0x88, 0x56, 0x02, 
			0x80, 0xC3, 0x10, 0x73, 0xED, 0xBF, 0x02, 0x00, 0x83, 0x7E, 0x16, 0x00, 0x75, 0x45, 0x8B, 0x46, 
			0x1C, 0x8B, 0x56, 0x1E, 0xB9, 0x03, 0x00, 0x49, 0x40, 0x75, 0x01, 0x42, 0xBB, 0x00, 0x7E, 0xE8, 
			0x5F, 0x00, 0x73, 0x26, 0xB0, 0xF8, 0x4F, 0x74, 0x1D, 0x8B, 0x46, 0x32, 0x33, 0xD2, 0xB9, 0x03, 
			0x00, 0x3B, 0xC8, 0x77, 0x1E, 0x8B, 0x76, 0x0E, 0x3B, 0xCE, 0x73, 0x17, 0x2B, 0xF1, 0x03, 0x46, 
			0x1C, 0x13, 0x56, 0x1E, 0xEB, 0xD1, 0x73, 0x0B, 0xEB, 0x27, 0x83, 0x7E, 0x2A, 0x00, 0x77, 0x03, 
			0xE9, 0xFD, 0x02, 0xBE, 0x7E, 0x7D, 0xAC, 0x98, 0x03, 0xF0, 0xAC, 0x84, 0xC0, 0x74, 0x17, 0x3C, 
			0xFF, 0x74, 0x09, 0xB4, 0x0E, 0xBB, 0x07, 0x00, 0xCD, 0x10, 0xEB, 0xEE, 0xBE, 0x81, 0x7D, 0xEB, 
			0xE5, 0xBE, 0x7F, 0x7D, 0xEB, 0xE0, 0x98, 0xCD, 0x16, 0x5E, 0x1F, 0x66, 0x8F, 0x04, 0xCD, 0x19, 
			0x41, 0x56, 0x66, 0x6A, 0x00, 0x52, 0x50, 0x06, 0x53, 0x6A, 0x01, 0x6A, 0x10, 0x8B, 0xF4, 0x60, 
			0x80, 0x7E, 0x02, 0x0E, 0x75, 0x04, 0xB4, 0x42, 0xEB, 0x1D, 0x91, 0x92, 0x33, 0xD2, 0xF7, 0x76, 
			0x18, 0x91, 0xF7, 0x76, 0x18, 0x42, 0x87, 0xCA, 0xF7, 0x76, 0x1A, 0x8A, 0xF2, 0x8A, 0xE8, 0xC0, 
			0xCC, 0x02, 0x0A, 0xCC, 0xB8, 0x01, 0x02, 0x8A, 0x56, 0x40, 0xCD, 0x13, 0x61, 0x8D, 0x64, 0x10, 
			0x5E, 0x72, 0x0A, 0x40, 0x75, 0x01, 0x42, 0x03, 0x5E, 0x0B, 0x49, 0x75, 0xB4, 0xC3, 0x03, 0x18, 
			0x01, 0x27, 0x0D, 0x0A, 0x49, 0x6E, 0x76, 0x61, 0x6C, 0x69, 0x64, 0x20, 0x73, 0x79, 0x73, 0x74, 
			0x65, 0x6D, 0x20, 0x64, 0x69, 0x73, 0x6B, 0xFF, 0x0D, 0x0A, 0x44, 0x69, 0x73, 0x6B, 0x20, 0x49, 
			0x2F, 0x4F, 0x20, 0x65, 0x72, 0x72, 0x6F, 0x72, 0xFF, 0x0D, 0x0A, 0x52, 0x65, 0x70, 0x6C, 0x61, 
			0x63, 0x65, 0x20, 0x74, 0x68, 0x65, 0x20, 0x64, 0x69, 0x73, 0x6B, 0x2C, 0x20, 0x61, 0x6E, 0x64, 
			0x20, 0x74, 0x68, 0x65, 0x6E, 0x20, 0x70, 0x72, 0x65, 0x73, 0x73, 0x20, 0x61, 0x6E, 0x79, 0x20, 
			0x6B, 0x65, 0x79, 0x0D, 0x0A, 0x00, 0x00, 0x00, 0x49, 0x4F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
			0x53, 0x59, 0x53, 0x4D, 0x53, 0x44, 0x4F, 0x53, 0x20, 0x20, 0x20, 0x53, 0x59, 0x53, 0x7E, 0x01, 
			0x00, 0x57, 0x49, 0x4E, 0x42, 0x4F, 0x4F, 0x54, 0x20, 0x53, 0x59, 0x53, 0x00, 0x00, 0x55, 0xAA
	};

	


	memcpy(array,data, 512);

	Utility::toArray(this->jmpBoot, array, false);
	Utility::CStringToArray(array+3,this->OEMName,8);
	Utility::toArray(this->bpb_BytesPerSec, array+11, false);
	array[13] = this->bpb_SecPerClu;
	Utility::toArray(this->bpb_RsvdSecCnt, array+0x0E, false);
	array[0x10] = this->bpb_NumFats;
	Utility::toArray(this->bpb_RootEntCnt, array+0x11, false);
	Utility::toArray(this->bpb_TotSec16, array+0x13, false);
	array[0x15] = this->bpb_Media;
	Utility::toArray(this->bpb_FatSz16, array+0x16, false);
	Utility::toArray(this->bpb_SecPerTrk, array+0x18, false);
	Utility::toArray(this->bpb_NumHeads, array+0x1A, false);
	Utility::toArray(this->bpb_HiddSec, array+0x1C, false);
	Utility::toArray(this->bpb_TotSec32, array+0x20, false);

	//FAT 32 差異從此開始
	Utility::toArray(this->fatSz32, array+0x24, false);
	Utility::toArray(this->extFlags, array+0x28, false);
	Utility::toArray(this->fsVer, array+0x2A, false);
	Utility::toArray(this->rootClus, array+0x2C, false);
	Utility::toArray(this->FSInfo, array+0x30, false);
	Utility::toArray(this->BkBootSec, array+0x32, false);

	array[0x40] = this->drvNum;
	array[0x41] = 0;//reserve
	array[0x42] = this->bootSig;

	//volID為4個BYTE ，並不是String
	//Utility::CStringToArray(array+0x43, this->volID, 4);

	Utility::CStringToArray(array+0x47, this->volLab, 11);
	Utility::CStringToArray(array+0x52, this->filSysType, 8);

}


void Fat32::load(ULONG totalSec, int sizeMB,int hiddenSec){
	int h,s;
	_getHS(h,s,sizeMB);

	this->jmpBoot = 0x009058EB;
	this->OEMName = "MSWIN4.1";
	this->bpb_BytesPerSec = 512;
	this->bpb_SecPerClu = _getSecPerCluster(sizeMB);
	this->bpb_RsvdSecCnt  = 32;
	this->bpb_NumFats  = 2;
	this->bpb_RootEntCnt  = 0;
	this->bpb_TotSec16  = 0;

	this->bpb_Media  = 0xF8;
	this->bpb_FatSz16  = 0;

	this->bpb_SecPerTrk  = s;
	this->bpb_NumHeads  = h;

	this->bpb_HiddSec  = hiddenSec;
	this->bpb_TotSec32  = totalSec;

	this->drvNum  = 0x80;
	this->bootSig  = 0x29;

	//volID為4個BYTE ，並不是String
	//this->volID  = "MP FAT 32";
	this->volLab  = "NO NAME";
	this->filSysType  = "FAT32";

	this->fatSz32 = getSecPerFat(totalSec);
	this->extFlags = 0x00;
	this->fsVer = 0x0000;
	this->rootClus = 2;
	this->FSInfo = 1;
	this->BkBootSec = 6;
}

int Fat32::_getSecPerCluster(int fat32_SizeMB){
	int result(0);
	if( fat32_SizeMB<=512 ){
		result = 2;
	}else if(fat32_SizeMB<=1024){
		result = 4;
	}else if(fat32_SizeMB<=8192){
		result = 8;
	}else if(fat32_SizeMB<=16384){
		result = 16;
	}else if(fat32_SizeMB<=32768){
		result = 32;
	}else{
		CString msg;
		msg.Format("FAT 32 can not support that the card size = %d", fat32_SizeMB);
		throw exception(msg);
	}
	return result;
}

int Fat32::getSecPerFat(int ts){
	int rootDirSectors;
	int tempVal1,tempVal2;
	int fatSz;
	int DskSize = ts;

	rootDirSectors = ((this->bpb_RootEntCnt*32)+(this->bpb_BytesPerSec-1))/this->bpb_BytesPerSec;
	tempVal1 = DskSize - (this->bpb_RsvdSecCnt + rootDirSectors );
	tempVal2 = (256* this->bpb_SecPerClu ) + this->bpb_NumFats;

	tempVal2 = tempVal2/2;
	fatSz = (tempVal1+ (tempVal2-1))/tempVal2;
	return fatSz;
}


void Fat32::_getHS( int& h, int& s, int sizeMB){
	if( sizeMB<=128 ){
		h = 0x20;
		s = 0x3F;
	}else if(sizeMB<=256){
		h = 0x20;
		s = 0x3F;
	}else if(sizeMB<=512){
		h = 0x20;
		s = 0x3F;
	}else if(sizeMB<=1024){
		h = 0x40;
		s = 0x3F;
	}else if(sizeMB<=2048){
		h = 0x80;
		s = 0x3F;
	}else if(sizeMB<=4096){
		h = 0x80;
		s = 0x3F;
	}else if(sizeMB<=32768){
		h = 0xFF;
		s = 0x3F;
	}else{
		throw exception("FAT32 Error : Can`t get header and sector");
	}
}
