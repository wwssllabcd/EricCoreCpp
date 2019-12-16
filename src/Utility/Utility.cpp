//Copyright © 2006- EricWang(wwssllabcd@gmail.com). All rights reserved

#include "stdafx.h"
#include "Utility.h"

#include <bitset>
#include <algorithm>
#include <time.h>

#include "EricException.h"
#include "StringUtility.h"

//// for gcc
//#include <string.h>
//#include <stdio.h>
//#include <cctype> // for tolower function pointer
using namespace EricCore;

#ifdef USE_BOOST
#include <boost/lexical_cast.hpp>
#endif

//Utility::Observers Utility::m_observers;
Utility::Utility(void) {
}

Utility::~Utility(void) {
}

bool Utility::isHexFormat(const estring& s) {
	if(s.length() < 2) {
		return false;
	}
	if(s.at(0) == _ET('0') && s.at(1) == _ET('x')) {
		return true;
	}
	if(s.at(s.length() - 1) == _ET('h')) {
		return true;
	}
	// if no 0x*** or ***h, it should be decimal
	return false;
}

void Utility::_remove_0x_And_H(estring& cs) {
	if(cs.find(_ET("h")) != estring::npos) {
		cs.replace(cs.find(_ET("h")), 1, _ET(""));
	}

	if(cs.find(_ET("0x")) != estring::npos) {
		cs.replace(cs.find(_ET("0x")), 2, _ET(""));
	}
}

inline echar charToLower(echar c) {
	return tolower(c);
}

int Utility::toInt(const estring& cs) {
	estring s = cs;
	if(s.length() == 0) {
		return 0;
	}
	transform(s.begin(), s.end(), s.begin(), charToLower);

	//if it's not hex format
	if(isHexFormat(s) == false) {
		return tatoi(s.c_str());
	}

	estring tmpEw(s), aChar;
	int sum(0), aInt(0);

	_remove_0x_And_H(tmpEw);

	estring::size_type len = tmpEw.length();
	for(estring::size_type i = 0; i < len; i++) {
		aChar = tmpEw.at(len - i - 1);
		if(aChar.compare(_ET("a")) == 0) {
			aInt = 10;
		} else if(aChar.compare(_ET("b")) == 0) {
			aInt = 11;
		} else if(aChar.compare(_ET("c")) == 0) {
			aInt = 12;
		} else if(aChar.compare(_ET("d")) == 0) {
			aInt = 13;
		} else if(aChar.compare(_ET("e")) == 0) {
			aInt = 14;
		} else if(aChar.compare(_ET("f")) == 0) {
			aInt = 15;
		} else {
			aInt = tatoi(aChar.c_str());
		}

		sum += aInt*(0x01 << (4 * i));
	}
	return sum;
}

int Utility::toInt(const bool& boolean) {
	int intTemp;
	if(boolean) {
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
	if(intValue == 0) {
		return false;
	}
	return true;
}

eu32 Utility::hexStringToULONG(estring hex) {
	eu32 b = tstrtoul(hex.c_str(), NULL, 16);
	return b;
}

void Utility::toArray(const eu16& source, eu8* array, bool isMSB) {
	eu8 h = static_cast<eu8>((source >> 8) & 0xFF);
	eu8 l = static_cast<eu8>(source & 0xFF);
	if(isMSB == true) {
		array[0] = h;
		array[1] = l;
	} else {
		array[0] = l;
		array[1] = h;
	}
}

void Utility::toArray(const eu32& source, eu8* array, bool isMSB) {
	if(isMSB == true) {
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

void Utility::toArray(const estring& str, eu8* ary, int length, eu8 stuffAsciiNum) {
	estring::size_type len = str.length();
	estring::size_type aryLen = (estring::size_type)length;
	for(estring::size_type l = 0; l < aryLen; l++) {
		if(l < len) {
			ary[l] = (eu8)str.at(l);
		} else {
			ary[l] = stuffAsciiNum;
		}
	}
}

int Utility::ceil(int dividend, int divisor) {
	int result = dividend / divisor;
	if(dividend%divisor) {
		result++;
	}
	return result;
}

void Utility::makeBuf(eu32 number, int length, eu8* buf) {
	//align 512
	if((length % 512) == 0) {
		eu8 wsl[512];
		for(int a = 0; a < 512; a += 4) {
			toArray(number, wsl + a);
		}
		int cnt = length / 512;

		for(int b = 0; b < cnt; b++) {
			memcpy(buf + b * 512, wsl, 512);
		}
		return;
	}

	for(int c = 0; c < length; c += 4) {
		toArray(number, buf + c);
	}

	// make signature
	int cnt = length / 512;
	for(int d = 0; d < cnt; d++) {
		toArray(number + d, buf + d * 512);
	}
}

estring Utility::getDiffStringInTwoBuf(eu32 lbaAddr, int length, eu8* writeBuf, eu8* readBuf) {
	estring result, msg;
    StringUtility su;
	int errorCnt = 0;
	result = StringUtility::crLf() + su.strFormat(_ET("  cmp error in 0x%x"), lbaAddr);
	
	for(int g = 0; g < length; g++) {
		if(writeBuf[g] != readBuf[g]) {
			if(errorCnt < 20) {
				msg = su.strFormat(_ET("  writeBuf[%x]=0x%x,readBuf[%x]=0x%x"), g, writeBuf[g], g, readBuf[g]);
				result += StringUtility::crLf() + msg;
			}
			errorCnt++;
		}
	}

	if(errorCnt >= 20) {
		result += StringUtility::crLf() + _ET("over 20 error");
	}

	result += StringUtility::crLf() + su.strFormat(_ET("Fail ECC Cnt = 0x%X"), errorCnt);
	return result;
}

void Utility::swap(eu8* a, eu8* b) {
	eu8 wsl = a[0];
	a[0] = b[0];
	b[0] = wsl;
}

eu16 Utility::getCheckSum_u16(eu8* ary, int length) {
	eu16 ckSum = 0;
	for(int m = 0; m < length; m++) {
		ckSum += ary[m];
	}
	return ckSum;
}

eu32 Utility::getCheckSum_u32(eu8* ary, int length) {
	eu32 ckSum = 0;
	for(int m = 0; m < length; m++) {
		ckSum += ary[m];
	}
	return ckSum;
}

eu32 Utility::get_4_byte_CheckSum(eu8* ary, int length) {
	eu32 ckSum = 0;

	if(length % 4) {
		THROW_MYEXCEPTION(0, "get_4_byte_CheckSum: MUST BE 4 byte align");
	}

	for(int m = 0; m < length; m+=4) {
		ckSum += arrayToUlong_le(ary + m);
	}
	return ckSum;
}

eu32 Utility::arrayToUlong(eu8* ary) {
	eu32 value = 0;
	value |= ary[0] << 24;
	value |= ary[1] << 16;
	value |= ary[2] << 8;
	value |= ary[3];
	return value;
}

eu32 Utility::arrayToUlong_le(eu8* ary) {
	eu32 value = 0;
	value |= ary[3] << 24;
	value |= ary[2] << 16;
	value |= ary[1] << 8;
	value |= ary[0];
	return value;
}

eu16 Utility::arrayToWord(eu8* ary) {
	eu16 value = 0;
	value |= ary[0] << 8;
	value |= ary[1];
	return value;
}

// '12' -> 12h
eu8 Utility::hexCharToNumber(echar ascHexNo) {
	eu8 res = 0xFF;
	// hex 0~9
	if((0x30 <= ascHexNo) && (ascHexNo <= 0x39)) {
		res = ascHexNo - '0';
	}

	//hex A~F
	if(('A' <= ascHexNo) && (ascHexNo <= 'F')) {
		res = ascHexNo - 'A';
	}

	//hex a~f
	if(('a' <= ascHexNo) && (ascHexNo <= 'f')) {
		res = ascHexNo - 'a';
	}
	return res;
}

estring Utility::get_time_string(void) {
    time_t curtime = time(NULL);
    echar str[26];
    tctime_s(str, sizeof(str), &curtime);
    estring res(str);
    return res;
}