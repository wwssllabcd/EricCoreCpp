#pragma once

#include "../DefineFiles/DefineFile.h"

#include <string>
using namespace std;

namespace EricCore{

	class MyException : public std::exception
	{
	public:
		MyException(void){};
		~MyException(void){};

		MyException(int errorCode, tstring msg);
		MyException(int errorCode, LPCTSTR fmt, ...);
		MyException(LPCTSTR fmt, ...);

		

		int getErrorCode(void);
		tstring what(void);
		const char * what(void) const;


		static tstring separeString;


	//private:

		int m_errorCode;
		tstring m_errMsg;

	};
}




