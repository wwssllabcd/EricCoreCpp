#pragma once


#include "DefineFiles/DefineFile.h"


#define THROW_MYEXCEPTION(errCode, fmt, ...) EricCore::EricException(errCode, fmt, __VA_ARGS__)

namespace EricCore {

	class EricException : public std::exception
	{
	public:
        EricException(void)
		:m_errorCode(0)
		{
		};
		virtual ~EricException(void) throw() {
		};

        EricException(int errorCode, const estring& msg);
        EricException(int errorCode, ELPCTSTR fmt, ...);
        EricException(ELPCTSTR fmt, ...);

        const echar* what(void);
        int get_error_code(void) const;
	private:

		int m_errorCode;
		estring m_errMsg;


	};
}
