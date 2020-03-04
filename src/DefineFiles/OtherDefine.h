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


#ifdef _UNICODE
	typedef std::wstring                 estring;
	#define tstring                      wstring
	#define tofstream                    wofstream
	#define tatoi(x)                     _wtoi(x)
	#define tfopen(x,y)                  _wfopen(x,y) //NT only
	#define tfopen_s(x, y, z)            _wfopen_s(x,y, z) 	
	#define tstrtoul(x, y, z)            wcstoul(x,y,z) 	
	#define tvsnprintf(i, x, y, z)       _vsnwprintf(i, x, y, z) 	
	#define tvsnprintf_s(a, b, c, d, e)  _vsnwprintf_s(a, b, c, d, e) 	
    #define tvscprintf(x, y)             _vscwprintf(x, y) 	
    #define tctime_s(x, y, z)             _wctime_s(x, y, z) 	

#else
	typedef std::string                    estring;
	#define tofstream                      ofstream
	#define tatoi(x)                       atoi(x)
	#define tfopen(x,y)                    fopen(x,y)
	#define tstrtoul(x, y, z)              strtoul(x,y,z) 	
    #define tvsnprintf(i, x, y, z)         _vsnprintf(i, x,y,z) 	
	#define tvsnprintf_s(a, b, c, d, e)    _vsnprintf_s(a, b, c, d, e) 	
	#define tvscprintf(x, y)               _vscprintf(x, y) 


#ifdef _ERIC_WIN_MFC_
#define tfopen_s(x,y,z)       fopen_s(x,y,z) 	
#define eric_remove(x)        _tremove(x)
#else
//for old interface
#define tfopen_s(x,y,z)       fopen(y,z)
#define eric_remove(x)        remove(x)
#endif

#endif

typedef const estring&    estring_cr;


