#include "StdAfx.h"
#include ".\sortbase.h"
#include "../Utility/DefineFiles/DefineFile.h"


SortBase::SortBase(void)
:sortTbAddr(0)
,sortTbVer(0)
{
	memset(sortingTb, 0, SORTBASE_READ_8k);
}

SortBase::~SortBase(void)
{
}

void SortBase::modify8000(BYTE* ary8k, const SbOption& opt, BYTE sn, BYTE version){
	WORD offset = 0x0FC0;
	ary8k[offset] = sn;
	ary8k[offset+1] = opt.flashIdCheck;
	ary8k[offset+2] = opt.makerCode;
	ary8k[offset+3] = opt.deviceCode;
	ary8k[offset+4] = opt.testType;
	
	int petternCheckNo = _setPattern(ary8k, opt);

	ary8k[offset+5] = petternCheckNo&0xFF;
	ary8k[offset+6] = (BYTE)(opt.lowEven&0xFF);
	ary8k[offset+7] = (BYTE)(opt.highEven&0xFF);
	ary8k[offset+8] = (BYTE)(opt.lowOdd&0xFF);
	ary8k[offset+9] = (BYTE)(opt.highOdd&0xFF);
	ary8k[offset+10] = (BYTE)(opt.maxErrorBit&0xFF);

	ary8k[offset+11] = (BYTE)(opt.pcNo/0x100);
	ary8k[offset+12] = (BYTE)(opt.pcNo%0x100);
	ary8k[offset+13] = version;

	ary8k[offset+0x1A] = opt.flash_ID3;
	ary8k[offset+0x1B] = opt.flash_ID4;
	ary8k[offset+0x1C] = opt.flash_ID5;
	ary8k[offset+0x1D] = opt.flash_ID6;
	ary8k[offset+0x1E] = opt.flash_ID7;
	ary8k[offset+0x1F] = opt.flash_ID8;


	_setBinValue( ary8k+0xFE0+0, opt.bin1, opt.blockCnt);
	_setBinValue( ary8k+0xFE0+2, opt.bin2, opt.blockCnt);
	_setBinValue( ary8k+0xFE0+4, opt.bin3, opt.blockCnt);
	_setBinValue( ary8k+0xFE0+6, opt.bin4, opt.blockCnt);
	_setBinValue( ary8k+0xFE0+8, opt.bin5, opt.blockCnt);
	_setBinValue( ary8k+0xFE0+10, opt.bin6, opt.blockCnt);
	_setBinValue( ary8k+0xFE0+12, opt.bin7, opt.blockCnt);
	_setBinValue( ary8k+0xFE0+14, opt.bin8, opt.blockCnt);
	_setBinValue( ary8k+0xFE0+16, opt.bin9, opt.blockCnt);
	_setBinValue( ary8k+0xFE0+18, opt.bin10, opt.blockCnt);
	_setBinValue( ary8k+0xFE0+20, opt.bin11, opt.blockCnt);
	_setBinValue( ary8k+0xFE0+22, opt.bin12, opt.blockCnt);
	_setBinValue( ary8k+0xFE0+24, opt.bin13, opt.blockCnt);
	_setBinValue( ary8k+0xFE0+26, opt.bin14, opt.blockCnt);
	_setBinValue( ary8k+0xFE0+28, opt.bin15, opt.blockCnt);
	_setBinValue( ary8k+0xFE0+30, opt.bin16, opt.blockCnt);	

	if(opt.custom_bin1 != NULL_CUSTON_BIN_VALUE){ 
		_setCustomBinValue(ary8k+0xFE0+0x00, opt.custom_bin1 );
	}

	if(opt.custom_bin2 != NULL_CUSTON_BIN_VALUE){ _setCustomBinValue(ary8k+0xFE0+0x02, opt.custom_bin2 );}
	if(opt.custom_bin3 != NULL_CUSTON_BIN_VALUE){ _setCustomBinValue(ary8k+0xFE0+0x04, opt.custom_bin3 );}
	if(opt.custom_bin4 != NULL_CUSTON_BIN_VALUE){ _setCustomBinValue(ary8k+0xFE0+0x06, opt.custom_bin4 );}
	if(opt.custom_bin5 != NULL_CUSTON_BIN_VALUE){ _setCustomBinValue(ary8k+0xFE0+0x08, opt.custom_bin5 );}
	if(opt.custom_bin6 != NULL_CUSTON_BIN_VALUE){ _setCustomBinValue(ary8k+0xFE0+0x0A, opt.custom_bin6 );}
	if(opt.custom_bin7 != NULL_CUSTON_BIN_VALUE){ _setCustomBinValue(ary8k+0xFE0+0x0C, opt.custom_bin7 );}
	if(opt.custom_bin8 != NULL_CUSTON_BIN_VALUE){ _setCustomBinValue(ary8k+0xFE0+0x0E, opt.custom_bin8 );}
	if(opt.custom_bin9 != NULL_CUSTON_BIN_VALUE){ _setCustomBinValue(ary8k+0xFE0+0x10, opt.custom_bin9 );}
	if(opt.custom_bin10 != NULL_CUSTON_BIN_VALUE){ _setCustomBinValue(ary8k+0xFE0+0x12, opt.custom_bin10 );}
	if(opt.custom_bin11 != NULL_CUSTON_BIN_VALUE){ _setCustomBinValue(ary8k+0xFE0+0x14, opt.custom_bin11 );}
	if(opt.custom_bin12 != NULL_CUSTON_BIN_VALUE){ _setCustomBinValue(ary8k+0xFE0+0x16, opt.custom_bin12 );}
	if(opt.custom_bin13 != NULL_CUSTON_BIN_VALUE){ _setCustomBinValue(ary8k+0xFE0+0x18, opt.custom_bin13 );}
	if(opt.custom_bin14 != NULL_CUSTON_BIN_VALUE){ _setCustomBinValue(ary8k+0xFE0+0x1A, opt.custom_bin14 );}
	if(opt.custom_bin15 != NULL_CUSTON_BIN_VALUE){ _setCustomBinValue(ary8k+0xFE0+0x1C, opt.custom_bin15 );}
	if(opt.custom_bin16 != NULL_CUSTON_BIN_VALUE){ _setCustomBinValue(ary8k+0xFE0+0x1E, opt.custom_bin16 );}




	ary8k[offset+4] |= BIT3;
}

void SortBase::_setCustomBinValue(BYTE* ary8k, WORD binValue){
	ary8k[0] = ((BYTE)(binValue>>8))&0xFF;
	ary8k[1] = ((BYTE)(binValue))&0xFF;
}

void SortBase::_setBinValue(BYTE* ary8k, int binValue, int totalBlock){
	ULONG tmp = (totalBlock * binValue)/100;
	ary8k[0] = ((BYTE)(tmp>>8))&0xFF;
	ary8k[1] = ((BYTE)(tmp))&0xFF;
}


int SortBase::_setPattern(BYTE* ary8k, const SbOption& opt){
	int petternCheckNo=0;
	if( _setPattern_1(0, petternCheckNo, opt.pettern0, opt.quickPattern0, ary8k) ==true) {
		petternCheckNo++;
	}

	if( _setPattern_1(1, petternCheckNo, opt.pettern1, opt.quickPattern1, ary8k) ==true) {
		petternCheckNo++;
	}

	if( _setPattern_1(2, petternCheckNo, opt.pettern2, opt.quickPattern2, ary8k) ==true) {
		petternCheckNo++;
	}

	if( _setPattern_1(3, petternCheckNo, opt.pettern3, opt.quickPattern3, ary8k) ==true) {
		petternCheckNo++;
	}

	if( _setPattern_1(4, petternCheckNo, opt.pettern4, opt.quickPattern4, ary8k) ==true) {
		petternCheckNo++;
	}

	if( _setPattern_1(5, petternCheckNo, opt.pettern5, opt.quickPattern5, ary8k) ==true) {
		petternCheckNo++;
	}

	if( _setPattern_1(6, petternCheckNo, opt.pettern6, false, ary8k) ==true) {
		petternCheckNo++;
	}

	return petternCheckNo;
}

bool SortBase::_setPattern_1(int patternNo, int ptnOffset, bool isLong, bool isShort, BYTE* ary8k){
	if( (isLong==true) || (isShort==true) ){
		if(isLong==true){
			ary8k[0xFD0+ptnOffset]= patternNo;
		}
		if( isShort==true){
			ary8k[0xFD0+ptnOffset]= patternNo|0x80;
		}
		return true;
	}
	return false;
}

//
//bool SortBase::findSortingPage(const UsbCommand& usbCmd, int ceNo){
//	BYTE temp[ SORTBASE_READ_8k ];
//	return findSortingPage(usbCmd, ceNo, temp);
//}

bool SortBase::findSortingPage(const UsbCommand& usbCmd, int ceNo){
	BYTE buf[SORTBASE_READ_8k];
	int pbStart=0;
	int pbEnd=0;
	int repeat=0;
	ULONG cycleList=0;
	ULONG addr=0;
	int blockLen = 0x80; // it's always 0x80, no matter D2/D3

	for(repeat=0; repeat<5; repeat++){
		if(repeat==0){pbStart=0    ; pbEnd=0x40;}
		if(repeat==1){pbStart=0x80 ; pbEnd=0xC0;}
		if(repeat==2){pbStart=0x100; pbEnd=0x140;}
		if(repeat==3){pbStart=0x200; pbEnd=0x240;}
		if(repeat==4){pbStart=0x400; pbEnd=0x440;}

		for(pbStart; pbStart<pbEnd; pbStart++){
			addr = pbStart*blockLen;
			cycleList = (ceNo<<24)|addr;
			usbCmd.read8K( cycleList, buf);
			if( checkSignature(buf)==false){
				addr+=1;
				usbCmd.read8K(cycleList, buf);
				if( checkSignature(buf)==false){
					addr+=2;
					usbCmd.read8K(cycleList, buf);
					if( checkSignature(buf)==false){
						continue;
					}
				}
			}
			     
			if( checkRepeat64(buf) ==true){
				this->sortTbAddr = cycleList;
				memcpy(sortingTb, buf, SORTBASE_READ_8k);
				_loadSortingTbParam(sortingTb);
				return true;
			}
		}
	}
	return false;
}

void SortBase::_loadSortingTbParam(BYTE* arySortTb){
	this->sortTbVer = arySortTb[0x12] & 0x0F;
}

bool SortBase::findBootBlock(ULONG& addr, UsbCommand& usbCmd){
	BYTE buf[9216];
	int pbStart=0;
	int pbEnd=0;
	int repeat=0;
	int blockLen=0x80;

	for(repeat=0; repeat<5; repeat++){
		if(repeat==0){pbStart=0    ; pbEnd=0x40;}
		if(repeat==1){pbStart=0x80 ; pbEnd=0xC0;}
		if(repeat==2){pbStart=0x100; pbEnd=0x140;}
		if(repeat==3){pbStart=0x200; pbEnd=0x240;}
		if(repeat==4){pbStart=0x400; pbEnd=0x440;}

		for(pbStart; pbStart<pbEnd; pbStart++){
			addr = pbStart*blockLen;
			usbCmd.read8K(addr, buf);
			if(buf[8192]==0xFC){
				if( checkRepeat64(buf)==true){
					return true;
				}
			}
		}
	}
	return false;
}



bool SortBase::checkRepeat64(BYTE* ary){
	BYTE ary64[64];
	memcpy(ary64,ary,64);
	int i;
	for( i=0; i<32; i++){
		int result = memcmp(ary, ary64, 64);
		if(result!=0){
			return false;
		}
	}
	return true;
}

bool SortBase::checkSignature(BYTE* ary){
	BYTE sig[16]={'3', 'S', ' ', '2', '0', '9', '0', ' ', 'S', 'O', 'R', 'T', 'I', 'N', 'G', ' '};
	int result = memcmp(ary, sig, 16);
	if(result!=0){
		return false;
	}else{
		return true;
	}
}

bool SortBase::checkSignature_98(BYTE* ary){
	BYTE sig[16]={'3', 'S', ' ', '2', '0', '9', '8', ' ', 'S', 'O', 'R', 'T', 'I', 'N', 'G', ' '};
	int result = memcmp(ary, sig, 16);
	if(result!=0){
		return false;
	}else{
		return true;
	}
}

bool SortBase::checkSignature_98_debugMsg(BYTE* ary){
	BYTE sig[16]={'3', 'S', ' ', '2', '0', '9', '8', ' ', 'D', 'E', 'B', 'U', 'G', 'M', 'S', 'G'};
	int result = memcmp(ary, sig, 16);
	if(result!=0){
		return false;
	}else{
		return true;
	}
}



bool SortBase::searchBootBlk(const UsbCommand& usbCmd, ULONG& bootblk, BYTE maxSLCpage)
{
	BYTE searchPgTb[] = {0x00, 0x05, 0x80, 0x85};			// 搜尋時需要檢查的 page number
	BYTE buf[0x2400];										// 資料站存區
	int searchCnt	(0x00);									// 記錄目前每次 start block number 開始的搜尋數量
	//int searchPg	(0x00);									// 目前搜尋的 page address
	//int basePg		(0x00);									// 記錄目前 block number 的 page address 為 bace address

	usbCmd.homogeneousDisable();

	/*
	// boot block 搜尋規則 :
	// search blocks 0, 0x10, 0x20, 0x40, 0x80, 0x100, 0x210, 0x420, 0x840, 0x1080, 0x2000, 0x2010, 0x4020
	// 當 block number 小於     0x20 且大於等於 0x420 時找 page 00h, 05h, 80h, 85h
	// 當 block number 大於等於 0x20 且小於     0x420 時找 page 00h
	*/
	while(bootblk<0x8020) {
		searchCnt=0x00;	// 重置 searchCnt 變數為 0
		while(searchCnt < 0x10) {
			for (int searchPgLen=0; searchPgLen<sizeof(searchPgTb); searchPgLen++) {
				if ( maxSLCpage <= searchPgTb[searchPgLen]) break;		// 避免會掃描到下一個 block


				usbCmd.read8K( blkToCycleList(0, bootblk+searchCnt, searchPgTb[searchPgLen]), buf);

				// check boot record signature
				if (buf[0x00]==0x5A && buf[0x01]==0xA5 && buf[0x02]==0xA5 && buf[0x03]==0x5A) {
					bootblk += searchCnt;
					return true;
				}
				if ((bootblk>=0x20) && (bootblk<0x420)) break;
			}
			searchCnt++;
		}
		if (bootblk== 0x00 || bootblk==0x100 || bootblk==0x2000) bootblk += 0x0008;
		else if (bootblk==0x1080)								 bootblk  = 0x1000;

		bootblk <<= 1;
	}
	bootblk = -1;
	return false;
}


ULONG SortBase::blkToCycleList(int win, const int blk, int page)
{
	int ce=0;
	ULONG cycle(0);

	cycle = (((ULONG)(0|ce))<<24) | ((ULONG)(blk*0x100)) | (page);

	return cycle;
}