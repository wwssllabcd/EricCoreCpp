#pragma once

#include <string>
#include <string.h>
#include <stdio.h>

#include <tchar.h>

#ifdef _VC2008
#define  FOPEN fopen_s
#define  STRCPY strcpy_s
#else
#define  FOPEN fopen
#define  STRCPY strcpy
#endif

#define ERIC_EXTERN_C extern "C"