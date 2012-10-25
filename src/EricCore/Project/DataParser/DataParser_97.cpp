#include "StdAfx.h"
#include ".\dataparser_97.h"

using namespace EricCore;

DataParser_97::DataParser_97(void)
{
}

DataParser_97::~DataParser_97(void)
{
}

tstring DataParser_97::makeRam0String(BYTE* ram0){
	tstring result;
	result += "CycleList = " + Utility::arrayToHexString(ram0+0x55, 4) + Utility::CrLf();

	result += "now_window   = " + Utility::toHexString(ram0[0x40]) + Utility::CrLf();
	result += "cur_window   = " + Utility::toHexString(ram0[0x5E]) + Utility::CrLf();
	result += "cur_zone     = " + Utility::toHexString(ram0[0x5F]) + Utility::CrLf();
	result += "LB           = " + Utility::toHexString(ram0[0x60]) + Utility::CrLf();

	WORD csegSp = Utility::arrayToWord(ram0+0x61);
	tstring csegSpInfo = Utility::strFormat("  (%X/3 = %X, remain=%X)", csegSp, csegSp/3, csegSp%3);

	result += "CSegSP       = " + Utility::toHexString(csegSp) + csegSpInfo + Utility::CrLf();
	result += "CSegSP_Plane = " + Utility::toHexString(ram0[0x63]) + Utility::CrLf();
	result += "CSegSP_Sec   = " + Utility::toHexString(ram0[0x64]) + Utility::CrLf();


	BYTE globle_flag = ram0[0x2A];
	result += Utility::CrLf();
	result += "sram cache   = "  + Utility::toString( Utility::toBool( globle_flag&BIT4 ) ) + Utility::CrLf();
	result += "cache_window = "  + Utility::toHexString( ram0[0x45]) + Utility::CrLf();
	result += "cache_zone   = "  + Utility::toHexString( ram0[0x46]) + Utility::CrLf();
	result += "cache_LB     = "  + Utility::toHexString( ram0[0x47]) + Utility::CrLf();
	
	WORD cache_csegSp = Utility::arrayToWord(ram0+0x48);
	result += "cache_CSegSP = " + Utility::toHexString( cache_csegSp ) + Utility::CrLf();
	result += "cache_CSegSP_Plane = " + Utility::toHexString( ram0[0x4A]) + Utility::CrLf();
	result += "cache_CSegSP_Sec   = " + Utility::toHexString( ram0[0x4B]) + Utility::CrLf();

	result += Utility::CrLf();
	result += "error_code = " + Utility::toHexString( ram0[0x31]) + Utility::CrLf();
	result += "total_W = " + Utility::toHexString( ram0[0x39]) + Utility::CrLf();
	result += "spare_block_number = " + Utility::toHexString( ram0[0x33]) + Utility::CrLf();
	result += "empty_spare_space = " + Utility::toHexString( ram0[0x36]) + Utility::CrLf();

	result += Utility::CrLf();
	result += "read_Q_index = " + Utility::toHexString(ram0[0x75]) +  Utility::CrLf();
	result += "read_Q_zone  = " + Utility::toHexString(ram0[0x76]) + Utility::CrLf();

	result += Utility::CrLf();
	result += "min_age_spare      = " + Utility::toHexString(ram0[0x34]) +  Utility::CrLf();
	result += "max_age_spare      = " + Utility::toHexString(ram0[0x35]) + Utility::CrLf();
	result += "empty_spare_space  = " + Utility::toHexString(ram0[0x36]) + Utility::CrLf();
	result += "min_age_MLC_spare  = " + Utility::toHexString(ram0[0x37]) +  Utility::CrLf();
	result += "max_age_MLC_spare  = " + Utility::toHexString(ram0[0x38]) + Utility::CrLf();


	//result += "swap_endptr = " + Utility::toHexString(ram0[0x34]) + Utility::CrLf();
	//result += "max_SLC_page = " + Utility::toHexString(ram0[0x37]) + Utility::CrLf();
	//result += "max_plane = " + Utility::toHexString(ram0[0x38]) + Utility::CrLf();
	//result += "max_MLC_page = " + Utility::toHexString(ram0[0x39]) + Utility::CrLf();


	//result += "total_W = " + Utility::toHexString(ram0[0x44]) + Utility::CrLf();
	//result += "error_code = " + Utility::toHexString(ram0[0x6A]) + Utility::CrLf();

	return result;
}

tstring DataParser_97::checkSpareAge_AndSpareTb(BYTE* ramC000){
	
	// Spare Age Table is equal Spare table?
	tstring msg;
	for( int i=0; i<0xB0; i++){
		if( ramC000[0xD00+i] !=0xFF ){
			if( ramC000[0x200+i*0x10] == 0xFF){
				msg+="SpareTableError(" + Utility::toHexString(i) + ")" + Utility::CrLf();
			}
		}
	}
	return msg;
}

tstring DataParser_97::check_CTable(BYTE* ramB000, BYTE* ramC000){
	
	tstring msg;
	
	// ( eD3 )
	const WORD mlcPageCnt = 258;
	const BYTE slcPageCnt = 128 ;

	//const BYTE offset_H   = 86 ;
	//const BYTE offset_L   = 86 ;

	WORD mapOffset[5][slcPageCnt]={0xFFFF};
	WORD mlc_pageCnt = getPageNo(mlcPageCnt, ramB000);

	if( mlc_pageCnt > 0x100){
		return msg;
	}


	BYTE map[5][slcPageCnt]={0xFF};
	for( int wcNo=0; wcNo<5; wcNo++){
		memset(map, 0xFF, sizeof(map));

		int offset = 0xF00 + wcNo*0x20;
		BYTE* pBuf=0;

		//is W enable
		if( ramC000[offset]==0xFF){continue;}

		WORD cTableoffset = Utility::arrayToWord( ramB000+0xF70+wcNo*2  );

		//get cTableAddr
		if( cTableoffset<0xB000){
			msg += Utility::strFormat("have ram9000?");
			return msg;

			//pBuf = ram9000;
		}else{
			pBuf = ramB000;
		}

		cTableoffset&= 0x0FFF;

		// check CTable
		for( int pageNo=0; pageNo<mlc_pageCnt; pageNo++){
			BYTE cBlockNo =     pBuf[cTableoffset+pageNo*2];
			BYTE cBlockPageNo = pBuf[cTableoffset+pageNo*2+1];
			WORD badOffset = cTableoffset+pageNo*2;

			//is D-block, then continue
			if(cBlockNo==0x40){	continue;}

			//is All-0, then continue
			if(cBlockNo==0x00){	continue;}

			// I have no idea to detect totalPageNo
			if(cBlockPageNo==0xFF){	continue;}


			cBlockNo&=0x0F;

			// C-BlockNo should be less 5
			if( cBlockNo>5){
				msg += Utility::strFormat("cBlockPageNo over 5 in offset(0xB%03X)", badOffset) + Utility::CrLf();
				continue;
			}

			if( cBlockPageNo >=slcPageCnt ){
				msg += Utility::strFormat("cBlockPageNo over 0x80 in offset(0xB%03X)", badOffset) + Utility::CrLf();
				continue;
			}

			if( map[cBlockNo][cBlockPageNo] == 0xFF ){
				map[cBlockNo][cBlockPageNo]=0;
				mapOffset[cBlockNo][cBlockPageNo]=badOffset;
			}else{
				WORD beforeOffset = mapOffset[cBlockNo][cBlockPageNo];
				msg+=Utility::strFormat("find duplicate in C-table offset(0xB%03X - 0xB%03X)", beforeOffset, badOffset) + Utility::CrLf();

			}
		}



		// check CTable and CUsed Cnt
		for(int cBlkNo=0; cBlkNo<5; cBlkNo++){
			WORD used=0;
			for(int i=0; i<slcPageCnt; i++){
				if( map[cBlkNo][i] != 0xFF){
					used++;
				}
			}
			WORD cUsedCnt = ramC000[offset+0x06+cBlkNo];
			if( cUsedCnt == 0xFF){
				cUsedCnt=0;
			}

			if( cUsedCnt != used){
				msg += Utility::strFormat("Find diff cUsedCnt in W(%d-%d), cUsedCnt=%X, nowUse=%X", wcNo, cBlkNo, cUsedCnt, used );
				msg += Utility::CrLf();
			}
		}

	}

	return msg;
}

tstring DataParser_97::checkC000(BYTE* ram9000, BYTE* ramA000, BYTE* ramB000, BYTE* ramC000){
	tstring msg;
	if( ramC000!=0 ){
		msg += checkSpareAge_AndSpareTb(ramC000);
		msg += checkMlcTable(ramC000);
		msg+= _checkWrongCurC_Ptr(ramC000);
		msg+= _checkSpareTableAndCnt(ramC000);
	}

	if( ramB000!=0 ){
		msg += check_CTable(ramB000, ramC000);
	}

	if( ramA000!=0 ){
		msg += check_duplicateBlock(ramC000, ramA000);
	}

	return msg;
}

tstring DataParser_97::_checkWrongCurC_Ptr(BYTE* ramC000){
	tstring msg;
	

	//for(int wNo=0; wNo<7; wNo++){
	//	int wPtr = 0xF00 + wNo*0x20;

	//	BYTE enable = ramC000[wPtr];
	//	if( enable == 0xFF){
	//		continue;
	//	}

	//	int cCnt = 0;
	//	for(int i=0; i<5; i++){
	//		int offset = 0x14 + i*2;
	//		if( i>=2){
	//			offset = 0x1A + (i-2)*2;
	//		}

	//		BYTE c = ramC000[wPtr+offset];
	//		if( c != 0xFF){
	//			cCnt++;
	//		}
	//	}

	//	if(cCnt==5){
	//		continue;
	//	}
	//	BYTE curC_No = ramC000[wPtr+2];
	//	if(curC_No==0xFF){
	//		continue;
	//	}
	//	
	//	if( cCnt != (curC_No+1) ){
	//		tstring tmp = Utility::strFormat("curC Ptr(%X) != curC-block at W(%X)", cCnt, wNo) + Utility::CrLf();
	//		msg += tmp;
	//	}
	//}
	return msg;
}

tstring DataParser_97::_checkSpareTableAndCnt(BYTE* ramC000){
	tstring msg;
	BYTE spareCnt = ramC000[0xDC0];

	BYTE tableCnt=0;
	for(int i=0; i<176; i++){
		BYTE tmp = ramC000[0xD00+i];
		if( (tmp&BIT7)==0 ){
			tableCnt++;
		}
	}

	if( tableCnt!=spareCnt){
		msg += Utility::strFormat("SpareTbCnt(0xCDC0) != 0xCD00's TbCnt(0x%X!=0x%X), Please check", spareCnt, tableCnt) + Utility::CrLf(); 
	}
	return msg;
}

tstring DataParser_97::makeC000_info(BYTE* ram9000, BYTE* ramA000, BYTE* ramB000, BYTE* ramC000 ){

	tstring msg,temp;

	WORD wcStartAddr = 0xF00;	
	int offset;
	tstring dBlockMsg;
	for(int wcNo=0; wcNo<7; wcNo++){
		temp = Utility::strFormat("%d)", wcNo) ;
		offset = wcStartAddr + wcNo*0x20;
		BYTE type = *(ramC000+offset+0x10);

		msg+= temp ;
		msg+= "Param=" +     Utility::arrayToHexString(ramC000 + offset + 0x00, 0x10) + Utility::CrLf();
		msg+= "  3sBlk=" +  Utility::arrayToHexString(ramC000 + offset + 0x10, 0x10) + Utility::CrLf();
		
		msg+= "  Type =" +  Utility::toHexString( type );
		msg+= ",curC=" +  Utility::toHexString( ramC000[offset+0x2] );
		msg+= ",Cend=" +  Utility::toHexString( ramC000[offset+0x3] );
		msg+= ",CUsed=" +  Utility::arrayToHexString( ramC000+offset+0x6, 5 ) + Utility::CrLf();


		if( type==0x40){
			msg+= "  C0   =" +  Utility::toHexString( Utility::arrayToWord(ramC000+offset+0x10+4) ) ;
			msg+= ", C1=" +  Utility::toHexString( Utility::arrayToWord(ramC000+offset+0x10+6) ) ;
			msg+= ", C2=" +  Utility::toHexString( Utility::arrayToWord(ramC000+offset+0x10+10)  ) ;
			msg+= ", C3=" +  Utility::toHexString( Utility::arrayToWord(ramC000+offset+0x10+12)  ) ;
			msg+= ", C4=" +  Utility::toHexString( Utility::arrayToWord(ramC000+offset+0x10+14)  ) + Utility::CrLf() ;
		}

		WORD dblockAddr = Utility::arrayToWord( ramC000+offset+0x12);
		msg+= "  D-blk=" + Utility::toHexString( dblockAddr );
		msg+= ", CTable=" +   Utility::toHexString( Utility::arrayToWord( ramB000+0xF70+wcNo*2  )) + Utility::CrLf() ;

		msg+= Utility::CrLf();

		if( dblockAddr !=0xFFFF ){
			if( dblockAddr>=0xC000){
				temp = Utility::strFormat("wc(%d)D-Block=", wcNo) ;
				dBlockMsg += temp + Utility::arrayToHexString(ramC000 + (dblockAddr&0x0FFF), 0x10) + Utility::CrLf();
			}
		}
	}

	msg += dBlockMsg + Utility::CrLf();

	BYTE errCode = ram9000[0xBEF];
	if( errCode != 0){
		// popup error code
		msg  = "Error Code = " + Utility::toHexString(errCode)  + Utility::CrLf() + msg;
	}else{
		msg += "Error Code = " + Utility::toHexString(errCode) + Utility::CrLf();
	}

	BYTE wiBlkNo = ramC000[0xDD1];
	msg+="Curr WI-BlockNo  = " + Utility::toHexString( wiBlkNo ) + Utility::CrLf();
	msg+="WI-BlockPageEnd  = " + Utility::toHexString( ramC000[0xDD2] ) + Utility::CrLf();

	msg += Utility::CrLf() + "==== Other(0xCDC0)  ====" + Utility::CrLf();
	offset = 0xDC0;
	msg += "x_spare_block_number = " + Utility::toHexString( ramC000[offset] ) + Utility::CrLf();
	msg += "x_min_age_spare = " + Utility::toHexString( ramC000[offset+1] ) + Utility::CrLf();
	msg += "x_max_age_spare = " + Utility::toHexString( ramC000[offset+2] ) + Utility::CrLf();
	msg += "x_empty_spare_space = " + Utility::toHexString( ramC000[offset+3] ) + Utility::CrLf();

	msg += "x_min_age_MLC_spare = " + Utility::toHexString( ramC000[offset+4] ) + Utility::CrLf();
	msg += "x_max_age_MLC_spare = " + Utility::toHexString( ramC000[offset+5] ) + Utility::CrLf();
	msg += "x_total_W = " + Utility::toHexString( ramC000[offset+6] ) + Utility::CrLf();


	msg+= Utility::CrLf() + "==== WI Table(0xCE00, 3s Struct) ====" + Utility::CrLf();
	for(int wiNo=0; wiNo<7; wiNo++){
		temp = Utility::strFormat("(%d)", wiNo) ;
		msg+= temp ;
		msg+= "WI = " + Utility::arrayToHexString(ramC000+0xE00+wiNo*0x10, 0x10) + Utility::CrLf();
	}

	msg += Utility::CrLf() + "==== WI Used Table (0xCDF0) ====" + Utility::CrLf();
	msg += Utility::makeHexTable(0x10, ramC000+0xDF0);

	msg += Utility::CrLf() + "==== Zone Table(0xC000) ====" + Utility::CrLf();
	for(int zoneNo=0; zoneNo<7; zoneNo++){
		temp = Utility::strFormat("(%d)", zoneNo) ;
		msg+= temp ;
		msg+= "z = " + Utility::arrayToHexString(ramC000+zoneNo*0x10, 0x10) + Utility::CrLf();
	}

	msg += Utility::CrLf() + "==== Spare Age Table ==== (0x80 = used)" + Utility::CrLf();
	msg += Utility::makeHexTable(0xB0, ramC000+0xD00, 0xCD00, false);


	//Final Wi Table
	msg+= Utility::CrLf() + "=== Final Wi Table(0xBF00)(B3:WI_BlkNo, B4:WI_BlkEnd) " + Utility::CrLf();
	for(int wiNo=0; wiNo<7; wiNo++){
		int offset = 0xF00 + wiNo*7;
		msg+= Utility::arrayToHexString(ramB000 + offset, 0x7) + Utility::CrLf();

	}

	//Relink L1 Table
	msg+= Utility::CrLf() + "=== Relink L1 Table(0xBF80)(col_skip, CE, 1, 2) === " + Utility::CrLf();
	msg += Utility::makeHexTable(0x80, ramB000+0xF80, 0xBF80, false);
	
	// NON ACTIVE W TABLE
	msg += Utility::CrLf() + "==== NON ACTIVE W TABLE ====" + Utility::CrLf();
	msg += Utility::makeHexTable((int)0x50, ramB000+0xE40, 0xBE40, false);

	// NON ACTIVE W WEIGHT TABLE
	msg += Utility::CrLf() + "==== NON ACTIVE W Count TABLE ====" + Utility::CrLf();
	msg += Utility::makeHexTable((int)0x30, ramB000+0xEA0, 0xBEA0, false);

	// NON ACTIVE W WEIGHT TABLE
	msg += Utility::CrLf() + "==== NON ACTIVE W WEIGHT TABLE ====" + Utility::CrLf();
	msg += Utility::makeHexTable((int)0x20, ramB000+0xED0, 0xBED0, false);


	// Column Skip table index
	msg += Utility::CrLf() + "==== Column Skip table index ====" + Utility::CrLf();
	msg += Utility::makeHexTable( (int)0x20, ram9000+0xB00, 0x9B00, false);

	return msg;
}

tstring DataParser_97::makeFwStructInfo(BYTE* ram9000, BYTE* ramA000, BYTE* ramB000, BYTE* ramC000 ){
	tstring msg,temp;

	msg+=checkC000(ram9000, ramA000, ramB000, ramC000);
	msg+=makeC000_info(ram9000, ramA000, ramB000, ramC000);

	return msg;
}



WORD DataParser_97::getPageNo(WORD defaultValue, BYTE* bufB000){
	WORD res = defaultValue;

	WORD cTableoffset      = Utility::arrayToWord( bufB000+0xF70  );
	WORD cTableOffset_Next = Utility::arrayToWord( bufB000+0xF72  );
	if( cTableOffset_Next!=0xFFFF){
		res = (cTableOffset_Next - cTableoffset)/2;
	}

	if( cTableOffset_Next >258){
		res = 258;
	}

	return res;
}

tstring DataParser_97::check_3sBlock_struct(BYTE* ptr3sBlock, WORD offset)
{
	tstring msg;
	BYTE type = ptr3sBlock[0];

	//ck solumn index
	if( type != 0x40 ){
		//BYTE col_idx = ptr3sBlock[8];

		//if( col_idx<2){
		//	msg += " fail col skip index in 0x" + Utility::toHexString(offset)  + Utility::CrLf();
		//}
	}
	return msg;
}


tstring DataParser_97::check_duplicateBlock(BYTE* ramC000, BYTE* ramA000){
	tstring msg;
	int offset;
	int slcCnt = _getSlcTableLen(ramC000);
	//int mlcCnt = _getMlcTableLen(ramC000);
	int entCnt = _getEntryTableLen(ramA000);

	// get MLC Block ( it should check ce)
	typedef pair<ULONG, WORD> BlkNo_And_Addr;
	vector<BlkNo_And_Addr> blockColls, dupColls;
	BlkNo_And_Addr blockNoAndAddr;
	WORD ptr3sBlock;

	offset = 0x100;
	for( int i=0; i<0x10; i++){
		if( ramC000[0xF0+i] == 0xFF ){continue;}

		ptr3sBlock = offset+i*0x10;
		BYTE type = ramC000[ptr3sBlock];

		if( (type&0x7F)==01 ||  (type&0x7F)==05 ){

			blockNoAndAddr.first = Utility::arrayToWord(ramC000+ptr3sBlock + 3);
			blockNoAndAddr.second = 0xC000+ptr3sBlock + 3;
			blockColls.push_back(blockNoAndAddr);

			blockNoAndAddr.first = Utility::arrayToWord(ramC000+ptr3sBlock + 6);
			blockNoAndAddr.second = 0xC000+ptr3sBlock + 6;
			blockColls.push_back(blockNoAndAddr);

			msg += check_3sBlock_struct(ramC000+ptr3sBlock, ptr3sBlock+0xC100);
		}
	}

	// get SLC Block 
	offset = 0x200;
	for( int i=0; i<slcCnt; i++){
		BYTE spareAge = ramC000[0xD00+i];
		if( spareAge == 0xFF){continue;}

		ptr3sBlock = offset+i*0x10;
		BYTE type = ramC000[ptr3sBlock];
		if( (type&0x7F)==01 ||  (type&0x7F)==05 ){
			blockNoAndAddr.first = Utility::arrayToWord(ramC000+ptr3sBlock + 3);
			blockNoAndAddr.second = 0xC000+ptr3sBlock + 3;
			blockColls.push_back(blockNoAndAddr);

			blockNoAndAddr.first = Utility::arrayToWord(ramC000+ptr3sBlock + 6);
			blockNoAndAddr.second = 0xC000+ptr3sBlock + 6;
			blockColls.push_back(blockNoAndAddr);

			msg += check_3sBlock_struct(ramC000+ptr3sBlock, ptr3sBlock+0xC200);
		}
	}

	// get entry Table Block 
	offset = 0x0;
	for( int i=0; i<entCnt; i++){
		ptr3sBlock = offset+i*0x10;
		BYTE type = ramA000[ptr3sBlock];
		if( (type&0x7F)==01 ||  (type&0x7F)==05 ){
			blockNoAndAddr.first = Utility::arrayToWord(ramA000 + ptr3sBlock + 3);
			blockNoAndAddr.second = 0xA000 + ptr3sBlock + 3;
			blockColls.push_back(blockNoAndAddr);

			blockNoAndAddr.first = Utility::arrayToWord(ramA000 + ptr3sBlock + 6);
			blockNoAndAddr.second = 0xA000 + ptr3sBlock + 6;
			blockColls.push_back(blockNoAndAddr);

			msg += check_3sBlock_struct(ramC000+ptr3sBlock, ptr3sBlock+0xA000);
		}
	}


	Utility::findDuplicateItem(blockColls, dupColls);
	//check duplicate by bitmap
	for(size_t i=0; i<dupColls.size(); i++){
		if( dupColls[i].first == 0xFFFF){
			continue;
		}
		msg += Utility::strFormat("find duplicate block = %04X( addr=0x%X)",  dupColls[i].first, dupColls[i].second ) + Utility::CrLf();
	}
	return msg;
}

int DataParser_97::_getMlcTableLen(BYTE* ramC000){
	//int len = 0x0F;
	int cnt = 0;
	int offset = 0xf0;
	for( int i=0; i<0x10; i++ ){
		if( (ramC000[offset+i] )!=0xFF){
			cnt++;
		}
	}
	return cnt;
}

int DataParser_97::_getSlcTableLen(BYTE* ramC000){
	//int len = 0xaF;
	int cnt = 0;
	int offset = 0xd00;

	// 0x9F means SLC queue, 0xA0~0xAF means MLC queue
	for( int i=0; i<0xAF; i++ ){
		if( (ramC000[offset+i] )!=0xFF){
			cnt++;
		}
	}
	return cnt;
}

int DataParser_97::_getEntryTableLen(BYTE* ramA000){
	int len = 0x100;
	int cnt = 0;
	int offset = 0x000;
	for( int i=0; i<len; i++ ){
		if( (ramA000[offset+i*0x10] )!=0xFF){
			cnt++;
		}
	}
	return cnt;
}


tstring DataParser_97::checkMlcTable(BYTE* ramC000){
	int mlcCnt = _getMlcTableLen(ramC000);
	tstring msg;

	if( mlcCnt==0){
		msg+="MLC Table Cnt = 0" + Utility::CrLf();
	}

	for(int i=0; i<0x10; i++ ){
		if( ramC000[0xF0+i] != 0xFF){
			if( ramC000[0x100+i*0x10] == 0xFF){
				msg+="MLC Table Fail" + Utility::CrLf();
			}
		}
	}
	return msg;
}


