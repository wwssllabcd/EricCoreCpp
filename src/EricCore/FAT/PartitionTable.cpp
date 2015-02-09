#include "StdAfx.h"
#include ".\partitiontable.h"
#include <exception>

#include "..\Utility\Utility.h"

PartitionTable::PartitionTable(void)
:isActive(false)
,headStart(0)
,cylinderStart(0)
,sectorStart(0)
,partitionType(0)
,headEnd(0)
,sectorEnd(0)
,cylinderEnd(0)
,relativeSector(0)
,totalSec(0)
{
}

PartitionTable::~PartitionTable(void)
{
}

void PartitionTable::toArray(BYTE* array){
	if(this->isActive){
		array[0] = 0x80;
	}else{
		array[0] = 0x00;
	}
	array[1] = this->headStart;
	array[2] = this->sectorStart;
	array[2] = array[2] | ( (this->cylinderStart>>8)<<6 );
	array[3] = this->cylinderStart&0xFF;
	array[4] = this->partitionType;
	array[5] = this->headEnd;
	array[6] = this->sectorEnd;
	array[6] = array[6] | ( (this->cylinderEnd>>8)<<6 );
	array[7] = this->cylinderEnd&0xFF;

	Utility::toArray(this->relativeSector, array+8, false);
	Utility::toArray(this->totalSec, array+12, false);
}

void PartitionTable::load(const UCHAR* partitionTable){

	//���o�Ұʪ��A
	switch(partitionTable[0]){
	 case 0x00:
		 this->isActive = false;
		 break; 
	 case 0x80:
		 this->isActive = true;
		 break; 
	 default:
		 throw exception("Invaild Partition Table");
		 break; 
	}


	//�}�l���Y
	this->headStart = partitionTable[1];

	//�P00111111(b)��and���ʧ@�A�i�H��̰��줸�����B���A�N�o��sectorStart
	this->sectorStart = partitionTable[2] & 0x3F;

	//��cylinderStart�A�n��lowbyte���줸���Ӹ�(�аѾ\����MBR���y)
	this->cylinderStart = partitionTable[3] + (partitionTable[2]>>6)*0x100;

	//���Ϋ��A
	this->partitionType = partitionTable[4];
	//�������Y
	this->headEnd = partitionTable[5];

	//�P00111111(b)��and���ʧ@�A�i�H��̰��줸�����B���A�N�o��sectorEnd
	this->sectorEnd = partitionTable[6] & 0x3F;

	//��cylinderEnd�A�n��lowbyte���줸���Ӹ�(�аѾ\����MBR���y)
	this->cylinderEnd = partitionTable[7] + (partitionTable[6]>>6)*0x100;
		
	//���ΰϬ۹��m
	//������partitionTable+8�A�A���ptr�૬�qUCHAR*�নULONG*�A�A��*�Ÿ�����
	this->relativeSector = *(ULONG*)(partitionTable+8);
	//�ϰϼ�
	this->totalSec = *(ULONG*)(partitionTable+12);

}

void PartitionTable::loadNewPartition(int c, int h, int s){
	int totalPhySec = c*h*s;
	int relativeSec = s;

	this->totalSec = totalPhySec - relativeSec;
	this->relativeSector = relativeSec;
	this->headStart =  ( relativeSec% (h*s) )/s;
	this->sectorStart = (relativeSec%s)+1;
	this->cylinderStart = ( relativeSec/(h*s) );
	this->headEnd = ( (relativeSec+totalSec-1) % (h*s) ) / s;
	this->sectorEnd =  ( (relativeSec+totalSec-1)%s )+1;
	this->cylinderEnd = (relativeSec+totalSec-1)/(h*s);
}

//int PartitionTable::_getSecPerTrack(int sizeMB){
//	int result(0);
//	if( sizeMB<=2 ){
//		result = 16;
//	}else if(sizeMB<=256){
//		result = 32;
//	}else if(sizeMB<=2048){
//		result = 64;
//	}else{
//		result = 64;
//	}
//	return result;
//}

