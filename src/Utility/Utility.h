#pragma once

#include "DefineFiles/DefineFile.h"
#include <fstream>     // fstream
#include "EricException.h"
#include <thread>
#include <vector>

using namespace std;
using std::this_thread::sleep_for;
using std::vector;

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
        estring makeHexTable_u32(eu32 length, eu32_sp ary, bool needHeader = true);
        
        estring makeAsciiTable(eu8_sp ary, int length);

		void convertToWchar(wchar_t* wc, const char* c, int maxLen);

        vector<estring> stringSplit(const estring& s, echar delim);

        ERIC_STATIC estring crLf();
        
        //---------- for file
        void toFile(estring_cr filePath, estring_cr msg, bool isAppend = false);
        void toFile(estring_cr filePath, eu8_p data, eu32 length, bool isAppend = false);


        eu8_p getFileData(estring_cr filePath, eu32 fileSize, eu8_p data);
        eu32 getFileData(estring_cr filePath, eu8_p data);
        estring getFileData(estring_cr filePath);


        eu32 getFileSize(estring_cr filePath);
        bool isFileExist(estring_cr filePath);
        int deleteFile(estring_cr filePath);

        // for folder
        void createFolder(estring_cr folderName);

        //---------- for convert
        int toInt(const bool& boolean);
        int toInt(eu8 value);
        int toInt(estring value);

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
        estring table_adjust(eu32 curByteCnt, bool needSideBar);
        estring _toStringBase(estring_cr param, eu64 arg);
  

        // output to Text file
        template <typename T1, typename T2 >
        void _toFile(estring_cr filePath, T2 data, eu32 dataLen, bool isAppend = false) {
            if (filePath.length() == 0) {
                THROW_MYEXCEPTION(UTI_OPEN_FILE_FAIL, _ET("toFile: length of filePath = 0, name=") + filePath);
            }

            //if it have not the file , ofstream will make a new file
            T1 ofs;

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
                sleep_for(std::chrono::milliseconds(100));
            }

            if (ofs.fail() == true) {
                THROW_MYEXCEPTION(UTI_OPEN_FILE_FAIL, _ET("toFile: Can`t open file ") + filePath);
            }

            // output data by ANSI stream, because it's "byte array"
            ofs.write(data, dataLen);

            // even you didn't close, ofs will be close in destructor
            // fstream destruct call close for you. When an exception is thrown, the file is closed automatically.(RAII)
            ofs.close();
        }

    };
}


