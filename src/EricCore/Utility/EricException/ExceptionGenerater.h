#pragma once

#include "MyExcepBase.h"
#include "ErrorCode.h"


namespace EricCore{namespace Exception
{
#define GenClass(name, val) \
	class name : public MyExcepBase{\
	public:\
	name##(void):MyExcepBase(val,#name){};\
	};\


	GenClass(Out_Of_Range, 0x01);
	GenClass(Div_Zero, 0x02);
	GenClass(UTI_OBS_REG_FAIL, UTI_OBS_REG_FAIL_NO);
}}