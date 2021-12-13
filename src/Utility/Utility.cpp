
#include "stdafx.h"
#include "Utility.h"
#include "EricException.h"
#include <vector>

#include <stdarg.h>    // va_start, va_end
#include <windows.h>   // CreateDirectory
#include <fstream>     // fstream
#include <bitset>      // bitset

using namespace EricCore;
using namespace std;

Utility::Utility(void) {
}

Utility::~Utility(void) {
}

estring Utility::crLf() {
	return _ET("\r\n");
}

void Utility::convertToWchar(wchar_t* wc, const char* c, int maxLen) {
	swprintf(wc, maxLen, L"%hs", c);
}

estring Utility::strFormatValist(int forDispatch, echar_sp fmt, va_list marker) {
	estring retStr(_ET(""));
	if (NULL != fmt) {
		// Get formatted tstring length adding one for NULL
#ifdef _ERIC_WIN_MFC_
#if _MSC_VER == 1200  // Some VC6 specific code
		size_t len = 256;
#else
		size_t len = tvscprintf(fmt, marker) + 1;
#endif
#endif
		// Create a char vector to hold the formatted tstring.
		vector<echar> buffer(len, _ET('\0'));
		int nWritten = 0;

#ifdef _WINDOWS
		// for vc 2003
		nWritten = tvsnprintf_s(&buffer[0], buffer.size(), buffer.size(), fmt, marker);

		// for VC 2005
		//nWritten = _vsnprintf_s(&buffer[0], buffer.size(), fmt, marker);
#else
		nWritten = tvsnprintf_s(&buffer[0], buffer.size(), buffer.size(), fmt, marker);

#endif

		if (nWritten > 0) {
			retStr = &buffer[0];
		}
	}
	return retStr;
}

estring Utility::strFormat(echar_sp fmt, ...) {
	estring retStr(_ET(""));
	if (NULL != fmt) {
		va_list marker;
		va_start(marker, fmt); 	// initialize variable arguments
		retStr = strFormatValist(0, fmt, marker);
		va_end(marker);         // Reset variable arguments
	}
	return retStr;
}

estring Utility::toString(eu8_p array, eu32 len) {
    vector<char> v(array, array + len);
    estring str(v.begin(), v.end());
    return str;
}

estring Utility::char_to_estring(const char* charStr) {
	string msg(charStr);
	return toString((eu8_p)charStr, (eu32)msg.length());
}

estring Utility::_toStringBase(estring_cr param, eu64 arg) {
	estring str = strFormat(param.c_str(), arg);
	return str;
}

estring Utility::toString(const eu32& val) {
	return _toStringBase(_ET("%d"), val);
}

estring Utility::toHexString(const eu8& val) {
    return _toStringBase(_ET("%02X"), val);
}

estring Utility::toHexString(const eu16& val) {
    return _toStringBase(_ET("%04X"), val);
}

estring Utility::toHexString(const eu32& val, estring_cr param) {
	return _toStringBase(param, val);
}

estring Utility::toHexString(const eu64& val, estring_cr param) {
	return _toStringBase(param, val);
}

//array usually use "0x00" value for the end
estring Utility::arrayToString(eu8_sp charArray, int length, bool isEndWhenZero) {
	estring  str, temp;
	for (int w = 0; w < length; w++) {
		//0x00 is mean terminal sign for tstring, if an array have a 0x00,
		if (charArray[w] == 0x00) {
			if (isEndWhenZero == true) {
				return str;
			}
			//if it value is "0x00", use "dot"
			temp = _ET(".");
		} else {
			temp = strFormat(_ET("%c"), charArray[w]);
		}
		str += temp;
	}
	return str;
}

estring Utility::arrayToHexString(eu8_sp charArray, int length, estring_cr filler) {
	estring  str, temp;
	int s;
	for (s = 0; s < length; s++) {
		temp = strFormat(_ET("%02X"), charArray[s]);
		str += temp + filler;
	}
	return str;
}

estring Utility::makeAsciiTable(eu8_sp ary, int length) {
	estring msg;
	int width = 0x10;
	int row = 0;
	int rowCnt = length / width;
	if ((length % width) != 0) {
		rowCnt++;
	}

	for (row = 0; row < rowCnt; row++) {
		msg += arrayToString(ary + row * width, width);
		msg += Utility::crLf();
	}

	int mod = length % width;
	msg += arrayToString(ary + length - mod, mod);
	msg += Utility::crLf();
	return msg;
}

estring Utility::table_adjust(eu32 curByteCnt) {
	estring msg;

	if (curByteCnt != 0) {
		// one crlf per 16 byte
		if ((curByteCnt % 0x10) == 0) {
			msg += Utility::crLf();
		}

		//one zone per 512 byte
		if ((curByteCnt % 512) == 0) {
			msg += Utility::crLf();
		}
	}
	
	// add side list
	if ((curByteCnt % 0x10) == 0) {
		estring row = strFormat(_ET("%04X"), curByteCnt);
		msg += row;
		msg += _ET(" | ");
	}
	return msg;
}

estring Utility::gen_header_u32() {
	estring head;
	head += _ET("0000 |  3 2 1 0  7 6 5 4  B A 9 8  F E D C ") + Utility::crLf();
	head += _ET("==== | ===================================") + Utility::crLf();
	return head;
}

estring Utility::gen_header_u8() {
	estring head;
	head += _ET("0000 | 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F ") + Utility::crLf();
	head += _ET("==== | ===============================================") + Utility::crLf();
	return head;
}

estring Utility::makeHexTable_u32(eu32 byteCnt, eu32_sp ary) {
	estring msg = gen_header_u32();
	for (eu32 i = 0; i < byteCnt; i+=4) {
		msg += table_adjust(i);
		msg += toHexString(ary[i/4], _ET("%08X")) + _ET(" ");
	}
	return msg;
}

estring Utility::makeHexTable(int length, eu8_sp ary, eu16 offset, bool needHeader) {
	estring msg;
	estring head;
	if (needHeader) {
		msg += gen_header_u8();
	}

	int i = 0;
	while (i < length) {
		msg += table_adjust(i);
		msg += toHexString(ary[i]) + _ET(" ");
		i++;
	}
	return msg;
}

eu32 Utility::getFileSize(estring_cr filePath) {
	struct _stat buffer;
	_tstat(filePath.c_str(), &buffer);
	return buffer.st_size;
}

eu8_p Utility::getFileData(estring_cr filePath, eu32 length, eu8_p data) {
	ifstream ifs(filePath, ios::in | ios::binary);
	if (ifs.fail()) {
		THROW_MYEXCEPTION(UTI_OPEN_FILE_FAIL, _ET("getFileData: Can`t open file ") + filePath);
	}

	ifs.read((char*)data, length); // binary input
	ifs.close();
	return data;
}

eu32 Utility::getFileData(estring_cr filePath, eu8_p data) {
    eu32 fileSize = getFileSize(filePath);
    getFileData(filePath, fileSize, data);
    return fileSize;
}

// output to Text file
void Utility::toFile(estring_cr filePath, estring_cr msg, bool isAppend) {
	toFile(filePath, (eu8_p)msg.c_str(), (int)msg.length(), isAppend);
}

// output Array to Binary file
void Utility::toFile(estring_cr filePath, eu8_p data, int length, bool isAppend) {
	//if it have not the file , ofstream will make a new file
	ofstream ofs;
	int mode;
	if (isAppend == true) {
		// ios::binary is mean "Do not replace CRLF"
		mode = ios::out | ios::binary | ios::app;
	} else {
		mode = ios::out | ios::binary | ios::trunc;
	}

	for (int i = 0; i < 10; i++) {
		ofs.open(filePath.c_str(), mode);
		if (ofs.fail() == false) {
			break;
		}
		Sleep(100);
	}

	if (ofs.fail() == true) {
		THROW_MYEXCEPTION(UTI_OPEN_FILE_FAIL, _ET("toFile: Can`t open file ") + filePath);
	}

	// output data by ANSI stream, because it's "byte array"
	ofs.write((char*)data, length);

	// even you didn't close, ofs will be close in destructor
	// fstream destruct call close for you. When an exception is thrown, the file is closed automatically.(RAII)
	ofs.close();
}

bool Utility::isFileExist(estring_cr filePath) {
	struct _stat buffer;
	return (_tstat(filePath.c_str(), &buffer) == 0);
}

int Utility::deleteFile(estring_cr filePath) {
	return _tremove(filePath.c_str());
}

void Utility::createFolder(estring_cr folderName) {
	if (CreateDirectory(folderName.c_str(), NULL) == 0) {
		if (ERROR_ALREADY_EXISTS != GetLastError()) {
			THROW_MYEXCEPTION(0, _ET("createFolder: Can`t create folder file ") + folderName);
		}
	}
}

int Utility::toInt(const bool& boolean) {
	int intTemp;
	if (boolean) {
		intTemp = 1;
	} else {
		intTemp = 0;
	}
	return intTemp;
}

int Utility::toInt(eu8 value) {
	return (int)value;
}
int Utility::getBit(const eu16& word, int bitNo) {
	//using Bitset of STD(C++ Primer 3rd Edition ch4.11 , ch4.12)
	std::bitset<32> result(word);
	return result[bitNo];
}

int Utility::getBit(const eu8& byte, int bitNo) {
	// it MUST change the type or infinite loop
	eu16 target(byte);
	return getBit(target, bitNo);
}

bool Utility::toBool(eu32 intValue) {
	if (intValue == 0) {
		return false;
	}
	return true;
}

void Utility::toArray(const eu16& source, eu8_p array, bool isMSB) {
	eu8 h = static_cast<eu8>((source >> 8) & 0xFF);
	eu8 l = static_cast<eu8>(source & 0xFF);
	if (isMSB == true) {
		array[0] = h;
		array[1] = l;
	} else {
		array[0] = l;
		array[1] = h;
	}
}

void Utility::toArray(const eu32& source, eu8_p array, bool isMSB) {
	if (isMSB == true) {
		array[0] = static_cast<eu8>((source >> 24) & 0xFF);
		array[1] = static_cast<eu8>((source >> 16) & 0xFF);
		array[2] = static_cast<eu8>((source >> 8) & 0xFF);
		array[3] = static_cast<eu8>((source) & 0xFF);
	} else {
		array[3] = static_cast<eu8>((source >> 24) & 0xFF);
		array[2] = static_cast<eu8>((source >> 16) & 0xFF);
		array[1] = static_cast<eu8>((source >> 8) & 0xFF);
		array[0] = static_cast<eu8>((source) & 0xFF);
	}
}

void Utility::toArray(estring_cr str, eu8_p ary, int length, eu8 stuffAsciiNum) {
	estring::size_type len = str.length();
	estring::size_type aryLen = (estring::size_type)length;
	for (estring::size_type l = 0; l < aryLen; l++) {
		if (l < len) {
			ary[l] = (eu8)str.at(l);
		} else {
			ary[l] = stuffAsciiNum;
		}
	}
}

int Utility::ceil(int dividend, int divisor) {
	int result = dividend / divisor;
	if (dividend % divisor) {
		result++;
	}
	return result;
}

void Utility::makeBuf(eu32 number, eu32 length, eu8_p buf) {
	//align 512
	if ((length % 512) == 0) {
		eu8 wsl[512];
		for (eu32 a = 0; a < 512; a += 4) {
			toArray(number, wsl + a);
		}
		int cnt = length / 512;

		for (int b = 0; b < cnt; b++) {
			memcpy(buf + b * 512, wsl, 512);
		}
		return;
	}

	for (eu32 c = 0; c < length; c += 4) {
		toArray(number, buf + c);
	}

	// make signature
    eu32 cnt = length / 512;
	for (eu32 d = 0; d < cnt; d++) {
		toArray(number + d, buf + d * 512);
	}
}

eu32 Utility::hexToU32(estring_cr hex) {
	eu32 b = tstrtoul(hex.c_str(), NULL, 16);
	return b;
}

eu16 Utility::toU16(eu8_sp ary, bool isBigEndian) {
	eu16 value = 0;
	if (isBigEndian) {
		value |= ary[0] << 8;
		value |= ary[1];
	} else {
		value |= ary[1] << 8;
		value |= ary[0];
	}
	return value;
}

eu32 Utility::toU32(eu8_sp ary, bool isBigEndian) {
	eu32 value = 0;
	if (isBigEndian) {
		value |= ary[0] << 24;
		value |= ary[1] << 16;
		value |= ary[2] << 8;
		value |= ary[3];
	} else {
		value |= ary[3] << 24;
		value |= ary[2] << 16;
		value |= ary[1] << 8;
		value |= ary[0];
	}
	return value;
}

eu32 Utility::getCheckSum(eu8_sp ary, eu32 length) {
	eu32 ckSum = 0;
	for (eu32 m = 0; m < length; m++) {
		ckSum += ary[m];
	}
	return ckSum;
}

eu32 Utility::getRandom(eu32 min, eu32 max) {
	int x = rand();
	eu32 y = (eu32)(x * ((float)((max)-(min))) / (float)(RAND_MAX)+(min));
	return y;
}

