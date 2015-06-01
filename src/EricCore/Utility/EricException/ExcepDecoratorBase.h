#pragma once

#include "MyException.h"
#include "ExceptionDecorator.h"

namespace EricCore{
	namespace Exception{
		template< int v, int u>
		class ExcepBase : public ExceptionDecorator
		{
		public:
			MyException e;
			const int myErrNo;

			ExcepBase(MyException mye)
				:e(mye)
				,myErrNo(v)
			{};

			ExcepBase(void)
				:myErrNo(v)
			{
				MyException mye;
				e = mye;
			};

			~ExcepBase(void){};


			tstring what(){
				return e.what();
			};

			int getErrorCode(){
				return  myErrNo;
			};

		};
	}
}