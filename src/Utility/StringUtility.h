#pragma once

#include "DefineFiles/DefineFile.h"
#include <vector>

using namespace std;


namespace EricCore {
    class StringUtility
    {
    public:
        StringUtility(void);
        ~StringUtility(void);

        estring strFormat(const echar *fmt, ...);
        estring strFormat(int forDispatch, const echar* fmt, va_list marker);

        estring toString(const size_t& arg);
        estring toString(const int& arg);

        estring toHexString(const eu8& arg);
        estring toHexString(const eu16& arg);
        estring toHexString(const eu32& arg);

        estring arrayToString(const eu8* charArray, int length, bool isEndWhenZero = false);
        estring arrayToHexString(const eu8* charArray, int length, const estring& filler = _ET(" "));

        estring makeHexTable(int length, eu8* ary, eu16 offset = 0, bool needHeader = true);
        estring makeAsciiTable(eu8* ary, int length);

        bool stringSplit(const estring& sign, const estring& orgStr, estring& str1, estring& str2);
        void stringSplit(vector<estring>& array, const estring& sign, const estring& orgStr);

        ERIC_STATIC estring crLf();

    private:
        estring _toStringBase(estring param, const int& arg);
    };
}