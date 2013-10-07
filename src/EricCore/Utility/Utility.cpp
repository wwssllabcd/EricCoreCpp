// Written by EricWang 2006-2013

#include "stdafx.h"	
#include "Utility.h"
#include <fstream>
#include <cmath>
#include <bitset>
#include <vector>
#include <algorithm>
#include <time.h>

#include "./EricException/MyException.h"

// for gcc
#include <string.h>
#include <stdio.h>
//#include <cctype> // for tolower function pointer
using namespace EricCore;


#ifdef USE_BOOST
#include <boost/lexical_cast.hpp>
#endif

#pragma warning( disable : 4702 )


Utility::Observers Utility::m_observers;

Utility::Utility(void)
{

}

Utility::~Utility(void)
{
}

void Utility::observerRegister( Utility::MsgFunPtr pF ){
	if(pF==0){
		throw MyException(UTI_OBSERVER_REGISTER_FAIL, "observerRegister error: function ptr is empty" );
	}
	for(Utility::Observers::size_type i=0; i<m_observers.size(); i++){
		Utility::MsgFunPtr testFP = m_observers[i];
		if(testFP==pF){
			return;
		}
	}
	m_observers.push_back(pF);
}

void Utility::sendMsg(tstring& msg,bool isCrLf, bool isClean){
	Utility::Observers::size_type size = m_observers.size();
	if(size==0){
		throw MyException(UTI_SEMD_MSG_FAIL, "sendMsg error: no any send msg function" );
	}

	if(isCrLf){
		msg = CrLf() + msg;
	}

	for(Utility::Observers::size_type i=0; i<size; i++){
		Utility::MsgFunPtr pF = m_observers[i];
		if(pF==0){
			throw MyException(UTI_SEMD_MSG_FAIL, "sendMsg error: no any send msg function" );
			return;
		}
		pF(msg, isClean);
	}
}

void Utility::sendMsg(bool isCrLf, bool isClean, LPCTSTR fmt, ...){
	tstring	str;
	va_list	arg;
	va_start( arg, fmt );
	str = strFormat(0, fmt, arg );
	va_end( arg );
	sendMsg(str, isCrLf, isClean);
}

void Utility::sendMsg(LPCTSTR fmt, ...){
	tstring	str;
	va_list	arg;
	va_start( arg, fmt );
	str = strFormat(0, fmt, arg );
	va_end( arg );
	sendMsg(str);
}

//UCHAR to ASCII, If the value out of range, return default signal(defeult= "dot")
tstring Utility::uCharToAvailableString(const BYTE& c, tstring stuffAsciiNum){
	tstring  str;
	if (31< c && c<127){
		str = strFormat( "%c",c);
	}else{
		str = stuffAsciiNum;
	}
	return str;
}


//array usually use "0x00" value for the end
tstring Utility::arrayToString(const BYTE* charArray, int length, bool isEndWhenZero){
	tstring  str,temp;
	for(int i=0; i<length; i++){
		//0x00 is mean terminal sign for tstring, if an array have a 0x00,
		if(charArray[i] == 0x00){
			if(isEndWhenZero == true){
				return str;
			}
			//if it value is "0x00", use "dot"
			temp = _T(".");
		}else{
			temp = strFormat( _T("%c"), charArray[i]);
		}
		str+= temp;
	}
	return str;
}


tstring Utility::arrayToHexString(const BYTE* charArray, int length, const tstring& filler){
	tstring  str,temp;
	int i;
	for(i=0; i<length; i++){
		temp = Utility::strFormat( _T("%02X"), charArray[i]);
		str+= temp+filler;
	}
	return str;
}

//highLowByteChange, Athe length MUST be even
void Utility::highLowByteChange(BYTE* array,int length){
	if(length%2 !=0){
		throw MyException(UTI_HI_LO_CHG_MUST_EVEN, _T("Length MUST even numbers") );
	}

	BYTE *temp = new BYTE[length];
	try{
		//backup origninal value
		memcpy(temp,array,sizeof(BYTE)*length);
		int count=0;
		if (length %2 == 0){
			count = length/2;
		}else{
			count = length/2 +1;
		}

		for(int i=0; i<count; i++){
			temp[i*2] = array[i*2+1];
			temp[i*2+1] = array[i*2];
		}

		memcpy(array,temp,sizeof(BYTE)*length);
		delete[] temp;
	}catch (exception& e){
		delete[] temp;
		throw MyException(UTI_HI_LO_CHG_MUST_EVEN,  e.what() );
	}
}

bool Utility::isHexFormat(const tstring& s){
	if(s.length()<2 ){
		return false;
	}

	if(s.at(0) ==_T('0') && s.at(1)==_T('x') ){
		return true;
	}

	if(s.at( s.length()-1) ==_T('h')){
		return true;
	}

	// if no 0x*** or ***h, it should be decimal 

	return false;
}

void Utility::_remove_0x_And_H(tstring& cs){
	if( cs.find(_T("h")) != tstring::npos){
		cs.replace( cs.find(_T("h")), 1, _T("") );
	}

	if( cs.find(_T("0x")) != tstring::npos){
		cs.replace( cs.find(_T("0x")), 2, _T("") );
	}
}

inline char charToLower(char c)
{
	//return std::tolower(c);
	return tolower(c);
}

int Utility::toInt(const tstring& cs){
	tstring s = cs;
	if(s.length()==0){
		return 0;
	}
	transform(s.begin(), s.end(), s.begin(), charToLower);

	//if it's not hex format
	if( isHexFormat(s) == false){
		return tatoi( s.c_str() );
	}

	tstring tmp(s), aChar;
	int sum(0), aInt(0);

	_remove_0x_And_H(tmp);

	tstring::size_type len = tmp.length();
	for(tstring::size_type i=0; i<len; i++){
		aChar = tmp.at(len-i-1);
		if( aChar.compare(_T("a"))==0 ){aInt = 10;}
		else if( aChar.compare(_T("b"))==0 ){aInt = 11;}
		else if( aChar.compare(_T("c"))==0 ){aInt = 12;}
		else if( aChar.compare(_T("d"))==0 ){aInt = 13;}
		else if( aChar.compare(_T("e"))==0 ){aInt = 14;}
		else if( aChar.compare(_T("f"))==0 ){aInt = 15;}
		else { 
			aInt = tatoi( aChar.c_str() );
		}

		sum += aInt*(0x01<<(4*i));
	}
	return sum;
}

int Utility::toInt(const bool& boolean){
	int intTemp;
	if(boolean){
		intTemp = 1;
	}else{
		intTemp = 0;
	}
	return intTemp;
}

int Utility::toInt(BYTE value){
	return (int)value;
}


int Utility::getBit(const WORD& word,int bitNo){
	//using Bitset of STD(C++ Primer 3rd Edition ch4.11 , ch4.12)
	std::bitset<32> result(word);	
	return result[bitNo];
}

int Utility::getBit(const BYTE& byte,int bitNo){
	// it MUST change the type or infinite loop
	WORD target(byte);
	return getBit(target,bitNo);
}



// output to Text file
void Utility::toFile(const tstring& filePath, const tstring& msg, bool isAppend){
	toFile(filePath, (BYTE*)msg.c_str(), msg.length(), isAppend);
}

// output Array to Binary file
void Utility::toFile(const tstring& filePath, BYTE* data, int length, bool isAppend){
	//if it have not the file , ofstream will make a new file 
	tofstream ofs;
	if(isAppend==true){
		// ios::binary is mean," Do not replace CRLF "
		ofs.open(filePath.c_str(), ios::out|ios::binary|ios::app);
	}else{
		ofs.open(filePath.c_str(), ios::out|ios::binary|ios::trunc);
	}

	if (ofs == NULL) {
		throw MyException(UTI_TOFILE_FUNCTION_FAIL, _T("Can`t open file ") + filePath);
	}


	ofs.write( (const char*) data, length);

	// even you didn't close, ofs will be close in destructor
	// fstream destruct call close for you. When an exception is thrown, the file is closed automatically.(RAII)
	ofs.close();
}

// output Array to Binary file
bool Utility::isFileExist(const tstring& filePath){
	bool result=false;
	FILE* fpSource =  fopen(filePath.c_str(), _T("rb"));
	if (fpSource != NULL) {
		result=true;
		fclose(fpSource);
	}
	return result;
}

void Utility::setBit(BYTE& target,BYTE numberOfbit,bool isSet_1){
	int value(0);
	if(isSet_1==true){
		value = 1;
	}else{
		value = 0;
	}
	setBit(target,numberOfbit,value);
}

void Utility::setBit(ULONG& target,BYTE numberOfbit,int intValue){
	//check range
	if(numberOfbit < 0 || numberOfbit >31){throw MyException(UTI_SET_BIT_FAIL, _T("[Setting Bit function]numberOfbit Parameter out of range"));}
	if(intValue < 0 || intValue >1){throw MyException(UTI_SET_BIT_FAIL, _T("[Setting Bit function]Value Parameter Error: It Should be 0 or 1"));}
	std::bitset<32> result(target);	
	result[numberOfbit] = toBool(intValue);
	target = result.to_ulong();
}

void Utility::setBit(WORD& target,BYTE numberOfbit,int intValue){
	//check range
	if(numberOfbit < 0 || numberOfbit >15){throw MyException(UTI_SET_BIT_FAIL, _T("[Setting Bit function]numberOfbit Parameter out of range"));}
	if(intValue < 0 || intValue >1){
		throw MyException(UTI_SET_BIT_FAIL, _T("[Setting Bit function]Value Parameter Error: It Should be 0 or 1"));}

	std::bitset<32> result(target);	
	result[numberOfbit] = toBool(intValue);
	target = static_cast<WORD>( result.to_ulong() );
}

void Utility::setBit(BYTE& target,BYTE numberOfbit,int intValue){
	//check range
	if(numberOfbit < 0 || numberOfbit >7){
		throw MyException(UTI_SET_BIT_FAIL, _T("Setting Bit function out of range") );
	}

	WORD tempTarget(target);
	setBit(tempTarget,numberOfbit,intValue);
	target = static_cast<BYTE>(tempTarget);
}



WORD* Utility::toWordPtr(const BYTE* ptr){
	WORD* wPrt;
	wPrt = (WORD  *)ptr;
	return wPrt;
}

BYTE* Utility::toBytePtr(const WORD* ptr){
	BYTE* bPrt;
	bPrt = (BYTE  *)ptr;
	return bPrt;
}



bool Utility::toBool(int intValue){
	if( intValue==0){
		return false;
	}
	return true;
}

ULONG Utility::hexStringToULONG(tstring& hex){
	//_remove_0x_And_H(hex);
	//ULONG a = strtoul(hex.c_str(), NULL, 10);

	ULONG b = strtoul(hex.c_str(), NULL, 16);

	return b;
}

tstring Utility::CrLf(){
	return _T("\r\n");
}

void Utility::toArray(const WORD& value, BYTE* array,bool isHighFirst){
	BYTE h = static_cast<BYTE>( (value>>8)&0xFF );
	BYTE l = static_cast<BYTE>(value&0xFF) ;
	if(isHighFirst == true){
		array[0] = h;
		array[1] = l;
	}else{
		array[0] = l;
		array[1] = h;
	}
}

void Utility::toArray(const ULONG& value, BYTE* array,bool isHighFirst){
	if(isHighFirst == true){
		array[0] = static_cast<BYTE>((value>>24)&0xFF);
		array[1] = static_cast<BYTE>((value>>16)&0xFF);
		array[2] = static_cast<BYTE>((value>>8)&0xFF);
		array[3] = static_cast<BYTE>((value    )&0xFF);
	}else{
		array[3] = static_cast<BYTE>((value>>24)&0xFF);
		array[2] = static_cast<BYTE>((value>>16)&0xFF);
		array[1] = static_cast<BYTE>((value>>8)&0xFF);
		array[0] = static_cast<BYTE>((value    )&0xFF);
	}
}

void Utility::toArray(const tstring str, BYTE* ary, int length, BYTE stuffAsciiNum){
	tstring::size_type len = str.length();
	tstring::size_type aryLen = (tstring::size_type)length;
	for(tstring::size_type i=0; i<aryLen; i++){
		if(i<len){		
			ary[i] = str.at(i);
		}else{
			ary[i] = stuffAsciiNum;
		}
	}
}


void Utility::toArray(const tstring str, WORD* ary, int length, BYTE stuffAsciiNum){
	BYTE* ptr= toBytePtr(ary);
	toArray(str, ptr, length, stuffAsciiNum);
	highLowByteChange(ptr,length);
}


//tstring Utility::makeHexTable(BYTE* array,int secCnt){
//	tstring msg;
//	int i;
//	for(i=0; i<secCnt; i++){
//		msg+= makeHexTable(array+i*512);
//		if( (secCnt-1) !=i ){
//			msg+= CrLf();
//		}
//	}
//	return msg;
//}
//
//tstring Utility::makeHexTable(WORD* array,int secCnt){
//	tstring msg;
//	int i;
//	for(i=0; i<secCnt; i++){
//		msg+= makeHexTable(array+i*256);
//		msg+= CrLf();
//	}
//	return msg;
//}


//tstring Utility::makeHexTable(BYTE* ary){
//	tstring head,msg;
//	head += _T("00 | 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F ") + Utility::CrLf();
//	head += _T("== | ============================") + Utility::CrLf();
//	msg += head;
//	for(int i=0; i<32; i++){
//		tstring row;
//		row = Utility::strFormat(_T("%02X"), i);
//		msg +=  row;
//		msg += _T(" | ");
//		for(int j=0; j<16; j++){
//			msg+=Utility::toHexString( ary[i*16+j] );
//			msg+= _T(" ");
//		}
//		msg+= Utility::CrLf();
//	}
//	return msg;
//}
//
//tstring Utility::makeHexTable(WORD* ary){
//	tstring head, msg;
//	head += _T("0000 | 0000 0001 0002 0003 0004 0005 0006 0007 0008 0009 000A 000B 000C 000D 000E 000F") + Utility::CrLf();
//	head += _T("==== | ===================================================================") + Utility::CrLf();
//	msg += head;
//
//	for(int i=0; i<16; i++){
//		tstring row = Utility::strFormat(_T("%04X"), i);
//		msg +=  row;
//		msg += _T(" | ");
//		for(int j=0; j<16; j++){
//			msg+=Utility::toHexString(ary[i*16+j]);
//			msg+=_T(" ");
//		}
//		msg+= Utility::CrLf();
//	}
//	return msg;
//}




tstring Utility::makeAsciiTable(BYTE* ary, int length){
	tstring msg;
	int width=0x10;
	int row=0;
	int rowCnt = length/width;
	if( (length%width)!=0){
		rowCnt++;
	}

	for(row=0; row<rowCnt; row++){
		msg+= Utility::arrayToString(ary+row*width, width);
		msg+= Utility::CrLf();
	}

	int mod = length%width;
	msg+= Utility::arrayToString(ary+length-mod,mod);
	msg+= Utility::CrLf();
	return msg;
}

tstring Utility::makeHexTable(int length, BYTE* ary, WORD offset, bool needHeader){
	tstring msg;
	tstring head;
	tstring row;

	if( needHeader ){
		head += _T("0000 | 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F ") + Utility::CrLf();
		head += _T("==== | ============================") + Utility::CrLf();
		msg += head;
	}

	int i=0;
	while(i<length){
		if( (i%512)==0){
			if(i!=0){
				msg+=Utility::CrLf();
			}
		}

		if( (i%0x10)==0 ){
			tstring row = Utility::strFormat(_T("%04X"), i+offset);
			msg +=  row;
			msg += _T(" | ");
		}

		msg+=Utility::toHexString(ary[i]) + _T(" ");
		if( ((i+1)%0x10)==0){
			msg+=Utility::CrLf();
		}
		i++;
	}
	return msg;
}


void Utility::sourceDataToTarget(FILE* source, FILE* target, int length){
	int i;
	BYTE c;
	for (i=0; i<length; i++){
		c = fgetc(source);
		fputc(c, target);
	}
}

void Utility::getFileData(tstring filePath, BYTE* data){
	long fileSize = Utility::getFileSize(filePath);
	getFileData(filePath, 0, fileSize, data );
}

void Utility::getFileData(tstring filePath, int startAddr, int length, BYTE* data){
	int i;
	FILE* fpSource =  fopen(filePath.c_str(), _T("rb") );
	if (fpSource == NULL) {
		throw MyException(UTI_GET_FILE_DATA_FAIL, _T("Can`t open file ")  +filePath);
	}

	fseek (fpSource , startAddr , SEEK_SET );
	for(i=0; i<length; i++){
		data[i] = fgetc(fpSource);
	}
	fclose(fpSource);
}

long Utility::getFileSize(tstring filePath){
	FILE* fpSource =  fopen(filePath.c_str(), _T("rb"));
	if (fpSource == NULL) {
		throw MyException(UTI_GET_FILE_SIZE_FAIL, _T("Can`t open file ")+filePath);
	}

	fseek (fpSource , 0 , SEEK_END );
	long fileLength = 0;
	fileLength = ftell(fpSource);
	fclose(fpSource);
	return fileLength;
}

int Utility::ceil(int dividend, int divisor){
	int result = dividend/divisor;
	if(dividend%divisor){
		result++;
	}
	return result;
}

void Utility::ulongToArray(ULONG source, BYTE* array){
	array[0] = (BYTE)((source>>24)&0xFF);
	array[1] = (BYTE)((source>>16)&0xFF);
	array[2] = (BYTE)((source>>8 )&0xFF);
	array[3] = (BYTE)((source    )&0xFF);
}

void Utility::wordToArray_MSB(WORD val, BYTE* array){

	array[0] = (BYTE)((val>>8 )&0xFF);
	array[1] = (BYTE)((val    )&0xFF);
}


void Utility::makeBuf(ULONG number, int length, BYTE* buf){
	BYTE lbaAddr0,lbaAddr1,lbaAddr2,lbaAddr3;
	lbaAddr0= (BYTE)((number>>24)&0xFF);
	lbaAddr1= (BYTE)((number>>16)&0xFF);
	lbaAddr2= (BYTE)((number>> 8)&0xFF);
	lbaAddr3= (BYTE)((number    )&0xFF);

	int i=0;
	for(i=0; i<length; i+=4){
		buf[i+0]= lbaAddr0;
		buf[i+1]= lbaAddr1;
		buf[i+2]= lbaAddr2;
		buf[i+3]= lbaAddr3;
	}
}


tstring Utility::getDiffStringInTwoBuf(ULONG lbaAddr, int length, BYTE* writeBuf, BYTE* readBuf){
	tstring result,msg;
	int errorCnt=0;
	result = strFormat( _T("  cmp error in 0x%x"), lbaAddr);
	int i=0;
	for(i=0; i<length; i++){
		if( writeBuf[i]!=readBuf[i] ){
			if(errorCnt<20){
				msg = strFormat( _T("  writeBuf[%x]=0x%x,readBuf[%x]=0x%x"), i, writeBuf[i], i, readBuf[i] );
				result += Utility::CrLf() + msg;
			}
			errorCnt++;
		}
	}

	if(errorCnt>=20){
		result += Utility::CrLf() + _T("over 20 error");
	}

	result += Utility::CrLf() + strFormat( _T("Fail ECC Cnt = 0x%X"), errorCnt);
	return result;
}

void Utility::swap(BYTE* a, BYTE* b){
	BYTE tmp = a[0];
	a[0] = b[0];
	b[0] = tmp;
}

WORD Utility::getCheckSum(BYTE* ary, int length){
	WORD ckSum=0;
	int i(0);
	for(i=0; i<length; i++){
		ckSum+=ary[i];
	}
	return ckSum;
}

bool Utility::uintToBool(UINT value){
	if(value==0){
		return false;
	}
	return true;
}

ULONG Utility::arrayToUlong(BYTE* ary){

	ULONG value=0;
	value |= ary[0]<<24;
	value |= ary[1]<<16;
	value |= ary[2]<<8;
	value |= ary[3];
	return value;
}

WORD Utility::arrayToWord(BYTE* ary){
	WORD value=0;
	value |= ary[0]<<8;
	value |= ary[1];
	return value;
}




int Utility::getRandomValue(int min, int max){
	float tmp = rand()*((float)((max)-(min)))/(float)(RAND_MAX)+(min);
	return (int)tmp;
}



void Utility::not_exec(BYTE* aryBin, int len){
	int bit;
	for(int i=0; i<len; i++){
		for(int bitNo=0; bitNo<8; bitNo++){
			bit = getBit(aryBin[i], bitNo);
			if(bit==0){
				setBit(aryBin[i], bitNo, 1);
			}else{
				setBit(aryBin[i], bitNo, 0);
			}
		}
	}
}

int Utility::getModulus(const int& a ,const int& b){
	if(a==b){return 0;}
	if(a>b){
		return a-b;
	}
	return b-a;
}


tstring Utility::strFormat(const TCHAR *fmt, ...){ 
	tstring retStr("");
	if (NULL != fmt)   {
		va_list marker = NULL; 
		va_start(marker, fmt); 	// initialize variable arguments
		retStr = strFormat(0, fmt, marker);
		va_end(marker);         // Reset variable arguments
	}
	return retStr; 
}

void Utility::makeRandomBuffer(BYTE* pBuf, int length)
{
	unsigned int seed = (unsigned int)time(0);
	srand(seed);

	ULONG val;
	for( int i=0; i<length; i++){
		val     = (ULONG)GetRandom(0, 0xFF) ;
		pBuf[i] = (BYTE)val;
	}
}


tstring Utility::strFormat(int forDispatch, const char* fmt, va_list marker) { 
	tstring retStr(_T(""));

	if (NULL != fmt){
		// Get formatted tstring length adding one for NULL
		size_t len = 256;

#ifdef _ERIC_WIN_MFC_
#if _MSC_VER == 1200  // Some VC6 specific code
		len = 256;
#else
		len = _vscprintf(fmt, marker) + 1;
#endif
#endif


		// Create a char vector to hold the formatted tstring.
		vector<TCHAR> buffer(len, _T('\0'));
		int nWritten=0;

#ifdef _WINDOWS

		// for vc 2003
		nWritten = _vsnprintf(&buffer[0], buffer.size(), fmt, marker);  

		// for VC 2005
		//nWritten = _vsnprintf_s(&buffer[0], buffer.size(), fmt, marker);    
#else
		nWritten = vsnprintf(&buffer[0], buffer.size(), fmt, marker);   

#endif

		if (nWritten > 0){
			retStr = &buffer[0];
		}
	}
	return retStr; 
}

tstring Utility::_toStringBase(tstring param, const int& arg){
	tstring str = strFormat( _T( param.c_str() ), arg);
	return str;
}

tstring Utility::toString(const size_t& arg){
	return _toStringBase(_T("%ld"), (int)arg);
}

tstring Utility::toString(const int& arg){
	return _toStringBase(_T("%ld"), arg);
}

tstring Utility::toString(const ULONG& arg){
	return _toStringBase(_T("%ld"), arg);
}

tstring Utility::toHexString(const BYTE& arg){
	return _toStringBase(_T("%02X"), arg);
}

tstring Utility::toHexString(const WORD& arg){
	return _toStringBase(_T("%04X"), arg);
}

tstring Utility::toHexString(const ULONG& arg, tstring param){

	return _toStringBase(_T(param), arg);
}

tstring Utility::toHexString(const int& arg){
	return _toStringBase(_T("%X"), arg);
}

bool Utility::isAllNumberType(tstring& data){
	return 1;
}


bool Utility::stringSplit(tstring sign, tstring& orgStr, tstring& str1, tstring& str2){
	tstring::size_type locate = orgStr.find(sign);
	if(locate == tstring::npos){
		return false;
	}

	str1 = orgStr.substr(0, locate);
	if( (locate+1)<orgStr.length()){
		str2 = orgStr.substr(locate+1);
	}
	return true;
}

void Utility::stringSplit(vector<tstring>& array, tstring sign, tstring& orgStr){
	tstring strOp = orgStr;
	while(1){
		tstring str1, str2;
		if( stringSplit(sign, strOp, str1, str2) == false){
			array.push_back(strOp);
			break;
		}
		array.push_back(str1);
		strOp = str2;
	}
}



#ifdef _ERIC_WIN_MFC_
void Utility::getFileColls(tstring strFilePath, vector<tstring>& fileColls){
	CFileFind finder;
	BOOL bWorking = finder.FindFile( strFilePath.c_str() );
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if( finder.IsDirectory() == FALSE){
			int len = finder.GetFileName().GetLength();
			fileColls.push_back( finder.GetFileName().GetBuffer(len) );
		}
	}
}

void Utility::showMessageBox(const TCHAR* msg){
	AfxMessageBox(msg);
}

void Utility::showMessageBox(tstring& msg){
	AfxMessageBox(msg.c_str());
}

tstring Utility::getStringParamFromIni(tstring path, tstring appName, tstring keyName){
	TCHAR cKey[200];
	GetPrivateProfileString(appName.c_str(), keyName.c_str(), _T(""), cKey, 200, path.c_str() );
	return strFormat(_T("%s"),cKey);
}

bool Utility::getBoolParamFromIni(tstring modelName, tstring appName, INT defValue, tstring filePath){
	return  uintToBool( GetPrivateProfileInt(modelName.c_str(), appName.c_str(), defValue, filePath.c_str()) );
}

UINT Utility::getIntParamFromIni(tstring modelName, tstring keyName, INT nDefault, tstring filePath){
	return GetPrivateProfileInt(modelName.c_str(), keyName.c_str(), nDefault, filePath.c_str() );
}

tstring Utility::showErrorCodeMFC(void){
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(), 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL);

	tstring msg;
	msg = Utility::strFormat("%s", lpMsgBuf);
	LocalFree(lpMsgBuf); 
	return msg;
}

tstring Utility::genTimeString(const SYSTEMTIME& current_date_time){
	return genTimeString(current_date_time, _T(":"), _T("%02d"), 0);
}

tstring Utility::genTimeString(const SYSTEMTIME& current_date_time, const tstring& separateSign, const tstring& format, const int separateType){
	tstring yy, mo, dd, hh, mm, ss;
	if(format==_T("") ){
		yy = toString(current_date_time.wYear);
		mo = toString(current_date_time.wMonth);
		dd = toString(current_date_time.wDay);
		hh = toString(current_date_time.wHour);
		mm = toString(current_date_time.wMinute);
		ss = toString(current_date_time.wSecond);
	}else {
		yy = toString(current_date_time.wYear);

		mo = strFormat( format.c_str(), current_date_time.wMonth);
		dd = strFormat( format.c_str(), current_date_time.wDay);
		hh = strFormat( format.c_str(), current_date_time.wHour);
		mm = strFormat( format.c_str(), current_date_time.wMinute);
		ss = strFormat( format.c_str(), current_date_time.wSecond);
	}

	tstring res;

	if( separateType ==0 ){
		res = yy + separateSign + mo + separateSign + dd + separateSign + hh + separateSign + mm + separateSign + ss;
	}else{
		res = yy + separateSign +  mo + dd;
	}

	return res;
}

tstring Utility::getCurrnetPath(void){
	TCHAR str[200];
	GetCurrentDirectory(sizeof(str),str);
	return  tstring(str);
}

bool Utility::saveToIni(const tstring& appName,const tstring& keyName, const tstring& result, const tstring& filePath){
	return toBool(WritePrivateProfileString(appName.c_str(), keyName.c_str(), result.c_str(), filePath.c_str() ));
}

bool Utility::saveToIni(const tstring& appName,const tstring& keyName, const int& result, const tstring& filePath){
	return saveToIni(appName, keyName, toString(result), filePath);
}

bool Utility::saveToIni(const tstring& appName,const tstring& keyName, const bool& result, const tstring& filePath){
	return saveToIni(appName,keyName, toString(result),filePath);
}
#endif