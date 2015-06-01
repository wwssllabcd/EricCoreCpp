#pragma once
#include "MyException.h"

namespace EricCore{
	namespace Exception{
		class ExceptionDecorator : public MyException
		{
		public:
			virtual  ~ExceptionDecorator(void){};
			virtual tstring what() = 0;
			virtual int getErrorCode(void) = 0;
		};
	}
}
