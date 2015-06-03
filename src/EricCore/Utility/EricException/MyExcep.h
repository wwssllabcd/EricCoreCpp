#pragma once

#include "../DefineFiles/DefineFile.h"
#include <string>
using namespace std;

namespace EricCore{
	namespace Exception{

		class MyExcep 
		{
		public:
			MyExcep(void){};
			~MyExcep(void){};


			virtual int getErrorCode(void){
				return m_errorCode;
			};
			virtual tstring what(void){
				return m_errMsg;
			};


			int m_errorCode;
			tstring m_errMsg;

		};
	}
}




