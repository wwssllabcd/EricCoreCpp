#pragma once
//Copyright © EricWang(wwssllabcd@gmail.com). All rights reserved

#ifndef _ERIC_TYPE_
#define _ERIC_TYPE_

#include "EricConfig.h"

#define _ET(x)                _T(x)
#define ERIC_STATIC static
#define ERIC_GEN_POINTER_TYPE(T)        \
	typedef const T* const T##_sp;    \
	typedef T* const T##_p;           \
	typedef T* T##_usp;               \
    typedef volatile T  v##T;         
	

typedef unsigned char         eu8;
ERIC_GEN_POINTER_TYPE(eu8);

typedef unsigned short        eu16;
ERIC_GEN_POINTER_TYPE(eu16);

typedef unsigned long         eu32;
ERIC_GEN_POINTER_TYPE(eu32);

typedef unsigned long long    eu64; 
ERIC_GEN_POINTER_TYPE(eu64);


typedef char                   es8;
ERIC_GEN_POINTER_TYPE(es8);




#ifdef _UNICODE

typedef eu8*                  eBufPtr;
typedef unsigned int          euint;
typedef wchar_t               echar;
typedef const wchar_t*        ELPCTSTR;
typedef int lpts_wsl;

#else

typedef eu8* const            eu8BufPtr; // 此 type 還是有需要, 因為只靠_p不能區分ptr or array
typedef eu32* const           eu32BufPtr;

typedef unsigned int          euint;
typedef char                  echar;
typedef const char*           ELPCTSTR;
typedef int lpts_wsl;

#endif 

ERIC_GEN_POINTER_TYPE(echar);




#endif