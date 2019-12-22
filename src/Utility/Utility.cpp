//Copyright © 2006- EricWang(wwssllabcd@gmail.com). All rights reserved

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

estring Utility::strFormat(const echar* fmt, ...) {
    estring retStr(_ET(""));
    if (NULL != fmt) {
        va_list marker;
        va_start(marker, fmt); 	// initialize variable arguments
        retStr = strFormat(0, fmt, marker);
        va_end(marker);         // Reset variable arguments
    }
    return retStr;
}

estring Utility::strFormat(int forDispatch, const echar* fmt, va_list marker) {
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

estring Utility::_toStringBase(estring param, const int& arg) {
    estring str = strFormat(param.c_str(), arg);
    return str;
}

estring Utility::toString(const size_t& arg) {
    return _toStringBase(_ET("%ld"), (int)arg);
}

estring Utility::toString(const int& arg) {
    return _toStringBase(_ET("%ld"), arg);
}

estring Utility::toString(const eu32& val, estring param) {
    return _toStringBase(param, val);
}

estring Utility::toHexString(const eu8& arg) {
    return _toStringBase(_ET("%02X"), arg);
}

estring Utility::toHexString(const eu16& arg) {
    return _toStringBase(_ET("%04X"), arg);
}

estring Utility::toHexString(const eu32& arg) {
    return _toStringBase(_ET("%08X"), arg);
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

estring Utility::arrayToHexString(eu8_sp charArray, int length, const estring& filler) {
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

estring Utility::makeHexTable(int length, eu8_sp ary, eu16 offset, bool needHeader) {
    estring msg;
    estring head;
    if (needHeader) {
        head += _ET("0000 | 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F ") + Utility::crLf();
        head += _ET("==== | ============================") + Utility::crLf();
        msg += head;
    }

    int i = 0;
    while (i < length) {
        if ((i % 512) == 0) {
            if (i != 0) {
                msg += Utility::crLf();
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
            msg += Utility::crLf();
        }
        i++;
    }
    return msg;
}

eu32 Utility::getFileSize(const estring& filePath) {
    FILE* fpSource = getFilePtr(filePath, _ET("rb"));

    if (fpSource == NULL) {
        estring msg = _ET("GetFileSize: Can`t open file ") + filePath;
        THROW_MYEXCEPTION(UTI_GET_FILE_SIZE_FAIL, msg.c_str());
    }

    fseek(fpSource, 0, SEEK_END);
    eu32 fileLength = 0;
    fileLength = ftell(fpSource);
    fclose(fpSource);
    return fileLength;
}

eu8_p Utility::_getFileDataNew(const estring& filePath, int length, eu8_p data) {
    fstream file;
    file.open(filePath, ios::in | ios::binary);
    file.read((char*)data, length);
    file.close();
    return data;
}

eu8_p Utility::getFileData(const estring& filePath, int startAddr, int length, eu8_p data) {
    if (startAddr == 0) {
        return _getFileDataNew(filePath, length, data);
    }

    int i;
    FILE* fpSource = getFilePtr(filePath, _ET("rb"));

    if (fpSource == NULL) {
        estring msg = _ET("GetFileData: Can`t open file ") + filePath;
        THROW_MYEXCEPTION(UTI_GET_FILE_DATA_FAIL, msg.c_str());
    }

    long fileSize = getFileSize(filePath);

    if ((startAddr + length) > fileSize) {
        THROW_MYEXCEPTION(UTI_GET_FILE_DATA_FAIL, _ET("GetFileData: get file size out of boundary"));
    }

    fseek(fpSource, startAddr, SEEK_SET);
    for (i = 0; i < length; i++) {
        data[i] = fgetc(fpSource);
    }
    fclose(fpSource);
    return data;
}

eu8_p Utility::getFileData(const estring& filePath, eu32 fileSize, eu8_p data) {
    getFileData(filePath, 0, fileSize, data);
    return data;
}

eu8_p Utility::getFileData(const estring& filePath, eu8_p data) {
    long fileSize = getFileSize(filePath);
    getFileData(filePath, fileSize, data);
    return data;
}

// output to Text file
void Utility::toFile(const estring& filePath, const estring& msg, bool isAppend) {
    toFile(filePath, (eu8*)msg.c_str(), (int)msg.length(), isAppend);
}

// output Array to Binary file
void Utility::toFile(const estring& filePath, eu8_p data, int length, bool isAppend) {
    //if it have not the file , ofstream will make a new file
    ofstream ofs;
    int i;
    for (i = 0; i < 10; i++) {
        if (isAppend == true) {
            // ios::binary is mean," Do not replace CRLF "
            ofs.open(filePath.c_str(), ios::out | ios::binary | ios::app);
        } else {
            ofs.open(filePath.c_str(), ios::out | ios::binary | ios::trunc);
        }

        if (ofs.fail() == false) {
            break;
        }
        Sleep(500);
    }

    if (ofs.fail() == true) {
        estring msg = _ET("toFile: Can`t open file ") + filePath;
        THROW_MYEXCEPTION(UTI_TO_FILE_FAIL, msg.c_str());
    }

    // output data by ANSI stream, because it's "byte array"
    ofs.write((char*)data, length);

    // even you didn't close, ofs will be close in destructor
    // fstream destruct call close for you. When an exception is thrown, the file is closed automatically.(RAII)
    ofs.close();
}

// output Array to Binary file
FILE* Utility::getFilePtr(const estring& filePath, const estring& condition) {
    FILE* fpSource = 0;
    tfopen_s(&fpSource, filePath.c_str(), condition.c_str());
    return fpSource;
}

bool Utility::isFileExist(const estring& filePath) {
    struct _stat buffer;
    return (_tstat(filePath.c_str(), &buffer) == 0);
}

int Utility::deleteFile(const estring& filePath) {
    return _tremove(filePath.c_str());
}

void Utility::createFolder(const estring& folderName) {
    if (CreateDirectory(folderName.c_str(), NULL) == 0) {
        if (ERROR_ALREADY_EXISTS != GetLastError()) {
            estring msg = _ET("createFolder: Can`t create folder file ") + folderName;
            THROW_MYEXCEPTION(0, msg.c_str());
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

void Utility::toArray(const estring& str, eu8_p ary, int length, eu8 stuffAsciiNum) {
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

void Utility::makeBuf(eu32 number, int length, eu8_p buf) {
    //align 512
    if ((length % 512) == 0) {
        eu8 wsl[512];
        for (int a = 0; a < 512; a += 4) {
            toArray(number, wsl + a);
        }
        int cnt = length / 512;

        for (int b = 0; b < cnt; b++) {
            memcpy(buf + b * 512, wsl, 512);
        }
        return;
    }

    for (int c = 0; c < length; c += 4) {
        toArray(number, buf + c);
    }

    // make signature
    int cnt = length / 512;
    for (int d = 0; d < cnt; d++) {
        toArray(number + d, buf + d * 512);
    }
}

eu32 Utility::hexToU32(estring hex) {
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