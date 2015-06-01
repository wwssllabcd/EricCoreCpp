#pragma once

//#define USE_BOOST

// put include MyException.h in here, if you wanna use Utility, you msut include MyException.h, too

#include "./DefineFiles/DefineFile.h"
#include "./EricException/MyException.h" // for less include
#include <EricCore\Utility\EricException\ExceptionGenerater.h>

#include <string>
#include <vector>

// for gcc
#include "stdarg.h"

#ifdef USE_BOOST
//#include <boost/function.hpp>
//#include <boost/function_equal.hpp>
#endif

using namespace std;

#define GetRandom(min,max) rand()*((float)((max)-(min)))/(float)(RAND_MAX)+(min)

namespace EricCore{
	class Utility
	{
	public:
		Utility(void);
		~Utility(void);

#define FunT void(*)(tstring&,bool)
#define FunWithParam(x) void(*x)(tstring&, bool)

#ifdef _ERIC_WIN_MFC_
		typedef void(* Utility::MsgFunPtr )(tstring&,bool); 
#else
		typedef void(* MsgFunPtr )(tstring&,bool); 
#endif

		typedef vector<MsgFunPtr> Observers;

		static Observers m_observers;
		static void observerRegister(MsgFunPtr);

		static void sendMsg( tstring& msg,bool isCrLf=true, bool isClean=false);
		static void sendMsg(bool isCrLf, bool isClean, LPCTSTR fmt, ...);
		static void sendMsg(LPCTSTR fmt, ...);

		static tstring toHexString(const BYTE& arg);
		static tstring toHexString(const WORD& arg);
		static tstring toHexString(const ULONG& arg, tstring param="%X");
		static tstring toHexString(const int& arg);

		static tstring toString(const size_t& arg);
		static tstring toString(const int& arg);
		static tstring toString(const ULONG& arg);

		static int toInt(const tstring& cs);
		static int toInt(const bool& boolean);
		static int toInt(BYTE value);
		static BYTE hexCharToByte(BYTE hign, BYTE low);
		static BYTE hexCharToByte(BYTE ascHexNo);

		static ULONG hexStringToULONG(tstring& hex);
		static bool toBool(int intValue);

		static void toArray(const WORD& source, BYTE* array,bool isMSB = true);
		static void toArray(const ULONG& source, BYTE* array,bool isMSB = true);
		static void toArray(const tstring str, BYTE* ary, int length, BYTE stuffAsciiNum = 0x20);
		static void toArray(const tstring str, WORD* ary, int length, BYTE stuffAsciiNum = 0x20);

		static tstring uCharToAvailableString(const BYTE& c, tstring stuffAsciiNum = ".");
		static void highLowByteChange(BYTE* array,int length);

		static bool stringSplit(tstring sign, tstring& orgStr, tstring& str1, tstring& str2);
		static void stringSplit(vector<tstring>& array, tstring sign, tstring& orgStr);


		//BYTE ptr <--> WORD ptr
		static WORD* toWordPtr(const BYTE* ptr);
		static BYTE* toBytePtr(const WORD* ptr);

		static tstring arrayToString(const BYTE* charArray,int length,bool isEndWhenZero = false);
		static tstring arrayToHexString(const BYTE* charArray, int length, const tstring& filler= " " );

		// for bit
		static int getBit(const BYTE& byte,int bitNo);
		static int getBit(const WORD& word,int num);

		static void setBit(ULONG& target,BYTE numberOfbit,int intValue);
		static void setBit(WORD& target,BYTE numberOfbit,int intValue);
		static void setBit(BYTE& target,BYTE numberOfbit,int intValue);
		static void setBit(BYTE& target,BYTE numberOfbit,bool isSet_1);

		//for file
		static void toFile(const tstring& filePath, const tstring& msg, bool isAppend);
		static void toFile(const tstring& filePath, BYTE* data, int length, bool isAppend = false);
		

		static FILE* getFilePtr(string filePath, string condition);
		static void getFileData(tstring filePath, BYTE* data);
		static void getFileData(tstring filePath, int startAddr, int length, BYTE* data);
		static long getFileSize(tstring filePath);
		static bool isFileExist(const tstring& filePath);

		// gen table
		static tstring makeHexTable(int length, BYTE* ary, WORD offset=0, bool needHeader=true);


		static tstring makeAsciiTable(BYTE* ary, int length);
		static tstring CrLf();

		static int ceil(int dividend, int divisor);

		static void sourceDataToTarget(FILE* source, FILE* target, int length);
		static void swap(BYTE* a, BYTE* b);

		static void makeBuf(ULONG number, int length, BYTE* buf);
		static tstring getDiffStringInTwoBuf(ULONG lbaAddr, int length, BYTE* writeBuf, BYTE* readBuf);

		static WORD getCheckSum(BYTE* ary, int length);
		static void makeRandomBuffer(BYTE* pBuf, int length);

		//For warnning
		static bool uintToBool(UINT value);
		static ULONG arrayToUlong(BYTE* ary);
		static WORD arrayToWord(BYTE* ary);

		static int getRandomValue(int min, int max);

		static void not_exec(BYTE* aryBin, int len);
		static int getModulus(const int& a ,const int& b);

		static tstring strFormat(const TCHAR* fmt, ...) ;
		static tstring strFormat(int forDispatch, const char* fmt, va_list marker);

		static bool isHexFormat(const tstring& s);
		static bool isAllNumberType(tstring& data);

#ifdef _ERIC_WIN_MFC_
		//window only
		static void getFileColls(tstring strFilePath, vector<tstring>& fileColls);
		static void showMessageBox(const TCHAR* msg);
		static void showMessageBox(tstring& msg);
		static tstring getStringParamFromIni( tstring path, tstring appName, tstring keyName);
		static bool getBoolParamFromIni(tstring modelName, tstring appName, int defValue, tstring filePath);
		static UINT getIntParamFromIni(tstring modelName, tstring keyName, int nDefault, tstring filePath);

		static tstring getCurrnetPath(void);
		static tstring showErrorCodeMFC(void);

		static tstring genTimeString(const SYSTEMTIME& current_date_time);
		static tstring genTimeString(const SYSTEMTIME& current_date_time, const tstring& separateSign, const tstring& format, const int separateType);
		static bool saveToIni(const tstring& appName, const tstring& keyName, const int& result, const tstring& filePath);
		static bool saveToIni(const tstring& appName, const tstring& keyName, const bool& result, const tstring& filePath);
		static bool saveToIni(const tstring& appName, const tstring& keyName, const tstring& result, const tstring& filePath);
#endif
		
		template<class T>
		static void arrayToVector( T* source, size_t len, vector<T>& target){
			T tmp;
			for(size_t i=0; i<len; i++){
				tmp = source[i];
				target.push_back(tmp);
			}
		}

		template<class T>
		static bool comparePair(const T& p1, const T& p2 ){
			if( p1.first == p2.first ){
				if( p1.second < p2.second ){
					return true;
				}
				return false;
			}

			if( p1.first < p2.first ){
				return true;
			}
			return false;
		}

	private:
		static void _remove_0x_And_H(tstring& cs);

		//string
		static tstring _toStringBase(tstring param, const int& arg);

	};
}