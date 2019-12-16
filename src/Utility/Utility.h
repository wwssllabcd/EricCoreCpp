#pragma once

#include "DefineFiles/DefineFile.h"

using namespace std;

namespace EricCore {
    class Utility
    {
    public:
        Utility(void);
        ~Utility(void);

        //---------- for string 
        estring strFormat(const echar* fmt, ...);
        estring strFormat(int forDispatch, const echar* fmt, va_list marker);

        estring toString(const size_t& arg);
        estring toString(const int& arg);

        estring toHexString(const eu8& arg);
        estring toHexString(const eu16& arg);
        estring toHexString(const eu32& arg);

        estring arrayToString(eu8_sp charArray, int length, bool isEndWhenZero = false);
        estring arrayToHexString(eu8_sp charArray, int length, const estring& filler = _ET(" "));

        estring makeHexTable(int length, eu8_sp ary, eu16 offset = 0, bool needHeader = true);
        estring makeAsciiTable(eu8_sp ary, int length);

        ERIC_STATIC estring crLf();
        
        //---------- for file

        void toFile(const estring& filePath, const estring& msg, bool isAppend);
        void toFile(const estring& filePath, eu8_p data, int length, bool isAppend = false);

        eu8_p getFileData(const estring& filePath, eu8_p data);
        eu8_p getFileData(const estring& filePath, eu32 fileSize, eu8_p data);
        eu8_p getFileData(const estring& filePath, int startAddr, int length, eu8_p data);

        eu32 getFileSize(const estring& filePath);
        FILE* getFilePtr(const estring& filePath, const estring& condition);
        bool isFileExist(const estring& filePath);
        int deleteFile(const estring& filePath);

        // for folder
        void createFolder(const estring& folderName);

        //---------- for convert
        int toInt(const bool& boolean);
        int toInt(eu8 value);

        bool toBool(eu32 intValue);

        void toArray(const eu16& source, eu8_p array, bool isMSB = true);
        void toArray(const eu32& source, eu8_p array, bool isMSB = true);
        void toArray(const estring& str, eu8_p ary, int length, eu8 stuffAsciiNum = 0x20);

        eu32 hexToU32(estring hex);

        eu16 toU16(eu8_sp ary, bool isBigEndian = true);
        eu32 toU32(eu8_sp ary, bool isBigEndian = true);

        // for bit
        int getBit(const eu8& byte, int bitNo);
        int getBit(const eu16& word, int num);

        int ceil(int dividend, int divisor);
        void makeBuf(eu32 number, int length, eu8_p buf);

        eu32 getCheckSum(eu8_sp ary, eu32 length);
        eu32 getRandom(eu32 min, eu32 max);

    private:
        estring _toStringBase(estring param, const int& arg);
        eu8_p _getFileDataNew(const estring& filePath, int length, eu8_p data);
    };
}

using namespace EricCore;
