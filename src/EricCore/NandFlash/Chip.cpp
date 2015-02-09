#include "stdafx.h"
#include ".\chip.h"
#include "..\Utility\Utility.h"

#include "..\Utility\EricException\MyException.h"


using namespace EricCore;

Chip::Chip(void)
:makerCode(0)
,deviceCode(0)
,idData3(0)
,idData4(0)
,idData5(0)
,blockType( BlockType::null() )
,blockPerChip(0)
,size(0)
,levelCellType( LevelCellType::null() )
,dieNum(0)
,hasInterleave(0)
,hasCacheProgram(0)
,blockSize(0)
,pagePerBlock(0)
,secPerBlock(0)
,secPerPage(0)
,maxPage(0)
,max_C_endPtr(0)
,has2Plane(false)
,pageSize(0)
,nanometer(0)
,flashAddr5Mask(0)
,has2PlaneErase(false)

,dieStart_1st(0)
,dieStart_2st(0)
,dieStart_3st(0)
,dieStart_4st(0)
,dieEnd_1st(0)
,dieEnd_2st(0)
,dieEnd_3st(0)
,dieEnd_4st(0)
,defectMarkColumnOffset(0)
{
	memset(lowPageTable, 0x00, sizeof(lowPageTable));
	memset(orgLowPage, 0x00, sizeof(orgLowPage));
	memset(mlcPageTable, 0x00, sizeof(mlcPageTable));
	
	
	twoPlaneCmd[0] = 0x80;
	twoPlaneCmd[1] = 0x10;
	twoPlaneCmd[2] = 0x80;
	twoPlaneCmd[3] = 0x10;
	twoPlaneCmd[4] = 0x80;
	twoPlaneCmd[5] = 0x10;

	twoPlaneEraseCmd[0]=0x00;
	twoPlaneEraseCmd[1]=0x00;
	twoPlaneEraseCmd[2]=0x00;
}

Chip::~Chip(void)
{
}//------------------------------------------------------------------------------------

//To Get the chip object by Id table of Flash
Chip Chip::getChipById(const BYTE *buf){
	BYTE	mfd = buf[0];//BYTE 1¡G maker code
	BYTE	id  = buf[1];//BYTE 2¡G device code
	BYTE	id3 = buf[2];//BYTE 3¡G 3rd ID Data
	BYTE	id4 = buf[3];//BYTE 4¡G 4th ID Data
	BYTE	id5 = buf[4];//BYTE 5¡G 5th ID Data
	Chip objChip;

	objChip.makerCode = mfd;
	objChip.deviceCode = id;
	objChip.idData3 = id3;
	objChip.idData4 = id4;
	objChip.idData5 = id5;

	_setSpecialFlash(objChip);

	//Get Block Type
	_loadBlockTypeAndSize(objChip.deviceCode,objChip);

	objChip.dieNum = _getDieNum(buf);

	objChip.levelCellType = objChip._getLevelCellType(buf, objChip.blockType );

	////fixed 8LC`s chip array size
	//if( objChip.levelCellType== LevelCellType::_8LC() ){
	//	objChip.size = objChip.size*2;
	//}

	objChip.hasInterleave = this->_getInterLeave(objChip.idData3);

	objChip.hasCacheProgram = this->_getCacheProgram(objChip.idData3);

	//Get PageSize
	objChip.pageSize = _getPageSize(buf);

	//Get Page Per Block
	objChip.pagePerBlock = _getPagePerBlock(objChip.levelCellType,objChip.blockType);

	//Get blockSize (kB)
	objChip.blockSize = _getBlockSize(buf, objChip.blockType,objChip.levelCellType, objChip.pageSize, objChip.pagePerBlock);

	//double check PagePerBlock
	ASSERT( (objChip.pagePerBlock ==  (objChip.blockSize/ objChip.pageSize) ));

	//div 0.5K equal multiply 2
	objChip.secPerBlock = objChip.blockSize*2;

	//calculate secPerPage
	objChip.secPerPage = objChip.secPerBlock / objChip.pagePerBlock;

	//calculate 2 plane Mode 
	objChip.has2Plane = _has2Plane(mfd, id3, id5);

	if(objChip.has2Plane){
		_set2PlaneErase(objChip);
	}

	//================ calculate maxPage( For J ver) ==========================
	objChip.maxPage = _getMaxPage(objChip.has2Plane, objChip.pagePerBlock);
	objChip.max_C_endPtr = _getMax_C_endPtr(objChip.has2Plane);

	//To change the objChip.size from KB to MB
	objChip.blockPerChip = objChip.size*1024/objChip.blockSize;
	objChip.nanometer = objChip._getNanometer();
	objChip.flashAddr5Mask = objChip._getFlashAddr5Mask();

	_getLowPageTable( objChip.lowPageTable, 
					   buf,
					 objChip.levelCellType,
					 objChip.nanometer );

	_get2PlaneWriteCmd(objChip.twoPlaneCmd, objChip.makerCode, objChip.has2Plane, objChip.hasCacheProgram);


	//================================= To check objChip ===================================
	////Small block has not id4
	//if( (objChip.blockType == BlockType::largeBlock() )){
	//	if (objChip.blockSize == 128){ // SLC Block
	//		ASSERT( objChip.levelCellType == LevelCellType::SLC());
	//	}else if(objChip.blockSize == 256){ // MLC Block
	//		if( objChip.pageSize==4){
	//			//50nm
	//			ASSERT( objChip.levelCellType == LevelCellType::SLC() );
	//		}else{
	//			//MLC
	//			ASSERT( objChip.levelCellType == LevelCellType::MLC() );
	//		}
	//	}else if(objChip.blockSize == 752){ // 8LC Block
	//		ASSERT( objChip.levelCellType == LevelCellType::_8LC() );
	//		ASSERT( objChip.pagePerBlock == 94 );
	//		ASSERT( objChip.pageSize == 8 );
	//	}else{
	//		//other blockSize => MLC
	//		ASSERT(objChip.levelCellType == LevelCellType::MLC() );
	//	}
	//}
	//ASSERT(objChip.dieNum != 0);
	return objChip;
}//------------------------------------------------------------------------------------

void Chip::_set2PlaneErase(Chip& chip){
	chip.twoPlaneEraseCmd[0]=0x60;
	chip.twoPlaneEraseCmd[1]=0x60;
	chip.twoPlaneEraseCmd[2]=0xD0;

	chip.has2PlaneErase = true;

	//almost intel 50nm 4k page erase cmd are "0x60,0xd1,0xd0"
	if(chip.makerCode==FLASHID_INTEL || chip.makerCode==FLASHID_MICRON)
	{
		if(chip.pageSize>2){
			chip.has2PlaneErase = false;
			chip.twoPlaneEraseCmd[0]=0x60;
			chip.twoPlaneEraseCmd[1]=0xD1;
			chip.twoPlaneEraseCmd[2]=0xD0;
		}
	}
}

int Chip::_getBlockSize(const BYTE* buf, const BlockType& blockType, const LevelCellType& lct, int pageSize ,int pagePerBlock){
	if (blockType == BlockType::smallBlock()){
		return 16;
	}

	int blockSizeKB = pageSize * pagePerBlock;
	return blockSizeKB;
}

//90=0 ¡A70 or 60=1¡A50=2, 40=3
int Chip::_getNanometer(void){
	int makerCode = this->makerCode;
	int deviceCode = this->deviceCode;
	int id3 = this->idData3;
	int id4 = this->idData4;
	int id5 = this->idData5;

	if( makerCode==FLASHID_SAMSUNG ){
		//K9G2G08U0M(60nm)
		if(deviceCode==0xDA && id3==0x14 && id4==0x25 && id5==0x44){return 1;}

		//K9G4G08U0A(60nm-MLC)
		if(deviceCode==0xDC && id3==0x14 && id4==0x25 && id5==0x54){return 1;}
	}

	if(this->pageSize==8){
		return 3;
	}

	if(this->pageSize>2){
		return 2;
	}

	if(blockPerChip>=0x1000){
		return 1;
	}
	return 0;
}

BYTE Chip::_getMax_C_endPtr(const bool is2Plane){
	if(is2Plane==true){
		return 128;
	}else{
		return 64;
	}
}

int Chip::_getPageSize(const BYTE* buf){
	BYTE	mfd = buf[0];//BYTE 1¡G maker code
	BYTE	id  = buf[1];//BYTE 2¡G device code
	BYTE	id3 = buf[2];//BYTE 3¡G 3rd ID Data
	BYTE	id4 = buf[3];//BYTE 4¡G 4th ID Data
	BYTE	id5 = buf[4];//BYTE 5¡G 5th ID Data

	int pageSize(0);
	int value = Utility::getBit(id4,1)*2 + Utility::getBit(id4,0);
	
	switch(value){
		case 0:
			pageSize = 1;//1 k
			break;
		case 1:
			pageSize = 2;//2 k
			break;
		case 2:
			pageSize = 4;//4 k
			break;
		case 3:
			pageSize = 8;//5 k
			break;
		default:					
			throw MyException(CHIP_GET_PAGE_SIZE_FAIL, "Can`t recognize the PageSize By Id4 = %X", id4);
			break;	
	}


	//except, 8kPage
	bool isExcept = false;
	if(mfd==0x98 && id3==0x94 && id4==0x32 && id5==0x76){
		isExcept  = true;
	}
	if(mfd==0x98 && id3==0x95 && id4==0x32 && id5==0x7A){
		isExcept  = true;
	}

	if(mfd==0x98 && id3==0x99 && id4==0xb2 && id5==0x7A){
		isExcept  = true;
	}

	if(mfd==0x98 && id== 0xD3 && id3==0x84 && id4==0x32 && id5==0x72){
		isExcept  = true;
	}
	
	if(mfd==0x45 && id3==0x94 && id4==0x32 && id5==0x76){
		isExcept  = true;
	}
	if(mfd==0x45 && id3==0x95 && id4==0x32 && id5==0x7A){
		isExcept  = true;
	}

	if(mfd==0xEC && id== 0xD7 && id3==0x94 && id4==0x72 && id5==0x54){
		isExcept  = true;
	}

	if(mfd==0xEC && id== 0xD5 && id3==0x84 && id4==0x72 && id5==0x50){
		isExcept  = true;
	}


	// Hynix 32nm D2
	if(mfd==0xAD && id== 0xD7 && id3==0x94 && id4==0x9A && id5==0x74){
		isExcept  = true;
	}

	// TSB 24D2
	if(mfd==0x98 && id== 0xDE && id3==0x94 && id4==0x82 && id5==0x76){
		isExcept  = true;
	}

	if(isExcept){
		switch(value){
		case 0:
			pageSize = 2;//2 k
			break;
		case 1:
			pageSize = 4;//4 k
			break;
		case 2:
			pageSize = 8;//8 k
			break;
		default:					
			throw MyException(CHIP_GET_PAGE_SIZE_FAIL, "Can`t recognize the PageSize By Id4 = %X", id4);
			break;	
		}
	}
	return pageSize;
}

bool Chip::_has2Plane(const BYTE& makerCode, const BYTE& id3, const BYTE& id5){
	bool result = true;
	if( (Utility::getBit(id3,4) == 0) && (Utility::getBit(id3,5) == 0) ){
		result =  false;
	}

	int planeNo = id5&0x0C;
	//double Check
	if( result==true ){
		if( planeNo==0 ) {
			ASSERT(0);
			result= false;
		}
	}

	//double Check¡AASK BEN
	//if( result==false ){
	//	if( planeNo!=0 ) {
	//		ASSERT(0);
	//	}
	//}

	//TSB have special way to allcate 2 plane block, so turn off first 
	//if( makerCode==FLASHID_TOSHIBA){
	//	result= false;
	//}
	return result;
}


bool Chip::_getInterLeave(const BYTE& id3){
	bool result;
	if( (id3&BIT6)>>6 ==0){
		result = false;
	}else{
		result = true;
	}
	return result;
}

bool Chip::_getCacheProgram(const BYTE& id3){
	bool result;
	if( (id3&BIT7)>>7 ==0){
		result = false;
	}else{
		result = true;
	}
	return result;
}


int Chip::_getDieNum(const UCHAR *buf){
	UCHAR id3 = buf[2];//BYTE 3¡G 3rd ID Data
	int dieNum(0);

	if ((id3& (BIT0|BIT1) ) == 0){ dieNum=1; }
	if ((id3& (BIT0|BIT1) ) == 1){ dieNum=2; }
	if ((id3& (BIT0|BIT1) ) == 2){ dieNum=4; }
	if ((id3& (BIT0|BIT1) ) == 3){ dieNum=8; }

	return dieNum;
}//------------------------------------------------------------------------------------

//SLC =0¡AMLC = 1
Chip::LevelCellType Chip::_getLevelCellType(const UCHAR *buf, const BlockType& blockType){

	//our Product no support small block + MLC
	if(blockType == Chip::BlockType::smallBlock()){
		return LevelCellType::SLC();
	}

	unsigned char	mfd = buf[0];//BYTE 1¡G maker code
	unsigned char	id3 = buf[2];//BYTE 3¡G 3rd ID Data
	
	LevelCellType lcType = LevelCellType::SLC();

	//Some SS Nanad Flash don`t care id3 value
	int lcNo = (id3&(BIT2|BIT3))>>2;

	switch(lcNo){
		case 0:
			lcType = LevelCellType::SLC();
			break;	
		case 1 :
			lcType = LevelCellType::MLC();
			break;	
		case 2 :
			lcType = LevelCellType::TLC();
			break;
		default:		
			throw MyException(CHIP_GET_LEVEL_CELL_FAIL, "Can`t recognize the flash Level Cell");
			break;	
	}

	//The way to get the level cell type are all different in every flash maker
	switch(mfd){
		case FLASHID_TOSHIBA:
			if (id3==0x84){
				lcType = LevelCellType::MLC();
			}
			break;
	}
	return lcType;
}//------------------------------------------------------------------------------------


void Chip::_loadBlockTypeAndSize(BYTE deviceCode, Chip& chip){
	switch(deviceCode){
		case 0x39 :
		case 0x49 :
		case 0x59 :
		case 0xE6 : 					
			chip.blockType = BlockType::smallBlock();
			chip.size=8;//MB
			break;
		case 0x33 :
		case 0x43 :
		case 0x53 :
		case 0x73:
			chip.blockType=BlockType::smallBlock();
			chip.size=16;//MB
			break;
		case 0x75:
		case 0x55:
		case 0x35:
		case 0x45:
			chip.blockType=BlockType::smallBlock();
			chip.size=32;//MB
			break;
		case 0x76:
		case 0x56:
		case 0x36:
		case 0x46:
			chip.blockType=BlockType::smallBlock();
			chip.size=64;//MB
			break;

		case 0x79:
		case 0x74:
		case 0x78:
		case 0x72:
			chip.blockType=BlockType::smallBlock();
			chip.size=128;//MB
			break;

		case 0xF0:
		case 0xC0:
		case 0xA0:
		case 0xB0:
			chip.blockType = BlockType::largeBlock();
			chip.size=64;//MB
			break;

		case 0xF1:
		case 0xC1:
		case 0xA1:
		case 0xB1:
			chip.blockType = BlockType::largeBlock();
			chip.size=128;//MB
			break;
		
		case 0xAA:
		case 0xBA:
		case 0xCA:
		case 0xDA:
		case 0x71://SB
			chip.blockType = BlockType::largeBlock();
			chip.size=256;//MB
			break;

		case 0xAC:
		case 0xBC:
		case 0xCC:
		case 0xDC:
			chip.blockType = BlockType::largeBlock();
			chip.size=512;//MB
			break;

		case 0xA3:
		case 0xB3:
		case 0xC3:
		case 0xD3:
			chip.blockType = BlockType::largeBlock();
			chip.size=1024;//MB
			break;

		case 0xA5:
		case 0xB5:
		case 0xC5:
		case 0xD5:
			chip.blockType = BlockType::largeBlock();
			chip.size=2048;//MB
			break;	

		case 0xD7:
		case 0xC7:
			chip.blockType = BlockType::largeBlock();
			chip.size=4096;//MB
			break;	

		case 0xD9:
			chip.blockType = BlockType::largeBlock();
			chip.size=8192;//MB
			break;	

		case 0xDE:
			chip.blockType = BlockType::largeBlock();
			chip.size=8192;//MB
			break;	
		
		case 0x48:
			chip.blockType = BlockType::largeBlock();
			chip.size=2048;//MB
			break;
		// IM 32 256 Page
		case 0x68:
			chip.blockType = BlockType::largeBlock();
			chip.size=4096;//MB
			break;

		// IM 28 256 Page
		case 0x88:
			chip.blockType = BlockType::largeBlock();
			chip.size=8192;//MB
			break;

		// TSB 32D2LGA
		case 0xE7:
			chip.blockType = BlockType::largeBlock();
			chip.size=4096;//MB
			break;

		// TSB 32D2LGA
		case 0xEE:
			chip.blockType = BlockType::largeBlock();
			chip.size=8192;//MB
			break;

		// TSB 32D2LGA
		case 0x3A:
			chip.blockType = BlockType::largeBlock();
			chip.size=16384;//MB
			break;


		default:		
			throw MyException(CHIP_LOAD_BLOCK_SIZE_TYPE_FAIL, "Can`t recognize the flash Card block type & size, the deviceCode = %X ",deviceCode);
			break;	
	}
}

int Chip::_getPagePerBlock(const LevelCellType& levelType, const BlockType& blockType){
	int pagePerBlock=0;
	int const PAGE_PER_BLOCK_SB  = 32;
	int const PAGE_PER_BLOCK_LB  = 64;
	int const PAGE_PER_BLOCK_MLC = 128;
	int const PAGE_PER_BLOCK_8LC = 192;



	if(blockType == Chip::BlockType::smallBlock() ){
		pagePerBlock = PAGE_PER_BLOCK_SB;
	}else{
		pagePerBlock = PAGE_PER_BLOCK_LB;
	}


	if(levelType == Chip::LevelCellType::MLC() ){
		pagePerBlock = PAGE_PER_BLOCK_MLC;
	}

	if(levelType == Chip::LevelCellType::TLC() ){
		pagePerBlock = PAGE_PER_BLOCK_8LC;
	}

	return pagePerBlock;
}

WORD Chip::_getMaxPage(const bool is2PlaneMode, int pagePerBlock){
	WORD maxPage = pagePerBlock;
	if(is2PlaneMode ==true){
		maxPage = maxPage*2;
	}
	ASSERT(maxPage !=0);
	return maxPage;
}

BYTE Chip::_getFlashAddr5Mask(void){

	if(this->makerCode==FLASHID_HYNIX){
		if(this->has2Plane==false){
			return 0xFF;
		}else{
			//except flash
			if( this->deviceCode == 0xD3 && this->idData3==0x51 && this->idData4==0x95 && this->idData5==0x58){
				return 0x04;
			}
			return 0;
		}
	}

	//Samsuang
	if(this->makerCode==FLASHID_SAMSUNG){
		if(this->has2Plane==false){
			return 0xFF;
		}else{
			if(this->dieNum<=1){
				return 0;//fixed Low
			}
			int totalPageCnt = this->blockPerChip * this->pagePerBlock;
			BYTE maskBit = (BYTE)(totalPageCnt>>16);
			maskBit = (maskBit+1)/2;
			return maskBit;
		}
	}

	return 0xFF;
}



void Chip::_getLowPageTable(BYTE* ary64byte, const BYTE* buf, LevelCellType& lct, int nanometer){
	//BYTE	mfd = buf[0];//BYTE 1¡G maker code
	BYTE	makerCode  = buf[1];//BYTE 2¡G device code
	//BYTE	id3 = buf[2];//BYTE 3¡G 3rd ID Data
	//BYTE	id4 = buf[3];//BYTE 4¡G 4th ID Data
	//BYTE	id5 = buf[4];//BYTE 5¡G 5th ID Data

	
	//SLC:0,1,2,3,4,5,6,7,8.....
	BYTE i,value;
	BYTE convertTb_SLC[0x40];
	for(i=0;i<0x40;i++){
		convertTb_SLC[i]=i;
	}

	//1,3,5,7....
	BYTE convertTb_MLC_TSB_90nm[0x40];
	for(i=0, value=1; i<0x40; i++, value+=2){
		convertTb_MLC_TSB_90nm[i]=value;
	}

	//0,2,4,6,8....
	BYTE convertTb_MLC_SS_90nm[0x40];
	for(i=0, value=0; i<0x40;i++, value+=2){
		convertTb_MLC_SS_90nm[i]=value;
	}

	//0,1,2,3,6,7,A,B
	BYTE ctb_MLC_70_50nm[0x40]={
		0,1,2,3,6,7,10,11,14,15,18,19,22,23,26,27,30,31,34,35,38,39,42,43,46,47,50,51,54,55,58,59,
		62,63,66,67,70,71,74,75,78,79,82,83,86,87,90,91,94,95,98,99,102,103,106,107,110,111,
		114,115,118,119,122,123
	};

	BYTE _8LC_LowPage[0x40];
	memset(_8LC_LowPage,0xFF, sizeof(_8LC_LowPage) );
	
	BYTE tmp_8LC_LowPage[32]= {
		0,1,4,7,10,13,16,19,22,25,28,31,34,37,40,43,46,49,52,55,58,61,64,67,70,73,76,79,82,85,88,91
	};

	memcpy(_8LC_LowPage, tmp_8LC_LowPage, sizeof(tmp_8LC_LowPage) );


	//=========== 43nm D2 TSB =============
	//0,1,3,5,7....125
	BYTE _43nmD2_LowPage_TSB[0x40];
	_43nmD2_LowPage_TSB[0]=0;
	for(i=1, value=1; i<0x40;i++, value+=2){
		_43nmD2_LowPage_TSB[i]=value;
	}

	if( lct==LevelCellType::SLC() ){
		memcpy(ary64byte, convertTb_SLC,0x40);
		return ;
	}

	if( lct==LevelCellType::TLC() ){
		memcpy(ary64byte, _8LC_LowPage, sizeof(_8LC_LowPage) );
		return ;
	}

	if(nanometer==3){
		memcpy(ary64byte, _43nmD2_LowPage_TSB,0x40);
		return ;
	}

	//MLC 90nm
	if( nanometer ==0 ){
		//TODO:the default vale equal SS , but it maybe wrong 
		memcpy(ary64byte, convertTb_MLC_SS_90nm, 0x40);
		if( makerCode == FLASHID_TOSHIBA){
			memcpy(ary64byte, convertTb_MLC_TSB_90nm, 0x40);
		}

		if( makerCode == FLASHID_SAMSUNG){
			memcpy(ary64byte, convertTb_MLC_SS_90nm, 0x40);
		}
	}else{//MLC not 90nm
		//TODO:the default vale equal ctb_MLC_70_50nm , but it maybe wrong 
		memcpy(ary64byte, ctb_MLC_70_50nm, 0x40);
		if( makerCode == FLASHID_TOSHIBA){
			memcpy(ary64byte, ctb_MLC_70_50nm, 0x40);
		}

		if( makerCode == FLASHID_SAMSUNG){
			memcpy(ary64byte, ctb_MLC_70_50nm, 0x40);
		}
	}
}

void Chip::_get2PlaneWriteCmd(BYTE* twoPlaneCmd, BYTE makerCode, bool has2Plane, bool hasWriteCache){
	twoPlaneCmd[0]  = 0x80;
	twoPlaneCmd[1]  = 0x10;
	twoPlaneCmd[2]  = 0x80;
	twoPlaneCmd[3]  = 0x10;
	twoPlaneCmd[4]  = 0x80;
	twoPlaneCmd[5]  = 0x10;

	//2plane,non write cache
	if( (makerCode==FLASHID_SAMSUNG) &&  (has2Plane==true) ){
		twoPlaneCmd[0] = 0x80;
		twoPlaneCmd[1] = 0x11;
		twoPlaneCmd[2] = 0x81;
		twoPlaneCmd[3] = 0x10;
		twoPlaneCmd[4] = 0x81;
		twoPlaneCmd[5] = 0x10;
	}

	if( (makerCode==FLASHID_HYNIX) &&  (has2Plane==true) ){
		twoPlaneCmd[0] = 0x80;
		twoPlaneCmd[1] = 0x11;
		twoPlaneCmd[2] = 0x81;
		twoPlaneCmd[3] = 0x10;
		twoPlaneCmd[4] = 0x81;
		twoPlaneCmd[5] = 0x10;
	}

	if( (makerCode==FLASHID_TOSHIBA) &&  (has2Plane==true) ){
		twoPlaneCmd[0] = 0x80;
		twoPlaneCmd[1] = 0x11;
		twoPlaneCmd[2] = 0x80;
		twoPlaneCmd[3] = 0x15;
		twoPlaneCmd[4] = 0x80;
		twoPlaneCmd[5] = 0x10;
	}

	
	if( ((makerCode==FLASHID_MICRON)||(makerCode==FLASHID_INTEL)) &&  (has2Plane==true) ){
		if(hasWriteCache==true){
			twoPlaneCmd[0] = 0x80;
			twoPlaneCmd[1] = 0x11;
			twoPlaneCmd[2] = 0x80;
			twoPlaneCmd[3] = 0x15;
			twoPlaneCmd[4] = 0x80;
			twoPlaneCmd[5] = 0x10;
		}else{
			twoPlaneCmd[0] = 0x80;
			twoPlaneCmd[1] = 0x11;
			twoPlaneCmd[2] = 0x81;
			twoPlaneCmd[3] = 0x10;
			twoPlaneCmd[4] = 0x81;
			twoPlaneCmd[5] = 0x10;
		}
	}
}

void Chip::_setSpecialFlash(Chip& chip) {
	//don`t care id3
	if(chip.makerCode==0xEC && chip.deviceCode==0xDA && chip.idData3==0xC1 
		&& chip.idData4==0x15 && chip.idData5==0x44 )
	{
		chip.idData3 =0;
	}
}

string Chip::toString(void) const{
	tstring msg,id;
	id = Utility::strFormat("ID=%X, %X, %X, %X, %X",this->makerCode,this->deviceCode,this->idData3,this->idData4,this->idData5);

	msg+=id + Utility::CrLf();
	msg+="Block Type = " + this->blockType.toString()  + Utility::CrLf();
	msg+="Level Cell Type = " + this->levelCellType.toString() + Utility::CrLf();
	msg+=Utility::CrLf();
	msg+="DieNum = " + Utility::toString(this->dieNum) + Utility::CrLf();
	msg+="BlockPerChip = " + Utility::toString(this->blockPerChip) + Utility::CrLf();
	msg+="BlockSize = " + Utility::toString(this->blockSize) + " KB" + Utility::CrLf();
	msg+="Size = " + Utility::toString(this->size) + " MB" + Utility::CrLf();
	msg+=Utility::CrLf();

	msg+="Has CacheProgram = " + Utility::toString(this->hasCacheProgram) + Utility::CrLf();
	msg+="Has Interleave = " + Utility::toString(this->hasInterleave) + Utility::CrLf();
	msg+=Utility::CrLf();
	msg+="Page Size = " + Utility::toString(this->pageSize) + " KB"+ Utility::CrLf();
	msg+="Page Per Block = " + Utility::toString(this->pagePerBlock) + Utility::CrLf();
	msg+="Sec Per Block = " + Utility::toString(this->secPerBlock) + Utility::CrLf();
	msg+="Sec Per Page = " + Utility::toString(this->secPerPage) + Utility::CrLf();

	// for J version
	msg+="flashAddr5Mask = 0x" + Utility::toHexString(this->flashAddr5Mask) + Utility::CrLf();
	return msg;
}


void Chip::load(const BYTE *buf){
	this->makerCode  = buf[0]; //BYTE 1¡G maker code
	this->deviceCode = buf[1]; //BYTE 2¡G device code
	this->idData3    = buf[2]; //BYTE 3¡G 3rd ID Data
	this->idData4    = buf[3]; //BYTE 4¡G 4th ID Data
	this->idData5    = buf[4]; //BYTE 5¡G 5th ID Data


	if( this->makerCode == FLASHID_INTEL ) {
		_lookup_intel();
	}

	if( this->makerCode == FLASHID_MICRON ) {
		_lookup_micron();
	}

	if( this->makerCode == FLASHID_HYNIX ) {
		_lookup_Hynix();
	}

}

void Chip::_lookup_micron(void){
	//IM 28nm 64Gb
	if( this->makerCode==FLASHID_MICRON  && this->deviceCode==0x88 && (this->idData3==0x04||this->idData3==0x24) && this->idData4==0x4B && this->idData5==0xA9 ){	
		this->dieNum = 1;
		this->pageSize = 8;
		this->nanometer = 28;
		this->pagePerBlock = 256;
		this->blockPerChip = 4096;
		this->dieStart_1st = 0;
		this->dieEnd_1st = 4095; //include, not 4096
		this->defectMarkColumnOffset = 0; // SPEC, P73, first page and first BYTE

		this->secPerPage = _getSecPerPage(this->pageSize);
		this->secPerBlock = _getSecPerBlock(this->secPerPage, this->pagePerBlock);
		this->blockSize = _getBlockSize(this->pageSize, this->pagePerBlock);

		BYTE lowPageTb[0x40]=
		{ 
			0,  2,  6, 10, 14, 18, 22, 26, 30, 34, 38, 42, 46, 50, 54, 58,
				62, 66, 70, 74, 78, 82, 86, 90, 94, 98,102,106,110,114,118,122,
				126,130,134,138,142,146,150,154,158,162,166,170,174,178,182,186,
				190,194,198,202,206,210,214,218,222,226,230,234,238,242,246,250
		};

		memcpy( this->lowPageTable, lowPageTb, sizeof(lowPageTb) );
	}

}

void Chip::_lookup_intel(void){

	//IM 28nm 64Gb
	if( this->makerCode==0x89  && this->deviceCode==0x88 && (this->idData3==0x04||this->idData3==0x24) && this->idData4==0x4B && this->idData5==0xA9 ){	
		this->dieNum = 1;
		this->pageSize = 8;
		this->nanometer = 28;
		this->pagePerBlock = 256;
		this->blockPerChip = 4096;
		this->dieStart_1st = 0;
		this->dieEnd_1st = 4095; //include, not 4096
		this->defectMarkColumnOffset = 0; // SPEC, P73, first page and first BYTE
			
		this->secPerPage = _getSecPerPage(this->pageSize);
		this->secPerBlock = _getSecPerBlock(this->secPerPage, this->pagePerBlock);
		this->blockSize = _getBlockSize(this->pageSize, this->pagePerBlock);

		BYTE lowPageTb[0x40]=
		{ 
			0,  2,  6, 10, 14, 18, 22, 26, 30, 34, 38, 42, 46, 50, 54, 58,
				62, 66, 70, 74, 78, 82, 86, 90, 94, 98,102,106,110,114,118,122,
				126,130,134,138,142,146,150,154,158,162,166,170,174,178,182,186,
				190,194,198,202,206,210,214,218,222,226,230,234,238,242,246,250
		};

		memcpy( this->lowPageTable, lowPageTb, sizeof(lowPageTb) );
	}
}

void Chip::_lookup_Hynix(void){

	//Hynix 32nm 32Gb
	if( this->makerCode==0xAD  && this->deviceCode==0xd7 && this->idData3==0x94 && this->idData4==0x9A && this->idData5==0x74 ){	
		this->dieNum = 1;
		this->pageSize = 8;
		this->nanometer = 32;
		this->pagePerBlock = 256;
		this->blockPerChip = 4096;
		this->dieStart_1st = 0;
		this->dieEnd_1st = 2047; //include, not 4096
		this->defectMarkColumnOffset = 0;
			
		this->secPerPage = _getSecPerPage(this->pageSize);
		this->secPerBlock = _getSecPerBlock(this->secPerPage, this->pagePerBlock);
		this->blockSize = _getBlockSize(this->pageSize, this->pagePerBlock);

		BYTE lowPageTb[0x40]=
		{ 
			0,  2,  6, 10, 14, 18, 22, 26, 30, 34, 38, 42, 46, 50, 54, 58,
				62, 66, 70, 74, 78, 82, 86, 90, 94, 98,102,106,110,114,118,122,
				126,130,134,138,142,146,150,154,158,162,166,170,174,178,182,186,
				190,194,198,202,206,210,214,218,222,226,230,234,238,242,246,250
		};

		memcpy( this->lowPageTable, lowPageTb, sizeof(lowPageTb) );
	}
}


int Chip::_getSecPerPage(int pageSize){
	return pageSize*2;
}

int Chip::_getSecPerBlock(int secPerPage, int pagePerBlock){
	return secPerPage*pagePerBlock;
}

int Chip::_getBlockSize(int pageSize, int pagePerBlock){
	return pageSize*pagePerBlock;
}








