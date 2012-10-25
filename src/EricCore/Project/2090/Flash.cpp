#include "StdAfx.h"
#include ".\flash.h"
#include "..\..\Utility\Utility.h"
using namespace EricCore;

Flash::Flash(void)
:maxPlane(0)
,maxLBperWin(0)
,maxSLCpage(0)
,maxMLCpage(0)
,interExterInterleave(0)
,evenDieStart(0)
,evenDieEnd(0)
,oddDieStart(0)
,oddDieEnd(0)
,blockLen(0)
,is_TSB_43nmD2(false)
,is_TSB_43nmD2_8Gb(false)
,is_TSB_43nmD2_LGA(false)
,is_TSB_43nmD3(false)
,is_TSB_43nmD3_Special(false)
,is_TSB_56nmD3(false)
,is_TSB_32nmD2(false)
,is_TSB_32nmD2_LGA(false)
,is_TSB_24nmD2(false)
,is_TSB_24nmD2_128Page(false)

,is_IM_50nmD2(false)
,is_IM_34nmD2(false)
,is_IM_34nmD2_256Page(false)
,is_IM_28nmD2_256Page(false)

,is_SanDisk_43nmD2(false)
,is_SanDisk_43nmD3(false)
,is_SanDisk_56nmD3(false)
,is_SanDisk_32nmD2(false)
,is_SanDisk_24nmD2(false)

,is_Hynix_32nmD2_256Page(false)

,is_Samsuang_32nmD2(false)
,is_Samsuang_32nmD3(false)
,is_Samsuang_35nmD2_LGA(false)

,isCM(false)
,is2kDefect(false)
,is4ce(false)
,is16_IO(false)
,defectBlockAddr(0)
,defectMarkColumnOffset(0)
,pageSize(0)
,winCntPerPlane(0)
,is2PageMode(false)
{
}

Flash::~Flash(void)
{
}


Flash Flash::getFlash(BYTE* flashIdTb){
	Flash objFlash;
	Chip c;
	objFlash.modeType = getModeType_8ByteFormat(flashIdTb);
	objFlash.chips = objFlash.getChips_8ByteFormat(flashIdTb);
	objFlash._checkChips(objFlash.chips);
	objFlash.totalSize = objFlash._getTotalSize();
	objFlash.totalBlock = objFlash._getTotalBlock();
	objFlash._loadDefaultCHS(objFlash.totalSize);
	objFlash.maxLBA = (ULONG)objFlash.cylinder * (ULONG)objFlash.header * (ULONG)objFlash.sector; 
	//objFlash.suggesTiming = objFlash._getTimting();

	// 2: 2 ce external interleave
	// 1: 2 die internal interleave
	// 0: no interleave
	if( objFlash.diePerChip()==2){
		objFlash.interExterInterleave = INTERNAL_INTERLEAVE;
	}else if(objFlash.chips.size()>1 ) {
		objFlash.interExterInterleave = EXTERNAL_INTERLEAVE;
	}else{
		objFlash.interExterInterleave = NO_INTERLEAVE;
	}

	//目前全部都是以2 plane為主，先不做2 ce interleave
	objFlash.interExterInterleave=NO_INTERLEAVE;

	if( objFlash.interExterInterleave==NO_INTERLEAVE ){
		objFlash.maxPlane=2;
	}else{
		objFlash.maxPlane=4;
	}

	objFlash.loadAge(flashIdTb);
	objFlash.blockLen = 128;
	objFlash.maxMLCpage = objFlash.pagePerBlock();
	setProperty(objFlash);

	


	if(objFlash.is_TSB_43nmD3){
		size_t i;
		objFlash.blockLen = 256;
		BYTE _8LC_LowPage[0x40];
		BYTE value;
		_8LC_LowPage[0]=0;
		for(i=1, value=2; i<0x40;i++, value+=3){
			_8LC_LowPage[i]=value;
		}
		for(i=0; i<objFlash.chips.size(); i++){
			Chip c = objFlash.chips[i];
			int blockCnt = 1404;
			if(c.deviceCode==0xD7){
				blockCnt = 2780;
			}

			c.blockPerChip = blockCnt*c.dieNum;
			c.blockSize = 1536;//kb
			c.pageSize = 8;//8k
			c.size = (c.blockSize*c.blockPerChip)/1024;//mb
			c.secPerPage = 16;
			c.nanometer = 3;
			c.maxPage = 192;
			c.pagePerBlock = 192;

			c.secPerBlock = c.secPerPage* c.pagePerBlock;
			memcpy( c.lowPageTable, _8LC_LowPage, sizeof(_8LC_LowPage) );
			objFlash.chips[i] = c;
		}

		objFlash.maxMLCpage = 192;
	}

	//減去cluster2 所多出來的，造成後面讀不到
	objFlash.maxLBA  = objFlash.maxLBA -32;
	objFlash._showFlashTypeAndAge();
	return objFlash;
}


void Flash::_showFlashTypeAndAge(void){
	
	//if(  this->is_TSB_43nmD2 ){	Utility::sendMsg("It's TSB TSB_43nmD2");}
	if(  this->is_TSB_43nmD2_LGA ){	Utility::sendMsg("It's TSB 43nmD2_LGA");}
	if(  this->is_TSB_32nmD2 ){	Utility::sendMsg("It's TSB 32nmD2");}
	if(  this->is_TSB_43nmD3 ){	Utility::sendMsg("It's TSB TSB_43nmD3");}
	if(  this->is_TSB_43nmD3_Special ){	Utility::sendMsg("It's TSB 43nmD3_Special");}
	if(  this->is_TSB_56nmD3 ){	Utility::sendMsg("It's TSB 56nmD3");}
	if(  this->isCM ){	Utility::sendMsg("It's CM");}
	if(  this->is_IM_34nmD2 ){	Utility::sendMsg("It's IM 34nmD2");}
	if(  this->is_IM_50nmD2 ){	Utility::sendMsg("It's IM 50nmD2");}

	if(  this->is_SanDisk_43nmD2 ){	Utility::sendMsg("It's SanDisk_43nmD2");}
	if(  this->is_SanDisk_43nmD3 ){	Utility::sendMsg("It's SanDisk_43nmD3");}
	if(  this->is_SanDisk_56nmD3 ){	Utility::sendMsg("It's SanDisk_56nmD3");}

}

void Flash::setProperty(Flash& f){

	//default value
	f.pageSize = f.secPerPage()/2;

	if( f.is_TSB_43nmD2==true){
		f.maxLBperWin = 0x776;
		f.maxSLCpage = 64;
		//1個block=1024k, = 2048個sector
		f.maxLBA = f.maxLBperWin*1024*2;
		f.maxLBA = f.maxLBA * f.chips.size() * f.diePerChip() ;
		f.pageSize = 8;
		f.defectMarkColumnOffset = 0;

		f.evenDieStart=0;
		f.evenDieEnd=2084;
		if(f.diePerChip()==2){
			f.oddDieStart=4096;
			f.oddDieEnd=6180;
		}
	}

	if( f.is_TSB_43nmD2_8Gb==true){
		f.maxLBperWin = 0x3bb;
		f.maxSLCpage = 64;
		//1個block=1024k, = 2048個sector
		f.maxLBA = f.maxLBperWin*1024*2;
		f.maxLBA = f.maxLBA * f.chips.size() * f.diePerChip() ;
		f.pageSize = 8;
		f.defectMarkColumnOffset = 0;

		f.evenDieStart=0;
		f.evenDieEnd=1042;
		if(f.diePerChip()==2){
			f.oddDieStart=2048;
			f.oddDieEnd=3090;
		}
	}

	if(f.is_TSB_43nmD2_LGA){
		f.maxLBperWin = 0x776;
		f.maxSLCpage = 64;
		//1個block=1024k, = 2048個sector
		f.maxLBA = f.maxLBperWin*1024*2;
		f.maxLBA = f.maxLBA * f.chips.size() * f.diePerChip()*2 ;
		f.evenDieEnd = 4148;
		f.evenDieStart = 0;
		f.oddDieStart = 0;
		f.oddDieEnd = 0;
		f.pageSize = 8;
		f.defectMarkColumnOffset = 0;

		if(f.diePerChip()==2){
			f.oddDieStart = 8192;
			f.oddDieEnd = 12340;
		}
	}


	if(f.is_TSB_32nmD2){
		f.maxLBperWin = 0x776;
		//1個block=1024k, = 2048個sector
		f.maxLBA = f.maxLBperWin*1024*2;
		f.maxLBA = f.maxLBA * f.chips.size() * f.diePerChip()*2 ;
		f.evenDieEnd = 4100;
		f.evenDieStart = 0;
		f.oddDieStart = 0;
		f.oddDieEnd = 0;
		f.pageSize = 8;
		f.defectMarkColumnOffset = 0;

		if(f.diePerChip()==2){
			f.oddDieStart = 8192;
			f.oddDieEnd = 12292;
		}
	}

	if(f.is_TSB_32nmD2 && (f.is_TSB_43nmD2_LGA==false) ){
		f.maxLBperWin = 0x776;
		//1個block=1024k, = 2048個sector
		f.maxLBA = f.maxLBperWin*1024*2;
		f.maxLBA = f.maxLBA * f.chips.size() * f.diePerChip()*2 ;
		f.evenDieEnd = 2076;
		f.evenDieStart = 0;
		f.oddDieStart = 0;
		f.oddDieEnd = 0;
		f.pageSize = 8;
		f.defectMarkColumnOffset = 0;

		if(f.diePerChip()==2){
			f.oddDieStart = 4096;
			f.oddDieEnd = 6172;
		}
	}

	if(f.is_TSB_32nmD2_LGA){
		f.maxLBperWin = 0x776;
		//1個block=1024k, = 2048個sector
		f.maxLBA = f.maxLBperWin*1024*2;
		f.maxLBA = f.maxLBA * f.chips.size() * f.diePerChip()*2 ;
		f.evenDieEnd = 4100;

		if( (f.chips[0].deviceCode==0xEE) && (f.chips[0].idData3==0x94) ){
			f.evenDieEnd = 8200;
		}

		f.evenDieStart = 0;
		f.oddDieStart = 0;
		f.oddDieEnd = 0;

		f.pageSize = 8;
		f.defectMarkColumnOffset = 0;

		if(f.diePerChip()==2){
			f.oddDieStart = 0;
			f.oddDieEnd = 0;
		}

		if( (f.chips[0].deviceCode==0xEE) && (f.chips[0].idData3==0x95) ){
			f.oddDieStart = 8192;
			f.oddDieEnd = 12292;
		}

	}

	if(f.is_TSB_24nmD2 || f.is_SanDisk_24nmD2){
		f.maxLBperWin = 0x776;
		f.evenDieStart = 0;
		f.oddDieStart = 0;
		f.oddDieEnd = 0;

		f.evenDieEnd = 4116;
		if( f.chips[0].idData3&0x03 ){
			throw MyException(0, "NO Support 2 die yet");
		}

		//f.pageSize = 8;
		f.defectMarkColumnOffset = 0;

		f.maxSLCpage = 0x40;
		f.maxMLCpage = 0x80; 
		f.blockLen = 256;

		f.chips[0].secPerPage = 0x10;
		f.chips[0].secPerBlock = 0x1000;

		f.is2PageMode = true;

		BYTE lowPage_2PageMode[0x40]=
		{ 
			0,  2,  6, 10, 14, 18, 22, 26, 30, 34, 38, 42, 46, 50, 54, 58,
				62, 66, 70, 74, 78, 82, 86, 90, 94, 98,102,106,110,114,118,122,
				126,130,134,138,142,146,150,154,158,162,166,170,174,178,182,186,
				190,194,198,202,206,210,214,218,222,226,230,234,238,242,246,250
		};

		memcpy( f.chips[0].lowPageTable, lowPage_2PageMode, sizeof(lowPage_2PageMode) );

	}

	if(f.is_TSB_24nmD2_128Page){
		f.maxLBperWin = 0x776;
		f.evenDieStart = 0;
		f.oddDieStart = 0;
		f.oddDieEnd = 0;

		f.evenDieEnd = 2056;
		if( f.chips[0].idData3&0x03 ){
			throw MyException(0, "NO Support 2 die yet");
		}

		//f.pageSize = 8;
		f.defectMarkColumnOffset = 0;

		f.maxSLCpage = 0x40;
		f.maxMLCpage = 0x80; 
		f.blockLen = 128;

		f.chips[0].secPerPage = 0x10;
		f.chips[0].secPerBlock = 0x800;

		f.is2PageMode = false;

		BYTE lowPage_2PageMode[0x40]= { 
			0,		1,		3,		5,		7,		9,		11,		13,		15,		17,		19,		21,		23,		25,		27,		29,
			31,		33,		35,		37,		39,		41,		43,		45,		47,		49,		51,		53,		55,		57,		59,		61,
			63,		65,		67,		69,		71,		73,		75,		77,		79,		81,		83,		85,		87,		89,		91,		93,
			95,		97,		99,		101,	103,	105,	107,	109,	111,	113,	115,	117,	119,	121,	123,	125
		};

		memcpy( f.chips[0].lowPageTable, lowPage_2PageMode, sizeof(lowPage_2PageMode) );

	}

	if( f.is_TSB_56nmD3==true){
		f.maxLBperWin = 0x776;
		f.maxSLCpage = 32;
		f.pageSize = 8;
		f.defectMarkColumnOffset = 0;

		f.maxLBA = f.maxLBperWin* f.pagePerBlock()* 8 *f.maxPlane;
		f.maxLBA = f.maxLBA * f.chips.size() * (f.evenDieEnd-f.evenDieStart)*f.diePerChip();

		f.evenDieStart=0;
		f.evenDieEnd= 2836;

		if(f.diePerChip()==2){
			f.oddDieStart=4096;
			f.oddDieEnd=6932;
		}
		//BYTE lowPage_56D3[0x40]=
		//{ 
		//	0x00,0x01,0x04,0x07,0x0A,0x0D,0x10,0x13,0x16,0x19,0x1C,0x1F,0x22,0x25,0x28,0x2B,
	 //       0x2E,0x31,0x34,0x37,0x3A,0x3D,0x40,0x43,0x46,0x49,0x4C,0x4F,0x52,0x55,0x58,0x5B,
  //          0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		//	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
		//};

		//memcpy( f.chips[0].lowPageTable, lowPage_56D3, sizeof(lowPage_56D3) );

	}

	if( f.is_TSB_43nmD3==true){
		f.maxLBperWin = 0x4FA;
		f.maxSLCpage = 64;
		//1個block=1024k, = 2048個sector
		f.maxLBA = f.maxLBperWin * f.chips[0].blockSize * 2;
		f.maxLBA = f.maxLBA * f.chips.size() * f.diePerChip();
		f.defectMarkColumnOffset = 0;
		f.pageSize = 8;

		f.evenDieStart=0;
		f.evenDieEnd= 1404;

		if(f.diePerChip()==2){
			f.oddDieStart=2048;
			f.oddDieEnd=3452;
		}

		if( f.is_TSB_43nmD3_Special){
			// TSB 32Gb 98,D7,98,B2,76,54
			if( f.chips[0].deviceCode == 0xd7){
				f.evenDieStart = 0;
				f.evenDieEnd = 2780;
				if(f.diePerChip()==2){
					throw MyException(FLASH_SET_PRIPERTY_FAIL, "not support");
					f.oddDieStart=4096;
					f.oddDieEnd=6876;
				}
			}
		}
	}

	


	if( f.is_IM_34nmD2){
		f.blockLen = 128;
		//f.maxPlane=1;
		f.maxLBperWin = 955;
		f.maxSLCpage = 64;
		f.defectMarkColumnOffset = 0x1000;
		f.pageSize = 4;
		

		//lba = totalWin * secCntPerWin
		f.maxLBA = (f.totalSize/512)* (f.maxLBperWin*1024);
		f.evenDieStart=0;
		f.evenDieEnd=8192;
		if(f.diePerChip()==2){
			f.oddDieStart=8192;
			f.oddDieEnd=16384;
		}
        
		BYTE IMD2_lowPage[0x40]=
		{ 
			0x00,0x01,0x06,0x07,0x0A,0x0b,0x0e,0x0f,0x12,0x13,0x16,0x17,0x1a,0x1b,0x1e,0x1f,
	        0x22,0x23,0x26,0x27,0x2a,0x2b,0x2e,0x2f,0x32,0x33,0x36,0x37,0x3a,0x3b,0x3e,0x3f,
            0x42,0x43,0x46,0x47,0x4a,0x4b,0x4e,0x4f,0x52,0x53,0x56,0x57,0x5a,0x5b,0x5e,0x5f,
            0x62,0x63,0x66,0x67,0x6a,0x6b,0x6e,0x6f,0x72,0x73,0x76,0x77,0x7a,0x7b,0x7e,0x7f

		};

		memcpy( f.chips[0].lowPageTable, IMD2_lowPage, sizeof(IMD2_lowPage) );
	}

	if( f.is_IM_50nmD2){
		f.blockLen = 128;
		//f.maxPlane=1;
		f.maxLBperWin = 955;
		f.maxSLCpage = 64;
		f.maxPlane = 2;
		f.defectMarkColumnOffset = 0x1000;
		f.pageSize = 4;

		//lba = totalWin * secCntPerWin
		f.maxLBA = (f.totalSize/512)* (f.maxLBperWin*1024);
		f.evenDieStart=0;
		f.evenDieEnd=4096;
		if(f.diePerChip()==2){
			f.oddDieStart=4096;
			f.oddDieEnd=8192;
		}
        
		BYTE IMD2_lowPage[0x40]=
		{ 
			0x00,0x01,0x06,0x07,0x0A,0x0b,0x0e,0x0f,0x12,0x13,0x16,0x17,0x1a,0x1b,0x1e,0x1f,
	        0x22,0x23,0x26,0x27,0x2a,0x2b,0x2e,0x2f,0x32,0x33,0x36,0x37,0x3a,0x3b,0x3e,0x3f,
            0x42,0x43,0x46,0x47,0x4a,0x4b,0x4e,0x4f,0x52,0x53,0x56,0x57,0x5a,0x5b,0x5e,0x5f,
            0x62,0x63,0x66,0x67,0x6a,0x6b,0x6e,0x6f,0x72,0x73,0x76,0x77,0x7a,0x7b,0x7e,0x7f

		};

		memcpy( f.chips[0].lowPageTable, IMD2_lowPage, sizeof(IMD2_lowPage) );
	}

	

	if( f.is_IM_34nmD2_256Page){
		f.blockLen = 256;
		f.maxLBperWin = 0x776;
		f.maxSLCpage = 64;
		f.defectMarkColumnOffset = 0x1000;
		f.pageSize = 4;
		f.chips[0].secPerPage = 8;
		f.chips[0].secPerBlock = 8*256;

		f.is2PageMode = true;

		//lba = totalWin * secCntPerWin
		f.maxLBA = (f.totalSize/512)* (f.maxLBperWin*1024);
		f.evenDieStart=0;
		f.evenDieEnd=2048;
		if(f.diePerChip()==2){
			f.oddDieStart=2048;
			f.oddDieEnd=4096;
		}

		if( f.chips[0].size == 4096){
			f.evenDieStart=0;
			f.evenDieEnd=4096;
			if(f.diePerChip()==2){
				f.oddDieStart=4096;
				f.oddDieEnd=8192;
			}
		}

        
		BYTE IMD2_lowPage[0x40]=
		{ 
			0,  2,  6, 10, 14, 18, 22, 26, 30, 34, 38, 42, 46, 50, 54, 58,
				62, 66, 70, 74, 78, 82, 86, 90, 94, 98,102,106,110,114,118,122,
				126,130,134,138,142,146,150,154,158,162,166,170,174,178,182,186,
				190,194,198,202,206,210,214,218,222,226,230,234,238,242,246,250
		};

		memcpy( f.chips[0].lowPageTable, IMD2_lowPage, sizeof(IMD2_lowPage) );
	}


	if(f.is_SanDisk_43nmD2){
		if(f.is16_IO){
			f.blockLen = 128;
			f.maxLBperWin = 955;
			f.maxSLCpage = 64;
			f.defectMarkColumnOffset = 0x1000;
			f.pageSize = 4;

			f.evenDieStart=0;
			f.evenDieEnd=2084;
			if(f.diePerChip()==2){
				f.oddDieStart=4096;
				f.oddDieEnd=6180;
			}
		}
	}

	if(f.is_Hynix_32nmD2_256Page){
		f.blockLen = 256;
		f.maxLBperWin = 0x776;
		f.maxSLCpage = 64;
		f.defectMarkColumnOffset = 0x1000;
		f.pageSize = 8;

		f.is2PageMode = true;

		f.evenDieStart=0;
		f.evenDieEnd=2048;
		if(f.diePerChip()==2){
			f.oddDieStart=2048;
			f.oddDieEnd=4096;
		}
	}


	// for IM 28nm D2
	if( (f.chips[0].makerCode == FLASHID_INTEL) ){
		if( f.chips[0].nanometer == 28){
			if( f.secPerPage() == 0x10){
				// for algorithm side
				f.maxSLCpage = 0x40;
				f.maxMLCpage = 0x80; 
				f.blockLen = 256;
				f.evenDieStart = f.chips[0].dieStart_1st ;
				f.evenDieEnd = f.chips[0].dieEnd_1st+1;
				f.defectMarkColumnOffset = f.chips[0].defectMarkColumnOffset;

				f.is2PageMode = true;
			}
		}
	}

	// for Micron L74A
	if( (f.chips[0].makerCode == FLASHID_MICRON) ){
		if( f.chips[0].nanometer == 28){
			if( f.secPerPage() == 0x10){
				// for algorithm side
				f.maxSLCpage = 0x40;
				f.maxMLCpage = 0x80; 
				f.blockLen = 256;
				f.evenDieStart = f.chips[0].dieStart_1st ;
				f.evenDieEnd = f.chips[0].dieEnd_1st+1;
				f.defectMarkColumnOffset = f.chips[0].defectMarkColumnOffset;

				f.is2PageMode = true;
			}
		}
	}

	// for Hynix 32nm D2
	if(f.chips[0].makerCode == FLASHID_HYNIX){
		if( f.chips[0].nanometer == 32){
			if( f.secPerPage() == 0x10){
				// for algrithm side
				f.maxSLCpage = 0x40;
				f.maxMLCpage = 0x80; 
				f.blockLen = 256;
				f.evenDieStart = f.chips[0].dieStart_1st ;
				f.evenDieEnd = f.chips[0].dieEnd_1st+1;
				f.defectMarkColumnOffset = f.chips[0].defectMarkColumnOffset;
			}
		}
	}



	if( f.is_Samsuang_32nmD2==true){
		f.maxLBperWin = 0x776;
		f.maxSLCpage = 64;
		f.blockLen = 128;

		//1個block=1024k, = 2048個sector
		f.maxLBA = f.maxLBperWin*1024*2;
		f.maxLBA = f.maxLBA * f.chips.size() * f.diePerChip() ;
		f.pageSize = 8;
		f.defectMarkColumnOffset = 0;

		f.evenDieStart=0;
		f.evenDieEnd=2076;
		if(f.diePerChip()==2){
			f.oddDieStart=4096;
			f.oddDieEnd=9172;
		}

		BYTE lowPageTb[0x40];

		for(int i=2; i<0x40; i++){
			lowPageTb[i] = 2*(i-1) +1;
		}
		lowPageTb[0]=0;
		lowPageTb[1]=1;

		memcpy( f.chips[0].lowPageTable, lowPageTb, sizeof(lowPageTb) );
	}



	if( f.is_Samsuang_32nmD3==true){
		f.maxLBperWin = 0x776;
		f.maxSLCpage = 64;
		f.blockLen = 256;
		//1個block=1024k, = 2048個sector
		f.maxLBA = f.maxLBperWin*1024*2;
		f.maxLBA = f.maxLBA * f.chips.size() * f.diePerChip() ;
		f.pageSize = 8;
		f.defectMarkColumnOffset = 0;

		f.evenDieStart=0;
		f.evenDieEnd=2804;
		if(f.diePerChip()==2){
			f.oddDieStart=4096;
			f.oddDieEnd=6900;
		}

		BYTE lowPageTb[0x40];

		for(int i=2; i<0x40; i++){
			lowPageTb[i] = 3*(i-1);
		}
		lowPageTb[0]=0;
		lowPageTb[1]=1;

		memcpy( f.chips[0].lowPageTable, lowPageTb, sizeof(lowPageTb) );
	}

	if( f.is_Samsuang_35nmD2_LGA==true){
		f.maxLBperWin = 0x776;
		f.maxSLCpage = 64;
		f.blockLen = 128;
		f.pageSize = 8;
		f.defectMarkColumnOffset = 0;

		f.evenDieStart=0;
		f.evenDieEnd=4152;
		//if(f.diePerChip()==2){
		//	//f.oddDieStart=4096;
		//	//f.oddDieEnd=6900;
		//}

		BYTE lowPageTb[0x40];

		for(int i=1; i<0x40; i++){
			lowPageTb[i] = 1 + 2*(i-1);
		}
		lowPageTb[0]=0;
		memcpy( f.chips[0].lowPageTable, lowPageTb, sizeof(lowPageTb) );
	}




}

Chip::Chips Flash::getChips_8ByteFormat(const BYTE* flashIdTb){
	Chip::Chips chips;
	BYTE const emtpyChar= 0xFF;
	BYTE const emtpyChar2= 0x00;//for Hynix

	int base,i;
	for(i=0; i<33; i++){
		base = i*9;
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
			c.load(flashIdTb+base+1);
			chips.push_back(c);
		}catch(MyException& e){
			string errorMsg("Can not create chip object \r\n");
			errorMsg += e.what();
			throw MyException(FLASH_GETCHIPS_8BYTE_FORMAT_FAIL, errorMsg);
		}
	}
	return chips;
}

ModeType Flash::getModeType_8ByteFormat(BYTE* flashIdTb){
	if( (flashIdTb[0x0A]==0xFF) && (flashIdTb[0x0B]==0xFF)){
		return ModeType::byteMode();
	}
	if( memcmp(flashIdTb+1,flashIdTb+0x0A, 8) != 0){
		throw MyException(FLASH_GET_MMODE_TYPE_8BYTE_FORMAT_FAIL, "Flash Id Table Error: please check flash is ALL the same");
	}else{
		return ModeType::wordMode();
	}
}

void Flash::loadAge(BYTE* flashIdTb){
	BYTE maker = flashIdTb[1];
	BYTE device = flashIdTb[2];
	BYTE id3 = flashIdTb[3];
	BYTE id4 = flashIdTb[4];
	BYTE id5 = flashIdTb[5];
	BYTE id6 = flashIdTb[6];

	_judge_TSB_Age(maker, device, id3, id4, id5, id6);
	_judge_IM_Age(maker, device, id3, id4, id5, id6);
	_judge_sandisk_Age(maker, device, id3, id4, id5, id6);
	_judge_Hynix_Age(maker, device, id3, id4, id5, id6);
	_judge_Samsung_Age(maker, device, id3, id4, id5, id6);
}

void Flash::_judge_Samsung_Age(BYTE maker, BYTE device, BYTE id3, BYTE id4, BYTE id5, BYTE id6){
	
	//32nm d3
	if( maker==0xEC && device==0xD7, id3==0x98 && id4==0xC6 && id5==0x54  ){
		this->is_Samsuang_32nmD3=true;
	}

	//35nm d2 LGA (EC D7 94 72 54 42 EC D7 00)
	if( maker==0xEC && device==0xD7, id3==0x94 && id4==0x72 && id5==0x54  ){
		this->is_Samsuang_35nmD2_LGA=true;
	}

	//35nm d2  
	if( maker==0xEC && device==0xD5, id3==0x84 && id4==0x72 && id5==0x50  ){
		this->is_Samsuang_32nmD2=true;
	}


}

void Flash::_judge_TSB_Age(BYTE maker, BYTE device, BYTE id3, BYTE id4, BYTE id5, BYTE id6){
	//43nm d2
	if( maker==0x98 && id3==0x94 && id4==0x32  ){
		this->is_TSB_43nmD2=true;
	}

	if( maker==0x98 && id3==0x95 && id4==0x32  ){
		this->is_TSB_43nmD2=true;
	}

	if( maker==0x98 && id3==0x98 && id4==0x83  ){
		this->is_TSB_43nmD2=true;
	}

	//TSB 43nm D2 1Gb
	if( (maker==0x98) && (device==0xD3) && (id3==0x84) && (id4==0x32) && (id5==0x72) ){
		is_TSB_43nmD2 = true;
		is_TSB_43nmD2_8Gb = true;
	}

	//TSB 56nmD3
	if( maker==0x98 && id3==0x98 && id4==0x83 ){
		this->is_TSB_56nmD3=true;
	}

	// TSB 43nm LGA
	if( (maker==0x98) && (device==0xD7) && (id3==0x94) && (id4==0x32) && (id5==0x76)){
		is_TSB_43nmD2_LGA = true;			
	}

	// eddie comment: we have no idea to know while one is LGA
	if( (maker==0x98) && (device==0xDE) && (id3==0x95) && (id4==0x32) && (id5==0x7A)){
		is_TSB_43nmD2_LGA = true;			
	}



	// TSB 32nm D2
	if( (maker==0x98) && (device==0xD7) && (id3==0x94) && (id4==0x32) && (id5==0x76) && (id6==0x55)){
		is_TSB_43nmD2_LGA = true;	
		is_TSB_32nmD2 = true;
	}

	// TSB 32nm D2
	if( (maker==0x98) && (device==0xD5) && (id3==0x94) && (id4==0x32) && (id5==0x76) && (id6==0x55)){
		is_TSB_32nmD2 = true;
	}

	//TSB 43nm D3
	if( (maker==0x98) && (device==0xD5) && (id3==0x98) && (id4==0xb2) && (id5==0x76) && (id6==0x54)){
		is_TSB_43nmD3 = true;
	}

	//TSB 43nm D3
	if( (maker==0x98) && (device==0xD7) && (id3==0x99) && (id4==0xb2) && (id5==0x7a) && (id6==0x54)){
		is_TSB_43nmD3 = true;
	}

	//TSB 43nm D3
	if( (maker==0x98) && (device==0xD7) && (id3==0x98) && (id4==0xb2) && (id5==0x76) && (id6==0x54)){
		is_TSB_43nmD3 = true;
		is_TSB_43nmD3_Special = true;
	}

	// TSB 32nm D2 LGA
	if( (maker==0x98) && (device==0xE7) && (id3==0x94) && (id4==0x32) && (id5==0x76) && (id6==0x55)){
		is_TSB_32nmD2_LGA = true;
	}

	// TSB 32nm D2 LGA
	if( (maker==0x98) && (device==0xEE) && (id3==0x94) && (id4==0x32) && (id5==0x76) && (id6==0x55)){
		is_TSB_32nmD2_LGA = true;
	}

	// TSB 32nm D2 LGA
	if( (maker==0x98) && (device==0xEE) && (id3==0x95) && (id4==0x32) && (id5==0x7a) && (id6==0x55)){
		is_TSB_32nmD2_LGA = true;
	}
	// TSB 32nm D2 LGA
	if( (maker==0x98) && (device==0x3A) && (id3==0x96) && (id4==0x32) && (id5==0x7E) && (id6==0x55)){
		is_TSB_32nmD2_LGA = true;
	}

	// TSB 24nm D2
	if( (maker==0x98) && (device==0xDE) && (id3==0x94) && (id4==0x82) && (id5==0x76) && (id6==0x56)){
		is_TSB_24nmD2 = true;
	}

	// TSB 24nm D2
	if( (maker==0x98) && (device==0xD5) && (id3==0x84) && (id4==0x32) && (id5==0x72) && (id6==0x56)){
		is_TSB_24nmD2_128Page = true;
	}
}

void Flash::_judge_sandisk_Age(BYTE maker, BYTE device, BYTE id3, BYTE id4, BYTE id5, BYTE id6){
	// sanDisk
	if( maker==0x45 && id3==0x94 && id4==0x32  ){
		this->is_TSB_43nmD2=true;
		this->is_SanDisk_43nmD2=true;
	}

	if( maker==0x45 && device==0xC7 && id3==0xd5 && id4==0x72 && id5==0x7A  ){
		this->is_SanDisk_43nmD2=true;
		this->is16_IO=true;
	}

	if( maker==0x45 && id3==0x95 && id4==0x32 ){
		this->is_TSB_43nmD2=true;
		this->is_SanDisk_43nmD2=true;
	}

	if( maker==0x45 && device==0xC7 && id3==0x99 && id4==0x83 && id5==0xAA ){
		this->is_SanDisk_56nmD3=true;
		this->is_TSB_56nmD3=true;
	}


	if( maker==0x45 && device==0xC5 && id3==0x98 && id4==0xB2 && id5==0x76 ){
		this->is_TSB_43nmD3=true;
		this->is_SanDisk_43nmD3=true;
	}
		
	if( maker==0x45 && device==0xDE && id3==0x94 && id4==0x82 && id5==0x76 ){
		this->is_TSB_24nmD2=true;
		this->is_SanDisk_24nmD2=true;
	}

	if( maker==0x45 && device==0xD7 && id3==0x94 && id4==0x32 && id5==0x76 ){
		this->is_TSB_32nmD2 = true;
		this->is_TSB_32nmD2_LGA=true;
		this->is_SanDisk_32nmD2 = true;
	}
}

void Flash::_judge_IM_Age(BYTE maker, BYTE device, BYTE id3, BYTE id4, BYTE id5, BYTE id6){
	// IM Flash(34nmD2)
	if(maker==0x2C && id3==0x94 && id4==0x3E && id5==0x84 ){		
		is_IM_34nmD2 = true;
	}

	//im 34nm 32/64/128Gb
	if(maker==0x89 && id3==0x94 && id4==0x3E && id5==0x84 ){		
		is_IM_34nmD2 = true;
	}

	//i34 m128Gb
	if(maker==0x2C && id3==0xd5 && id4==0x3E && id5==0x88 ){		
		is_IM_34nmD2 = true;
	}

	
	if( maker==0x89 && id3==0x94 && id4==0x3E && id5==0x74 ){		
		is_IM_34nmD2 = true;
	}

	//im 50nm
	if( maker==0x2C && device==0xD5 && id3==0xD5 && id4==0x3E && id5==0x78 ){		
		is_IM_50nmD2 = true;
		is_IM_34nmD2 = false;
	}

	//im 50nm
	if( maker==0x2C && device==0xD5 && id3==0x94 && id4==0x3E && id5==0x74 ){		
		is_IM_50nmD2 = true;
		is_IM_34nmD2 = false;
	}

	if( maker==0x89  && id3==0x94 && id4==0x3E && id5==0x74 ){		
		is_IM_50nmD2 = true;
		is_IM_34nmD2 = false;
	}

	if( ( maker==0x89 || maker==0x2c)  && device==0x48 && id3==0x04 && id4==0x46 && id5==0x85 ){		
		is_IM_34nmD2_256Page = true;
	}
	if( ( maker==0x89 || maker==0x2c)  && device==0x68 && id3==0x04 && id4==0x46 && id5==0x89 ){		
		is_IM_34nmD2_256Page = true;
	}
	if( ( maker==0x89 || maker==0x2c)  && device==0x68 && id3==0x04 && id4==0x46 && id5==0xA9 ){		
		is_IM_34nmD2_256Page = true;
	}

	//im 23nm
	if( (maker==0x2C||maker==0x88)  && device==0x88 && (id3==0x04||id3==0x24) && id4==0x4B && id5==0xA9 ){		
		is_IM_28nmD2_256Page = true;
		winCntPerPlane = 2;
	}
}

void Flash::_judge_Hynix_Age(BYTE maker, BYTE device, BYTE id3, BYTE id4, BYTE id5, BYTE id6){
	if( maker==0xAD  && device==0xd7 && id3==0x94 && id4==0x9A && id5==0x74){
		is_Hynix_32nmD2_256Page = true;
	}
}

void Flash::getLowPageCheckTable(BYTE* ary){
	if( this->is_TSB_56nmD3 || this->is_TSB_43nmD3){
		// for D3 homo Seed
		BYTE TSB_56D3_RCT[0x80]={0};

		for(int i=4; i<92; i+=3){
			TSB_56D3_RCT[i+0]=0x00;
			TSB_56D3_RCT[i+1]=0x01;
			TSB_56D3_RCT[i+2]=0x54;

		}
		TSB_56D3_RCT[93]=0x00;

		TSB_56D3_RCT[0]=0x00;
		TSB_56D3_RCT[1]=0x00;
		TSB_56D3_RCT[2]=0x01;
		TSB_56D3_RCT[3]=0x03;
		memcpy(ary, TSB_56D3_RCT, sizeof(TSB_56D3_RCT) );
		return ;
	}

	if( this->is_TSB_43nmD2){
		BYTE ckTb[128]={0};
		int i;
		for(i=0; i<128; i++){
			if( (i%2)==0){
				ckTb[i]=0x03;
			}
		}
		ckTb[0]=0;
		ckTb[2]=2;
		ckTb[127]=2;
		memcpy(ary, ckTb, sizeof(ckTb) );
		return;
	}

	if( this->is_IM_34nmD2){
		BYTE lowPageCheckTb_128[0x80]={
			0X00,0X00,0X00,0X00,0x00,0x00,0X34,0X45,0x00,0x00,0X56,0X67,0x00,0x00,0X56,0X67,
				0x00,0x00,0X56,0X67,0x00,0x00,0X56,0X67,0x00,0x00,0X56,0X67,0x00,0x00,0X56,0X67,
				0x00,0x00,0X56,0X67,0x00,0x00,0X56,0X67,0x00,0x00,0X56,0X67,0x00,0x00,0X56,0X67,
				0x00,0x00,0X56,0X67,0x00,0x00,0X56,0X67,0x00,0x00,0X56,0X67,0x00,0x00,0X56,0X67,
				0x00,0x00,0X56,0X67,0x00,0x00,0X56,0X67,0x00,0x00,0X56,0X67,0x00,0x00,0X56,0X67,
				0x00,0x00,0X56,0X67,0x00,0x00,0X56,0X67,0x00,0x00,0X56,0X67,0x00,0x00,0X56,0X67,
				0x00,0x00,0X56,0X67,0x00,0x00,0X56,0X67,0x00,0x00,0X56,0X67,0x00,0x00,0X56,0X67,
				0x00,0x00,0X56,0X67,0x00,0x00,0X56,0X67,0x00,0x00,0X56,0X67,0x00,0x00,0X56,0X67
		};
		memcpy(ary, lowPageCheckTb_128, sizeof(lowPageCheckTb_128) );
		return;
	}
}

BYTE Flash::getPhyPage(BYTE logicPage) const{
	//int threshold=0;
	//if( this->levelCellType() == LevelCellType::TLC() ){
	//	threshold=32;
	//}else{
	//	threshold=64;
	//}
	//if(logicPage>=threshold){
	//	throw MyException(PF_CONVERT_LOWPAGE_ERROR, "logicPage(%X)>=threshold(%X)", logicPage, threshold);
	//}
	return this->chips[0].lowPageTable[logicPage];
}



bool Flash::getCETab(BYTE* ceTab, BYTE* rdyTab, BYTE* buf) {
	// 20101001 for 2092 auto-CE function
	// 要搭配 2092 vendor-command FF-30 來使用
	// 使 CE No 的分布可以依照 rdy0,1,0,1....來排序

	int evenceidx(0), oddceidx(1), ffCnt(0);
	for (int ceCnt=0;ceCnt<8;ceCnt++) {
		if (buf[ceCnt]==0 && ((ceCnt%2)==0)) {
			// 當偶數的ce搭配 rdy0 時
			ceTab[evenceidx]  = ceCnt;
			rdyTab[evenceidx] = buf[ceCnt];
			evenceidx+=2;
		}
		else if ((buf[ceCnt]==1) && ((ceCnt%2)==0)) {
			// 當偶數的ce搭配 rdy1 時
			ceTab[oddceidx]  = ceCnt;
			rdyTab[oddceidx] = buf[ceCnt];
			oddceidx+=2;
		}
		else if ((buf[ceCnt]==0) && ((ceCnt%2)==1)) {
			// 當奇數的ce搭配 rdy0 時
			ceTab[evenceidx]  = ceCnt;
			rdyTab[evenceidx] = buf[ceCnt];
			evenceidx+=2;
		}
		else if ((buf[ceCnt]==1) && ((ceCnt%2)==1)) {
			// 當奇數的ce搭配 rdy1 時
			ceTab[oddceidx]  = ceCnt;
			rdyTab[oddceidx] = buf[ceCnt];
			oddceidx+=2;
		}
		else
			ffCnt++;
	}

	//
	if (oddceidx-evenceidx!=1 && ffCnt<7) {
		return true;
	}
	return false;
}



