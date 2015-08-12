#pragma once

#include "../DefineFiles/DefineFile.h"

#include <string>
using namespace std;

namespace EricCore{

	class MyExcepBase : public std::exception
	{
	public:
		MyExcepBase(void){};
		~MyExcepBase(void){};

		MyExcepBase(int errorCode, tstring msg);
		MyExcepBase(int errorCode, LPCTSTR fmt, ...);
		MyExcepBase(LPCTSTR fmt, ...);

		int getErrorCode(void);
		tstring what(void);
		const char * what(void) const;

	private:
		int m_errorCode;
		tstring m_errMsg;
	};
}

