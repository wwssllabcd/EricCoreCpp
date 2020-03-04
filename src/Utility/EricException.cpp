#include "stdafx.h"
#include "EricException.h"
#include "Utility.h"
#include <stdarg.h> // for va_start, va_end

using namespace EricCore;

EricException::EricException(int errorCode, estring_cr msg)
    :m_errorCode(errorCode)
    ,m_errMsg(msg) {
}

EricException::EricException(int errorCode, ELPCTSTR fmt, ...)
    :m_errorCode(errorCode) {
    estring str;
    va_list	arg;
    va_start( arg, fmt );
    Utility su;
    str = su.strFormatValist(0, fmt, arg);
    va_end( arg );

    m_errMsg = str;
}

const echar* EricException::what(void) {
    return m_errMsg.c_str();
}

int EricException::get_error_code(void) const {
    return m_errorCode;
}

