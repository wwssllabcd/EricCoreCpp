#pragma once

#include "MyException.h"
#include "ExceptionDecorator.h"

namespace EricCore{namespace Exception
{
	class DivZero : public MyExpDecorator
	{
	public:
		DivZero( MyExcep* component)
			:MyExpDecorator(component){};
		virtual tstring what(){
			tstring msg = MyExpDecorator::what();
			return  "Div Zero," + msg;
		}

	};


	class OutOfRange : public MyExpDecorator
	{
	public:
		OutOfRange( MyExcep* component)
			:MyExpDecorator(component){};
		virtual tstring what(){
			tstring msg = MyExpDecorator::what();
			return  "OutOfRange," + msg;
		}

	};

}}