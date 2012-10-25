#include "stdafx.h"
#include ".\FlashBase.h"

#include <EricCore\Utility\EricException\MyException.h>

using namespace std;
using namespace EricCore;

FlashBase::FlashBase(void)
:modeType(ModeType::null())
,totalSize(0)
,totalBlock(0)
,cylinder(0)
,header(0)
,sector(0)
,maxLBA(0)
,suggesTiming(0)
{
}

FlashBase::~FlashBase(void)
{
}

Chip::Chips FlashBase::_getChips(const BYTE* flashIdTb){
	Chip::Chips chips;
	BYTE const emtpyChar= 0xFF;
	BYTE const emtpyChar2= 0x00;//for Hynix

	int base,i;
	for(i=0; i<33; i++){
		base = i*6;
		if( ((flashIdTb[base+0]==0x55 && flashIdTb[base+1]==0xAA) == true)
			||
			((flashIdTb[base+0]==0xAA && flashIdTb[base+1]==0x55) == true)
		  )
		{
			break;
		}

		//is empty
		if( (flashIdTb[base+1]==emtpyChar && flashIdTb[base+2]==emtpyChar && flashIdTb[base+3]==emtpyChar) == true){
			continue;
		}

		if( (flashIdTb[base+1]==emtpyChar2 && flashIdTb[base+2]==emtpyChar2 && flashIdTb[base+3]==emtpyChar2) == true){
			continue;
		}

		BYTE dataReg = flashIdTb[base+1];
		if( (flashIdTb[base+1]==dataReg && flashIdTb[base+2]==dataReg && flashIdTb[base+3]==dataReg) == true){
			continue;
		}

		if( (flashIdTb[base+1]==0xFF) || (flashIdTb[base+1]==0x00) ){
			continue;
		}

		try{
			Chip c = c.getChipById(flashIdTb+base+1);
			chips.push_back(c);
		}catch (MyException& e){
			string errorMsg("Can not create chip object \r\n");
			errorMsg += e.what();
			throw MyException(FB_GET_CHIP_FAIL, errorMsg);
		}
	}
	return chips;
}

ModeType FlashBase::_getModeType(BYTE* flashIdTb){
	if( (flashIdTb[7]==0xFF) && (flashIdTb[8]==0xFF)){
		return ModeType::byteMode();
	}
	if( memcmp(flashIdTb+1,flashIdTb+7,5) != 0){
		throw MyException(FB_GET_MODE_TYPE_FAIL, "Flash Id Table Error: please check flash is ALL the same");
	}else{
		return ModeType::wordMode();
	}
}

void FlashBase::_checkChips(const Chip::Chips& chips){
	if ( (this->totalCE()==0) || ( this->_getTotalSize()==0) ){
		throw MyException(FB_CHECK_CHIPS_FAIL, "No flash found");
	}

	int totalCE = (int)chips.size();
	int i;
	for(i=1; i<totalCE; i++){
		if( ( chips[0].makerCode  != chips[i].makerCode)  ||
			( chips[0].deviceCode != chips[i].deviceCode) ||
			( chips[0].idData3    != chips[i].idData3)    ||
			( chips[0].idData4    != chips[i].idData4)    ||
			( chips[0].idData5    != chips[i].idData5)
			)
		{
			throw MyException(FB_CHECK_CHIPS_FAIL, "It`s not all the same chip");
		}
	}
}

void FlashBase::_loadDefaultCHS(int sizeMB){
	int	index=0;
	bool outOfRange = true;

	const WORD DiskSize[22] = {   8,  16,  32,  48,  64,  80,  96, 112, 128, 144, 160, 192, 208, 224, 256, 512,1024,2048,4096, 8192,16384,32768 };
	const WORD Cylinder[22] = { 248, 248, 496, 747, 978, 611, 733, 855, 978, 550, 611, 733, 794, 855, 978, 993,1985,3954,7889,15778,31556,63112};
	const BYTE Header[22]   = {   2,   4,   4,   4,   4,   8,   8,   8,   8,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,   16,   16,   16 };
	const BYTE Sector[22]   = {  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  63,  63,  63,  63,   63,   63,   63 };
	for(index=0;index<22;index++){
		if (sizeMB<=DiskSize[index]){
			outOfRange = false;
			break;
		}
	}

	if(outOfRange==true){throw MyException(FB_LOAD_CHS_FAIL, "can`t find CHS By size");}

	this->cylinder = Cylinder[index];
	this->header = Header[index];
	this->sector = Sector[index];
}//------------------------------------------------------------------------------------

LevelCellType FlashBase::levelCellType() const {
	return chips[0].levelCellType;
}//------------------------------------------------------------------------------------

BlockType FlashBase::blockType() const {
	return chips[0].blockType;
}//------------------------------------------------------------------------------------

int FlashBase::diePerChip() const {
	int a =chips[0].dieNum; 
	return a;
}//------------------------------------------------------------------------------------

int FlashBase::blockPerChip() const {
	return chips[0].blockPerChip;
}//------------------------------------------------------------------------------------

int FlashBase::getMakerCode() const{
	return chips[0].makerCode;
}//------------------------------------------------------------------------------------

int FlashBase::pagePerBlock() const {
	return chips[0].pagePerBlock;
}//------------------------------------------------------------------------------------

int FlashBase::secPerBlock() const{
	return chips[0].secPerBlock;
}//------------------------------------------------------------------------------------


int FlashBase::secPerPage() const {
	return chips[0].secPerPage;;
}//------------------------------------------------------------------------------------

int FlashBase::totalCE() const{
	size_t cnt = chips.size();
	return (int)cnt;
}//------------------------------------------------------------------------------------

bool FlashBase::hasCacheProgram() const{
	return chips[0].hasCacheProgram;
}//------------------------------------------------------------------------------------

bool FlashBase::hasInterleave() const{
	return chips[0].hasInterleave;
}//------------------------------------------------------------------------------------

int FlashBase::_getTotalBlock(){
	int totalBlock=0;
	for (Chips::iterator iter = this->chips.begin(); iter!= this->chips.end(); ++iter){
		totalBlock = totalBlock + iter->blockPerChip;
	}
	return totalBlock;
}//------------------------------------------------------------------------------------

int FlashBase::_getTotalSize(){
	int totalSize=0;
	for (Chips::iterator iter = this->chips.begin(); iter!= this->chips.end(); ++iter){
		totalSize = totalSize + iter->size;
	}
	return totalSize;
}//------------------------------------------------------------------------------------

tstring FlashBase::toString(void) const{
	tstring msg,chs;

	chs = Utility::strFormat("C.H.S = %X, %X, %X",this->cylinder,this->header,this->sector);

	msg+= chs + Utility::CrLf();
	msg+="MaxLBA = " + Utility::toString(this->maxLBA) + Utility::CrLf();
	msg+="Mode Type = " + this->modeType.toString() + Utility::CrLf();
	msg+="Suggest Timing = " +  Utility::toString(this->suggesTiming) + Utility::CrLf();
	msg+="Total CE Number = " + Utility::toString(this->totalCE() ) + Utility::CrLf();
	msg+="Total Block = " + Utility::toString(this->totalBlock) + Utility::CrLf();
	msg+="Total Size = " + Utility::toString(this->totalSize) +" MB" + Utility::CrLf() ;

	msg+= Utility::CrLf();
	msg+="== Chip Info ==" + Utility::CrLf();
	msg+=this->chips[0].toString();

	return msg;
}//------------------------------------------------------------------------------------



