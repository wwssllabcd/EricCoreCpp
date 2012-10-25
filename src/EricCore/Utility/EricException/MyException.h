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