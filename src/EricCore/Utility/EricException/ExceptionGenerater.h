#pragma once

#include "MyException.h"
#include "ExceptionDecorator.h"

namespace EricCore{namespace Exception
{

#define GenClass(name, val) \
	class name : public ExceptionDecorator{public:\
	MyExcep m_e;\
	name##(void){m_errMsg = #name;};\
	name##(MyExcep mye):m_e(mye){m_errMsg = #name;};\
	tstring what(){return m_errMsg + " <- " + m_e.what();};\
	int getErrorCode(){return  val;};\
	};\


	GenClass(Out_Of_Range, 0x01);
	GenClass(Div_Zero, 0x02);
	GenClass(UTI_OBS_REG_FAIL, UTI_OBS_REG_FAIL_NO);

}}