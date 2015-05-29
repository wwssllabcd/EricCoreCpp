#pragma once

#include "MyE_OutOfRange.h"


#define GenErrorClass(name, val) namespace EricCore{\
	namespace Exception{\
		class name : public ExcepBase<val>{};\
	}\
}\


GenErrorClass(OutOfRange,0x01)
GenErrorClass(DivZero,0x02)


