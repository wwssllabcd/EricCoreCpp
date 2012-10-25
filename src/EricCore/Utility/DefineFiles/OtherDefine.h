//Type Define

#include <string>
#include <string.h>


typedef unsigned char  BYTE;
typedef unsigned long  ULONG;

//for gcc
typedef unsigned short WORD;
typedef char           TCHAR;

typedef unsigned int   UINT;
typedef const char*    LPCTSTR;

#ifndef _ERIC_WIN_MFC_
#ifndef _T(x)
	#define _T(x)      x
#endif
#endif

#ifdef _UNICODE
#define tstring       wstring
	#define tofstream     wofstream
	#define tatoi(x)      _wtoi(x)
	#define tfopen(x,y)   _wfopen(x,y) //NT only
#else
	#define tstring       string
	#define tofstream     ofstream
	#define tatoi(x)      atoi(x)
	#define tfopen(x,y)   fopen(x,y)
#endif




#define BIT0                    		0x01
#define BIT1                    		0x02
#define BIT2                    		0x04
#define BIT3                    		0x08
#define BIT4                    		0x10
#define BIT5                    		0x20
#define BIT6                    		0x40
#define BIT7                    		0x80
#define wBIT0                    		0x0001
#define wBIT1                    		0x0002
#define wBIT2                    		0x0004
#define wBIT3                    		0x0008
#define wBIT4                    		0x0010
#define wBIT5                    		0x0020
#define wBIT6                    		0x0040
#define wBIT7                    		0x0080
#define wBIT8                    		0x0100
#define wBIT9                    		0x0200
#define wBIT10                    		0x0400
#define wBIT11                    		0x0800
#define wBIT12                    		0x1000
#define wBIT13                    		0x2000
#define wBIT14                    		0x4000
#define wBIT15                    		0x8000

#if !defined FTEST_0P
#define FTEST_0P 0
#endif

#if !defined FTEST_4P
#define FTEST_4P 1
#endif

#if !defined FTEST_10P
#define FTEST_10P 2
#endif

#if !defined FTEST_20P
#define FTEST_20P 3
#endif

#if !defined FTEST_100P
#define FTEST_100P 4
#endif

#if !defined FTEST_ALL
#define FTEST_ALL 5
#endif
