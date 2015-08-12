#include "StdAfx.h"
#include "MyExcepBase.h"
#include "../Utility.h"

using namespace EricCore;

MyExcepBase::MyExcepBase(int errorCode, tstring msg)
:m_errorCode(errorCode)
{
	m_errMsg = msg;
}

MyExcepBase::MyExcepBase(int errorCode, LPCTSTR fmt, ...)
:m_errorCode(errorCode)
{
	tstring str;
	va_list	arg;
	va_start( arg, fmt );
	str = Utility::strFormat(0, fmt, arg);
	va_end( arg );

	m_errMsg = str;
}

MyExcepBase::MyExcepBase(LPCTSTR fmt, ...)
:m_errorCode(0)
{
	tstring str;
	va_list	arg;
	va_start( arg, fmt );
	str = Utility::strFormat(0, fmt, arg);
	va_end( arg );
	m_errMsg = str;
}

tstring MyExcepBase::what(void){
	return m_errMsg;
}

const char * MyExcepBase::what(void) const{
	return m_errMsg.c_str();
}

int MyExcepBase::getErrorCode(void){
	return m_errorCode;
}