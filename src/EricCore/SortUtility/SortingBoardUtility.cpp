#include "StdAfx.h"
#include ".\sortingboardutility.h"

using namespace EricCore;


SortingBoardUtility::SortingBoardUtility(void)

{
	int i=0;
	for(i=0; i<MAX_CE_NO; i++){
		memset( _defectTBs[i], 0x00, sizeof(_defectTBs[i]));
	}

	memset(_tempDT, 0, sizeof(_tempDT));
	memset(sortRecordAddr, 0, sizeof(sortRecordAddr));
}

SortingBoardUtility::~SortingBoardUtility(void)
{
}


//bool SortingBoardUtility::checkRepeat64(BYTE* ary)
//{
//	BYTE ary64[64];
//	memcpy(ary64,ary,64);
//	int i;
//	for( i=0; i<32; i++){
//		int result = memcmp(ary, ary64, 64);
//		if(result!=0){
//			return false;
//		}
//	}
//	return true;
//}
//
//bool SortingBoardUtility::checkSignature(BYTE* ary)
//{
//	BYTE sig[16]={'3', 'S', ' ', '2', '0', '9', '0', ' ', 'S', 'O', 'R', 'T', 'I', 'N', 'G', ' '};
//	int result = memcmp(ary, sig, 16);
//	if(result!=0){
//		return false;
//	}else{
//		return true;
//	}
//}





void SortingBoardUtility::_loadRecord_IM34_less8192(const UsbCommand& usbCmd, const Flash& f, ULONG defectPageAddr, int recordStartPtr, int recordLen)
{
	BYTE ary_ce0_ce2[9216];
	BYTE ary_ce1_ce3[9216];
	BYTE* pDataTable=0;

	usbCmd.read8K(defectPageAddr,   ary_ce0_ce2);
	usbCmd.read8K(defectPageAddr+1, ary_ce1_ce3);

	int ce_offset=0;

	BYTE bitMap[DEFECT_TABLE_SIZE];
	for(int ceNo=0; ceNo<f.totalCE(); ceNo++){
		memset(bitMap, 0, DEFECT_TABLE_SIZE);

		ce_offset = recordStartPtr+ceNo*recordLen;

		switch(ceNo){
			case 0:
				ce_offset = recordStartPtr;	
				pDataTable = ary_ce0_ce2;
				break;
			case 1:
				ce_offset = recordStartPtr;	
				pDataTable = ary_ce1_ce3;
				break;
			case 2:
				ce_offset = recordStartPtr+recordLen;	
				pDataTable = ary_ce0_ce2;
				break;
			case 3:
				ce_offset = recordStartPtr+recordLen;	
				pDataTable = ary_ce1_ce3;
				break;
		}



		_combineDefectTb(bitMap, pDataTable+ce_offset, f.evenDieStart, f.evenDieEnd-1);
		
		if(f.diePerChip()==2){
			_combineDefectTb(bitMap, pDataTable+ce_offset, f.oddDieStart, f.oddDieEnd-1);
		}
		setDefect(ceNo, bitMap);
	}
}


void SortingBoardUtility::loadDefectFromSortingBlock(const UsbCommand& usbCmd, const Flash& f, SortBase& sb){
	if( sb.sortTbVer == 3){
		loadDefectFromSortingBlock_V3(usbCmd, f, sb);
	}else{
		loadDefectFromSortingBlock_V1V2(usbCmd, f, sb);
	}
}


void SortingBoardUtility::loadDefectFromSortingBlock_V1V2(const UsbCommand& usbCmd, const Flash& f, SortBase& sb)
{
	int dieCnt = f.diePerChip();
	if(f.is_IM_34nmD2==true){
		if(dieCnt==2){
			_loadRecord_IM34_over8192(usbCmd, f, sb.sortTbAddr);
		}else{
			//IM less 8192 block/CE
			_loadRecord(usbCmd, f, sb.sortTbAddr, 2048, 1024);
		}
	}else{
		_loadRecord(usbCmd, f, sb.sortTbAddr, 2048, 1024);
	}
}

void SortingBoardUtility::loadDefectFromSortingBlock_V3(const UsbCommand& usbCmd, const Flash& f, SortBase& sb){
	
	int ceCnt = f.totalCE();
	int ce_offset=0x800;

	BYTE bitMap[DEFECT_TABLE_SIZE];
	memset(bitMap, 0, DEFECT_TABLE_SIZE);
	
	_combineDefectTb(bitMap, sb.sortingTb+ce_offset, f.evenDieStart, f.evenDieEnd-1);
	if(f.diePerChip()==2){
		_combineDefectTb(bitMap, sb.sortingTb+ce_offset, f.oddDieStart, f.oddDieEnd-1);
	}


	setUse(sb.sortTbAddr/ f.blockLen, bitMap);
	setDefect(0, bitMap);


	int logicCeNo=1;
	for(int ceNo=1; ceNo<16; ceNo++){
		if( logicCeNo>=ceCnt){
			break;
		}
		if( sb.findSortingPage(usbCmd, ceNo) == true){
			memset(bitMap, 0, DEFECT_TABLE_SIZE);
			_combineDefectTb(bitMap, sb.sortingTb+ce_offset, f.evenDieStart, f.evenDieEnd-1);
			if(f.diePerChip()==2){
				_combineDefectTb(bitMap, sb.sortingTb+ce_offset, f.oddDieStart, f.oddDieEnd-1);
			}

			setUse( (sb.sortTbAddr&0x00FFFFFF)/f.blockLen, bitMap);
			setDefect(logicCeNo, bitMap);
			logicCeNo++;
		}
		
	}

}

void SortingBoardUtility::setUse(ULONG blockNo, BYTE* sortTb){
	int idx = blockNo>>3;
	BYTE mask = 1 << (blockNo % 8);
	sortTb[idx] &= (~mask);
}


void SortingBoardUtility::_loadRecord(const UsbCommand& usbCmd, const Flash& f, ULONG defectPageAddr, int recordStartPtr, int recordLen)
{
	BYTE dt[9216];
	usbCmd.read8K(defectPageAddr, dt);
	int ce_offset=0;

	BYTE bitMap[DEFECT_TABLE_SIZE];
	for(int ceNo=0; ceNo<f.totalCE(); ceNo++){
		
		memset(bitMap, 0, DEFECT_TABLE_SIZE);
		ce_offset = recordStartPtr+ceNo*recordLen;
		_combineDefectTb(bitMap, dt+ce_offset, f.evenDieStart, f.evenDieEnd-1);
		
		if(f.diePerChip()==2){
			_combineDefectTb(bitMap, dt+ce_offset, f.oddDieStart, f.oddDieEnd-1);
		}
		setDefect(ceNo, bitMap);
	}
}




// 43nmD2 需傳0 ~ 2083，不是2084
void SortingBoardUtility::_combineDefectTb(BYTE* bitMap, BYTE* recordBuf, int startAddr, int endAddr)
{
	int len = endAddr - startAddr+1;
	int bitMapLen = len/8;
	int ptr = startAddr/8;
	memcpy( bitMap+ptr, recordBuf+ptr, bitMapLen);

	if(len%8){
		//it nedd to adjust last record
		int mask = _adjustValue(endAddr);
		bitMap[ ptr+bitMapLen ] = recordBuf[ptr+bitMapLen] & mask;
	}
}

void SortingBoardUtility::_loadRecord_IM34_over8192(const UsbCommand& usbCmd, const Flash& f, ULONG defectPageAddr)
{
	BYTE dt_1[9216];
	BYTE dt_2[9216];
	usbCmd.read8K(defectPageAddr, dt_1);
	usbCmd.read8K(defectPageAddr+1, dt_2);

	BYTE bitMap[DEFECT_TABLE_SIZE];
	memset(bitMap, 0, DEFECT_TABLE_SIZE);	

	_combineDefectTb(bitMap, dt_1+2048, f.evenDieStart, f.evenDieEnd-1);
	if(f.diePerChip()==2){
		_combineDefectTb(bitMap, dt_1+2048, f.oddDieStart, f.oddDieEnd-1);
	}
	setDefect(0, bitMap);

	if( f.totalCE() ==2){
		_combineDefectTb(bitMap, dt_2+2048, f.evenDieStart, f.evenDieEnd-1);
		if(f.diePerChip()==2){
			_combineDefectTb(bitMap, dt_2+2048, f.oddDieStart, f.oddDieEnd-1);
		}
		setDefect(1, bitMap);
	}
}



BYTE SortingBoardUtility::_adjustValue(int endPtr){
	endPtr+=1;

	//BYTE result=0;
	int modValue = endPtr%8;

	switch(modValue){
		case 0:
			return 0x00;
		case 1:
			return 0x01;
		case 2:
			return 0x03;
		case 3:
			return 0x07;
		case 4:
			return 0x0F;
		case 5:
			return 0x1F;
		case 6:
			return 0x3F;
		case 7:
			return 0x7F;
		default:
			throw MyException(SBU_ADJUST_VALUE_FAIL, "_adjustValue fail");
	}
}

//bool SortingBoardUtility::findSortingPage(const UsbCommand& usbCmd, int ceNo, ULONG& addr, int pagePerBlock, BYTE* aryCfg2048 )
//{
//	BYTE buf[9216];
//	int pbStart=0;
//	int pbEnd=0;
//	int repeat=0;
//	ULONG cycleList=0;
//
//	for(repeat=0; repeat<5; repeat++){
//		if(repeat==0){pbStart=0    ; pbEnd=0x40;}
//		if(repeat==1){pbStart=0x80 ; pbEnd=0xC0;}
//		if(repeat==2){pbStart=0x100; pbEnd=0x140;}
//		if(repeat==3){pbStart=0x200; pbEnd=0x240;}
//		if(repeat==4){pbStart=0x400; pbEnd=0x440;}
//
//		for(pbStart; pbStart<pbEnd; pbStart++){
//			addr = pbStart*pagePerBlock;
//			cycleList = (ceNo<<24)|addr;
//			usbCmd.read8K( cycleList, buf);
//
//			if( checkSignature(buf)==false){
//				addr+=1;
//				usbCmd.read8K(cycleList, buf);
//				if( checkSignature(buf)==false){
//					addr+=2;
//					usbCmd.read8K(cycleList, buf);
//					if( checkSignature(buf)==false){
//						continue;
//					}
//				}
//			}
//
//			     
//			if( checkRepeat64(buf) ==true){
//				addr = cycleList;
//				memcpy(aryCfg2048, buf, 2048);
//				return true;
//			}
//		}
//	}
//
//	return false;
//}


void SortingBoardUtility::setDefect(int ceNo, BYTE* aryDefect){
	memcpy( _defectTBs[ceNo], aryDefect, DEFECT_TABLE_SIZE);
}

void SortingBoardUtility::getDefect(int ceNo, BYTE* aryDefect){
	memcpy( aryDefect, _defectTBs[ceNo], DEFECT_TABLE_SIZE);
}

void SortingBoardUtility::getPartial_Retry(BYTE* arySbCfg, BYTE& enablePartial, BYTE& enableRetry){

	//auto select full/partial page
	BYTE fwVer = arySbCfg[0x12];
	BYTE binRange = arySbCfg[0x13];

	Utility::sendMsg("Auto Select Partialpage, param=%X, %X", fwVer, binRange);

	enablePartial=0;

	if( fwVer==0x41){
		enablePartial=0;
	}

	// 1/2 page
	if( fwVer==0x42 || fwVer==2 ){
		enablePartial=0;
	}

	// 1/2 page
	if(fwVer==0x81){
		enablePartial=1;
	}

	// 1/2 page
	if( fwVer==0x82){
		enablePartial=1;
	}

	// 1/2 page
	if(fwVer==0xC1){
		enablePartial=1;
		enableRetry=1;
	}

	// 1/4 page
	if(fwVer==0xC2){
		enablePartial=2;
	}
	
	Utility::sendMsg("enablePartialPage(Sorting Table) = %d", enablePartial);
	Utility::sendMsg("enableRetryWrite(Sorting Table) = %d", enableRetry);
}

void SortingBoardUtility::loadMultiRecord(const UsbCommand& usbCmd, const Flash& f)
{

	//BYTE arySbCfg[SORTBASE_READ_8k];
	SortBase sb;
	for(int ceNo=0; ceNo<f.totalCE(); ceNo++){
		sortRecordAddr[ceNo]=0xFFFFFFFF;
		if( sb.findSortingPage(usbCmd, ceNo) == true){
			_loadRecord_OneCe(usbCmd, f, sb.sortTbAddr);

			//整理好的Defect Table都放到_defectTBs中(只含defect information)
			memcpy( _defectTBs[ceNo], _tempDT, sizeof(_tempDT));
			sortRecordAddr[ceNo]=sb.sortTbAddr;
		}
	}
}

void SortingBoardUtility::_loadRecord_OneCe(const UsbCommand& usbCmd, const Flash& f, ULONG defectPageAddr)
{
	BYTE dt[9216];
	usbCmd.read8K(defectPageAddr, dt);
	//int ce_offset=0;
	int recordLen = 1024;
	BYTE bitMap[DEFECT_TABLE_SIZE];
	int ceCnt = _getCeCntPerPackage( dt[0x981] );

	memset( _tempDT, 0x00, sizeof(_tempDT) );

	for(int ceNo=0; ceNo<ceCnt; ceNo++){
		memset(bitMap, 0x00, DEFECT_TABLE_SIZE);
		_makeOneCeTabele(bitMap, dt+_getRecordOffset(f)+ recordLen*ceNo ,f);

		memcpy( _tempDT+2048*ceNo, bitMap, 2048);
	}
}

int SortingBoardUtility::_getCeCntPerPackage(BYTE idCheckFlag){
	idCheckFlag = idCheckFlag& 0x0F;
	if(idCheckFlag==1){return 1;}
	if(idCheckFlag==3){return 2;}
	if(idCheckFlag==7){return 3;}
	if(idCheckFlag==15){return 4;}

	return 0;
}

//從recordTb的位置，開始抓1024個Byte ，在根據幾個die組合其BitTable(都要傳齊頭位置進去)
void SortingBoardUtility::_makeOneCeTabele(BYTE* bitMap, BYTE* recordTb, const Flash& f){
	_combineDefectTb(bitMap, recordTb, f.evenDieStart, f.evenDieEnd-1);
	if( f.diePerChip()==2){
		_combineDefectTb(bitMap, recordTb, f.oddDieStart, f.oddDieEnd-1);
	}
}

int SortingBoardUtility::_getRecordOffset(const Flash& f){
	return 2048;
}


int SortingBoardUtility::getPagePerBlockPartialPage(BYTE partialPage, bool is56D3){
	int pagePerBlock=0;
	if(partialPage==1){
		pagePerBlock=64;
		if( is56D3){
			pagePerBlock=32;
		}
	}

	if(partialPage==2){
		pagePerBlock=32;
		if( is56D3 ){
			pagePerBlock=16;
		}
	}

	return pagePerBlock;
}
