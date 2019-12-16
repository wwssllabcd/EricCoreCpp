//Copyright © 2006- EricWang(wwssllabcd@gmail.com). All rights reserved

#include "stdafx.h"

#include "Utility\Utility.h"
#include "Utility\Observer.h"
#include "Utility\EricException.h"

using namespace EricCore;

estring m_string;

#define ERIC_EXTERN_C extern "C"

ERIC_EXTERN_C void* wrap_utility_strFormat(const echar* fmt, va_list marker) {
	Utility su;
	m_string = su.strFormat(0, fmt, marker);
	return &m_string;
}

ERIC_EXTERN_C void wrap_utility_sendmsg(void* peString, eu32 uIsTrue) {
	estring* str = (estring*)peString;
	bool isTrue;
	if(uIsTrue) {
		isTrue = true;
	} else {
		isTrue = false;
	}
    

    //Observer::sendMsg(*str, isTrue, false, 1);
}

ERIC_EXTERN_C void wrap_utility_clear_msg() {
	//Utility u;
	//u.sendMsgUartClean();
}

void throw_exception(int errCode, const echar* c) {
	estring msg(c);
    THROW_MYEXCEPTION(errCode, msg);
}

ERIC_EXTERN_C void wrap_exception(const echar* c) {
    throw_exception(0, c);
}

ERIC_EXTERN_C void wrap_exception_with_number(int errCode, const echar* c) {
    throw_exception(errCode, c);
}

ERIC_EXTERN_C void wrap_print_valist(const echar *format, va_list args) {
    void* str = wrap_utility_strFormat(format, args);
    wrap_utility_sendmsg(str, false);
}