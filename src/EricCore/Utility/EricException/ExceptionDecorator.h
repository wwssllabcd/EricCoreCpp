#pragma once
#include "MyException.h"

namespace EricCore{
	namespace Exception{
		class ExceptionDecorator : public MyException
		{
		public:
			ExceptionDecorator(void){};
			~ExceptionDecorator(void){};

			virtual tstring what() = 0;
			virtual int getErrorCode(void) = 0;
		};
	}
}
