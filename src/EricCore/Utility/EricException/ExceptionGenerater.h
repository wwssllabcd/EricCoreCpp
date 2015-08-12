#pragma once

#include "MyExcepBase.h"
#include "ErrorCode.h"


namespace EricCore{namespace Exception
{
#define GenClass(name, msg, code) \
	class name : public MyExcepBase{\
	public:\
	name##(void):MyExcepBase(code,#msg){};\
	};\


	GenClass(Out_Of_Range, "Out_Of_Range", 0x01);
	GenClass(Div_Zero, "Div_Zero", 0x02);
	GenClass(UTI_OBS_REG_FAIL, "UTI_OBS_REG_FAIL", UTI_OBS_REG_FAIL_NO);
	GenClass(FDC_PARAM_ERR, "value>=mapSize" ,UTI_PARAM_ERROR);


}}