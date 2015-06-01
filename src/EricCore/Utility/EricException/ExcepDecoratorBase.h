#pragma once

#include "MyException.h"
#include "ExceptionDecorator.h"

#include <string>
using namespace std;

namespace EricCore{
	namespace Exception{

		class OutOfRange : MyException
		{
		public:
			MyException e;

			OutOfRange(void)
				:MyException(0x10, "OutOfRange")
			{};


			tstring what(){
				return  m_errMsg + e.what();
			};

			int getErrorCode(){
				return  m_errorCode;
			};
		};
	}
}