#pragma once



#define VALUE_OF_PTR(T, PTR)                       (*((volatile T*)((PTR))))
#define PTR_TO_EU32(PTR)                           VALUE_OF_PTR(eu32, PTR)
#define PTR_TO_EU8(PTR)                            VALUE_OF_PTR(eu8, PTR)


#define TO_CEIL(dividend, divisor)                (dividend == 0 ? 0 : 1 + ( (dividend - 1) / divisor) )