

#include "stdafx.h"

#include "Utility\Utility.h"
#include "Utility\Observer.h"
#include "Utility\EricException.h"

#include <stdarg.h>

using namespace EricCore;

void throw_exception(int errCode, const char* c) {

#ifdef _UNICODE
	wchar_t ws[100];
	Utility u;
	u.convertToWchar(ws, c, sizeof(ws));
	estring msg(ws);
#else
	estring msg(c);
#endif

	THROW_MYEXCEPTION(errCode, msg);
}

ERIC_EXTERN_C void throw_exception_c(int errCode, char* c) {
	throw_exception(errCode, c);
	}
    
ERIC_EXTERN_C void print_msg_valist_c(const char *fmt, va_list arg) {

	Utility u;
	estring	str;

#ifdef _UNICODE
	wchar_t ws[100];
	u.convertToWchar(ws, fmt, sizeof(ws));
	str = u.strFormatValist(0, ws, arg);
#else
	str = u.strFormatValist(0, fmt, arg);
#endif 
	Observer::sendMsg(1, str, true, false);

}

ERIC_EXTERN_C void print_msg_c(const char* fmt, ...) {
	va_list	arg;
	va_start(arg, fmt);
	print_msg_valist_c(fmt, arg);
	va_end(arg);
}
