#pragma once

//#define USE_BOOST

// put include MyException.h in here, if you wanna use Utility, you msut include MyException.h, too


//#include "Observer.h" // for less include
#include "DefineFiles/DefineFile.h"
#include <string>
#include <vector>

// for gcc
//#include "stdarg.h"

using namespace std;

#define GetRandom(min,max) rand()*((float)((max)-(min)))/(float)(RAND_MAX)+(min)

namespace EricCore {
    class Utility
    {
    public:
        Utility(void);
        ~Utility(void);

        int toInt(const estring& cs);
        int toInt(const bool& boolean);
        int toInt(eu8 value);

        eu8 hexCharToNumber(echar ascHexNo);

        eu32 hexStringToULONG(estring hex);
        bool toBool(eu32 intValue);

        void toArray(const eu16& source, eu8* array, bool isMSB = true);
        void toArray(const eu32& source, eu8* array, bool isMSB = true);
        void toArray(const estring& str, eu8* ary, int length, eu8 stuffAsciiNum = 0x20);

        // for bit
        int getBit(const eu8& byte, int bitNo);
        int getBit(const eu16& word, int num);

        // gen table
        int ceil(int dividend, int divisor);
        void swap(eu8* a, eu8* b);

        void makeBuf(eu32 number, int length, eu8* buf);
        estring getDiffStringInTwoBuf(eu32 lbaAddr, int length, eu8* writeBuf, eu8* readBuf);
        eu16 getCheckSum_u16(eu8* ary, int length);
        eu32 getCheckSum_u32(eu8* ary, int length);
        eu32 get_4_byte_CheckSum(eu8* ary, int length);

        //For warnning
        eu32 arrayToUlong(eu8* ary);
        eu32 arrayToUlong_le(eu8* ary);
        eu16 arrayToWord(eu8* ary);


        bool isHexFormat(const estring& s);

        estring get_time_string(void);

        template<class T>
        void arrayToVector(T* source, size_t len, vector<T>& target) {
            T tmp;
            for (size_t i = 0; i < len; i++) {
                tmp = source[i];
                target.push_back(tmp);
            }
        }

        template<class T>
        bool comparePair(const T& p1, const T& p2) {
            if (p1.first == p2.first) {
                if (p1.second < p2.second) {
                    return true;
                }
                return false;
            }

            if (p1.first < p2.first) {
                return true;
            }
            return false;
        }

    private:
        void _remove_0x_And_H(estring& cs);
    };
}