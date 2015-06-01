#pragma once

#include "../DefineFiles/DefineFile.h"
#include <string>
using namespace std;

namespace EricCore{
	namespace Exception{

		class MyExcep : public std::exception
		{
		public:
			MyExcep(void){};
			~MyExcep(void){};


			int getErrorCode(void){
				return m_errorCode;
			};
			tstring what(void){
				return m_errMsg;
			};


			int m_errorCode;
			tstring m_errMsg;

		};
	}
}




