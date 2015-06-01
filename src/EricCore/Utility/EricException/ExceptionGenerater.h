#pragma once

#include "MyException.h"
#include "ExceptionDecorator.h"

namespace EricCore{namespace Exception
{

#define GenClass(name, val) \
	class name : public ExceptionDecorator{public:\
	MyException m_e;\
	name##(MyException mye):m_e(mye){m_errMsg = #name;};\
	tstring what(){return m_errMsg + "<-" + m_e.what();};\
	int getErrorCode(){return  val;};\
	};\


	GenClass(Out_Of_Range, 0x10);
	GenClass(Div_Zero, 0x20);
}}