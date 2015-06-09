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

			virtual ~MyExpDecorator(){};

			
			virtual tstring what(){
				tstring msg = _myExp->what();
				return msg;
			};
			virtual int getErrorCode(void) {
				return _myExp->getErrorCode();
			};
		};
	}
}
