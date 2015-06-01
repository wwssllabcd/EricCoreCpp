#pragma once
#include "MyExcep.h"

namespace EricCore{
	namespace Exception{
		class ExceptionDecorator : public MyExcep
		{
		public:
			virtual  ~ExceptionDecorator(void){};
			virtual tstring what() = 0;
			virtual int getErrorCode(void) = 0;
		};
	}
}
