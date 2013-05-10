#pragma once

#include "../DefineFiles/DefineFile.h"

#include <string>
using namespace std;

namespace EricCore{
	class MyException
	{
	public:
		MyException(int errorCode, tstring msg);
		MyException(int errorCode, LPCTSTR fmt, ...);
		MyException(LPCTSTR fmt, ...);

		~MyException(void);

		int getErrorCode(void);
		tstring what(void);


		static tstring separeString;


	private:

		int m_errorCode;
		tstring m_errMsg;

	};
}


class MyExcepBase
{
public:
	MyExcepBase(void);
	~MyExcepBase(void);
};



//#define THROW_EXCEPTION(msg, errorNo) \
//{\
//	class Excep_##msg : public MyExcepBase \
//	{\
//		public:\
//		  const int  errorCode; \
//	};\
//	Excep_##msg::errorCode=errorNo;\
//}
//
//THROW_EXCEPTION(TEST, 1)



class Excep_TEST : public MyExcepBase 
{
public:
	static const int  errorCode; 
};

#ifndef AAA
#define AAA
//const int Excep_TEST::errorCode=1;
#endif



