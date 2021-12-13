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
        estring strFormat(echar_sp fmt, ...);
        estring strFormatValist(int forDispatch, echar_sp fmt, va_list marker);

        estring toString(const eu32& val);

        //convert char array to estring
        estring toString(eu8_p array, eu32 len);
        estring char_to_estring(const char* charStr);

		estring toHexString(const eu8& val);
		estring toHexString(const eu16& val);
        estring toHexString(const eu32& val, estring_cr param = _ET("%X"));
        estring toHexString(const eu64& val, estring_cr param = _ET("%X"));

        estring arrayToString(eu8_sp charArray, int length, bool isEndWhenZero = false);
        estring arrayToHexString(eu8_sp charArray, int length, estring_cr filler = _ET(" "));

        estring makeHexTable(int length, eu8_sp ary, eu16 offset = 0, bool needHeader = true);
        
        estring makeHexTable_u32(eu32 length, eu32_sp ary);
        
        estring makeAsciiTable(eu8_sp ary, int length);

		void convertToWchar(wchar_t* wc, const char* c, int maxLen);

        ERIC_STATIC estring crLf();
        
        //---------- for file

        void toFile(estring_cr filePath, estring_cr msg, bool isAppend);
        void toFile(estring_cr filePath, eu8_p data, int length, bool isAppend = false);


        eu8_p getFileData(estring_cr filePath, eu32 fileSize, eu8_p data);
        eu32 getFileData(estring_cr filePath, eu8_p data);


        eu32 getFileSize(estring_cr filePath);
        bool isFileExist(estring_cr filePath);
        int deleteFile(estring_cr filePath);

        // for folder
        void createFolder(estring_cr folderName);

        //---------- for convert
        int toInt(const bool& boolean);
        int toInt(eu8 value);

        bool toBool(eu32 intValue);

        void toArray(const eu16& source, eu8_p array, bool isMSB = true);
        void toArray(const eu32& source, eu8_p array, bool isMSB = true);
        void toArray(estring_cr str, eu8_p ary, int length, eu8 stuffAsciiNum = 0x20);

        eu32 hexToU32(estring_cr hex);

        eu16 toU16(eu8_sp ary, bool isBigEndian = true);
        eu32 toU32(eu8_sp ary, bool isBigEndian = true);

        // for bit
        int getBit(const eu8& byte, int bitNo);
        int getBit(const eu16& word, int num);

        int ceil(int dividend, int divisor);
        void makeBuf(eu32 number, eu32 length, eu8_p buf);

        eu32 getCheckSum(eu8_sp ary, eu32 length);
        eu32 getRandom(eu32 min, eu32 max);

    private:
        estring gen_header_u8();
        estring gen_header_u32();
        estring table_adjust(eu32 curByteCnt);
        estring _toStringBase(estring_cr param, eu64 arg);
  
    };
}

using namespace EricCore;
