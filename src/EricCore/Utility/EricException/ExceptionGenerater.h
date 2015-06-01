#pragma once

#include "ExcepDecoratorBase.h"


#define GenErrorClass(name, val) namespace EricCore{\
	namespace Exception{\
		char name##_string[] = #name;\
		class name : public ExcepBase<val, name##_string>{};\
	}\
}\


#ifndef _myexception
#define _myexception



#endif


GenErrorClass(OutOfRange,0x01)
GenErrorClass(DivZero,0x02)

