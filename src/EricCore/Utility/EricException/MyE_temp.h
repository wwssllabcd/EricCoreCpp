#pragma once

#include "MyE_OutOfRange.h"


#define GenErrorClass(name, val) namespace EricCore{\
	namespace Exception{\
		char name##_string[] = #name;\
		class name : public ExcepBase<val, name##_string>{};\
	}\
}\


GenErrorClass(OutOfRange,0x01)
GenErrorClass(DivZero,0x02)


