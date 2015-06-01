#pragma once

#include "ExcepDecoratorBase.h"


#define GenErrorClass(name, val) namespace EricCore{\
	namespace Exception{\
		class name : public ExcepBase<val, val>{\
		public:\
		name##(void){\
		m_errMsg = #name;\
		}\
		};\
	}\
}\


#ifndef _myexception
#define _myexception



#endif


GenErrorClass(OutOfRange,0x01)
GenErrorClass(DivZero,0x02)

