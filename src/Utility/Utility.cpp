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


//estring Utility::getDiffStringInTwoBuf(eu32 lbaAddr, int length, eu8* writeBuf, eu8* readBuf) {
//	estring result, msg;
//    StringUtility su;
//	int errorCnt = 0;
//	result = StringUtility::crLf() + su.strFormat(_ET("  cmp error in 0x%x"), lbaAddr);
//	
//	for(int g = 0; g < length; g++) {
//		if(writeBuf[g] != readBuf[g]) {
//			if(errorCnt < 20) {
//				msg = su.strFormat(_ET("  writeBuf[%x]=0x%x,readBuf[%x]=0x%x"), g, writeBuf[g], g, readBuf[g]);
//				result += StringUtility::crLf() + msg;
//			}
//			errorCnt++;
//		}
//	}
//
//	if(errorCnt >= 20) {
//		result += StringUtility::crLf() + _ET("over 20 error");
//	}
//
//	result += StringUtility::crLf() + su.strFormat(_ET("Fail ECC Cnt = 0x%X"), errorCnt);
//	return result;
//}
