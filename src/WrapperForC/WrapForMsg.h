#pragma once


#define WRAP_THROW_EXCEPTION(MSG)                throw_exception_c(0, (MSG))
#define WRAP_PRINT_MSG(fmt, ...)                 print_msg_c(fmt, ##__VA_ARGS__)


void throw_exception_c(int errCode, char* c);
void print_msg_c(const char* format, ...);
void print_msg_valist_c(const char *fmt, va_list arg);