#include "StdAfx.h"

#include "MyException.h"

#include "../DefineFiles/ErrorCode.h"
#include "../Utility.h"

using namespace EricCore;

tstring MyException::separeString = _T("||");

MyException::MyException(int errorCode, tstring msg)
:m_errorCode(errorCode)
{
	m_errMsg = msg;
}


MyException::MyException(int errorCode, LPCTSTR fmt, ...)
:m_errorCode(errorCode)
{
	tstring str;
	va_list	arg;
	va_start( arg, fmt );
	str = Utility::strFormat(0, fmt, arg);
	va_end( arg );

	m_errMsg = str;
}

MyException::MyException(LPCTSTR fmt, ...)
:m_errorCode(0)
{
	tstring str;
	va_list	arg;
	va_start( arg, fmt );
	str = Utility::strFormat(0, fmt, arg);
	va_end( arg );
	m_errMsg = str;
}


MyException::~MyException(void)
{
}

tstring MyException::what(void){
	return m_errMsg;
}

const char * MyException::what(void) const{
	return m_errMsg.c_str();
}

int MyException::getErrorCode(void){
	return m_errorCode;
}







