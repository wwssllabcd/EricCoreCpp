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
			return  "Div Zero," + MyExpDecorator::what();
		}

	};


	class OutOfRange : public MyExpDecorator
	{
	public:
		OutOfRange( MyExcep* component)
			:MyExpDecorator(component){};
		virtual tstring what(){
			return  "OutOfRange," + MyExpDecorator::what();
		}

	};

}}