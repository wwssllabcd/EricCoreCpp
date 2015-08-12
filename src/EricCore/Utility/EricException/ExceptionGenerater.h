#pragma once

#include "MyExcepBase.h"
#include "ErrorCode.h"


namespace EricCore{namespace Exception
{
#define GenClass(name, code, msg) \
	class name : public MyExcepBase{\
	public:\
	name##(void):MyExcepBase(code,#msg){};\
	};\


	GenClass(Out_Of_Range, 0x01, "Out_Of_Range");
	GenClass(Div_Zero, 0x02, "Div_Zero");
	GenClass(FDC_PARAM_ERR, UTI_PARAM_ERROR, "value>=mapSize" );
	GenClass(UTI_OBS_REG_FAIL ,UTI_OBS_REG_FAIL_NO, "observerRegister error: function ptr is empty");


}}