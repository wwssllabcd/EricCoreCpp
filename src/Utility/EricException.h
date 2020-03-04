#pragma once


#include "DefineFiles/DefineFile.h"


#define THROW_MYEXCEPTION(errCode, msg)      throw EricCore::EricException(errCode, msg)


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

        EricException(int errorCode, estring_cr msg);
		EricException(int errorCode, ELPCTSTR fmt, ...);

        const echar* what(void);
        int get_error_code(void) const;
	private:

		int m_errorCode;
		estring m_errMsg;


	};
}
