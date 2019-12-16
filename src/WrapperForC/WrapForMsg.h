#pragma once

#include "../DefineFiles/EricType.h"
#include <stdarg.h>

#define CLEAR_MSG() wrap_utility_clear_msg()
#define SHOW_EXCEPT(x) wrap_exception(x)

void* wrap_utility_strFormat(const echar* fmt, va_list valist);
void wrap_utility_sendmsg(void* peString, eu32 isTrue);
void wrap_utility_clear_msg(void);
void wrap_exception(const echar* c);
void wrap_exception_with_number(int errCode, const echar* c);

void wrap_print_valist(const echar *format, va_list args);