#pragma once
#include "MyExcep.h"

namespace EricCore{
	namespace Exception{
		class MyExpDecorator : public MyExcep
		{
		public:
			MyExcep* _myExp;

			MyExpDecorator(MyExcep* myExp)
				:_myExp(myExp)
			{};

			
			virtual tstring what(){
				return _myExp->what();
			};
			virtual int getErrorCode(void) {
				return _myExp->getErrorCode();
			};
		};
	}
}
