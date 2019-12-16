//Copyright © 2006- EricWang(wwssllabcd@gmail.com). All rights reserved

#include "stdafx.h"
#include "StringUtility.h"
#include <stdarg.h> // for va_start, va_end
#include <vector>

using namespace EricCore;
using namespace std;

StringUtility::StringUtility(void) {
}

StringUtility::~StringUtility(void) {
}

estring StringUtility::crLf() {
    return _ET("\r\n");
}

estring StringUtility::strFormat(const echar *fmt, ...) {
    estring retStr(_ET(""));
    if (NULL != fmt) {
        va_list marker;
        va_start(marker, fmt); 	// initialize variable arguments
        retStr = strFormat(0, fmt, marker);
        va_end(marker);         // Reset variable arguments
    }
    return retStr;
}

estring StringUtility::strFormat(int forDispatch, const echar* fmt, va_list marker) {
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

estring StringUtility::_toStringBase(estring param, const int& arg) {
    estring str = strFormat(param.c_str(), arg);
    return str;
}

estring StringUtility::toString(const size_t& arg) {
    return _toStringBase(_ET("%ld"), (int)arg);
}

estring StringUtility::toString(const int& arg) {
    return _toStringBase(_ET("%ld"), arg);
}

estring StringUtility::toHexString(const eu8& arg) {
    return _toStringBase(_ET("%02X"), arg);
}

estring StringUtility::toHexString(const eu16& arg) {
    return _toStringBase(_ET("%04X"), arg);
}

estring StringUtility::toHexString(const eu32& arg) {
    return _toStringBase(_ET("%08X"), arg);
}

//array usually use "0x00" value for the end
estring StringUtility::arrayToString(const eu8* charArray, int length, bool isEndWhenZero) {
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

estring StringUtility::arrayToHexString(const eu8* charArray, int length, const estring& filler) {
    estring  str, temp;
    int s;
    for (s = 0; s < length; s++) {
        temp = strFormat(_ET("%02X"), charArray[s]);
        str += temp + filler;
    }
    return str;
}

estring StringUtility::makeAsciiTable(eu8* ary, int length) {
    estring msg;
    int width = 0x10;
    int row = 0;
    int rowCnt = length / width;
    if ((length%width) != 0) {
        rowCnt++;
    }

    for (row = 0; row < rowCnt; row++) {
        msg += arrayToString(ary + row * width, width);
        msg += StringUtility::crLf();
    }

    int mod = length % width;
    msg += arrayToString(ary + length - mod, mod);
    msg += StringUtility::crLf();
    return msg;
}

estring StringUtility::makeHexTable(int length, eu8* ary, eu16 offset, bool needHeader) {
    estring msg;
    estring head;
    if (needHeader) {
        head += _ET("0000 | 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F ") + StringUtility::crLf();
        head += _ET("==== | ============================") + StringUtility::crLf();
        msg += head;
    }

    int i = 0;
    while (i < length) {
        if ((i % 512) == 0) {
            if (i != 0) {
                msg += StringUtility::crLf();
            }
        }

        if (needHeader) {
            if ((i % 0x10) == 0) {
                estring row = strFormat(_ET("%04X"), i + offset);
                msg += row;
                msg += _ET(" | ");
            }
        }

        msg += toHexString(ary[i]) + _ET(" ");
        if (((i + 1) % 0x10) == 0) {
            msg += StringUtility::crLf();
        }
        i++;
    }
    return msg;
}

bool StringUtility::stringSplit(const estring& sign, const estring& orgStr, estring& str1, estring& str2) {
    estring::size_type locate = orgStr.find(sign);
    if (locate == estring::npos) {
        return false;
    }

    str1 = orgStr.substr(0, locate);
    if ((locate + 1) < orgStr.length()) {
        str2 = orgStr.substr(locate + 1);
    }
    return true;
}

void StringUtility::stringSplit(vector<estring>& array, const estring& sign, const estring& orgStr) {
    estring strOp = orgStr;
    while (1) {
        estring str1, str2;
        if (stringSplit(sign, strOp, str1, str2) == false) {
            array.push_back(strOp);
            break;
        }
        array.push_back(str1);
        strOp = str2;
    }
}