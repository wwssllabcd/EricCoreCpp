// CEdit_eric.cpp : 實作檔
//

#include "stdafx.h"
#include "CEdit_eric.h"
#include <windows.h>

#define VK_CONTROL        0x11
#define WM_KEYDOWN                      0x0100

// CEdit_eric

IMPLEMENT_DYNAMIC(CEdit_eric, CEdit)
CEdit_eric::CEdit_eric()
{
}

CEdit_eric::~CEdit_eric()
{
}


BEGIN_MESSAGE_MAP(CEdit_eric, CEdit)
END_MESSAGE_MAP()


// CEdit_eric 訊息處理常式

BOOL CEdit_eric::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{
		BOOL b = GetKeyState(VK_CONTROL) & 0x80;
		if( b ){
			if(LOWORD(pMsg->wParam) =='A'){	
				SetSel(0,-1);
				return TRUE;
			}
		}
	}
	return CEdit::PreTranslateMessage(pMsg);
}